#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX
String fromNrf = "";
String fromSerial = "";

void setup()
{  
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }


  // Serial.print("hello,");
  // delay(100);
  // Serial.print("battery 40%,");
  // delay(100);
  // Serial.print("dive true,");
  // delay(100);
  // Serial.print("temp 1,");
  // delay(100);
  // Serial.print("duration 20,");
  // delay(100);
  // Serial.print("health false,");
  // delay(100);
  // Serial.print("nice,");
  // delay(100);
  // Serial.print("nice,");
  // delay(100);
  // Serial.print("temp 35,");
  // delay(100);
  // Serial.print("nice");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);
  // mySerial.print("Hello,world,");
}

void loop() // run over and over
{
  while(!Serial.available())
  {
    if(fromSerial != "")
    {
      mySerial.print(fromSerial);
      Serial.print(fromSerial);
      fromSerial = "";
    }
    else if(mySerial.available())
    {
      char data2 = mySerial.read();
      fromNrf += data2;
    }
    else if(fromNrf != "")
    {
      Serial.print(fromNrf);
      fromNrf = "";
    }
    else
    {
      delay(200);
    }
  }
  char data = Serial.read();
  fromSerial += data;
}
