#include <zephyr.h>
#include <nrf_socket.h>
#include <dk_buttons_and_leds.h>
#include <kernel.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
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

enum glider_event_type
{
	EVT_INIT,
	EVT_AWAIT_MISSION,
	EVT_DIVE,
	EVT_SURFACE,
	EVT_IDLE
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

struct mission_start_t
{
	int year;
	int month;
	int day;

	int hour;
	int minute;
};

struct mission_param_t
{
	int mission;
	int depth_max;
	int depth_min;
	uint8_t unsent_data_files[10][32];
	int msg_max;
	double wp_arr[10][2];
	int wp_max;
	int wp_cur;
	struct mission_start_t mission_start;
	struct sens_param_t sens_param;
};

struct glider_t
{
	uint8_t uid[7];
	struct mission_param_t mission_param;
	struct prog_param_t prog_param;

	bool mission_started;
};

/* VARIABLE DECLARATIONS */

struct glider_t glider;
static const char file_mission[] = "M_PARAM.TXT";

// Main message queue, used for getting responses from other modules
struct k_msgq main_msg_q;
static uint8_t main_msgq_buffer[3 * 256];
static uint8_t main_msg[256];

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

// Gcloud variables
struct k_msgq gcloud_msg_q;
static uint8_t gcloud_msgq_buffer[512];
static uint8_t gcloud_msg[512];

static bool data_available_to_send = false;
static bool gcloud_init_complete = false;

// SD Card variables
struct k_msgq sd_msg_q;
static uint8_t sd_msgq_buffer[5 * sizeof(sd_msg_t)];

// GPS data message queue
struct k_msgq gps_msg_q;
static uint8_t gps_msgq_buffer[3 * 256];
static uint8_t gps_msg[256];

// Time variables
static int current_year = -1;
static int current_month = -1;
static int current_day = -1;

// TEMPORARY, FOR TESTING
static int current_hour = -1;
static int current_minute = -1;
static int current_seconds = -1;

/* FUNCTION DECLARATIONS */

/* HELPER FUNCTIONS */

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

// fills out current date as filename, sends string to sd module to write
static int sd_save_data(void *data_string)
{
	sd_msg_t sd_msg;
	sd_msg.event = WRITE_FILE;

	// printk("saving to sd card\n");

	// create filename, format (assuming mission 2): [M2.TXT]
	uint8_t filename[16] = "";
	uint8_t temp_str[16] = "";

	// snprintf(temp_str, sizeof(temp_str), "%u", glider.mission_param.mission);
	// strcat(filename, temp_str);

	snprintf(temp_str, sizeof(temp_str), "M%u.TXT", glider.mission_param.mission);
	strcat(filename, temp_str);

	// copy string over to struct
	strcpy(sd_msg.filename, filename);
	//sd_msg.filename[strlen(filename)] = 0;
	//printk("\n%s\n", sd_msg.filename);

	strcpy(sd_msg.string, data_string);

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
	k_msgq_init(&gps_msg_q, gps_msgq_buffer, sizeof(gps_msg), 1);

	// MQTT
	// k_msgq_init(&mqtt_msg_q, mqtt_msgq_buffer, sizeof(mqtt_msg), 3);

	// GCloud
	k_msgq_init(&gcloud_msg_q, gcloud_msgq_buffer, sizeof(gcloud_msg), 3);

	// SD
	k_msgq_init(&sd_msg_q, sd_msgq_buffer, sizeof(sd_msg_t), 5);

	return 0;
}

//TODO: pass devices to initialise
static int device_inits()
{
	button_dev_init();
	dk_leds_init();
	ext_led_init();
	uart_dev_init(uart_dev1);
	uart_dev_init(uart_dev2);
	return 0;
}

// purge leftover messages from message queue
static int message_queue_reset()
{
	// NOTE: THIS IS ONLY HERE UNTIL SD CARD STORAGE IS OKAY
	// data_size = 0;
	// int i = 0;
	// while (strcmp(uart_data_array[i], "") != 0)
	// {
	// 	strcpy(uart_data_array[i], "");
	// 	i++;
	// }

	k_msgq_purge(&button_msg_q);
	k_msgq_purge(&uart_msg_q);
	// k_msgq_purge(&mqtt_msg_q);
	k_msgq_purge(&gcloud_msg_q);

	return 0;
}

// TODO: pass in filenames
static int publish_data_4G()
{
	// printk("\nRunning test, press button 1 to start\n");
	// button_wait();

	// TODO: fetch parameters from mission params
	uint8_t msg_max = 2;
	uint8_t msg_sent = 0;

	sd_msg_t sd_msg;

	uint8_t temp_str[16] = "";

	k_msgq_purge(&main_msg_q);

	sd_msg.event = READ_JSON_4G;

	snprintf(temp_str, sizeof(temp_str), "M%d.TXT", glider.mission_param.mission);
	strcpy(sd_msg.filename, temp_str);

	printk("\nname: %s", sd_msg.filename);

	snprintf(temp_str, sizeof(temp_str), "max:%d\r", msg_max);
	strcat(sd_msg.string, temp_str);
	snprintf(temp_str, sizeof(temp_str), "wp_cur:%d\r", glider.mission_param.wp_cur);
	strcat(sd_msg.string, temp_str);

	// printk("\n%s", sd_msg.string);
	k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);

