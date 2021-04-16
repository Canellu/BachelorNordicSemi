#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>
#include "materialize.h"

// VARIABLES

// Wifi
#ifndef STASSID
#define STASSID "OASYS-ESP"
#define STAPSK "aaaaaaaa"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

int arr_rx = 0;
char uart_rx[256] = "";

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

boolean on_wifi = false;

// convert single char to int
int digit_to_int(char d)
{
  char str[2];

  str[0] = d;
  str[1] = '\0';
  return (int)strtol(str, NULL, 10);
}

// load root site
void handleRoot()
{
  digitalWrite(LED_BUILTIN, LOW);
  server.send(200, "text/html", web);
}

// error loading site
void handleNotFound()
{
  digitalWrite(LED_BUILTIN, HIGH);
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}

// host website and open websocket
void website_init()
{
  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.print(ssid);
  Serial.print(" IP address: ");
  Serial.println(WiFi.softAPIP());

  // setup webpages
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  // setup websocket and callback function
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  Serial.println("HTTP server started");
}

// read commands from nrf and perform respective operations
void read_nrf_commands()
{
  if (Serial.available() > 0)
  {
    char nrf_command = Serial.read();

    switch (nrf_command)
    {
    case '0':
      on_wifi = true;
      website_init();

      break;
    default:
      break;
    }
  }
}

void flush_serial()
{
  while(Serial.available() > 0)
  {
    Serial.read();
  }
}

void setup(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  flush_serial();

  Serial.println("Waiting for command 0");
}

void loop(void)
{

  if (on_wifi)
  { 
    webSocket.loop();      // keeps tcp connection alive?
    server.handleClient(); // checks if client is connected

    // read input from serial and send to webpage
    if (Serial.available() > 0)
    {
      char b = Serial.read();
      if (b == ';')
      {    
        if (strcmp(uart_rx, "wifi_end") == 0)
        {
          // Serial.print("ending wifi");
          webSocket.close();
          server.close();
          WiFi.mode(WIFI_OFF);
          on_wifi = false;
          flush_serial();
        }
        else
        {
          webSocket.broadcastTXT(uart_rx, strlen(uart_rx));
        }
        memset(uart_rx, 0, sizeof(uart_rx));
        arr_rx = 0;
      }
      else
      {   
        uart_rx[arr_rx++] = b;
      }

    }
  }
  
  else
  {
    read_nrf_commands();
  }
}

// callback function whenever we receive data from webpage
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  if (type == WStype_CONNECTED)
  {
    Serial.print("\n{connected}");
  }
  if (type == WStype_TEXT)
  {
    char ws_text[256] = "";
    // print to terminal
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
      ws_text[i] = (char)payload[i];
    }
    Serial.print(ws_text);
  }
  
}
