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
#include <cJSON.h>
#include <time.h>
#include <date_time.h>

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

// --------------------------------------------------
// STRUCTS AND ENUMS
// --------------------------------------------------

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
	int msg_sent;
	int msg_max;
	uint8_t unsent_data_files[10][32]; // TODO: check if this is necessary
	uint8_t wp_arr[10][2][16];
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

// --------------------------------------------------
// VARIABLE DECLARATIONS
// --------------------------------------------------

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
static int current_year = 0;
static int current_month = 0;
static int current_day = 0;

// TEMPORARY, FOR TESTING
static int current_hour = 0;
static int current_minute = 0;
static int current_seconds = 0;

// --------------------------------------------------
// FUNCTION DECLARATIONS
// --------------------------------------------------

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

	// LOG_INF("saving to sd card");

	// create filename, format (assuming mission 2): [M2.TXT]
	uint8_t filename[16] = "";
	uint8_t tmp_str[16] = "";

	// snprintf(tmp_str, sizeof(tmp_str), "%u", glider.mission_param.mission);
	// strcat(filename, tmp_str);

	snprintf(tmp_str, sizeof(tmp_str), "M%u.TXT", glider.mission_param.mission);
	strcat(filename, tmp_str);

	// copy string over to struct
	strcpy(sd_msg.filename, filename);
	//sd_msg.filename[strlen(filename)] = 0;
	//LOG_INF("%s", log_strdup(sd_msg.filename));

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

