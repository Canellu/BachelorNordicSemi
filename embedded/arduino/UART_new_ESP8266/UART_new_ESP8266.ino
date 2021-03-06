
char fromSerial[128] = "";
char fromSerial_JSON[128] = "";

// used for char arrays
int arr_nrf = 0;
int arr_serial = 0;

long timestamp = 0;
long starting_time = 0;
long sensor_millis = 0;


// Read time starting value in millis to be used for timestamps
// Doesn't seem to be working
int receive_start_time()
{

  unsigned long start_time_no = 0;
  static char *eptr;

  static char start_time[32] = "";
  static boolean start_time_rcvd = false;
  static boolean data_rcvd = false;
  static int i = 0;

  Serial.println("Waiting for start time");

  while (!start_time_rcvd)
  {
    while (!Serial.available())
    {
      if (strcmp(start_time, "") != 0)
      {
        Serial.println(start_time);
        start_time_no = strtoul(start_time, &eptr, 10);
        Serial.println(start_time_no);
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

  return start_time_no;
}

void setup()
{
  // Open serial communications and wait for port to open:
  //Serial.setRxBufferSize(256);
  Serial.begin(115200);
  while (!Serial);

  int br = Serial.baudRate();

  Serial.printf("Serial is %d bps\n", br);

  /*
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
    // Serial.println("received data");
    char data = Serial.read();
    start_time[i] = data;
    i++;

    }

    // timestamp = receive_start_time();

    Serial.print("Time value in millis: ");
    Serial.print(timestamp);
    Serial.println();
    starting_time = millis();
  */
}
void loop() // run over and over
{
  while (!Serial.available())
  {
    if (strcmp(fromSerial, "") != 0)
    {
      /*char time_str[32] = "";
        // "t":"9999", timestamp + current time - starting time CANDO: maybe numerical instead of string value
        sprintf(time_str,"\"t\":\"%lu\",",timestamp+millis()-starting_time);

        strcat(fromSerial_JSON, "{");
        strcat(fromSerial_JSON, time_str);
        strcat(fromSerial_JSON, fromSerial);
        strcat(fromSerial_JSON, "}");

        //mySerial.print(fromSerial_JSON); */
      Serial.println(fromSerial);

      memset(fromSerial, 0, strlen(fromSerial));
      // memset(fromSerial_JSON, 0, strlen(fromSerial_JSON));
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
