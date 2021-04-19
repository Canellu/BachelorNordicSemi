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
#include "cJSON.h"

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
	EVT_SCIENTIFIC,
	EVT_4G,
	EVT_WIFI,
	EVT_SATELLITE,
	EVT_NAVIGATION
};

// Structs
struct prog_param_t
{ // currently not in use
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

	enum glider_event_type event_now;
	enum glider_event_type event_prev;
	bool mission_started;
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
	// k_msgq_init(&mqtt_msg_q, mqtt_msgq_buffer, sizeof(mqtt_msg), 3);

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
	// k_msgq_purge(&mqtt_msg_q);
	k_msgq_purge(&gcloud_msg_q);
	return 0;
}

/* MODULES */

static int sensor_module()
{
	button_wait();

	set_button_config(1);
	uart_start(uart_dev1);
	uart_send(uart_dev1, "time:36000000", strlen("time:36000000"));

	printk("\nsensor test start\n");

	printk("button 1: exit \nbutton 2: send uart signal\n\n");

	/* UART loop */
	for (int i = 0;; i++)
	{

		// memset(uart_msg, 0, sizeof(uart_msg));
		k_msgq_get(&uart_msg_q, &uart_msg, K_FOREVER);

		// TODO: create function to check uart msg similar to mqtt

		if (strcmp(uart_msg, "surfaced") == 0)
		{
			uart_send(uart_dev1, "sensor_end", strlen("sensor_end"));
			uart_exit(uart_dev1);
			data_available_to_send = true;
			break;
		}
		else
		{
			// printk("%s", uart_msg);

			// TODO: test timestamp to check if new day

			cJSON *sensor_JSON = cJSON_Parse(uart_msg);

			int sensor_n = cJSON_GetArraySize(sensor_JSON);

			if (sensor_n > 0)
			{
				// fetching timestamp (ms) to convert to hh:mm:ss format
				cJSON *ts_raw = cJSON_GetObjectItem(sensor_JSON, "ts");
				cJSON_DeleteItemFromObject(sensor_JSON, "ts");
				uint32_t tmp_int = (ts_raw->valueint) / 1000;
				cJSON_Delete(ts_raw);

				int hour = tmp_int / 3600 % 24;
				int min = (tmp_int / 60) % 60;
				int sec = tmp_int % 60;

				uint8_t ts_string[64] = "";
				uint8_t temp_str[16];

				snprintf(temp_str, sizeof(temp_str), "%02d:", hour);
				strcat(ts_string, temp_str);
				snprintf(temp_str, sizeof(temp_str), "%02d:", min);
				strcat(ts_string, temp_str);
				snprintf(temp_str, sizeof(temp_str), "%02d", sec);
				strcat(ts_string, temp_str);

				// adding back to JSON object
				cJSON_AddStringToObject(sensor_JSON, "ts", ts_string);

				// convert to string
				uint8_t sensor_str[256];
				strcpy(sensor_str, cJSON_Print(sensor_JSON));
				cJSON_Minify(sensor_str);

				// save to sd card
				// sd_save_data(sensor_str);
			}

			cJSON_Delete(sensor_JSON);

			// sd_save_data(uart_msg);

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
		else if (strcmp(wifi_response, "{wifi_end}") == 0)
		{
			printk("Received wifi end command\n");
			uart_send(UART_2, "wifi_end;", strlen("wifi_end;"));
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

	glider->mission_started = false;

	printk("Init complete. Press button 1 to start\n\n");

	button_wait();

	return 0;
}

static bool check_new_glider(struct glider_t *glider)
{
	oasys_data_t sd_msg;
	sd_msg.event = FIND_FILE;
	static const char file_mission[] = "MISSIONPARAMS.TXT";
	memcpy(sd_msg.filename, file_mission, sizeof(file_mission));

	k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);

	bool sd_msg_response = false;

	k_msgq_get(&main_msg_q, &sd_msg_response, K_FOREVER);

	if (sd_msg_response)
	{
		printk("Mission params found\n");
		return true;
	}

	printk("Mission params do not exist\n");
	return false;
}

/* MAIN */

void main(void)
{
	printk("\n\n**** NordicOasys v0.99 - Started ****\n\n");

	struct glider_t glider;
	glider.event_now = EVT_INIT;

	while (1)
	{
		message_queue_reset();

		switch (glider.event_now)
		{
		case EVT_INIT:
			glider_init(&glider);

			bool newglider = check_new_glider(&glider);

			// if (!newglider)
			// {
			// 	glider.event_prev = glider.event_now;
			// 	glider.event_now = EVT_IDLE;
			// }
			// else
			// {
			// 	glider.event_prev = glider.event_now;
			// 	glider.event_now = EVT_NEWGLIDER;
			// }

			glider.event_now = EVT_SCIENTIFIC;

			// IDEA: idle at wifi module no matter what
			// Instead, in case of reboot, test if it was on a mission

			break;
		case EVT_NEWGLIDER:
			// currently idles until button is pressed and local wifi is turned on
			// functions in the same way as case EVT_WIFI
			wifi_module();

			glider.event_prev = glider.event_now;
			glider.event_now = EVT_GPS;

			break;
		case EVT_IDLE:
			LOG_INF("Idling");
			k_sleep(K_SECONDS(5));

			// logic for test start time, if true start mission

			glider.event_prev = glider.event_now;
			glider.event_now = EVT_GPS;

			break;
		case EVT_GPS:
			gps_module();

			// upload mission params to database after receiving params from local wifi
			if (glider.event_prev == EVT_WIFI || glider.event_prev == EVT_NEWGLIDER)
			{
				glider.event_prev = glider.event_now;
				glider.event_now = EVT_4G;
			}

			// was idle waiting for mission to start
			else if (glider.event_prev == EVT_IDLE)
			{
				// test the start time vs gps time, if true, start mission

				if (glider.mission_started)
				{
					glider.event_prev = glider.event_now;
					glider.event_now = EVT_NAVIGATION;
				}

				// otherwise, continue idle
				else
				{
					glider.event_prev = glider.event_now;
					glider.event_now = EVT_IDLE;
				}
			}

			// mission ongoing, sensor readings performed
			else if (glider.mission_started)
			{
				glider.event_prev = glider.event_now;
				glider.event_now = EVT_4G;
			}

			break;
		case EVT_SCIENTIFIC:
			sensor_module();

			glider.event_prev = glider.event_now;
			glider.event_now = EVT_SCIENTIFIC;

			break;
		case EVT_4G:
			gcloud_module();

			// test if glider is on last waypoint, if true, mission ongoing false and go to idle

			// test if connection was successful, otherwise enable satellite modem

			// send command to control system, either continue dive or go to first waypoint
			glider.event_prev = glider.event_now;
			glider.event_now = EVT_NAVIGATION;

			break;
		case EVT_WIFI:
			wifi_module();

			glider.event_prev = glider.event_now;
			glider.event_now = EVT_GPS;

			break;
		case EVT_SATELLITE:
			satellite_module();

			// send command to control system, either continue dive or go to first waypoint
			glider.event_prev = glider.event_now;
			glider.event_now = EVT_NAVIGATION;

			break;
		case EVT_NAVIGATION:
			// placeholder, send some parameters and commands to control system of glider

			if (glider.mission_started)
			{
				glider.event_prev = glider.event_now;
				glider.event_now = EVT_SCIENTIFIC;
			}
			else
			{
				glider.event_prev = glider.event_now;
				glider.event_now = EVT_IDLE;
			}

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

// currently not in use
// MQTT variables
struct k_msgq mqtt_msg_q;
static uint8_t mqtt_msgq_buffer[3 * 128];
static uint8_t mqtt_msg[128];

static bool mqtt_init_complete = false;

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