static int publish_data_4G()
{
	// printk("\nRunning test, press button 1 to start\n");
	// button_wait();

	k_msgq_purge(&main_msg_q);

	// separate from mission_param msg_sent, this only keeps track locally
	int msg_sent = 0;

	sd_msg_t sd_msg;
	uint8_t tmp_str[16] = "";

	sd_msg.event = READ_JSON_4G;

	snprintf(tmp_str, sizeof(tmp_str), "M%d.TXT", glider.mission_param.mission);
	strcpy(sd_msg.filename, tmp_str);

	int msg_to_send = glider.mission_param.msg_max / glider.mission_param.wp_max;

	snprintf(tmp_str, sizeof(tmp_str), "max:%d\r", msg_to_send);
	strcat(sd_msg.string, tmp_str);
	snprintf(tmp_str, sizeof(tmp_str), "wp_cur:%d\r", glider.mission_param.wp_cur);
	strcat(sd_msg.string, tmp_str);

	// LOG_INF("%s", log_strdup(sd_msg.string));
	k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);

	uint8_t sd_msg_response[512] = "";

	while (1)
	{
		k_msgq_get(&main_msg_q, &sd_msg_response, K_FOREVER);

		// LOG_INF("%s", log_strdup(sd_msg_response));
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
			LOG_INF("payload: %s", log_strdup(payload_4G));
			gcloud_publish(payload_4G, strlen(payload_4G), MQTT_QOS_1_AT_LEAST_ONCE);

			msg_sent++;
			glider.mission_param.msg_sent++;
		}
		else
		{
			LOG_INF("sd message is not JSON");
		}
		cJSON_Delete(payload_JSON);

		if (msg_sent == msg_to_send || strcmp(sd_msg_response, "EOM") == 0)
		{
			gcloud_publish("{\"M\":5,\"data\":{\"lat\":\"62.4444\",\"lng\":\"2.4444\"}, \"ts\":\"20210506123456\"}",
						   strlen("{\"M\":5,\"data\":{\"lat\":\"62.4444\",\"lng\":\"2.4444\"}, \"ts\":\"20210506123456\"}"),
						   MQTT_QOS_1_AT_LEAST_ONCE);
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
			if (cJSON_IsNumber(tmp_JSON))
			{
				// test if mission received is new
				if (glider.mission_param.mission < tmp_JSON->valueint)
				{
					glider.mission_param.mission = tmp_JSON->valueint;
					new_mission = true;
				}
				else
				{
					LOG_INF("not a new mission");
					return 1;
				}
			}
			else
			{
				LOG_ERR("Mission needs to be number");
			}
		}

		// copy mission params if mission is new and save to SD card
		if (new_mission)
		{
			// resetting necessary parameters
			glider.mission_param.wp_cur = 0;
			glider.mission_param.msg_sent = 0;

			// waypoints - lat
			if (cJSON_HasObjectItem(mqtt_JSON, "lat"))
			{

				tmp_JSON = cJSON_GetObjectItem(mqtt_JSON, "lat");
				if (cJSON_IsArray(tmp_JSON))
				{
					glider.mission_param.wp_max = cJSON_GetArraySize(tmp_JSON);

					for (int i = 0; i < glider.mission_param.wp_max; i++)
					{
						tmpsub_JSON = cJSON_GetArrayItem(tmp_JSON, i);
						if (cJSON_IsString(tmpsub_JSON))
						{
							strcpy(glider.mission_param.wp_arr[i][0], tmpsub_JSON->valuestring);
							// LOG_INF("lng: %lf", glider.mission_param.wp_arr[i][1]);
						}
						else
						{
							LOG_ERR("lat value needs to be string");
						}
					}
				}
				else
				{
					LOG_ERR("lat needs to be array");
				}
			}

			// waypoints - lng
			if (cJSON_HasObjectItem(mqtt_JSON, "lng"))
			{
				tmp_JSON = cJSON_GetObjectItem(mqtt_JSON, "lng");
				if (cJSON_IsArray(tmp_JSON))
				{
					for (int i = 0; i < glider.mission_param.wp_max; i++)
					{
						tmpsub_JSON = cJSON_GetArrayItem(tmp_JSON, i);
						if (cJSON_IsString(tmpsub_JSON))
						{
							strcpy(glider.mission_param.wp_arr[i][1], tmpsub_JSON->valuestring);
							// LOG_INF("lng: %lf", glider.mission_param.wp_arr[i][1]);
						}
						else
						{
							LOG_ERR("lng value needs to be string");
						}
					}
				}
				else
				{
					LOG_ERR("lng needs to be array");
				}
			}

			// parameter max mqtt message count
			if (cJSON_HasObjectItem(mqtt_JSON, "4G"))
			{
				tmp_JSON = cJSON_GetObjectItem(mqtt_JSON, "4G");
				if (cJSON_IsNumber(tmp_JSON))
				{
					glider.mission_param.msg_max = tmp_JSON->valueint;
				}
				else
				{
					LOG_ERR("4G needs to be number");
				}
			}

			// parameter start time TODO: start into string instead of numbers
			if (cJSON_HasObjectItem(mqtt_JSON, "start"))
			{
				tmp_JSON = cJSON_GetObjectItem(mqtt_JSON, "start");
				if (cJSON_IsString(tmp_JSON))
				{
					uint8_t tmp_str[16] = "";

					memcpy(tmp_str, tmp_JSON->valuestring, 4);
					LOG_INF("year: %s", log_strdup(tmp_str));
					memset(tmp_str, 0, sizeof(tmp_str));

					memcpy(tmp_str, tmp_JSON->valuestring + 4, 2);
					LOG_INF("month: %s", log_strdup(tmp_str));
					memset(tmp_str, 0, sizeof(tmp_str));

					memcpy(tmp_str, tmp_JSON->valuestring + 6, 2);
					LOG_INF("day: %s", log_strdup(tmp_str));
					memset(tmp_str, 0, sizeof(tmp_str));

					memcpy(tmp_str, tmp_JSON->valuestring + 8, 2);
					LOG_INF("hour: %s", log_strdup(tmp_str));
					memset(tmp_str, 0, sizeof(tmp_str));

					memcpy(tmp_str, tmp_JSON->valuestring + 10, 2);
					LOG_INF("minute: %s", log_strdup(tmp_str));
					memset(tmp_str, 0, sizeof(tmp_str));
				}
				else
				{
					LOG_ERR("start needs to be string");
				}
			}

			// parameter max depth
			if (cJSON_HasObjectItem(mqtt_JSON, "maxD"))
			{
				tmp_JSON = cJSON_GetObjectItem(mqtt_JSON, "maxD");
				if (cJSON_IsNumber(tmp_JSON))
				{
					glider.mission_param.depth_max = tmp_JSON->valueint;
				}
				else
				{
					LOG_ERR("maxD needs to be number");
				}
			}

			// parameter min depth
			if (cJSON_HasObjectItem(mqtt_JSON, "minD"))
			{
				tmp_JSON = cJSON_GetObjectItem(mqtt_JSON, "minD");
				if (cJSON_IsNumber(tmp_JSON))
				{
					glider.mission_param.depth_min = tmp_JSON->valueint;
				}
				else
				{
					LOG_ERR("minD needs to be number");
				}
			}

			// parameter sensor frequencies
			if (cJSON_HasObjectItem(mqtt_JSON, "C"))
			{
				tmp_JSON = cJSON_GetObjectItem(mqtt_JSON, "C");
				if (cJSON_IsNumber(tmp_JSON))
				{
					glider.mission_param.sens_param.freq_c = tmp_JSON->valueint;
				}
				else
				{
					LOG_ERR("C needs to be number");
				}
			}
			if (cJSON_HasObjectItem(mqtt_JSON, "P"))
			{
				tmp_JSON = cJSON_GetObjectItem(mqtt_JSON, "P");
				if (cJSON_IsNumber(tmp_JSON))
				{
					glider.mission_param.sens_param.freq_p = tmp_JSON->valueint;
				}
				else
				{
					LOG_ERR("P needs to be number");
				}
			}
			if (cJSON_HasObjectItem(mqtt_JSON, "T"))
			{
				tmp_JSON = cJSON_GetObjectItem(mqtt_JSON, "T");
				if (cJSON_IsNumber(tmp_JSON))
				{
					glider.mission_param.sens_param.freq_t = tmp_JSON->valueint;
				}
				else
				{
					LOG_ERR("T needs to be number");
				}
			}

			sd_msg_t sd_msg;

			strcpy(sd_msg.filename, file_mission);

			sd_msg.event = OVERWRITE_FILE;

			strcpy(sd_msg.string, cJSON_Print(mqtt_JSON));
			cJSON_Minify(sd_msg.string);

			k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);

			cJSON_Delete(mqtt_JSON);
		}
	}
	else
	{
		LOG_ERR("not a JSON, could not parse mission parameters");
		cJSON_Delete(mqtt_JSON);

		return -1;
	}

	return 0;
}