	uint8_t sd_msg_response[512] = "";

	printk("\n");

	while (1)
	{
		k_msgq_get(&main_msg_q, &sd_msg_response, K_FOREVER);

		// printk("\nmsg: %s\n", sd_msg_response);
		cJSON *payload_JSON = cJSON_Parse(sd_msg_response);

		if (cJSON_IsObject(payload_JSON))
		{
			uint8_t ts_str[32] = "";
			int ts_arr = 0;

			// fetching datetime (dt) and timestamp (ts) to combine into one variable
			cJSON *dt_JSON = cJSON_DetachItemFromObject(payload_JSON, "dt");
			cJSON *ts_JSON = cJSON_DetachItemFromObject(payload_JSON, "ts");

			// find only numeric values of dt
			for (int i = 0; i < strlen(dt_JSON->valuestring); i++)
			{
				if (isdigit(dt_JSON->valuestring[i]))
				{
					ts_str[ts_arr++] = dt_JSON->valuestring[i];
				}
			}

			// find only numeric values of ts
			for (int i = 0; i < strlen(ts_JSON->valuestring); i++)
			{
				if (isdigit(ts_JSON->valuestring[i]))
				{
					ts_str[ts_arr++] = ts_JSON->valuestring[i];
				}
			}

			cJSON_Delete(dt_JSON);
			cJSON_Delete(ts_JSON);

			// add new timestamp
			cJSON_AddStringToObject(payload_JSON, "ts", ts_str);
			// add mission number
			cJSON_AddNumberToObject(payload_JSON, "M", glider.mission_param.mission);

			// convert JSON to string
			uint8_t payload_4G[512] = "";
			strcpy(payload_4G, cJSON_Print(payload_JSON));
			cJSON_Minify(payload_4G);

			// publish through 4G
			// printk("\npayload: %s\n", payload_4G);
			gcloud_publish(payload_4G, strlen(payload_4G), MQTT_QOS_1_AT_LEAST_ONCE);

			msg_sent++;
		}
		else
		{
			printk("sd message is not valid JSON");
		}
		cJSON_Delete(payload_JSON);

		if (msg_sent == msg_max || strcmp(sd_msg_response, "EOF") == 0)
		{
			break;
		}
	}

	printk("\nAll messages sent");

	return 0;
}

