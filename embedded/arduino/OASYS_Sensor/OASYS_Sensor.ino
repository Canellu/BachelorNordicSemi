
int arr_rx = 0;
int arr_tx = 0;
char uart_rx[256] = "";

long timestamp = 0;
long start_time = 0;

boolean on_sensor = false;

long timeout_t = 1000;
long timeout_p = 1000;
long timeout_c = 1000;

long last_t = 0;
long last_p = 0;
long last_c = 0;

void flush_serial()
{
  while(Serial.available() > 0)
  {
    Serial.read();
  }
}

void setup(void)
{
  Serial.begin(115200);
  flush_serial();

  // Serial.println("Waiting for start time");
}

void loop(void)
{
  // check for any commands
  if (Serial.available() > 0)
  {
    char c = Serial.read();

    // delimiter, test against known commands
    if (c == '\r')
    {
      // Serial.print(uart_rx);
      
      // command turn on sensor
      if (strstr(uart_rx, "time:") != NULL && !on_sensor)
      {
        Serial.print("Turning on sensor\n\r");

        char *ptr = strstr(uart_rx, "time:");
        
        static char *eptr;
        timestamp = strtoul(ptr + 5, &eptr, 10);
        start_time = millis();

        on_sensor = true;

        // Serial.print(timestamp);
      }
      // command turn off sensor
      else if (strcmp(uart_rx, "sensor_end") == 0 && on_sensor)
      {
        Serial.print("Turning off sensor\n\r");
        on_sensor = false;

        timeout_t = 1000;
        timeout_p = 1000;
        timeout_c = 1000;
        
        flush_serial();
      }
      
      // parameter timeout temperature
      else if (strstr(uart_rx, "to_t:") != NULL && on_sensor)
      {
        char *ptr = strstr(uart_rx, "to_t:");
        static char *eptr;
        timeout_t = strtoul(ptr + 5, &eptr, 10);
        
        Serial.print("timeoutT set to: ");
        Serial.print(timeout_t);
        Serial.print("\r");
      }
      // parameter timeout pressure
      else if (strstr(uart_rx, "to_p:") != NULL && on_sensor)
      {
        char *ptr = strstr(uart_rx, "to_p:");
        static char *eptr;
        timeout_p = strtoul(ptr + 5, &eptr, 10);
        
        Serial.print("timeoutP set to: ");
        Serial.print(timeout_p);
        Serial.print("\r");
      }
      // parameter timeout conductivity
      else if (strstr(uart_rx, "to_c:") != NULL && on_sensor)
      {
        char *ptr = strstr(uart_rx, "to_c:");
        static char *eptr;
        timeout_t = strtoul(ptr + 5, &eptr, 10);
        
        Serial.print("timeoutC set to: ");
        Serial.print(timeout_c);
        Serial.println("\r");
      }

      memset(uart_rx, 0, sizeof(uart_rx));
      arr_rx = 0;
    }
    // append character to string
    else
    {   
      uart_rx[arr_rx++] = c;
    }
  }

  // sensor up and running
  if (on_sensor)
  { 
    // gives time from when sensor started
    long current_time = millis() - start_time;

    char value[128] = "";

    // temperature sensor
    if (current_time % timeout_t < last_t)
    {
      // log data
      char temp_str[16] = "";
      sprintf(temp_str,"\"T\":%d,", random(1, 20));
      strcat(value, temp_str);
      
      last_t = 0;
    }
    else
    {
      last_t = current_time % timeout_t;
    }

    // pressure sensor
    if (current_time % timeout_p < last_p)
    {
      // log data
      char temp_str[16] = "";
      sprintf(temp_str,"\"P\":%d,", random(1, 20));
      strcat(value, temp_str);
      
      last_p = 0;
    }
    else
    {
      last_p = current_time % timeout_p;
    }

    // conductivity sensor
    if (current_time % timeout_c < last_c)
    {
      // log data
      char temp_str[16] = "";
      sprintf(temp_str,"\"C\":%d,", random(1, 20));
      strcat(value, temp_str);
      
      last_c = 0;
    }
    else
    {
      last_c = current_time % timeout_c;
    }

    // if there are any sensor readings, print data
    if(strlen(value) != 0)
    {
      char data_JSON[256] = "";
      char temp_str[32] = "";
      
      sprintf(temp_str,"{\"ts\":%lu,\"data\": {", timestamp + current_time);
      strcat(data_JSON, temp_str);
      strcat(data_JSON, value);
      data_JSON[strlen(data_JSON)-1] = '\0';
      strcat(data_JSON, "}}\r");

      Serial.print(data_JSON);
    }
  }
}
