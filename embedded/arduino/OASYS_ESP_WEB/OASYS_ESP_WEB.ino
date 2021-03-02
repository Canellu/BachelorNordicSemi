#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>
#include "website.h"


// VARIABLES

#ifndef STASSID
#define STASSID "OASYS-ESP"
#define STAPSK  "aaaaaaaa"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);



// load root site
void handleRoot() {
  digitalWrite(LED_BUILTIN, LOW);
  server.send(200, "text/html", web);
}

// error loading site
void handleNotFound() {
 digitalWrite(LED_BUILTIN, HIGH);
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
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

void loop(void) {
  webSocket.loop();
  server.handleClient();

  // read input from serial and send to webpage
  if(Serial.available() > 0) {
    char c[] = {(char)Serial.read()};
    webSocket.broadcastTXT(c, sizeof(c));
  }
}


// callback function whenever we receive data from webpage
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if(type == WStype_TEXT) {

    // print to terminal
    for(int i = 0; i < length; i++) {
      Serial.print((char) payload[i]);
    }
    Serial.println();
  }
 
}
