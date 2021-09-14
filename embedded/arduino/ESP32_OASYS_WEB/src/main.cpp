#include <Arduino.h>
#include <SPIFFS.h>            // For filesystem
#include "ESPAsyncWebServer.h" // For webserver & websocket
#include "WiFi.h"              // For Access Point

// --------------------------------------------------
// Definition of macros
// --------------------------------------------------

// Wifi
#ifndef STASSID
#define STASSID "OASYS-ESP"
#define STAPSK "nordicoasys123"
#endif

#define HTTP_PORT 80

// --------------------------------------------------
// Definition of global variables
// --------------------------------------------------

// WiFi
const char *ssid = STASSID;
const char *password = STAPSK;
boolean on_wifi = false;

// Webserver & Socket
AsyncWebServer webServer(HTTP_PORT);
AsyncWebSocket webSocket("/ws");
AsyncEventSource events("/events");

// Satellite
boolean on_sat = false;
boolean test_sat = false;

// UART
#define RXnrf 13
#define TXnrf 12

#define RXsat 27
#define TXsat 14

int arr_nrf = 0;
char rx_nrf[512] = "";
int arr_sat = 0;
char rx_sat[128] = "";

// --------------------------------------------------
// ESP File system
// --------------------------------------------------
void initSPIFFS()
{
  Serial.println("Initializing SPIFFS");
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
  }
  else
  {
    Serial.println("SPIFFS Initialized");
  }
}

// --------------------------------------------------
// UART Utilities
// --------------------------------------------------
void flushSerial()
{
  while (Serial1.available() > 0)
  {
    Serial1.read();
  }
}

void emptyUartBuffer()
{
  memset(rx_nrf, 0, sizeof(rx_nrf));
  arr_nrf = 0;
}

void wifiEnd()
{
  webSocket.closeAll();
  webServer.end();
  WiFi.mode(WIFI_OFF);
  on_wifi = false;
  flushSerial();
}

// --------------------------------------------------
// Websocket
// --------------------------------------------------

void handleWebSocketMessage(AsyncWebSocket *server,
                            AsyncWebSocketClient *client,
                            AwsEventType type,
                            void *arg,
                            uint8_t *data,
                            size_t len)
{
  //data packet
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len)
  {
    //the whole message is in a single frame and we got all of it's data
    Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT) ? "text" : "binary", info->len);
    if (info->opcode == WS_TEXT)
    {
      if (strstr((char *)data, "wifi_end") != NULL)
      {
        Serial.println("Wifi ending...");
        wifiEnd();
      }
      else if (strstr((char *)data, "test:sat") != NULL)
      {
        Serial.println("test sat msg received");
        test_sat = true;
      }
      data[len] = 0;
      Serial.printf("%s\n", (char *)data);
      Serial1.printf("%s\r", (char *)data);
    }

     /* Uncomment the section below if the frames are big. */

    // else
    // {
    //   for (size_t i = 0; i < info->len; i++)
    //   {
    //     Serial.printf("%02x ", data[i]);
    //   }
    //   Serial.printf("\n");
    // }
    // if (info->opcode == WS_TEXT)
    //   client->text("I got your text message");
    // else
    //   client->binary("I got your binary message");
  }


  /* Uncomment the section below if the frames are big. */

  // else
  // {
  //   //message is comprised of multiple frames or the frame is split into multiple packets
  //   if (info->index == 0)
  //   {
  //     if (info->num == 0)
  //       Serial.printf("ws[%s][%u] %s-message start\n\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
  //     Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n\n", server->url(), client->id(), info->num, info->len);
  //   }

  //   Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT) ? "text" : "binary", info->index, info->index + len);
  //   if (info->message_opcode == WS_TEXT)
  //   {
  //     data[len] = 0;
  //     Serial.printf("%s\n\n", (char *)data);
  //   }
  //   else
  //   {
  //     for (size_t i = 0; i < len; i++)
  //     {
  //       Serial.printf("%02x ", data[i]);
  //     }
  //     Serial.printf("\n\n");
  //   }

  //   if ((info->index + len) == info->len)
  //   {
  //     Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n\n", server->url(), client->id(), info->num, info->len);
  //     if (info->final)
  //     {
  //       Serial.printf("ws[%s][%u] %s-message end\n\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
  //       if (info->message_opcode == WS_TEXT)
  //         client->text("I got your text message");
  //       else
  //         client->binary("I got your binary message");
  //     }
  //   }
  // }
}

// Async Websocket Event Types (AwsEventTypes)
// ******************************************************************
// WS_EVT_CONNECT     when a client has logged in
// WS_EVT_DISCONNECT  when a client has logged out
// WS_EVT_DATA        when a data packet is received from the client
// WS_EVT_PONG        in response to a ping request
// WS_EVT_ERROR       when an error is received from the client
// ******************************************************************

