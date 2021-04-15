#include <zephyr.h>
#include <nrf_socket.h>
#include <dk_buttons_and_leds.h>
#include <kernel.h>
#include <string.h>
#include <stdio.h>
#include <modem/at_cmd.h>
#include <logging/log.h>

#include "app_button.h"
#include "app_uart.h"
#include "app_mqtt.h"
#include "app_gps.h"
#include "app_sd.h"
#include "gcloud.h"

LOG_MODULE_REGISTER(main);

/* Stack definition for application */
static K_THREAD_STACK_DEFINE(sd_stack_area, 4096);

/* Threads */
static struct k_thread sd_thread;
k_tid_t sd_tid;

/* STRUCTS AND ENUMS */

// Enumerations
// CANDO: typedef everything

enum glider_event_type
{
	EVT_INIT,
	EVT_NEWGLIDER,
	EVT_IDLE,
	EVT_GPS,
	EVT_SENSOR,
	EVT_MQTT,
	EVT_WIFI,
	EVT_SATELLITE
};

// Structs
struct prog_param_t
{
	bool bttn_init;
	bool leds_init;
	bool uart_init;
	bool mqtt_init;
};

struct sens_param_t
{
	int freq_t;
	int freq_p;
	int freq_c;
};

struct mission_param_t
{
	int wp_max;
	int wp_cur;
	int depth_max;
	int depth_min;
	struct sens_param_t sens_param;
};

struct glider_t
{ // currently not in use
	uint8_t uid[7];
	struct mission_param_t mission_param;
	struct prog_param_t prog_param;
	enum glider_event_type event;
};

/* VARIABLE DECLARATIONS */

// Main message queue, used for getting responses from other modules
struct k_msgq main_msg_q;
static uint8_t main_msgq_buffer[3 * 128];
static uint8_t main_msg[128];

// button variables
struct k_msgq button_msg_q;
struct k_msgq button_msg_qr;
static uint8_t button_msgq_buffer[3 * sizeof(int)];
static int button_val; // value received from button, only used when button_config = 0

// UART variables
enum uart_device_type uart_dev1 = UART_1;
enum uart_device_type uart_dev2 = UART_2;

struct k_msgq uart_msg_q;
static uint8_t uart_msgq_buffer[3 * 128];
static uint8_t uart_msg[128];

// TODO: change to general data_array instead of just uart data
static uint8_t uart_data_array[16][128] = {""};
int data_size = 0; // keeps track of no. of messages stored in data array.
				   // UPDATE THIS VALUE WHEN SAVING INTO DATA ARRAY

// GPS variables
struct k_msgq gps_msg_q;
static uint8_t gps_msgq_buffer[2 * sizeof(oasys_gps_data_t)];
static oasys_gps_data_t gps_data;

// MQTT variables
struct k_msgq mqtt_msg_q;
static uint8_t mqtt_msgq_buffer[3 * 128];
static uint8_t mqtt_msg[128];

static bool mqtt_init_complete = false;
static bool data_available_to_send = false;

// Gcloud variables
struct k_msgq gcloud_msg_q;
static uint8_t gcloud_msgq_buffer[3 * 128];
static uint8_t gcloud_msg[128];

static bool gcloud_init_complete = false;

// SD Card variables
struct k_msgq sd_msg_q;
static uint8_t sd_msgq_buffer[5 * sizeof(oasys_data_t)];

// Time variables
static uint16_t current_year = -1;
static uint8_t current_month = -1;
static uint8_t current_day = -1;

/* FUNCTION DECLARATIONS */

/* HELPER FUNCTIONS */

// publish all data that is currently saved from uart
// REMINDER: Double check after implementing SD card
static int publish_uart_data()
{
	int i = 0;

	while (strcmp(uart_data_array[i], "") != 0)
	{
		app_data_publish(uart_data_array[i], strlen(uart_data_array[i]) + 1);
		k_sleep(K_MSEC(500));
		i++;
	}
	data_available_to_send = false;

	return 0;
}

