#include <SoftwareSerial.h>

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>
#include "materialize.h"

// VARIABLES

// UART
SoftwareSerial mySerial(5, 4); // RX, TX  GPIO5 = D1, GPIO4 = D2

// Wifi
#ifndef STASSID
#define STASSID "OASYS-ESP"
#define STAPSK "aaaaaaaa"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

int arr_rx = 0;
char uart_rx[128] = "";

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

boolean on_wifi = false;
enum
{
  WIFI_ON
} nrf_commands_type;

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
  if (mySerial.available() > 0)
  {
    char nrf_command = mySerial.read();
    int nrf_command_int = digit_to_int(nrf_command);

    switch (nrf_command_int)
    {
    case WIFI_ON:
      on_wifi = true;
      website_init();

      break;
    default:
      break;
    }
  }
}

void flush_nrf_serial()
{
  while(mySerial.available() > 0)
  {
    mySerial.read();
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
  mySerial.begin(115200);

  flush_serial();
  flush_nrf_serial();

  Serial.println("Waiting for command 0");
}

void loop(void)
{

  if (on_wifi)
  {
    webSocket.loop();      // keeps tcp connection alive?
    server.handleClient(); // checks if client is connected

    // read input from serial and send to webpage
    if (mySerial.available() > 0)
    {
      char b = mySerial.read();
      uart_rx[arr_rx++] = b;
    }
    else if (Serial.available() > 0)
    {
      char c = Serial.read();
      uart_rx[arr_rx++] = c;
    }
    else if (!mySerial.available() && !Serial.available() && strlen(uart_rx) != 0)
    {
      Serial.print(uart_rx);
      webSocket.broadcastTXT(uart_rx, strlen(uart_rx));
      memset(uart_rx, 0, sizeof(uart_rx));
      arr_rx = 0;
    }
  }
  else
  {
    read_nrf_commands();
    flush_nrf_serial();
  }
}

// callback function whenever we receive data from webpage
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  if (type == WStype_TEXT)
  {

    // print to terminal
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
  }
}