static bool check_mission()
{
	static sd_msg_t sd_msg;
	sd_msg.event = FIND_FILE;
	strcpy(sd_msg.filename, file_mission);

	k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);

	int sd_msg_response = 0;

	k_msgq_get(&main_msg_q, &sd_msg_response, K_FOREVER);

	if (sd_msg_response)
	{
		LOG_INF("Mission params found");

		uint8_t json_str[512] = "";
		sd_msg.event = READ_JSON;
		k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);

		k_msgq_get(&main_msg_q, &json_str, K_FOREVER);

		if (strcmp(json_str, "ERROR") != 0)
		{
			parse_mission_params(&json_str);

			LOG_INF("Current parameters:");
			LOG_INF("Mission: %d", glider.mission_param.mission);
			LOG_INF("Max depth: %d", glider.mission_param.depth_max);
			LOG_INF("Min depth: %d", glider.mission_param.depth_min);
			LOG_INF("Message max: %d", glider.mission_param.msg_max);
			LOG_INF("Waypoints: %d", glider.mission_param.wp_max);

			LOG_INF("Sensor parameters:");
			LOG_INF("C: %d", glider.mission_param.sens_param.freq_c);
			LOG_INF("P: %d", glider.mission_param.sens_param.freq_p);
			LOG_INF("T: %d", glider.mission_param.sens_param.freq_t);
		}

		return true;
	}

	LOG_INF("Mission params do not exist");
	return false;
}

