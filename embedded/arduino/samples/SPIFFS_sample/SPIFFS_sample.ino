#include "SPIFFS.h"
 
void setup() {
  Serial.begin(115200);
  
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  File file = SPIFFS.open("/test.txt");
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  
  
  Serial.print("Size: ");
  Serial.println(file.size());

  Serial.print("Cursor position: ");
  Serial.println(file.position());

  Serial.println("File content: ");
  while(file.available()){
    Serial.write(file.read());
  }
  Serial.print("Cursor position: ");
  Serial.println(file.position());
  file.seek(2);
  Serial.print("With seek(), moved cursor to: ");
  Serial.println(file.position());

  Serial.print("Char at pos 2: ");
  Serial.write(file.read());
  file.close();
}
 
void loop() {

}