static int parse_mission_params(void *json_str)
{
	cJSON *tmp_JSON;
	cJSON *tmpsub_JSON;
	cJSON *mqtt_JSON = cJSON_Parse(json_str);
	if (cJSON_IsObject(mqtt_JSON))
	{
		bool new_mission = false;

		// parameter mission number
		if (cJSON_HasObjectItem(mqtt_JSON, "M"))
		{
			tmp_JSON = cJSON_GetObjectItem(mqtt_JSON, "M");

			// test if mission received is new
			if (glider.mission_param.mission < tmp_JSON->valueint)
			{
				printk("\nnew mission received");
				glider.mission_param.mission = tmp_JSON->valueint;
				new_mission = true;
			}
			else
			{
				printk("\nnot a new mission");
				return 1;
			}
		}

		// copy mission params if mission is new and save to SD card
		if (new_mission)
		{
			// waypoints - lat
			if (cJSON_HasObjectItem(mqtt_JSON, "lat"))
			{
				tmp_JSON = cJSON_GetObjectItem(mqtt_JSON, "lat");
				for (int i = 0; i < cJSON_GetArraySize(tmp_JSON); i++)
				{
					tmpsub_JSON = cJSON_GetArrayItem(tmp_JSON, i);
					glider.mission_param.wp_arr[i][0] = tmpsub_JSON->valuedouble;
					// printk("\nlat: %lf", glider.mission_param.wp_arr[i][0]);

					glider.mission_param.wp_max = i;
				}
			}

			// waypoints - lng
			if (cJSON_HasObjectItem(mqtt_JSON, "lng"))
			{
				tmp_JSON = cJSON_GetObjectItem(mqtt_JSON, "lng");
				for (int i = 0; i < cJSON_GetArraySize(tmp_JSON); i++)
				{
					tmpsub_JSON = cJSON_GetArrayItem(tmp_JSON, i);
					glider.mission_param.wp_arr[i][1] = tmpsub_JSON->valuedouble;
					// printk("\nlng: %lf", glider.mission_param.wp_arr[i][1]);
				}
			}

			// parameter max mqtt message count
			if (cJSON_HasObjectItem(mqtt_JSON, "4G"))
			{
				tmp_JSON = cJSON_GetObjectItem(mqtt_JSON, "4G");
				glider.mission_param.msg_max = tmp_JSON->valueint;
			}

			// parameter sensor frequencies
			if (cJSON_HasObjectItem(mqtt_JSON, "C"))
			{
				tmp_JSON = cJSON_GetObjectItem(mqtt_JSON, "C");
				glider.mission_param.sens_param.freq_c = tmp_JSON->valueint;
			}
			if (cJSON_HasObjectItem(mqtt_JSON, "P"))
			{
				tmp_JSON = cJSON_GetObjectItem(mqtt_JSON, "P");
				glider.mission_param.sens_param.freq_p = tmp_JSON->valueint;
			}
			if (cJSON_HasObjectItem(mqtt_JSON, "T"))
			{
				tmp_JSON = cJSON_GetObjectItem(mqtt_JSON, "T");
				glider.mission_param.sens_param.freq_t = tmp_JSON->valueint;
			}

			// TODO: parse start time

			sd_msg_t sd_msg;

			strcpy(sd_msg.filename, file_mission);
			printk("\n%s\n", sd_msg.filename);

			sd_msg.event = OVERWRITE_FILE;

			strcpy(sd_msg.string, cJSON_Print(mqtt_JSON));
			cJSON_Minify(sd_msg.string);

			k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);

			cJSON_Delete(mqtt_JSON);
		}
	}
	else
	{
		printk("\nnot JSON");
		cJSON_Delete(mqtt_JSON);

		return -1;
	}

	return 0;
}

/* MODULES */