// --------------------------------------------------
// MODULES
// --------------------------------------------------

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

	uint8_t tmp_str[16] = "";
	snprintf(tmp_str, sizeof(tmp_str), "to_c:%d", glider.mission_param.sens_param.freq_c);
	uart_send(uart_dev1, tmp_str, strlen(tmp_str));
	snprintf(tmp_str, sizeof(tmp_str), "to_p:%d", glider.mission_param.sens_param.freq_p);
	uart_send(uart_dev1, tmp_str, strlen(tmp_str));
	snprintf(tmp_str, sizeof(tmp_str), "to_t:%d", glider.mission_param.sens_param.freq_t);
	uart_send(uart_dev1, tmp_str, strlen(tmp_str));

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
			LOG_INF("%s", log_strdup(uart_msg));

			// TODO: test timestamp to check if new day

			cJSON *sensor_JSON = cJSON_Parse(uart_msg);

			if (cJSON_IsObject(sensor_JSON))
			{
				// fetching timestamp (ms) to convert to hh:mm:ss format
				cJSON *ts_raw = cJSON_DetachItemFromObject(sensor_JSON, "ts");
				uint32_t tmp_int = (ts_raw->valuedouble) / 1000;
				cJSON_Delete(ts_raw);
				LOG_INF("tmp int: %u", tmp_int);

				// converting
				uint16_t hour = tmp_int / 3600 % 24;
				LOG_INF("hour: %u", hour);

				uint16_t min = (tmp_int / 60) % 60;
				LOG_INF("min: %u", min);

				uint16_t sec = tmp_int % 60;
				LOG_INF("sec: %u", sec);

				// variables for holding timestamp (ts) and datetime (dt)
				uint8_t ts_string[64] = "";
				uint8_t date_string[64] = "";
				uint8_t tmp_str[16] = "";

				// adding date to JSON
				snprintf(tmp_str, sizeof(tmp_str), "%02u-", current_year);
				strcat(date_string, tmp_str);
				snprintf(tmp_str, sizeof(tmp_str), "%02u-", current_month);
				strcat(date_string, tmp_str);
				snprintf(tmp_str, sizeof(tmp_str), "%02u", current_day);
				strcat(date_string, tmp_str);

				cJSON_AddStringToObject(sensor_JSON, "dt", date_string);

				// adding timestamp to JSON
				snprintf(tmp_str, sizeof(tmp_str), "%02d:", hour);
				strcat(ts_string, tmp_str);
				snprintf(tmp_str, sizeof(tmp_str), "%02d:", min);
				strcat(ts_string, tmp_str);
				snprintf(tmp_str, sizeof(tmp_str), "%02d", sec);
				strcat(ts_string, tmp_str);

				LOG_INF("%s", log_strdup(ts_string));
				LOG_INF("%s", log_strdup(date_string));

				cJSON_AddStringToObject(sensor_JSON, "ts", ts_string);

				// convert to string
				uint8_t sensor_str[256] = "";
				strcpy(sensor_str, cJSON_Print(sensor_JSON));
				cJSON_Minify(sensor_str);

				// save to sd card
				sd_save_data(sensor_str);
				data_available_to_send = true;
			}
			else
			{
				LOG_INF("not json object");
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
	LOG_INF("sent command wifi start to esp");

	while (1)
	{
		k_msgq_get(&uart_msg_q, wifi_response, K_FOREVER);
		LOG_INF("%s", log_strdup(wifi_response));
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
				LOG_INF("%s", log_strdup(sd_msg_response));
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
			LOG_INF("%s", log_strdup(sd_msg.filename));

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

static int gps_module()
{
	static glider_gps_data_t gps_data;

	// printk("\n\npress button 1 to start gps test\n\n");
	// button_wait();
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
		LOG_INF("%s", log_strdup(gps_data.gps_string));
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

	memset(gcloud_msg, 0, sizeof(gcloud_msg));

	// publish_data_4G();

	int ret = -1;

	// strcpy(gcloud_msg, "{\"M\":11,\"4G\":10,\"C\":0,\"P\":0,\"T\":0,\"lat\":[\"51.7558\",\"60.7558\"],\"lng\":[\"19.2726\",\"21.2726\"],\"maxD\":0,\"minD\":0,\"start\":\"202104280559\"}");

	// mqtt init
	if (!gcloud_init_complete)
	{
		ret = app_gcloud_init_and_connect();
		if (ret != 0)
		{
			return ret;
		}
		gcloud_init_complete = true;
	}
	// reconnect
	else
	{
		ret = app_gcloud_reconnect();
		if (ret != 0)
		{
			return ret;
		}
	}

	// /* Gcloud loop */
	while (1)
	{
		// button_wait();

		// gcloud function
		ret = app_gcloud();

		k_msgq_get(&gcloud_msg_q, &gcloud_msg, K_NO_WAIT);
		LOG_INF("gcloud string: %s", log_strdup(gcloud_msg));

		ret = parse_mission_params(&gcloud_msg);

		if (ret >= 0)
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

	ret = app_gcloud_disconnect();
	if (ret != 0)
	{
		LOG_ERR("Disconnect unsuccessful: %d", ret);
		LOG_ERR("Closing program, check for errors in code lol");
		return ret;
	}
	return 0;
}

static int sat_check_signal()
{
	int signal = 0;

	uint8_t sat_msg[64] = "";
	uint8_t sat_response[64] = "";
	uint8_t sat_response_test[64] = "";

	// send message
	strcpy(sat_msg, "AT+CSQ");
	uart_send(uart_dev1, sat_msg, strlen(sat_msg));

	// test response
	while (1)
	{
		// fetch from uart
		k_msgq_get(&uart_msg_q, &sat_response_test, K_FOREVER);

		// break on OK
		if (strcmp(sat_response_test, "OK") == 0)
		{
			break;
		}
		// end function on error
		else if (strcmp(sat_response_test, "ERROR") == 0)
		{
			LOG_ERR("satellite serial error, check connection/serial msg sent");
			return -1;
		}
		// copy response if valid
		else if (strcmp(sat_response_test, sat_msg) != 0 && strlen(sat_response_test) != 0)
		{
			strcpy(sat_response, sat_response_test);
		}
	}
	// clear response and any pending messages from satellite
	memset(sat_response_test, 0, sizeof(sat_response_test));
	k_msgq_purge(&uart_msg_q);

	// LOG_DBG("sat msg: %s", log_strdup(sat_response));

	static char *eptr;
	signal = strtol(sat_response + 5, &eptr, 10);
	LOG_INF("signal: %d", signal);

	return signal;
}

static int sat_send_msg(uint8_t *sat_msg)
{
	int sat_sbd_response[6];
	uint8_t sat_response[64] = "";
	uint8_t sat_response_test[64] = "";

	LOG_INF("sat msg: %s", log_strdup(sat_msg));

	// send payload
	uart_send(uart_dev1, sat_msg, strlen(sat_msg));

	// test response
	while (1)
	{
		// fetch from uart
		k_msgq_get(&uart_msg_q, &sat_response_test, K_FOREVER);

		// break on OK
		if (strcmp(sat_response_test, "OK") == 0)
		{
			break;
		}
		// end function on error
		else if (strcmp(sat_response_test, "ERROR") == 0)
		{
			LOG_ERR("satellite serial error, check connection/serial msg sent");
			return -1;
		}
		// copy response if valid
		else if (strcmp(sat_response_test, sat_msg) != 0 && strlen(sat_response_test) != 0)
		{
			strcpy(sat_response, sat_response_test);
		}
	}
	// clear response and any pending messages from satellite
	memset(sat_response_test, 0, sizeof(sat_response_test));
	k_msgq_purge(&uart_msg_q);

	// turn on SBD session (send/receive satellite messages)
	uart_send(uart_dev1, "AT+SBDI", strlen("AT+SBDI"));

	// test response
	while (1)
	{
		// fetch from uart
		k_msgq_get(&uart_msg_q, &sat_response_test, K_FOREVER);

		// break on OK
		if (strcmp(sat_response_test, "OK") == 0)
		{
			break;
		}
		// end function on error
		else if (strcmp(sat_response_test, "ERROR") == 0)
		{
			LOG_ERR("satellite serial error, check connection/serial msg sent");
			return -1;
		}
		// copy response if valid
		else if (strcmp(sat_response_test, sat_msg) != 0 && strlen(sat_response_test) != 0)
		{
			strcpy(sat_response, sat_response_test);
		}
	}
	// clear response and any pending messages from satellite
	memset(sat_response_test, 0, sizeof(sat_response_test));
	k_msgq_purge(&uart_msg_q);

	LOG_INF("response: %s", log_strdup(sat_response));

	return 0;
}

static int satellite_module()
{
	printk("\n\npress button 1 to start satellite test\n\n");
	button_wait();

	int ret = 0;
	int cnt = 0;
	int max_retries = 10;
	bool fix = false;
	bool msg_rdy = false;
	bool msg_sent = false;

	uint8_t sat_msg[64] = "";
	uint8_t sat_response[64] = "";
	uint8_t sat_response_test[64] = "";

	// start uart
	uart_start(uart_dev1);

	cnt = 0;
	while (!(cnt >= max_retries) && !msg_sent)
	{ // finding signal
		int signal = sat_check_signal();

		if (signal)
		{ // if signal found
			fix = true;
			LOG_INF("got signal");

			if (!msg_rdy)
			{
				// if no valid gps data, attempt to get fix
				if (k_msgq_num_used_get(&gps_msg_q) == 0)
				{
					LOG_INF("No gps message, attempting to get gps fix");
					gps_module();
				}

				cJSON *gps_JSON;
				cJSON *data_JSON;
				cJSON *lat_JSON;
				cJSON *lng_JSON;

				// fetch gps message to be sent
				k_msgq_get(&gps_msg_q, &gps_msg, K_NO_WAIT);

				// LOG_DBG("%s", log_strdup(gps_msg));
				gps_JSON = cJSON_Parse(gps_msg);
				if (cJSON_IsObject(gps_JSON))
				{
					data_JSON = cJSON_GetObjectItem(gps_JSON, "data");
					lat_JSON = cJSON_GetObjectItem(data_JSON, "lat");
					lng_JSON = cJSON_GetObjectItem(data_JSON, "lng");

					memset(sat_msg, 0, sizeof(sat_msg));
					strcpy(sat_msg, "AT+SBDWT=");
					strcat(sat_msg, lat_JSON->valuestring);
					strcat(sat_msg, ",");
					strcat(sat_msg, lng_JSON->valuestring);
					msg_rdy = true;
				}
				else
				{
					LOG_ERR("gps data not in JSON format");
					return -1;
				}
				cJSON_Delete(gps_JSON);
			}

			LOG_INF("attempting send");
			ret = sat_send_msg(sat_msg);
			if (ret == 0)
			{
				LOG_INF("satellite msg sent");
				msg_sent = true;
			}
			else if (ret < 0)
			{
				LOG_ERR("Error in sending satellite message");
				return -1;
			}

		} // if signal found
		else
		{
			cnt++;
			k_sleep(K_SECONDS(20));
		}
	} // finding signal end
	if (!fix)
	{
		LOG_INF("Unable to get satellite fix");
		ret = -1;
	}

	uart_exit(uart_dev1);

	// test if any messages were sent to satellite

	return ret;
}

static int glider_init()
{

	/* Fetch serial number (uid) */
	uint8_t imei[18] = "";
	uint8_t uid[18] = "";
	at_cmd_write("AT+CGSN", imei, sizeof(imei), NULL);
	memcpy(uid, imei + 8, 6);
	strcpy(glider.uid, uid);

	LOG_INF("UID: %s", log_strdup(glider.uid));

	// initialize values for glider details, TODO: Initialize other parameters
	glider.mission_param.mission = 0;
	glider.mission_param.wp_cur = 0;
	glider.mission_param.wp_max = 1;
	glider.mission_param.msg_sent = 0;
	glider.mission_param.msg_max = 1;

	current_year = 2021;
	current_month = 05;
	current_day = 06;

	/* device inits and configurations */
	device_inits();

	// message queues
	message_queue_init();

	// sd card thread
	sd_tid = k_thread_create(&sd_thread, sd_stack_area, K_THREAD_STACK_SIZEOF(sd_stack_area),
							 (k_thread_entry_t)app_sd_thread, NULL, NULL, NULL,
							 7, 0, K_NO_WAIT);

	glider.mission_started = false;

	check_mission();

	printk("\nInit complete. Press button 1 to start\n\n");

	return 0;
}

// --------------------------------------------------
// MAIN
// --------------------------------------------------

void main(void)
{
	printk("\n\n**** NordicOasys v0.99 - Started ****\n\n");

	enum glider_event_type event = EVT_INIT;

	// int64_t ntp;
	// date_time_now(&ntp);
	// int64_t unixtime = 1620378863;
	// LOG_INF("Unix Timestamp: %llu\n", unixtime);

	// struct tm *time_test = gmtime(&unixtime);

	// LOG_INF("Time struct: %d", time_test->tm_year);

	while (1)
	{
		message_queue_reset();

		switch (event)
		{
		case EVT_INIT:
			// initialize glider
			glider_init();

			// get gps fix
			// set_LED(22, 1);
			// gps_module();
			// set_LED(22, 0);

			// TODO: create check for finished missions
			if (glider.mission_param.mission != 0)
			{
				event = EVT_DIVE;
			}
			else
			{
				LOG_INF("No mission ongoing, on standby for new mission");
				event = EVT_AWAIT_MISSION;
			}

			break;
		case EVT_AWAIT_MISSION:

			gcloud_module();
			// if unsuccessful, check satellite for any commands
			// satellite_module();

			// function: on button press
			// // turn on wifi
			// set_LED(30, 1);
			// wifi_module();
			// set_LED(30, 0);

			if (glider.mission_param.mission != 0)
			{
				event = EVT_DIVE;
			}
			else
			{
				event = EVT_IDLE;
			}

			break;
		case EVT_DIVE:
			// function: send command to navigation

			// read sensor data
			// set_LED(20, 1);
			// sensor_module();
			// set_LED(20, 0);

			event = EVT_SURFACE;

			break;
		case EVT_SURFACE:

			glider.mission_param.wp_cur++;

			if (glider.mission_param.wp_cur == glider.mission_param.wp_max)
			{
				event = EVT_IDLE;
			}
			else
			{
				event = EVT_DIVE;
			}

			// set_LED(22, 1);
			// gps_module();
			// set_LED(22, 0);

			// set_LED(31, 1);
			// gcloud_module();
			// set_LED(31, 0);

			set_LED(30, 1);
			set_LED(31, 1);
			satellite_module();
			set_LED(30, 0);
			set_LED(31, 0);

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

			event = EVT_AWAIT_MISSION;

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

// --------------------------------------------------
// currently not in use
// --------------------------------------------------

/*
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
	uart_send(uart_dev1, "wifi", strlen("wifi"));

	// wifi loop 
for (int i = 0;; i++)
{

	// test messages from wifi controller
	k_msgq_get(&uart_msg_q, &uart_msg, K_FOREVER);
	LOG_INF("%s", log_strdup(uart_msg));

	if (strcmp(uart_msg, "{exit}") == 0)
	{
		uart_exit(uart_dev1);
		break;
	}
	// wifi setup success, send further commands
	else if (strcmp(uart_msg, "{wifi ok}") == 0)
	{
		LOG_INF("\nReceived wifi ok");
		uart_send(uart_dev1, "{mqtt}\n", strlen("{mqtt}\n"));
	}
	// send data from uart_data_array
	else if (strcmp(uart_msg, "{mqtt ok}") == 0)
	{
		LOG_INF("\nReceived mqtt ok");
		publish_uart_data();
	}
	// else if (strcmp(uart_msg, "{keyword}") == 0)
	// {
	// 	//something
	// }
	// default print to terminal
	else
	{
		LOG_INF("%s", log_strdup(uart_msg));
	}
}
// wifi loop end
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
	// MQTT loop
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
	// MQTT loop end

	err = app_mqtt_disconnect();
	if (err != 0)
	{
		printk("\nDisconnect unsuccessful: %d", err);
		printk("\n\nClosing program, check for errors in code lol");
		return err;
	}

	return 0;
}
*/