// test messages received from mqtt
static int check_mqtt_msg(void *data, size_t len)
{
	int ret_value = 0;

	if (strcmp(data, "LED") == 0)
	{
		toggle_LED(1);
	}

	if (strcmp(data, "exit") == 0)
	{
		ret_value = 1;
	}

	else if (strcmp(data, "button 2") == 0)
	{
		ret_value = 2;
	}

	return ret_value;
}

// changes button configuration
static int set_button_config(int local_button_config)
{
	k_msgq_purge(&button_msg_q);
	k_msgq_put(&button_msg_q, &local_button_config, K_NO_WAIT);

	return 0;
}

// waiting function, waits for button 1 to be pressed
static int button_wait()
{
	uint8_t test = 0;
	set_button_config(0);
	while (1)
	{
		k_msgq_get(&button_msg_qr, &test, K_FOREVER);
		if (test == 1)
		{
			break;
		}
	}

	return 0;
}

// fills out current date, sends string and event type
static int sd_save_data(void *data_string)
{
	oasys_data_t sd_msg;
	sd_msg.event = WRITE_FILE;

	// create filename, format: [yyyymmdd]
	uint8_t temp_str[16];

	snprintf(temp_str, sizeof(temp_str), "%02u", current_year);
	strcat(sd_msg.filename, temp_str);
	snprintf(temp_str, sizeof(temp_str), "%02u", current_month);
	strcat(sd_msg.filename, temp_str);
	snprintf(temp_str, sizeof(temp_str), "%02u.txt", current_day);
	strcat(sd_msg.filename, temp_str);

	// fill string to be saved
	memcpy(sd_msg.json_string, data_string, strlen(data_string));

	// add to sd card message queue
	k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);

	return 0;
}

// initialize message queues
//TODO: replace sizeof parameter with type instead of variable
static int message_queue_init()
{
	// main
	k_msgq_init(&main_msg_q, main_msgq_buffer, sizeof(main_msg), 3);

	// button
	k_msgq_init(&button_msg_q, button_msgq_buffer, sizeof(int), 3);
	k_msgq_init(&button_msg_qr, button_msgq_buffer, sizeof(int), 3);

	// UART
	k_msgq_init(&uart_msg_q, uart_msgq_buffer, sizeof(uart_msg), 3);

	// GPS
	k_msgq_init(&gps_msg_q, gps_msgq_buffer, sizeof(gps_data), 2);

	// MQTT
	k_msgq_init(&mqtt_msg_q, mqtt_msgq_buffer, sizeof(mqtt_msg), 3);

	// GCloud
	k_msgq_init(&gcloud_msg_q, gcloud_msgq_buffer, sizeof(gcloud_msg), 3);

	// SD
	k_msgq_init(&sd_msg_q, sd_msgq_buffer, sizeof(oasys_data_t), 5);

	return 0;
}

//TODO: pass devices to initialise
static int device_inits()
{
	button_dev_init();
	dk_leds_init();
	uart_dev_init(uart_dev1);
	uart_dev_init(uart_dev2);
	return 0;
}

// purge leftover messages from message queue
static int message_queue_reset()
{
	// NOTE: THIS IS ONLY HERE UNTIL SD CARD STORAGE IS OKAY
	data_size = 0;
	int i = 0;
	while (strcmp(uart_data_array[i], "") != 0)
	{
		strcpy(uart_data_array[i], "");
		i++;
	}

	k_msgq_purge(&button_msg_q);
	k_msgq_purge(&uart_msg_q);
	k_msgq_purge(&gps_msg_q);
	k_msgq_purge(&mqtt_msg_q);
	k_msgq_purge(&gcloud_msg_q);
	return 0;
}

/* MODULES */

static int sensor_module()
{
	set_button_config(1);
	uart_start(uart_dev1);

	printk("\nsensor test start\n");
	printk("button 1: exit \nbutton 2: send uart signal\n\n");

	/* UART loop */
	for (int i = 0;; i++)
	{
		k_msgq_get(&uart_msg_q, &uart_msg, K_FOREVER);

		// TODO: create function to check uart msg similar to mqtt

		if (strcmp(uart_msg, "{exit}") == 0)
		{
			uart_exit(uart_dev1);
			data_available_to_send = true;
			break;
		}
		else
		{

			// TODO: test timestamp to check if new day

			sd_save_data(uart_msg);

			// printk("\nUART message: %s", uart_msg);
			// add to data array
			// strcpy(uart_data_array[i], uart_msg);
		}
	}
	/* UART loop end */

	// print all uart data
	if (strcmp(uart_data_array[0], "") != 0)
	{
		int i = 0;
		while (strcmp(uart_data_array[i], "") != 0)
		{
			printk("\nArray %d: %s", i, uart_data_array[i]);
			i++;
			data_size++;
		}
	}
	else
	{
		printk("no data saved\n");
	}

	printk("\nsensor test end\n\n");

	return 0;
}