static int sensor_module()
{
	printk("\n\npress button 1 to start sensor test\n\n");
	button_wait();

	set_button_config(1);
	uart_start(uart_dev1);

	// send time to sensors
	uint8_t time_millis_str[16] = "";
	uint32_t time_millis = ((current_hour * 60 * 60) + (current_minute * 60) + current_seconds) * 1000;
	snprintf(time_millis_str, sizeof(time_millis_str), "time:%u", time_millis);

	uart_send(uart_dev1, time_millis_str, strlen(time_millis_str));

	printk("\nsensor test start\n");

	printk("button 1: exit \nbutton 2: send uart signal\n\n");

	/* UART loop */
	for (int i = 0;; i++)
	{

		// memset(uart_msg, 0, sizeof(uart_msg));
		k_msgq_get(&uart_msg_q, &uart_msg, K_FOREVER);

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

			if (cJSON_IsObject(sensor_JSON))
			{
				// fetching timestamp (ms) to convert to hh:mm:ss format
				cJSON *ts_raw = cJSON_DetachItemFromObject(sensor_JSON, "ts");
				uint32_t tmp_int = (ts_raw->valueint) / 1000;
				cJSON_Delete(ts_raw);

				// converting
				int hour = tmp_int / 3600 % 24;
				int min = (tmp_int / 60) % 60;
				int sec = tmp_int % 60;

				// variables for holding timestamp (ts) and datetime (dt)
				uint8_t ts_string[64] = "";
				uint8_t date_string[64] = "";
				uint8_t temp_str[16] = "";

				// adding date to JSON
				snprintf(temp_str, sizeof(temp_str), "%02u-", current_year);
				strcat(date_string, temp_str);
				snprintf(temp_str, sizeof(temp_str), "%02u-", current_month);
				strcat(date_string, temp_str);
				snprintf(temp_str, sizeof(temp_str), "%02u", current_day);
				strcat(date_string, temp_str);

				cJSON_AddStringToObject(sensor_JSON, "dt", date_string);

				// adding timestamp to JSON
				snprintf(temp_str, sizeof(temp_str), "%02d:", hour);
				strcat(ts_string, temp_str);
				snprintf(temp_str, sizeof(temp_str), "%02d:", min);
				strcat(ts_string, temp_str);
				snprintf(temp_str, sizeof(temp_str), "%02d", sec);
				strcat(ts_string, temp_str);

				cJSON_AddStringToObject(sensor_JSON, "ts", ts_string);

				// convert to string
				uint8_t sensor_str[256] = "";
				strcpy(sensor_str, cJSON_Print(sensor_JSON));
				cJSON_Minify(sensor_str);

				// save to sd card
				sd_save_data(sensor_str);
				data_available_to_send = true;
			}

			cJSON_Delete(sensor_JSON);
		}
	}
	/* UART loop end */

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

	uart_send(UART_2, "wifi_start", strlen("wifi_start"));
	printk("\nsent command wifi start to esp");

	while (1)
	{
		k_msgq_get(&uart_msg_q, wifi_response, K_FOREVER);
		printk("\n%s", wifi_response);
		//button_wait();

		//strcpy(wifi_response, "{D:20202020.txt}");

		if (strcmp(wifi_response, "connected") == 0)
		{
			uint8_t sd_msg_response[512] = "";

			sd_msg_t sd_msg;
			sd_msg.event = READ_JSON;
			strcpy(sd_msg.filename, file_mission);

			k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);

			k_msgq_get(&main_msg_q, &sd_msg_response, K_FOREVER);

			if (strcmp(sd_msg_response, "ERROR") != 0)
			{
				printk("\n%s", sd_msg_response);
				// uart_send(UART_2, sd_msg_response, strlen(sd_msg_response));
			}

			sd_msg.event = SEND_FILE_INFO;
			k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);

			printk("\n\nConnect successful, press button 1 to continue");
			// strcpy(wifi_response, "read1");
		}
		else if (wifi_response[0] == 'D')
		{
			sd_msg_t sd_msg;

			// fetch filename
			// format yyyymmdd.txt
			char *date = wifi_response + 2;
			date[strlen(date)] = 0;

			strcpy(sd_msg.filename, date);
			printk("\n%s", sd_msg.filename);

			sd_msg.event = READ_FILE;

			printk("\nStarting SD file read2");
			k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);
		}
		else if (strcmp(wifi_response, "wifi_end") == 0)
		{
			printk("\nReceived wifi end command\n");
			// uart_send(UART_2, "wifi_end;", strlen("wifi_end;"));
			break;
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
	static glider_gps_data_t gps_data;

	printk("\n\npress button 1 to start gps test\n\n");
	button_wait();
	printk("\ngps test start");

	app_gps(&gps_data, 1000, 400);

	// update date CANDO: create function?
	current_year = gps_data.year;
	current_month = gps_data.month;
	current_day = gps_data.day;

	current_hour = gps_data.hour;
	current_minute = gps_data.minute;
	current_seconds = gps_data.seconds;

	if (strcmp(gps_data.gps_string, "no fix") != 0)
	{
		// save gps data on sd card
		sd_save_data(gps_data.gps_string);
		// add to gps message queue
		k_msgq_put(&gps_msg_q, &gps_data.gps_string, K_NO_WAIT);

		data_available_to_send = true;
	}

	printk("\ngps test end");

	return 0;
}

