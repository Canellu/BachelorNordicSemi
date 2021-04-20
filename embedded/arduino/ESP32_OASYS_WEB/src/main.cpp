#include <Arduino.h>
#include <SPIFFS.h>
#include "ESPAsyncWebServer.h"

// --------------------------------------------------
// Variable declarations
// --------------------------------------------------

// Wifi
#ifndef STASSID
#define STASSID "OASYS-ESP"
#define STAPSK "aaaaaaaa"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

AsyncWebServer server(80);

// --------------------------------------------------
// Initialize file system
// --------------------------------------------------
void initSPIFFS()
{
  Serial.println("Initializing SPIFFS");
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  else
  {
    Serial.println("SPIFF Initialized");
  }
}

// Setup
void setup()
{
  initSPIFFS();
  Serial.begin(115200);
  Serial.println("Hello");
}

void loop()
{
  // put your main code here, to run repeatedly:
}