static int wifi_module()
{
	int err = 0;

	printk("\n\npress button 1 to start wifi test\n\n");

	button_wait();

	uint8_t wifi_response[128] = "";
	uart_start(UART_2);

	printk("wifi test start\n");

	uart_send(UART_2, "0", strlen("0"));
	printk("\nsent command 0 to esp");

	while (1)
	{
		k_msgq_get(&uart_msg_q, wifi_response, K_FOREVER);
		printk("\n%s", wifi_response);
		//button_wait();

		//strcpy(wifi_response, "{D:20202020.txt}");

		if (strcmp(wifi_response, "{connected}") == 0)
		{
			oasys_data_t sd_msg;
			sd_msg.event = SEND_FILE_INFO;
			k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);

			printk("\n\nConnect successful, press button 1 to continue");
			// strcpy(wifi_response, "read1");
		}
		else if (wifi_response[1] == 'D')
		{
			oasys_data_t sd_msg;

			// fetch filename
			// format yyyymmdd.txt
			char *date = wifi_response + 3;
			date[strlen(date) - 1] = 0;

			memcpy(sd_msg.filename, date, 13);
			printk("\n%s", sd_msg.filename);

			sd_msg.event = READ_FILE;

			printk("\nStarting SD file read2");
			k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);
		}
		else
		{
			printk("\n\nSomething went wrong, press button 1 to continue");
		}
	}
	uart_exit(UART_2);

	return err;
}

// TODO: Test on separate thread?
// Currently on hold until LTE and GPS can function at the same time
static int gps_module()
{
	printk("\n\npress button 1 to start gps test\n\n");

	button_wait();

	printk("gps test start\n");

	app_gps(3000, 400);
	k_msgq_get(&gps_msg_q, &gps_data, K_NO_WAIT);

	// print to terminal
	// {"lng":"23.771611","lat":"61.491275"}
	printk("\n%s", gps_data.gps_string);

	// update date CANDO: create function?
	current_year = gps_data.year;
	current_month = gps_data.month;
	current_day = gps_data.day;

	// save gps data on sd card
	sd_save_data(gps_data.gps_string);

	// send time to sensors
	uint8_t time_millis_str[16];
	uint32_t time_millis = ((gps_data.hour * 60 * 60) + (gps_data.minute * 60) + gps_data.seconds) * 1000;
	snprintf(time_millis_str, sizeof(time_millis_str), "%02u", time_millis);

	uart_send(uart_dev1, time_millis_str, sizeof(time_millis_str));

	// add to data array
	// strcpy(uart_data_array[data_size++], gps_data.gps_string);
	// data_available_to_send = true;

	printk("\ngps test end\n\n");

	return 0;
}

// currently not in use
static int wifi_mqtt_module()
{
	set_button_config(1);
	uart_start(uart_dev1);

	printk("\nwifi test start\n");

	// send wifi command to wifi controller
	uart_send(uart_dev1, "{wifi}\n", strlen("{wifi}\n"));

	/* wifi loop */
	for (int i = 0;; i++)
	{

		// test messages from wifi controller
		k_msgq_get(&uart_msg_q, &uart_msg, K_FOREVER);
		printk("\n%s", uart_msg);

		if (strcmp(uart_msg, "{exit}") == 0)
		{
			uart_exit(uart_dev1);
			break;
		}
		// wifi setup success, send further commands
		else if (strcmp(uart_msg, "{wifi ok}") == 0)
		{
			printk("\nReceived wifi ok");
			uart_send(uart_dev1, "{mqtt}\n", strlen("{mqtt}\n"));
		}
		// send data from uart_data_array
		else if (strcmp(uart_msg, "{mqtt ok}") == 0)
		{
			printk("\nReceived mqtt ok");
			publish_uart_data();
		}
		// else if (strcmp(uart_msg, "{keyword}") == 0)
		// {
		// 	//something
		// }
		// default print to terminal
		else
		{
			printk("\n%s", uart_msg);
		}
	}
	/* wifi loop end */
	return 0;
}