static int gcloud_module()
{
	printk("\n\npress button 1 to start gcloud test\n\n");
	button_wait();
	printk("gcloud test start\n");

	// publish_data_4G();

	int err;
	int ret = -1;

	// strcpy(gcloud_msg, "{\"M\":9,\"4G\":10,\"C\":0,\"P\":0,\"T\":0,\"lat\":[51.7558,60.7558],\"lng\":[19.2726,21.2726],\"maxD\":0,\"minD\":0,\"start\":\"202104210505\"}");

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

	// /* Gcloud loop */
	while (1)
	{
		// button_wait();

		// gcloud function
		err = app_gcloud();

		k_msgq_get(&gcloud_msg_q, &gcloud_msg, K_NO_WAIT);
		// printk("\nMain: %s", gcloud_msg);

		ret = parse_mission_params(&gcloud_msg);

		if (ret == 0)
		{
			break;
		}

		if (data_available_to_send)
		{
			publish_data_4G();
			data_available_to_send = false;
		}
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

	// uart_start(uart_dev1);

	// uart_send(uart_dev1, "AT\r", strlen("AT\r"));
	// uart_send(uart_dev1, "AT+CGSN\r", strlen("AT+CGSN\r"));

	// uart_exit(uart_dev1);

	if (k_msgq_num_used_get(&gps_msg_q) != 0)
	{
		cJSON *gps_JSON;
		cJSON *data_JSON;
		cJSON *lat_JSON;
		cJSON *lng_JSON;

		k_msgq_get(&gps_msg_q, &gps_msg, K_NO_WAIT);

		// printk("\n%s", gps_msg);
		gps_JSON = cJSON_Parse(gps_msg);
		if (cJSON_IsObject(gps_JSON))
		{
			data_JSON = cJSON_GetObjectItem(gps_JSON, "data");
			lat_JSON = cJSON_GetObjectItem(data_JSON, "lat");
			lng_JSON = cJSON_GetObjectItem(data_JSON, "lng");

			uint8_t sat_str[50] = "";
			uint8_t tmp_str[16] = "";

			snprintf(tmp_str, sizeof(tmp_str), "%.4f,", lat_JSON->valuedouble);
			strcat(sat_str, tmp_str);
			snprintf(tmp_str, sizeof(tmp_str), "%.4f", lng_JSON->valuedouble);
			strcat(sat_str, tmp_str);

			// send to satellite module
			printk("\n%s", sat_str);
		}
		cJSON_Delete(gps_JSON);
	}
	else
	{
		printk("\nNo gps message");
	}

	// test if any messages were sent to satellite

	return 0;
}

static int glider_init()
{

	/* Fetch serial number (uid) */
	uint8_t imei[18] = "";
	uint8_t uid[18] = "";
	at_cmd_write("AT+CGSN", imei, sizeof(imei), NULL);
	memcpy(uid, imei + 8, 6);
	strcpy(glider.uid, uid);
	// glider.uid[sizeof(glider.uid) - 1] = 0;

	// initialize values for glider details, TODO: Initialize other parameters
	glider.mission_param.mission = 0;
	glider.mission_param.wp_cur = 1;
	glider.mission_param.wp_max = 5;

	// printk("\n%s\n", uid);
	printk("\n%s\n", glider.uid);

	/* device inits and configurations */
	device_inits();

	// message queues
	message_queue_init();

	// sd card thread
	sd_tid = k_thread_create(&sd_thread, sd_stack_area, K_THREAD_STACK_SIZEOF(sd_stack_area),
							 (k_thread_entry_t)app_sd_thread, NULL, NULL, NULL,
							 7, 0, K_NO_WAIT);

	glider.mission_started = false;

	printk("Init complete. Press button 1 to start\n\n");

	return 0;
}

static bool check_new_glider()
{
	static sd_msg_t sd_msg;
	sd_msg.event = FIND_FILE;
	strcpy(sd_msg.filename, file_mission);

	k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);

	int sd_msg_response = 0;

	k_msgq_get(&main_msg_q, &sd_msg_response, K_FOREVER);

	if (sd_msg_response)
	{
		printk("Mission params found\n");

		uint8_t json_str[512] = "";

		sd_msg.event = READ_JSON;

		k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);

		k_msgq_get(&main_msg_q, &json_str, K_FOREVER);
		printk("\nparams: %s", json_str);
		if (strcmp(json_str, "ERROR") != 0)
		{
			parse_mission_params(&json_str);
		}

		return true;
	}

	printk("Mission params do not exist\n");
	return false;
}