// the signature of this function is defined by the `AwsEventHandler` interface
void onEvent(AsyncWebSocket *server,
             AsyncWebSocketClient *client,
             AwsEventType type,
             void *arg,
             uint8_t *data,
             size_t len)
{

  switch (type)
  {
  case WS_EVT_CONNECT:
    // Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    Serial1.println("connected");

    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    break;
  case WS_EVT_DATA:
    handleWebSocketMessage(server, client, type, arg, data, len);
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}

void initWebSocket()
{
  webSocket.onEvent(onEvent);
  webServer.addHandler(&webSocket);
}

void sendToClients(char *msg)
{
  webSocket.textAll(msg, strlen(msg));
}

// --------------------------------------------------
// Web Server
// --------------------------------------------------
void initWebServer()
{
  webServer.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  webServer.begin();
}

// --------------------------------------------------
// Access Point
// --------------------------------------------------
void initAP()
{
  Serial.println("Initializing Access Point (AP)");

  // Setting ESP as Access Point (AP)
  // Remove the password parameter for AP to be open
  WiFi.enableAP(true);
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
}

// --------------------------------------------------
// Start WiFi and initialize Server and Socket
// --------------------------------------------------
void wifiBegin()
{
  initAP();
  initWebServer();
  initWebSocket();
  flushSerial();

  events.onConnect([](AsyncEventSourceClient *client)
                   {
                     if (client->lastId())
                     {
                       Serial.printf("Client reconnected! Last message ID that it gat is: %u\n", client->lastId());
                     }
                   });
  webServer.addHandler(&events);
}

// --------------------------------------------------
// Waits for start command from nRF
// --------------------------------------------------
void awaitStart()
{
  if (Serial1.available() > 0)
  {
    char c = Serial1.read();
    if (c == '\r')
    {
      if (strstr(rx_nrf, "wifi_start") != NULL)
      {
        Serial.println("Starting wifi");
        Serial1.println("OK");
        on_wifi = true;
        wifiBegin();
        emptyUartBuffer();
      }
      else if (strstr(rx_nrf, "sat_start") != NULL)
      {
        Serial.println("Starting satellite");
        Serial1.println("OK");
        on_sat = true;
        emptyUartBuffer();
      }
      else
      {
        Serial.println("unrecognized start command");
        emptyUartBuffer();
      }
    }
    else
    {
      rx_nrf[arr_nrf++] = c;
    }
  }
}

// --------------------------------------------------
// SETUP
// --------------------------------------------------
void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, RXnrf, TXnrf);
  Serial2.begin(19200, SERIAL_8N1, RXsat, TXsat);
  delay(500);
  initSPIFFS();
}

// --------------------------------------------------
// MAIN LOOP
// --------------------------------------------------

void loop()
{

  if (on_wifi)
  {
    webSocket.cleanupClients(1); // Limiting number of clients, cleaning up and freeing resources, defaults to 8 connected clients

    // read input from serial and send to webpage
    if (Serial1.available() > 0)
    {
      char c = Serial1.read();
      Serial.print(c);
      if (c == '\r')
      {
        // sendToClients(rx_nrf);
        events.send(rx_nrf, NULL, millis());
        memset(rx_nrf, 0, sizeof(rx_nrf));
        arr_nrf = 0;
      }
      else
      {
        rx_nrf[arr_nrf++] = c;
      }
    }

    if (test_sat)
    {
      Serial.println("Running sat test");
      static int signal = -1;
      static boolean test_ok = true;
      static int max_tries = 3;
      static int tries = 0;

      Serial2.print("AT+CSQ\r");

      while (tries < max_tries)
      {
        if (Serial2.available() > 0) // satellite
        {
          char c = Serial2.read();
          if (c == '\r')
          {
            Serial.println("Received response: ");
            Serial.println(rx_sat);

            if (strstr((char *)rx_sat, "OK") != NULL)
            {
              if (signal > 0)
              {
                Serial.println("Received signal");
                test_ok = true;
                break;
              }
              else
              {
                Serial.println("Sending AT command");
                Serial2.print("AT+CSQ\r");
                tries++;
                delay(10000);
              }
            }
            else if (strcmp(rx_sat, "ERROR") == 0)
            {
              Serial.println("Received ERROR");
              test_ok = false;
              break;
            }
            else if (strcmp(rx_sat, "AT+CSQ\r") != 0 && strlen(rx_sat) != 0)
            {
              static char *eptr;
              signal = strtol(rx_sat + 5, &eptr, 10);
              Serial.println("Signal val: ");
              Serial.println(signal);
            }

            memset(rx_sat, 0, sizeof(rx_sat));
            arr_sat = 0;
          }
          else
          {
            rx_sat[arr_sat++] = c;
          }
        }
      }

      if (test_ok)
      {
        events.send("test:sat,OK", NULL, millis());
      }
      else
      {
        events.send("test:sat,ERROR", NULL, millis());
      }
      memset(rx_sat, 0, sizeof(rx_sat));
      arr_sat = 0;

      test_sat = false;
    }
  }
  else if (on_sat)
  {
    if (Serial1.available() > 0) // nRF
    {
      char c = Serial1.read();
      Serial.print(c);
      if (c == '\r')
      {
        if (strstr(rx_nrf, "sat_end") != NULL)
        {
          Serial1.println("sat_end OK");
          memset(rx_nrf, 0, sizeof(rx_nrf));
          memset(rx_sat, 0, sizeof(rx_sat));
          arr_nrf = 0;
          arr_sat = 0;
          on_sat = false;
        }
        else
        {
          Serial.println(rx_nrf);
          Serial2.println(rx_nrf);
          memset(rx_nrf, 0, sizeof(rx_nrf));
          arr_nrf = 0;
        }
      }
      else
      {
        rx_nrf[arr_nrf++] = c;
      }
    }

    if (Serial2.available() > 0) // satellite
    {
      char c = Serial2.read();
      if (c == '\r')
      {
        Serial.println(rx_sat);
        Serial1.println(rx_sat);
        memset(rx_sat, 0, sizeof(rx_sat));
        arr_sat = 0;
      }
      else
      {
        rx_sat[arr_sat++] = c;
      }
    }
  }
  else
  {
    awaitStart();
  }
}