static int mqtt_module()
{

	printk("\n\npress button 1 to start mqtt test\n\n");
	button_wait();

	set_button_config(2);

	printk("mqtt test start\n");

	int err;

	// mqtt init
	if (!mqtt_init_complete)
	{

		err = app_mqtt_init_and_connect();
		if (err != 0)
		{
			return err;
		}

		mqtt_init_complete = true;
	}
	// reconnect
	else
	{

		err = app_mqtt_connect();
		if (err != 0)
		{
			return err;
		}
	}

	// TODO: store mqtt commands into an array
	/* MQTT loop */
	while (1)
	{

		// mqtt function
		err = app_mqtt_run();

		// test messages received from mqtt
		k_msgq_get(&mqtt_msg_q, &mqtt_msg, K_NO_WAIT);
		int mqtt_val = check_mqtt_msg(&mqtt_msg, sizeof(mqtt_msg));

		if (mqtt_val == 1 || err != 0)
		{
			strcpy(mqtt_msg, "");
			break;
		}
		else if (mqtt_val == 2)
		{

			printk("button choice: 2\n\n");
			strcpy(mqtt_msg, "");
		}

		if (data_available_to_send)
		{
			publish_uart_data();
		}
	}
	/* MQTT loop end */

	err = app_mqtt_disconnect();
	if (err != 0)
	{
		printk("\nDisconnect unsuccessful: %d", err);
		printk("\n\nClosing program, check for errors in code lol");
		return err;
	}

	return 0;
}

static int gcloud_module()
{
	printk("\n\npress button 1 to start gcloud test\n\n");
	button_wait();
	printk("gcloud test start\n");

	int err;

	// mqtt init
	if (!gcloud_init_complete)
	{
		err = app_gcloud_init_and_connect();
		if (err != 0)
		{
			return err;
		}
		gcloud_init_complete = true;
	}
	// reconnect
	else
	{
		err = app_gcloud_reconnect();
		if (err != 0)
		{
			return err;
		}
	}

	/* Gcloud loop */
	while (1)
	{
		// gcloud function
		err = app_gcloud();

		k_msgq_get(&gcloud_msg_q, &gcloud_msg, K_NO_WAIT);
		printk("\nMain: %s", gcloud_msg);

		// if (strcmp(gcloud_msg, "test") == 0)
		// {
		// 	strcpy(gcloud_msg, "");
		// 	break;
		// }
	}
	/* Gcloud loop end */

	err = app_gcloud_disconnect();
	if (err != 0)
	{
		printk("\nDisconnect unsuccessful: %d", err);
		printk("\n\nClosing program, check for errors in code lol");
		return err;
	}
	return 0;
}

static int satellite_module()
{

	printk("\n\npress button 1 to start satellite test\n\n");
	button_wait();

	uart_start(uart_dev1);

	uart_send(uart_dev1, "AT\r", strlen("AT\r"));

	k_sleep(K_MSEC(2000));
	uart_send(uart_dev1, "AT+CGSN\r", strlen("AT+CGSN\r"));

	k_sleep(K_MSEC(2000));
	uart_exit(uart_dev1);

	return 0;
}

static int glider_init(struct glider_t *glider)
{

	/* Fetch serial number (uid) */
	uint8_t imei[18];
	uint8_t uid[18];
	at_cmd_write("AT+CGSN", imei, sizeof(imei), NULL);
	memcpy(uid, imei + 8, strlen(imei) - 8);
	memcpy(glider->uid, uid, 6);
	glider->uid[sizeof(glider->uid) - 1] = 0;

	/* device inits and configurations */
	device_inits();

	// message queues
	message_queue_init();

	// sd card thread
	sd_tid = k_thread_create(&sd_thread, sd_stack_area, K_THREAD_STACK_SIZEOF(sd_stack_area),
							 (k_thread_entry_t)app_sd_thread, NULL, NULL, NULL,
							 7, 0, K_NO_WAIT);

	printk("Init complete. Press button 1 to start\n\n");

	button_wait();

	return 0;
}