/* MAIN */

void main(void)
{
	printk("\n\n**** NordicOasys v0.99 - Started ****\n\n");

	enum glider_event_type event_now = EVT_INIT;
	enum glider_event_type event_prev = EVT_INIT;

	while (1)
	{
		message_queue_reset();

		switch (event_now)
		{
		case EVT_INIT:
			// initialize glider
			glider_init();

			bool newglider = check_new_glider();

			// get gps fix
			// set_LED(22, 1);
			// gps_module();
			// set_LED(22, 0);

			event_prev = event_now;
			event_now = EVT_DIVE;

			printk("\nglider event current: %d", event_now);

			break;
		case EVT_AWAIT_MISSION:
			// turn on wifi
			set_LED(30, 1);
			wifi_module();
			set_LED(30, 0);

			event_prev = event_now;
			event_now = EVT_AWAIT_MISSION;

			break;
		case EVT_DIVE:
			// function: send command to navigation

			// read sensor data
			set_LED(20, 1);
			sensor_module();
			set_LED(20, 0);

			event_prev = event_now;
			event_now = EVT_SURFACE;

			break;
		case EVT_SURFACE:

			set_LED(22, 1);
			gps_module();
			set_LED(22, 0);

			set_LED(31, 1);
			data_available_to_send = true;
			gcloud_module();
			set_LED(31, 0);

			set_LED(30, 1);
			set_LED(31, 1);
			satellite_module();
			set_LED(30, 0);
			set_LED(31, 0);

			event_prev = event_now;
			event_now = EVT_DIVE;

			break;
		case EVT_IDLE:
			set_LED(22, 1);
			set_LED(20, 1);
			set_LED(30, 1);
			set_LED(31, 1);
			LOG_INF("Idling");
			k_sleep(K_SECONDS(30));
			set_LED(22, 0);
			set_LED(20, 0);
			set_LED(30, 0);
			set_LED(31, 0);

			// logic for test start time, if true start mission

			event_prev = event_now;
			event_now = EVT_IDLE;

			break;
		default:
			LOG_ERR("Unknown event type");

			break;
		}
	}

	printk("**** NordicOasys test end ****\n");
}

/* Function ideas and TODOs

- Save messages received from MQTT into array similar to data.

- FIX FORMATTING OF DATA:
	DONE:
		gps data
		sensor data
		formatting on sd card (should not be necessary and should only copy string directly)
	WIP:

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

// TODO: change to general data_array instead of just uart data
static uint8_t uart_data_array[16][128] = {""};
int data_size = 0; // keeps track of no. of messages stored in data array.
				   // UPDATE THIS VALUE WHEN SAVING INTO DATA ARRAY

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
