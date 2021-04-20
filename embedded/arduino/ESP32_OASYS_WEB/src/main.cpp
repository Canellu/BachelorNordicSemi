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

// Access point name and password
const char *ssid = STASSID;
const char *password = STAPSK;

// Webserver & Socket
AsyncWebServer webServer(HTTP_PORT);
AsyncWebSocket webSocket("/ws");

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
      data[len] = 0;
      Serial.printf("%s\n", (char *)data);
    }
    else
    {
      for (size_t i = 0; i < info->len; i++)
      {
        Serial.printf("%02x ", data[i]);
      }
      Serial.printf("\n");
    }
    if (info->opcode == WS_TEXT)
      client->text("I got your text message");
    else
      client->binary("I got your binary message");
  }
  else
  {
    //message is comprised of multiple frames or the frame is split into multiple packets
    if (info->index == 0)
    {
      if (info->num == 0)
        Serial.printf("ws[%s][%u] %s-message start\n\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
      Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n\n", server->url(), client->id(), info->num, info->len);
    }

    Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT) ? "text" : "binary", info->index, info->index + len);
    if (info->message_opcode == WS_TEXT)
    {
      data[len] = 0;
      Serial.printf("%s\n\n", (char *)data);
    }
    else
    {
      for (size_t i = 0; i < len; i++)
      {
        Serial.printf("%02x ", data[i]);
      }
      Serial.printf("\n\n");
    }

    if ((info->index + len) == info->len)
    {
      Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n\n", server->url(), client->id(), info->num, info->len);
      if (info->final)
      {
        Serial.printf("ws[%s][%u] %s-message end\n\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
        if (info->message_opcode == WS_TEXT)
          client->text("I got your text message");
        else
          client->binary("I got your binary message");
      }
    }
  }
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
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
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
  webSocket.textAll(msg);
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
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
}

// --------------------------------------------------
// SETUP
// --------------------------------------------------
void setup()
{

  Serial.begin(115200);
  delay(500);

  initSPIFFS();
  initAP();
  initWebSocket();
  initWebServer();

  delay(2000);
  char msg[10] = "Hello";
  sendToClients(msg);
}

// --------------------------------------------------
// MAIN LOOP
// --------------------------------------------------

void loop()
{
  webSocket.cleanupClients(); // Limiting number of clients, cleaning up and freeing resources, defaults to 8 connected clients
}