/* MAIN */

void main(void)
{
	printk("\n\n**** NordicOasys v0.99 - Started ****\n\n");

	struct glider_t glider;
	glider.event = EVT_INIT;

	while (1)
	{
		message_queue_reset();

		switch (glider.event)
		{
		case EVT_INIT:
			glider_init(&glider);

			oasys_data_t sd_msg;
			sd_msg.event = FIND_FILE;
			static const char file[] = "MISSIONPARAMS.TXT";
			memcpy(sd_msg.filename, file, sizeof(file));

			k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);

			bool sd_msg_response = false;

			k_msgq_get(&main_msg_q, &sd_msg_response, K_FOREVER);

			if (sd_msg_response)
			{
				printk("Mission params found\n");
				glider.event = EVT_IDLE;
			}
			else
			{
				printk("Mission params do not exist\n");
				glider.event = EVT_NEWGLIDER;
			}

			break;
		case EVT_NEWGLIDER:
			// currently idles until button is pressed and local wifi is turned on
			// functions in the same way as case EVT_WIFI
			wifi_module();

			break;
		case EVT_IDLE:
			LOG_INF("Idling");
			k_sleep(K_SECONDS(5));

			break;
		case EVT_GPS:
			gps_module();

			break;
		case EVT_SENSOR:
			sensor_module();

			break;
		case EVT_MQTT:
			gcloud_module();

			break;
		case EVT_WIFI:
			wifi_module();

			break;
		case EVT_SATELLITE:
			satellite_module();

			break;
		default:
			LOG_ERR("Unknown event type");
		}

		// printk("\npress button 1 to start mqtt\n");
		// printk("press button 2 to start wifi\n\n");
		// set_button_config(3);
		// k_msgq_get(&button_msg_qr, &button_val, K_FOREVER);

		// if (button_val == 1)
		// {
		// 	mqtt_module();
		// }
		// else if (button_val == 2)
		// {
		// 	wifi_module();
		// }
	}

	printk("**** NordicOasys test end ****\n");
}

/* Function ideas and TODOs

- Allow option to choose between "main parts" 
  e.g. using switch case to choose between UART and MQTT.

- Fill up dev_param struct. Should contain parameters set by user.
  Should be changeable through MQTT.

- Save messages received from MQTT into array similar to data.

- FIX FORMATTING OF DATA:
	DONE:
	WIP:
		gps data
		sensor data
		formatting on sd card (should not be necessary and should only copy string directly)

*/

/* CURRENT FLOW FOR HOW MISSION SEQUENCE WILL OPERATE

1st time start (1 way to test is if .txt file for mission params exist):
	-turn on UART peripherals, SD card module
	-wait for local (wifi) connection trigger
		--might have to be separate thread so that it can be interrupted whenever triggered
	-get gps fix
	-search for 4G connection to upload mission params
		--this is necessary whenever it gets parameters through wifi
	-start mission loop


If local wifi is triggered:
	-enable wifi module (send data, receive commands)
	-get gps fix
	-search for 4G connection to upload mission params
		--this is necessary whenever it gets parameters through wifi
	-start mission loop


If mission is sent through 4G:
	-get gps fix
	-start mission loop


Mission loop:
	-get to first waypoint (signifies start of mission)
	-idle until start time
		--might have to periodically get gps fix to see if time/position is synchronized
	-send dive command
	-start sensor readings
		--send timestamp to sensors for logging purposes
	-save readings to sd card
	-on resurface:
		--get gps fix
		--turn on 4G
			---on success, send data, check for changes in commands (CHECK CLOUD FUNCTION LOGIC FOR SENDING CONFIGURATIONS INSTEAD OF ONE-TIME COMMANDS)
			---else, turn on satellite modem and send gps location + health status
	-continue dive
	-repeat until last waypoint
	-get gps fix
	-turn on 4G
		--check if any new missions
		--else, idle
			---periodically check 4G for any new missions, else enable satellite modem to send gps location + health status
*/