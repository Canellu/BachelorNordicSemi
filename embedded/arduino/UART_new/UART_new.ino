// #include <SoftwareSerial.h>

// SoftwareSerial mySerial(2, 3); // RX, TX

char fromSerial[128] = "";
char fromSerial_JSON[128] = "";

// used for char arrays
int arr_nrf = 0;
int arr_serial = 0;

long timestamp = 0;
long starting_time = 0;
long sensor_millis = 0;

void flush_serial()
{
  while(Serial.available() > 0)
  {
    Serial.read();
  }
}

void setup()
{  
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  // mySerial.begin(115200);

  flush_serial();

  static char *eptr;
  
  static char start_time[32] = "";
  static boolean start_time_rcvd = false;
  static boolean data_rcvd = false;
  static int i = 0;

  Serial.println("Waiting for start time");
  while(!start_time_rcvd)
  {
    while(!Serial.available())
    {
      if (strcmp(start_time, "")!= 0)
      {
        // Serial.println("running conversion");
        timestamp = strtoul(start_time, &eptr, 10);
        memset(start_time, 0, strlen(start_time));
        start_time_rcvd = true;
        break;
      }
      else
      {
          delay(200);
      }
    }
    char data = Serial.read();
    start_time[i] = data;
    i++;
    
  }

  // timestamp = receive_start_time();

  Serial.print("Time value in millis: ");
  Serial.print(timestamp);
  Serial.println();
  flush_serial();
  starting_time = millis();
}
void loop() // run over and over
{
  while(!Serial.available())
  {
    if(strcmp(fromSerial, "") != 0)
    {
      char time_str[32] = "";
      // "t":"9999", timestamp + current time - starting time CANDO: maybe numerical instead of string value
      sprintf(time_str,"\"t\":\"%lu\",",timestamp+millis()-starting_time);
      
      strcat(fromSerial_JSON, "{");
      strcat(fromSerial_JSON, time_str);
      strcat(fromSerial_JSON, fromSerial);
      strcat(fromSerial_JSON, "}");
      
      //mySerial.print(fromSerial_JSON);
      Serial.println(fromSerial_JSON);
      
      memset(fromSerial, 0, strlen(fromSerial));
      memset(fromSerial_JSON, 0, strlen(fromSerial_JSON));
      arr_serial = 0;
    }
    else
    {
      delay(100);
    }
  }
  char data = Serial.read();
  fromSerial[arr_serial] = data;
  arr_serial++;
}
