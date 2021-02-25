#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ArduinoMqttClient.h>

// UART
SoftwareSerial mySerial(5, 4); // RX, TX  GPIO5 = D1, GPIO4 = D2

String fromNrf = "";
String tmp_fromNrf = "";
String fromSerial = "";

// WIFI
#ifndef STASSID
#define STASSID "NETGEAR94"
#define STAPSK  "luckyroad468"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

WiFiClient client;

int wifi_on = 0;

// MQTT variables
MqttClient mqttClient(client);

const char broker[] = "broker.mqttdashboard.com"; // Address of the MQTT server
int        port     = 1883;
const char topic[]  = "projects/nordicoasys/topics/data";
const char subtopic[]  = "projects/nordicoasys/topics/commands";

String subMessage = "";

unsigned long timeoutMillis = 60000;
unsigned long startMillis = 0;
unsigned long currentMillis = 0;

int mqtt_on = 0;

// MISC
int led_on = 0;


// FUNCTIONS

void wifi_connect()
{
  // Connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  wifi_on = 1;
  
  // Serial.println("{wifi ok}");  
  mySerial.print("{wifi ok}");
}

void wifi_disconnect()
{
  // Close the connection
  Serial.println();
  Serial.println("closing connection");
  client.stop();

  wifi_on = 0;

  // Serial.println("{wifi disconnected}");
  mySerial.print("{wifi disconnected}");
}

void mqtt_connect()
{
  mqttClient.setId("einarNrf9160_WiFi");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  while (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
  
  // subscribe to a topic
  mqttClient.subscribe(subtopic);
  digitalWrite(LED_BUILTIN, LOW);
  mqtt_on = 1;

  startMillis = millis();

  // Serial.println("{mqtt ok}");
  mySerial.print("{mqtt ok}");
}

void mqtt_disconnect()
{
  mqttClient.stop();
  mqtt_on = 0;
  
  // Serial.println("{mqtt disconnected}");
  mySerial.print("{mqtt disconnected}");
}

void mqtt_receive()
{
  int messageSize = mqttClient.parseMessage();
  if (messageSize) {
    subMessage = "";
    // we received a message, print out the topic and contents
    Serial.print("Received a message with topic '");
    Serial.print(mqttClient.messageTopic());
    Serial.print("', length ");
    Serial.print(messageSize);
    Serial.println(" bytes:");

    // use the Stream interface to print the contents
    while (mqttClient.available()) {
      subMessage = subMessage + (char)mqttClient.read();
    }
   Serial.println(subMessage);
  }
}

void mqtt_keepalive()
{
    currentMillis = millis() - startMillis;
    
    if(currentMillis >= timeoutMillis)
    {
      mqttClient.beginMessage(topic);
      mqttClient.print("test");
      mqttClient.endMessage();
      Serial.println("Sent MQTT message.");

      startMillis = millis();
    }
}

void mqtt_publish()
{
  mqttClient.beginMessage(topic);
  mqttClient.print(fromNrf);
  mqttClient.endMessage();
  
  Serial.println("{msg sent}");
  mySerial.print("{msg sent}");
}


void process_nrf_msg()
{     
  Serial.print(fromNrf);

  if (fromNrf == "{\"hi\"}") // toggle LED
  {
    if(led_on == 0)
    {
      digitalWrite(LED_BUILTIN, LOW);
      led_on = 1;
    }
    else
    {
      digitalWrite(LED_BUILTIN, HIGH);
      led_on = 0;
    }
  } // toggle LED end
  else if (fromNrf == "{wifi}")
  {
    wifi_connect();
  }
  else if (fromNrf == "{mqtt}")
  {
    mqtt_connect();
  }
  else if (fromNrf == "exit")
  {
    mqtt_disconnect();
    wifi_disconnect();
  }
  else if (wifi_on == 1 && mqtt_on == 1)
  {
    mqtt_publish();
    
    startMillis = millis();
  }
  
  fromNrf = "";
}


// PROGRAM

void setup() {
  
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
  Serial.println();
  Serial.println();
  
  mySerial.begin(115200);
  mySerial.print("{Hello, world}");
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  
}

// the loop function runs over and over again forever
void loop() {
  while(!Serial.available())
  {
    // send data to nRF
    if(fromSerial != "")
    {
      mySerial.print(fromSerial);
      Serial.print(fromSerial);
      fromSerial = "";
    }

    // read data from nRF
    else if(mySerial.available())
    {
      char data2 = mySerial.read();
      if (data2 == '{')
      {
        tmp_fromNrf = "";
        tmp_fromNrf += data2;
      }
      else if (data2 == '}')
      {
        tmp_fromNrf += data2;
        fromNrf = tmp_fromNrf;
      }
      else
      {
        tmp_fromNrf += data2;
      }
    }

    // testing data from nRF
    else if(fromNrf != "")
    {
      process_nrf_msg();
    }

    // mqtt on, not receiving commands from nRF
    else if (wifi_on == 1 && mqtt_on == 1)
    {
      mqtt_keepalive();

      mqtt_receive();
    }

    // idle
    else
    {
      delay(200);
    }
  }

  // read from serial monitor
  char data = Serial.read();
  fromSerial += data;
}
