#include <SoftwareSerial.h>
int br = 0;

SoftwareSerial mySerial(5, 4); // RX, TX  GPIO5 = D1, GPIO4 = D2

void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);
}

void loop() {
  /*
    while(!Serial.available())
    {
    if(strcmp(fromSerial, "") != 0)
    {
      Serial.println(fromSerial);
      memset(fromSerial, 0, strlen(fromSerial));
      arr_serial = 0;
    }
    else
    {
      delay(10);
    }
    }
    char data = Serial.read();
    fromSerial[arr_serial] = data;
    arr_serial++;
  */
  if (mySerial.available())
  {
    byte data = mySerial.read();
    Serial.print((char)data);
  }
}
