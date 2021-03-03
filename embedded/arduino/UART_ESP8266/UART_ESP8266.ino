#include <SoftwareSerial.h>

SoftwareSerial mySerial(5, 4); // RX, TX  GPIO5 = D1, GPIO4 = D2
String fromNrf = "";
String fromSerial = "";

int led_on = 0;

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
      fromNrf += data2;
    }

    // testing data from nRF
    else if(fromNrf != "")
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
      
      fromNrf = "";
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
