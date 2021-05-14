#include <zephyr.h>
#include <nrf_socket.h>
#include <dk_buttons_and_leds.h>
#include <kernel.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <modem/lte_lc.h>
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
	EVT_AWAIT_MISSION,
	EVT_DIVE,
	EVT_SURFACE,
	EVT_IDLE
};

enum glider_mission_status
{
	MISSION_WAIT_START,
	MISSION_ONGOING,
	MISSION_FINISHED
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

struct m_param_t
{
	int mission;
	time_t m_start_s; // mission start (sec)
	int depth_max;
	int depth_min;
	int msg_max;
	int wp_max;
	uint8_t wp_arr[10][2][16];
	struct sens_param_t sens_param;
};

struct m_state_t
{
	int msg_sent;	// messages sent for current mission
	int wp_cur;		// waypoint current
	int surfaced;	// 0 if diving, greater than 0 on surface
	int m_database; // if mission received from wifi = 0, greater than 0 if present in database
					// sends mission to database if = 0
};

struct glider_t
{
	uint8_t uid[7];
	uint8_t sIMEI[20];
	struct m_param_t m_param;
	struct m_state_t m_state;
	struct prog_param_t prog_param;
};

// --------------------------------------------------
// VARIABLE DECLARATIONS
// --------------------------------------------------

struct glider_t glider;
static enum glider_event_type event = EVT_AWAIT_MISSION;
static enum glider_mission_status mission_state;

static const char file_mission[] = "M_PARAM.TXT";

// time variables
static time_t unix_time_ms = 0;
static time_t unix_time_CET = 0;
static time_t offset_CET = 7200;
struct tm *time_UTC = {0};
struct tm *time_CET = {0};

// Main message queue, used for getting responses from other modules
struct k_msgq main_msg_q;
static uint8_t main_msgq_buffer[3 * 512];
static uint8_t main_msg[512];

// button variables
struct k_msgq button_msg_q;
struct k_msgq button_msg_qr;
static uint8_t button_msgq_buffer[3 * sizeof(int)];

// UART variables
struct k_msgq uart_msg_q;
static uint8_t uart_msgq_buffer[3 * 256];
static uint8_t uart_msg[256];

// Gcloud variables
struct k_msgq gcloud_msg_q;
static uint8_t gcloud_msgq_buffer[2 * 512];
static uint8_t gcloud_msg[512];

static bool mission_params_wifi = false;
static bool data_available_to_send = false;
static bool gcloud_init_complete = false;

// SD Card variables
struct k_msgq sd_msg_q;
static uint8_t sd_msgq_buffer[5 * sizeof(sd_msg_t)];

// GPS data message queue
struct k_msgq gps_msg_q;
static uint8_t gps_msgq_buffer[256];
static uint8_t gps_msg[256];

// --------------------------------------------------
// FORWARD DECLARATIONS
// --------------------------------------------------

// building outputs error if not declared, although is valid function in zephyr library
char *strtok_r(char *str, const char *sep, char **state);

static int sensor_module();
static int wifi_module();
static int gps_module();
static int gcloud_module();
static int satellite_module();
static int glider_init();

// --------------------------------------------------
// FUNCTION DECLARATIONS
// --------------------------------------------------

/* HELPER FUNCTIONS */

// updates tm UTC and tm CET, also updates unix time CET
static int update_tm_all(time_t new_time)
{
	LOG_INF("New unix ts: %d", (int32_t)new_time);

	time_UTC = gmtime(&new_time);
	date_time_set(time_UTC);
	date_time_now(&unix_time_ms);
	unix_time_CET = (unix_time_ms / 1000) + offset_CET;
	time_CET = gmtime(&unix_time_CET);

	LOG_INF("Time updated: %04d-%02d-%02d %02d:%02d:%02d",
			time_CET->tm_year + 1900,
			time_CET->tm_mon + 1,
			time_CET->tm_mday,
			time_CET->tm_hour,
			time_CET->tm_min,
			time_CET->tm_sec);

	return 0;
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
	static uint8_t test = 0;
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
	static sd_msg_t sd_msg;
	sd_msg.event = WRITE_FILE;

	// LOG_INF("saving to sd card");

	// create filename, format (assuming mission 2): [M2.TXT]
	static uint8_t filename[16] = "";
	static uint8_t tmp_str[16] = "";

	// snprintf(tmp_str, sizeof(tmp_str), "%u", glider.m_param.mission);
	// strcat(filename, tmp_str);

	snprintf(tmp_str, sizeof(tmp_str), "M%u.TXT", glider.m_param.mission);
	strcpy(filename, tmp_str);

	// copy string over to struct
	strcpy(sd_msg.filename, filename);
	//sd_msg.filename[strlen(filename)] = 0;
	LOG_INF("filename: %s", log_strdup(sd_msg.filename));

	strcpy(sd_msg.string, data_string);

	// add to sd card message queue
	k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);

	return 0;
}

// initialize message queues
//CANDO: replace sizeof parameter with type instead of variable
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
	k_msgq_init(&gcloud_msg_q, gcloud_msgq_buffer, sizeof(gcloud_msg), 2);

	// SD
	k_msgq_init(&sd_msg_q, sd_msgq_buffer, sizeof(sd_msg_t), 5);

	return 0;
}

// minor function that causes LEDs to blink on nrf startup. NOTE: buttons need to be initialized before call
static int blink_LED()
{
	set_LED(21, 1);
	set_LED(22, 1);
	set_LED(20, 1);
	set_LED(30, 1);
	set_LED(31, 1);
	k_sleep(K_MSEC(200));
	set_LED(21, 0);
	set_LED(22, 0);
	set_LED(20, 0);
	set_LED(30, 0);
	set_LED(31, 0);
	k_sleep(K_MSEC(200));
	set_LED(21, 1);
	set_LED(22, 1);
	set_LED(20, 1);
	set_LED(30, 1);
	set_LED(31, 1);
	k_sleep(K_MSEC(200));
	set_LED(21, 0);
	set_LED(22, 0);
	set_LED(20, 0);
	set_LED(30, 0);
	set_LED(31, 0);

	return 0;
}

//CANDO: pass devices to initialise
static int device_inits()
{
	button_dev_init();
	dk_leds_init();
	ext_led_init();
	uart_dev_init(UART_1);
	uart_dev_init(UART_2);
	return 0;
}

// purge leftover messages from message queue
static int message_queue_reset()
{
	k_msgq_purge(&button_msg_q);
	k_msgq_purge(&uart_msg_q);
	// k_msgq_purge(&mqtt_msg_q);
	k_msgq_purge(&gcloud_msg_q);

	return 0;
}

// updates mission status and saves to sd card
static int update_mission_status(enum glider_mission_status new_mission_state)
{
	LOG_INF("Setting mission state to: %d", new_mission_state);
	mission_state = new_mission_state;

	date_time_now(&unix_time_ms);

	static sd_msg_t sd_msg;
	strcpy(sd_msg.filename, file_mission);

	static uint8_t json_str[512] = "";
	sd_msg.event = READ_JSON;

	k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);
	k_msgq_get(&main_msg_q, &json_str, K_FOREVER);

	if (strcmp(json_str, "ERROR") != 0)
	{
		cJSON *m_JSON = cJSON_Parse(json_str);
		if (cJSON_IsObject(m_JSON))
		{
			if (cJSON_HasObjectItem(m_JSON, "state"))
			{
				LOG_INF("Updating previously written state");
				cJSON_DeleteItemFromObject(m_JSON, "state");
			}

			cJSON *state_JSON = cJSON_CreateObject();
			cJSON_AddNumberToObject(state_JSON, "Ms", new_mission_state);
			cJSON_AddNumberToObject(state_JSON, "wp_cur", glider.m_state.wp_cur);
			cJSON_AddNumberToObject(state_JSON, "msg_sent", glider.m_state.msg_sent);
			cJSON_AddNumberToObject(state_JSON, "unix", (unix_time_ms / 1000));
			cJSON_AddNumberToObject(state_JSON, "surfaced", glider.m_state.surfaced);
			cJSON_AddNumberToObject(state_JSON, "m_database", glider.m_state.m_database);
			cJSON_AddItemToObject(m_JSON, "state", state_JSON);

			LOG_INF("Writing new state to SD card");
			// saving to sd card
			sd_msg.event = OVERWRITE_FILE;

			strcpy(sd_msg.string, cJSON_Print(m_JSON));
			cJSON_Minify(sd_msg.string);

			k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);
		}
		else
		{
			LOG_ERR("not a JSON, could not parse mission parameters");
			cJSON_Delete(m_JSON);

			return -1;
		}
		cJSON_Delete(m_JSON);
	}

	return 0;
}

// checks mission if it is new or not, saves parameters to glider struct and sd card
static int parse_m_params(void *json_str)
{
	cJSON *tmp_JSON;
	cJSON *tmpsub_JSON;

	LOG_INF("json string: %s", log_strdup(json_str));
	cJSON *m_JSON = cJSON_Parse(json_str);
	if (cJSON_IsObject(m_JSON))
	{
		static bool new_mission = false;

		time_t tmp_start_s = 0;
		struct tm tmp_start = {0};

		// parameter start time
		if (cJSON_HasObjectItem(m_JSON, "start"))
		{
			tmp_JSON = cJSON_GetObjectItem(m_JSON, "start");
			if (cJSON_IsString(tmp_JSON))
			{
				static char *eptr;
				uint8_t tmp_str[16] = "";

				memcpy(tmp_str, tmp_JSON->valuestring, 4);
				tmp_start.tm_year = strtol(tmp_str, &eptr, 10) - 1900;
				memset(tmp_str, 0, sizeof(tmp_str));

				memcpy(tmp_str, tmp_JSON->valuestring + 4, 2);
				tmp_start.tm_mon = strtol(tmp_str, &eptr, 10) - 1;
				memset(tmp_str, 0, sizeof(tmp_str));

				memcpy(tmp_str, tmp_JSON->valuestring + 6, 2);
				tmp_start.tm_mday = strtol(tmp_str, &eptr, 10);
				memset(tmp_str, 0, sizeof(tmp_str));

				memcpy(tmp_str, tmp_JSON->valuestring + 8, 2);
				tmp_start.tm_hour = strtol(tmp_str, &eptr, 10);
				memset(tmp_str, 0, sizeof(tmp_str));

				memcpy(tmp_str, tmp_JSON->valuestring + 10, 2);
				tmp_start.tm_min = strtol(tmp_str, &eptr, 10);
				memset(tmp_str, 0, sizeof(tmp_str));

				tmp_start.tm_sec = 0;
				tmp_start_s = mktime(&tmp_start);
				LOG_INF("start time unix: %d", (int32_t)tmp_start_s);
				date_time_now(&unix_time_ms);
				if (tmp_start_s < glider.m_param.m_start_s || tmp_start_s < (unix_time_ms / 1000 + offset_CET))
				{
					LOG_INF("Mission expired");
					return 1;
				}
				else
				{
					LOG_INF("Start time valid, newer than current mission and current time");
					LOG_INF("Now testing Mission number");
				}
			}
			else
			{
				LOG_ERR("start needs to be string");
			}
		}

		// parameter mission number
		if (cJSON_HasObjectItem(m_JSON, "M"))
		{
			tmp_JSON = cJSON_GetObjectItem(m_JSON, "M");
			if (cJSON_IsNumber(tmp_JSON))
			{
				LOG_INF("Mission: %d", tmp_JSON->valueint);
				// test if mission received is new
				if (glider.m_param.mission < tmp_JSON->valueint)
				{
					LOG_INF("Mission valid, parsing further");
					new_mission = true;

					glider.m_param.mission = tmp_JSON->valueint;
					glider.m_param.m_start_s = tmp_start_s;

					LOG_INF("new start time: %04d-%02d-%02d %02d:%02d", tmp_start.tm_year + 1900,
							tmp_start.tm_mon + 1,
							tmp_start.tm_mday,
							tmp_start.tm_hour,
							tmp_start.tm_min);
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
			glider.m_state.wp_cur = 1;
			glider.m_state.msg_sent = 0;

			// fetching last saved state, if it does not exist add with default values
			if (cJSON_HasObjectItem(m_JSON, "state"))
			{
				LOG_INF("state exists, fetching values");
				tmp_JSON = cJSON_GetObjectItem(m_JSON, "state");
				if (cJSON_IsObject(tmp_JSON))
				{
					if (cJSON_HasObjectItem(tmp_JSON, "Ms"))
					{
						tmpsub_JSON = cJSON_GetObjectItem(tmp_JSON, "Ms");
						if (cJSON_IsNumber(tmpsub_JSON))
						{
							mission_state = tmpsub_JSON->valueint;
						}
						else
						{
							LOG_ERR("Mission state needs to be number");
						}
					}
					if (cJSON_HasObjectItem(tmp_JSON, "wp_cur"))
					{
						tmpsub_JSON = cJSON_GetObjectItem(tmp_JSON, "wp_cur");
						if (cJSON_IsNumber(tmpsub_JSON))
						{
							glider.m_state.wp_cur = tmpsub_JSON->valueint;
						}
						else
						{
							LOG_ERR("Waypoint current needs to be number");
						}
					}
					if (cJSON_HasObjectItem(tmp_JSON, "msg_sent"))
					{
						tmpsub_JSON = cJSON_GetObjectItem(tmp_JSON, "msg_sent");
						if (cJSON_IsNumber(tmpsub_JSON))
						{
							glider.m_state.msg_sent = tmpsub_JSON->valueint;
						}
						else
						{
							LOG_ERR("Messages sent needs to be number");
						}
					}
					if (cJSON_HasObjectItem(tmp_JSON, "unix"))
					{
						tmpsub_JSON = cJSON_GetObjectItem(tmp_JSON, "unix");
						if (cJSON_IsNumber(tmpsub_JSON))
						{
							int64_t unix_time_start = tmpsub_JSON->valuedouble;
							time_UTC = gmtime(&unix_time_start);

							date_time_set(time_UTC);
							date_time_now(&unix_time_ms);
							update_tm_all(unix_time_ms / 1000);
						}
						else
						{
							LOG_ERR("Messages sent needs to be number");
						}
					}
					if (cJSON_HasObjectItem(tmp_JSON, "surfaced"))
					{
						tmpsub_JSON = cJSON_GetObjectItem(tmp_JSON, "surfaced");
						if (cJSON_IsNumber(tmpsub_JSON))
						{
							glider.m_state.surfaced = tmpsub_JSON->valueint;
						}
						else
						{
							LOG_ERR("Surface status needs to be number");
						}
					}
					if (cJSON_HasObjectItem(tmp_JSON, "m_database"))
					{
						tmpsub_JSON = cJSON_GetObjectItem(tmp_JSON, "m_database");
						if (cJSON_IsNumber(tmpsub_JSON))
						{
							glider.m_state.m_database = tmpsub_JSON->valueint;
						}
						else
						{
							LOG_ERR("mission database needs to be number");
						}
					}
				}
			}
			else
			{
				date_time_now(&unix_time_ms);

				cJSON *state_JSON = cJSON_CreateObject();
				LOG_INF("state not yet added, adding default values");
				cJSON_AddNumberToObject(state_JSON, "Ms", MISSION_WAIT_START);
				cJSON_AddNumberToObject(state_JSON, "wp_cur", 1);
				cJSON_AddNumberToObject(state_JSON, "msg_sent", 0);
				cJSON_AddNumberToObject(state_JSON, "unix", (unix_time_ms / 1000));
				cJSON_AddNumberToObject(state_JSON, "surfaced", 1);
				cJSON_AddNumberToObject(state_JSON, "m_database", glider.m_state.m_database);
				cJSON_AddItemToObject(m_JSON, "state", state_JSON);
			}

			// parameter max mqtt message count
			if (cJSON_HasObjectItem(m_JSON, "4G"))
			{
				tmp_JSON = cJSON_GetObjectItem(m_JSON, "4G");
				if (cJSON_IsNumber(tmp_JSON))
				{
					glider.m_param.msg_max = tmp_JSON->valueint;
				}
				else
				{
					LOG_ERR("4G needs to be number");
				}
			}

			// parameter max depth
			if (cJSON_HasObjectItem(m_JSON, "maxD"))
			{
				tmp_JSON = cJSON_GetObjectItem(m_JSON, "maxD");
				if (cJSON_IsNumber(tmp_JSON))
				{
					glider.m_param.depth_max = tmp_JSON->valueint;
				}
				else
				{
					LOG_ERR("maxD needs to be number");
				}
			}

			// parameter min depth
			if (cJSON_HasObjectItem(m_JSON, "minD"))
			{
				tmp_JSON = cJSON_GetObjectItem(m_JSON, "minD");
				if (cJSON_IsNumber(tmp_JSON))
				{
					glider.m_param.depth_min = tmp_JSON->valueint;
				}
				else
				{
					LOG_ERR("minD needs to be number");
				}
			}

			// parameter sensor frequencies
			if (cJSON_HasObjectItem(m_JSON, "C"))
			{
				tmp_JSON = cJSON_GetObjectItem(m_JSON, "C");
				if (cJSON_IsNumber(tmp_JSON))
				{
					glider.m_param.sens_param.freq_c = tmp_JSON->valueint;
				}
				else
				{
					LOG_ERR("C needs to be number");
				}
			}
			if (cJSON_HasObjectItem(m_JSON, "P"))
			{
				tmp_JSON = cJSON_GetObjectItem(m_JSON, "P");
				if (cJSON_IsNumber(tmp_JSON))
				{
					glider.m_param.sens_param.freq_p = tmp_JSON->valueint;
				}
				else
				{
					LOG_ERR("P needs to be number");
				}
			}
			if (cJSON_HasObjectItem(m_JSON, "T"))
			{
				tmp_JSON = cJSON_GetObjectItem(m_JSON, "T");
				if (cJSON_IsNumber(tmp_JSON))
				{
					glider.m_param.sens_param.freq_t = tmp_JSON->valueint;
				}
				else
				{
					LOG_ERR("T needs to be number");
				}
			}

			// waypoints - lat
			if (cJSON_HasObjectItem(m_JSON, "lat"))
			{

				tmp_JSON = cJSON_GetObjectItem(m_JSON, "lat");
				if (cJSON_IsArray(tmp_JSON))
				{
					glider.m_param.wp_max = cJSON_GetArraySize(tmp_JSON);

					for (int i = 0; i < glider.m_param.wp_max; i++)
					{
						tmpsub_JSON = cJSON_GetArrayItem(tmp_JSON, i);
						if (cJSON_IsString(tmpsub_JSON))
						{
							strcpy(glider.m_param.wp_arr[i][0], tmpsub_JSON->valuestring);
							// LOG_INF("lng: %lf", glider.m_param.wp_arr[i][1]);
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
			if (cJSON_HasObjectItem(m_JSON, "lng"))
			{
				tmp_JSON = cJSON_GetObjectItem(m_JSON, "lng");
				if (cJSON_IsArray(tmp_JSON))
				{
					for (int i = 0; i < glider.m_param.wp_max; i++)
					{
						tmpsub_JSON = cJSON_GetArrayItem(tmp_JSON, i);
						if (cJSON_IsString(tmpsub_JSON))
						{
							strcpy(glider.m_param.wp_arr[i][1], tmpsub_JSON->valuestring);
							// LOG_INF("lng: %lf", glider.m_param.wp_arr[i][1]);
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

			sd_msg_t sd_msg;

			strcpy(sd_msg.filename, file_mission);

			sd_msg.event = OVERWRITE_FILE;

			strcpy(sd_msg.string, cJSON_Print(m_JSON));
			cJSON_Minify(sd_msg.string);
			LOG_INF("Converted to string");

			k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);

			cJSON_Delete(m_JSON);
		}
	}
	else
	{
		LOG_ERR("not a JSON, could not parse mission parameters");
		cJSON_Delete(m_JSON);

		return -1;
	}

	return 0;
}

// reads mission params from sd card
static int check_mission()
{
	static sd_msg_t sd_msg;
	sd_msg.event = FIND_FILE;
	strcpy(sd_msg.filename, file_mission);

	k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);

	size_t sd_msg_response = 0;

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
			parse_m_params(&json_str);

			LOG_INF("Current parameters:");
			LOG_INF("Mission: %d", glider.m_param.mission);

			// TODO: change behavior based on this status
			LOG_INF("Max depth: %d", glider.m_param.depth_max);
			LOG_INF("Min depth: %d", glider.m_param.depth_min);
			LOG_INF("Message max: %d", glider.m_param.msg_max);
			LOG_INF("Waypoints: %d", glider.m_param.wp_max);

			LOG_INF("Sensor parameters:");
			LOG_INF("C: %d", glider.m_param.sens_param.freq_c);
			LOG_INF("P: %d", glider.m_param.sens_param.freq_p);
			LOG_INF("T: %d", glider.m_param.sens_param.freq_t);

			LOG_INF("State:");
			LOG_INF("Mission status: %d", mission_state);
			LOG_INF("Messages sent: %d", glider.m_state.msg_sent);
			LOG_INF("Current waypoint: %d", glider.m_state.wp_cur);
			LOG_INF("Surfaced: %d", glider.m_state.surfaced);
		}

		return 0;
	}

	LOG_INF("Mission params do not exist");
	return 1;
}

static int test_module(uint8_t *module_str)
{
	int ret = 0;
	int test = 0;
	static uint8_t test_response[128] = "";
	int retries = 0;
	int retry_max = 3;

	if (strstr(module_str, "gps") != NULL)
	{
		static glider_gps_data_t gps_data;

		test = app_gps(&gps_data, 10 * 1000, 500);
		if (test >= 0)
		{
			ret = 0;
		}
		else
		{
			ret = -1;
		}
	}
	else if (strstr(module_str, "4G") != NULL)
	{
		test = modem_configure();
		// lte off
		if (test == 0)
		{
			test = lte_lc_offline();
		}

		if (test == 0)
		{
			ret = 0;
		}
		else
		{
			ret = -1;
		}
	}
	else if (strstr(module_str, "sensor") != NULL)
	{
		// send start command to sensor
		uart_start(UART_1);
		uart_send(UART_1, "time:0", strlen("time:0"));

		// test reply. If no reply within 20 seconds, ret = -1
		k_msgq_get(&uart_msg_q, &test_response, K_SECONDS(20));
		if (strlen(test_response) != 0)
		{
			ret = -1;
			while (retries < retry_max)
			{
				// send off command to sensor
				uart_send(UART_1, "sensor_end", strlen("sensor_end"));

				// expects reply off, retries until max exceeded or valid reply
				k_msgq_get(&uart_msg_q, &test_response, K_SECONDS(20));
				if (strstr(test_response, "off sensor") != NULL)
				{
					ret = 0;
					break;
				}

				retries++;
			}
			uart_exit(UART_1);
		}
		else
		{
			ret = -1;
		}
	}
	LOG_INF("Test result: %d", ret);

	return ret;
}

// adds mission number, concatenates date and string to save bytes
static int format_payload_4G(char *dest_str, char *source_str)
{
	uint8_t ts_str[32] = "";
	int ts_arr = 0;

	// LOG_INF("%s", log_strdup(source_str));
	cJSON *payload_JSON = cJSON_Parse(source_str);

	if (cJSON_IsObject(payload_JSON))
	{
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
		cJSON_AddNumberToObject(payload_JSON, "M", glider.m_param.mission);

		// convert JSON to string
		strcpy(dest_str, cJSON_Print(payload_JSON));
		cJSON_Minify(dest_str);
	}
	else
	{
		LOG_INF("payload is not in JSON format");
		cJSON_Delete(payload_JSON);
		return -1;
	}
	cJSON_Delete(payload_JSON);
	LOG_INF("payload formatted");

	return 0;
}

// handles publishing of data to 4G. First message is always gps data and adds to total messages sent
static int publish_data_4G()
{
	// printk("\nRunning test, press button 1 to start\n");
	// button_wait();

	int ret = 0;

	// separate from m_param msg_sent, this only keeps track locally
	int msg_sent = 0;
	int msg_to_send = 0;
	if ((glider.m_param.wp_max - 1) > 0)
	{
		msg_to_send = glider.m_param.msg_max / (glider.m_param.wp_max - 1);
	}
	else
	{
		msg_to_send = glider.m_param.msg_max;
	}

	static uint8_t payload_4G[512] = "";

	static sd_msg_t sd_msg;
	static uint8_t sd_msg_response[256] = "";
	// helper variable
	static uint8_t tmp_str[16] = "";

	k_msgq_purge(&main_msg_q);

	// if mission parameters were provided from local wifi, send to database
	// this is to increase the chances of missions being synchronized
	// does not count towards msg max, as it is sent relatively infrequently
	if (!glider.m_state.m_database)
	{
		strcpy(sd_msg.filename, file_mission);
		sd_msg.event = READ_JSON;
		k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);

		k_msgq_get(&main_msg_q, &sd_msg_response, K_FOREVER);
		LOG_INF("Mission params: %s", log_strdup(sd_msg_response));
		if (strcmp(sd_msg_response, "ERROR") != 0)
		{
			cJSON *payload_JSON = cJSON_Parse(sd_msg_response);

			if (cJSON_IsObject(payload_JSON))
			{
				if (cJSON_HasObjectItem(payload_JSON, "state"))
				{
					cJSON_DeleteItemFromObject(payload_JSON, "state");
				}
				strcpy(payload_4G, cJSON_Print(payload_JSON));
				cJSON_Minify(payload_4G);

				LOG_INF("payload: %s", log_strdup(payload_4G));
				// gcloud_publish(payload_4G, strlen(payload_4G), MQTT_QOS_1_AT_LEAST_ONCE);
				glider.m_state.m_database = 1;
				update_mission_status(MISSION_ONGOING);
			}

			cJSON_Delete(payload_JSON);
		}
		else
		{
			LOG_ERR("Error in converting mission params to mqtt payload");
		}
	}

	// sending gps message first, this is to ensure at least 1 datapoint containing gps coordinates
	// check if there is gps data
	if (k_msgq_num_used_get(&gps_msg_q) == 0)
	{
		LOG_INF("No gps message to fetch");
	}
	else
	{
		k_msgq_get(&gps_msg_q, &gps_msg, K_NO_WAIT);

		ret = format_payload_4G(payload_4G, gps_msg);
		if (ret == 0)
		{
			LOG_INF("payload: %s", log_strdup(payload_4G));
			// gcloud_publish(payload_4G, strlen(payload_4G), MQTT_QOS_1_AT_LEAST_ONCE);
			glider.m_state.msg_sent++;
			msg_sent++;
		}
	}

	// fetching datapoints from sd card

	sd_msg.event = READ_JSON_4G;

	snprintf(tmp_str, sizeof(tmp_str), "M%d.TXT", glider.m_param.mission);
	strcpy(sd_msg.filename, tmp_str);

	snprintf(tmp_str, sizeof(tmp_str), "max:%d\r", msg_to_send);
	strcpy(sd_msg.string, tmp_str);
	snprintf(tmp_str, sizeof(tmp_str), "wp_cur:%d\r", glider.m_state.wp_cur);
	strcat(sd_msg.string, tmp_str);
	LOG_INF("Filename for fetching sd data: %s", log_strdup(sd_msg.filename));
	LOG_INF("string for fetching sd data: %s", log_strdup(sd_msg.string));

	// sending event to sd card
	// LOG_INF("%s", log_strdup(sd_msg.string));
	k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);

	LOG_INF("messages sent: %d", msg_sent);
	while (msg_sent < msg_to_send)
	{
		k_msgq_get(&main_msg_q, &sd_msg_response, K_FOREVER);

		// LOG_INF("%s", log_strdup(sd_msg_response));
		ret = format_payload_4G(payload_4G, sd_msg_response);
		if (ret == 0)
		{
			LOG_INF("payload: %s", log_strdup(payload_4G));
			// gcloud_publish(payload_4G, strlen(payload_4G), MQTT_QOS_1_AT_LEAST_ONCE);
			glider.m_state.msg_sent++;
			msg_sent++;
			LOG_INF("messages sent: %d", msg_sent);
		}

		// in case there are not enough datapoints to satisfy number of messages to be sent (reached end of read)
		if (strcmp(sd_msg_response, "EOM") == 0)
		{
			LOG_INF("Received EOM");
			break;
		}
		else if (strcmp(sd_msg_response, "ERROR") == 0)
		{
			LOG_ERR("Error in fetching data");
			return -1;
		}
	}

	printk("\nAll messages sent");

	return 0;
}

// fetch satellite IMEI
// NOTE: This function runs separate of satellite module and should not be used simultaneously
static int sat_fetch_IMEI()
{
	static int ret = 0;
	static int cnt = 0;
	static bool sat_start = false;

	static int max_retries_ESP = 3;

	static uint8_t sat_msg[64] = "";
	static uint8_t sat_response[64] = "";
	static uint8_t sat_response_test[64] = "";

	// start uart
	uart_start(UART_2);

	while (!sat_start && cnt < max_retries_ESP)
	{
		uart_send(UART_2, "", strlen(""));
		uart_send(UART_2, "sat_start", strlen("sat_start"));
		LOG_INF("sent command satellite start to esp");
		k_msgq_get(&uart_msg_q, sat_response, K_FOREVER);
		LOG_INF("response: %s", log_strdup(sat_response));
		if (strstr(sat_response, "OK") != NULL)
		{
			LOG_INF("Received OK");
			sat_start = true;
		}
		cnt++;
	}

	if (sat_start)
	{
		// send message
		strcpy(sat_msg, "AT+CGSN");
		uart_send(UART_2, sat_msg, strlen(sat_msg));

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
				uart_exit(UART_2);
				return -1;
			}
			// copy response if valid
			else if (strcmp(sat_response_test, sat_msg) != 0)
			{
				strcpy(sat_response, sat_response_test);
			}
		}
		// clear response and any pending messages from satellite
		memset(sat_response_test, 0, sizeof(sat_response_test));
		k_msgq_purge(&uart_msg_q);

		LOG_DBG("sat IMEI: %s", log_strdup(sat_response));
		strcpy(glider.sIMEI, sat_response);

		uart_send(UART_2, "sat_end", strlen("sat_end"));
		LOG_INF("sent command satellite end to esp");
	}
	else
	{
		LOG_ERR("ESP communication unsuccessful");
		ret = -1;
	}

	uart_exit(UART_2);

	return ret;
}

// checks signal, returns signal strength
static int sat_check_signal()
{
	static int signal = 0;

	static uint8_t sat_msg[64] = "";
	static uint8_t sat_response[64] = "";
	static uint8_t sat_response_test[64] = "";

	// send message
	strcpy(sat_msg, "AT+CSQ");
	uart_send(UART_2, sat_msg, strlen(sat_msg));

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
		else if (strcmp(sat_response_test, sat_msg) != 0)
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

// turn on SBD session (send/receive satellite messages)
// needs to be called to send / receive messages, currently only reads 1 message
static int sat_sbd_session()
{
	static int ret = 0;
	// ret values:
	// -1 - AT command problem, e.g. serial msg is sent wrong
	// 0 - message sent, no message rcv pending
	// 1 - message sent, message rcv pending

	// in most cases, error 2 also equates to error 3
	// 2 - message sent unsuccessful, maybe due to signal
	// 3 - message receive unsuccessful, maybe due to signal

	static uint8_t sat_msg[] = "AT+SBDI";
	static uint8_t sat_response[64] = "";
	static uint8_t sat_response_test[64] = "";

	static char *saveptr;
	static char *eptr;
	static char *test;

	static int i = 0;
	static int sbd_response[6];

	uart_send(UART_2, sat_msg, strlen(sat_msg));

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
		else if (strcmp(sat_response_test, sat_msg) != 0)
		{
			strcpy(sat_response, sat_response_test);
		}
	}
	// clear response and any pending messages from satellite
	memset(sat_response_test, 0, sizeof(sat_response_test));
	k_msgq_purge(&uart_msg_q);

	LOG_INF("response: %s", log_strdup(sat_response));

	test = strtok_r(sat_response, ",", &saveptr); // TODO: check param 1, was always zero under testing
	LOG_INF("test: %s", log_strdup(test));
	sbd_response[i++] = strtol(test, &eptr, 10);
	// LOG_INF("param %d: %d", i, sbd_response[0]);
	// splitting up parameters
	while ((test = strtok_r(NULL, ",", &saveptr)) != NULL)
	{
		sbd_response[i++] = strtol(test, &eptr, 10);
	}

	// for checking parameters
	for (i = 0; i < 6; i++)
	{
		LOG_INF("param %d: %d", i + 1, sbd_response[i]);
	}

	// testing parameters for success or not
	if (sbd_response[0] > 4)
	{
		LOG_INF("satellite error on send msg");
		ret = 2;
	}
	if (sbd_response[2] > 0)
	{
		if (sbd_response[2] > 1)
		{
			LOG_INF("satellite error on read msg, retry fetch");
			ret = 3;
		}
		else
		{
			// currently it will only fetch the first message that it receives
			// further logic for several messages pending needs to be set up e.g. returning no. of messages
			LOG_INF("satellite read msg(s) pending: %d msg(s)", sbd_response[5] + 1);
			ret = 1;
		}
	}

	return ret;
}

// if sat_sbd_session returns message pending, this function can be called
static int sat_rcv_msg()
{
	static int ret = 0;

	static uint8_t sat_msg[] = "AT+SBDRT";
	static uint8_t sat_response[64] = "";
	static uint8_t sat_response_test[64] = "";

	// fetch satellite message
	uart_send(UART_2, sat_msg, strlen(sat_msg));

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
		else if (strcmp(sat_response_test, sat_msg) != 0)
		{
			strcpy(sat_response, sat_response_test);
		}
	}
	// clear response and any pending messages from satellite
	memset(sat_response_test, 0, sizeof(sat_response_test));
	k_msgq_purge(&uart_msg_q);

	// TODO: parse satellite message
	LOG_INF("response: %s", log_strdup(sat_response));

	return ret;
}

// saves message to satellite MO buffer, call function sat_sbd_session after this
static int sat_send_msg(uint8_t *sat_payload)
{
	static int ret = 0;

	static uint8_t sat_msg[64] = "";
	static uint8_t sat_response[64] = "";
	static uint8_t sat_response_test[64] = "";

	strcpy(sat_msg, "AT+SBDWT=");
	strcat(sat_msg, sat_payload);

	LOG_INF("sat msg: %s", log_strdup(sat_msg));

	// send payload
	uart_send(UART_2, sat_msg, strlen(sat_msg));

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
		else if (strcmp(sat_response_test, sat_msg) != 0)
		{
			strcpy(sat_response, sat_response_test);
		}
	}
	// clear response and any pending messages from satellite
	memset(sat_response_test, 0, sizeof(sat_response_test));
	k_msgq_purge(&uart_msg_q);

	return ret;
}

// --------------------------------------------------
// MODULES
// --------------------------------------------------

static int sensor_module()
{
	set_LED(20, 1);

	printk("\n\npress button 1 to start sensor test\n\n");
	// button_wait();

	set_button_config(1);
	uart_start(UART_1);

	// update time before sending to sensors
	// NOTE: this does not work for some reason, is instead run right before module
	// date_time_now(&unix_time_ms);
	// update_tm_all(unix_time_ms / 1000);

	// send time to sensors
	uint8_t time_millis_str[32] = "";

	int32_t time_millis = ((time_CET->tm_hour * 60 * 60) + (time_CET->tm_min * 60) + time_CET->tm_sec) * 1000;
	LOG_INF("time (ms): %d", time_millis);
	snprintf(time_millis_str, sizeof(time_millis_str), "time:%d", time_millis);
	LOG_INF("time (str): %s", log_strdup(time_millis_str));

	uart_send(UART_1, time_millis_str, strlen(time_millis_str));

	uint8_t tmp_str[16] = "";
	snprintf(tmp_str, sizeof(tmp_str), "to_c:%d", glider.m_param.sens_param.freq_c);
	uart_send(UART_1, tmp_str, strlen(tmp_str));
	snprintf(tmp_str, sizeof(tmp_str), "to_p:%d", glider.m_param.sens_param.freq_p);
	uart_send(UART_1, tmp_str, strlen(tmp_str));
	snprintf(tmp_str, sizeof(tmp_str), "to_t:%d", glider.m_param.sens_param.freq_t);
	uart_send(UART_1, tmp_str, strlen(tmp_str));

	printk("\nsensor test start\n");

	printk("button 1: exit \nbutton 2: send uart signal\n\n");

	/* UART loop */
	for (int i = 0;; i++)
	{

		// memset(uart_msg, 0, sizeof(uart_msg));
		k_msgq_get(&uart_msg_q, &uart_msg, K_FOREVER);

		if (strcmp(uart_msg, "surfaced") == 0)
		{
			uart_send(UART_1, "sensor_end", strlen("sensor_end"));
			uart_exit(UART_1);
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

				// conversion from ms to respective type
				uint16_t hour = tmp_int / 3600 % 24;
				uint16_t min = (tmp_int / 60) % 60;
				uint16_t sec = tmp_int % 60;
				LOG_INF("ts: %u:%u:%u", hour, min, sec);

				// variables for holding timestamp (ts) and datetime (dt)
				uint8_t ts_string[64] = "";
				uint8_t date_string[64] = "";
				uint8_t tmp_str[16] = "";

				// fetching timestamp to ensure correct date
				date_time_now(&unix_time_ms);
				update_tm_all(unix_time_ms / 1000);

				// adding date to JSON
				snprintf(tmp_str, sizeof(tmp_str), "%04d-", time_CET->tm_year + 1900);
				strcat(date_string, tmp_str);
				snprintf(tmp_str, sizeof(tmp_str), "%02d-", time_CET->tm_mon + 1);
				strcat(date_string, tmp_str);
				snprintf(tmp_str, sizeof(tmp_str), "%02d", time_CET->tm_mday);
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

				LOG_INF("data to save:%s", log_strdup(sensor_str));

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
	set_LED(20, 0);
	k_sleep(K_SECONDS(2));
	return 0;
}

static int wifi_module()
{
	int ret = 0;
	int cnt = 0;
	int retries = 3;
	bool wifi_start = false;

	set_LED(30, 1);
	printk("\n\npress button 1 to start wifi test\n\n");
	// button_wait();

	uint8_t wifi_response[512] = "";
	uart_start(UART_2);

	printk("wifi test start\n");

	while (!wifi_start && cnt < retries)
	{
		uart_send(UART_2, "", strlen(""));
		uart_send(UART_2, "wifi_start", strlen("wifi_start"));
		LOG_INF("sent command wifi start to esp");
		k_msgq_get(&uart_msg_q, wifi_response, K_FOREVER);
		if (strstr(wifi_response, "OK") != NULL)
		{
			wifi_start = true;
		}
		cnt++;
	}

	if (wifi_start)
	{
		LOG_INF("wifi started");

		while (1)
		{
			k_msgq_get(&uart_msg_q, wifi_response, K_FOREVER);
			LOG_INF("%s", log_strdup(wifi_response));
			//button_wait();

			//strcpy(wifi_response, "{D:20202020.txt}");

			if (strstr(wifi_response, "connected") != NULL)
			{
				uint8_t sd_msg_response[256] = "";

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

				k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);
			}
			else if (strstr(wifi_response, "time:") != NULL)
			{
				// parse unix time
				time_t wifi_unix = 0;
				static char *eptr;
				wifi_unix = strtoll(wifi_response + 5, &eptr, 10) / 1000;
				LOG_INF("unix from wifi: %d", (int32_t)wifi_unix);

				time_UTC = gmtime(&wifi_unix);

				date_time_set(time_UTC);
				date_time_now(&unix_time_ms);
				update_tm_all(unix_time_ms / 1000);
				LOG_INF("time synchronized through wifi: %d", (int32_t)wifi_unix);
			}
			else if (strstr(wifi_response, "wifi_end") != NULL)
			{
				LOG_INF("\nReceived wifi end command\n");
				// uart_send(UART_2, "wifi_end;", strlen("wifi_end;"));
				break;
			}
			else if (strstr(wifi_response, "test:") != NULL)
			{
				int test = 0;
				uint8_t reply[64] = "";

				test = test_module(wifi_response);
				if (test == 0)
				{
					strcpy(reply, wifi_response);
					strcat(reply, ",OK");
					uart_send(UART_2, reply, strlen(reply));
				}
				else
				{
					strcpy(reply, wifi_response);
					strcat(reply, ",ERROR");
					uart_send(UART_2, reply, strlen(reply));
				}
			}
			else
			{
				ret = parse_m_params(&wifi_response);
				if (ret == 0)
				{
					mission_state = MISSION_WAIT_START;
					mission_params_wifi = true;
					glider.m_state.m_database = 0;
					LOG_INF("Received new mission parameters");
				}
				else
				{
					LOG_INF("Unknown request from ESP");
				}
			}
		}
	}
	else
	{
		LOG_ERR("wifi start unsuccessful");
		uart_exit(UART_2);
		set_LED(30, 0);
		return -1;
	}

	uart_exit(UART_2);
	set_LED(30, 0);
	return 0;
}

static int gps_module()
{
	int ret = 0;

	static glider_gps_data_t gps_data;

	set_LED(22, 1);
	// printk("\n\npress button 1 to start gps test\n\n");
	// button_wait();
	printk("\ngps test start");

	ret = app_gps(&gps_data, 120 * 1000, 500);

	if (ret == 0)
	{
		time_UTC->tm_year = gps_data.time.tm_year;
		time_UTC->tm_mon = gps_data.time.tm_mon;
		time_UTC->tm_mday = gps_data.time.tm_mday;

		time_UTC->tm_hour = gps_data.time.tm_hour;
		time_UTC->tm_min = gps_data.time.tm_min;
		time_UTC->tm_sec = gps_data.time.tm_sec;

		date_time_set(time_UTC);
		date_time_now(&unix_time_ms);
		update_tm_all(unix_time_ms / 1000);

		// save gps data on sd card
		LOG_INF("%s", log_strdup(gps_data.gps_string));
		sd_save_data(gps_data.gps_string);
		// add to gps message queue
		k_msgq_put(&gps_msg_q, &gps_data.gps_string, K_NO_WAIT);

		data_available_to_send = true;
	}
	else
	{
		LOG_INF("Unable to get gps data");
		set_LED(22, 0);
		return -1;
	}

	printk("\ngps test end");
	set_LED(22, 0);
	return 0;
}

static int gcloud_module()
{
	set_LED(31, 1);
	printk("\n\npress button 1 to start gcloud test\n\n");
	// button_wait();
	printk("gcloud test start\n");

	memset(gcloud_msg, 0, sizeof(gcloud_msg));

	// publish_data_4G();

	int ret = -1;
	int max_retries = 3;
	int64_t start_time = 0;
	int64_t current_time = 0;
	int64_t timeout = 300000; // 5 minutes

	// strcpy(gcloud_msg, "{\"M\":11,\"4G\":10,\"C\":0,\"P\":0,\"T\":0,\"lat\":[\"51.7558\",\"60.7558\"],\"lng\":[\"19.2726\",\"21.2726\"],\"maxD\":0,\"minD\":0,\"start\":\"202104280559\"}");

	// mqtt init
	if (!gcloud_init_complete)
	{
		ret = app_gcloud_init_and_connect(max_retries);
		if (ret != 0)
		{
			LOG_INF("Unable to connect to gcloud");
			set_LED(31, 0);
			return 1;
		}
		gcloud_init_complete = true;
	}
	// reconnect
	else
	{
		ret = app_gcloud_reconnect(max_retries);
		if (ret != 0)
		{
			LOG_INF("Unable to connect to gcloud");
			set_LED(31, 0);
			return 1;
		}
	}

	start_time = k_uptime_get();

	LOG_INF("Starting gcloud loop");
	// /* Gcloud loop */
	while (current_time <= timeout)
	{
		// button_wait();
		current_time = k_uptime_get() - start_time;

		// gcloud function
		ret = app_gcloud();
		if (ret != 0)
		{
			LOG_ERR("Error in running MQTT loop, exiting module");
			ret = app_gcloud_disconnect(max_retries);
			if (ret != 0)
			{
				LOG_ERR("Disconnect unsuccessful: %d", ret);
				LOG_ERR("Closing program, check for errors in code lol");
			}
			set_LED(31, 0);
			return -1;
		}

		// fetch message from gcloud
		k_msgq_get(&gcloud_msg_q, &gcloud_msg, K_NO_WAIT);
		LOG_INF("gcloud string: %s", log_strdup(gcloud_msg));

		// test if received payload is a mission
		ret = parse_m_params(&gcloud_msg);
		if (ret >= 0)
		{
			if (ret == 0)
			{
				glider.m_state.m_database = 1;
				mission_state = MISSION_WAIT_START;
			}
			break;
		}

		// send data
		if (data_available_to_send)
		{
			publish_data_4G();
			data_available_to_send = false;
		}
		// if mission is still ongoing and data has been sent, return
		else if (!data_available_to_send && mission_state == MISSION_ONGOING)
		{
			LOG_INF("Data sent, returning to mission");
			break;
		}
	}
	/* Gcloud loop end */

	ret = app_gcloud_disconnect(max_retries);
	if (ret != 0)
	{
		LOG_ERR("Disconnect unsuccessful: %d", ret);
		LOG_ERR("Closing program, check for errors in code lol");
		set_LED(31, 0);
		return -1;
	}

	set_LED(31, 0);
	return 0;
}

static int satellite_module()
{
	set_LED(30, 1);
	set_LED(31, 1);
	printk("\n\npress button 1 to start satellite test\n\n");
	// button_wait();

	static int ret = 0;
	static int cnt = 0;
	static int cnt2 = 0;
	static int max_retries_ESP = 3;
	static int max_retries = 2;

	static uint8_t sat_response[128] = "";
	static uint8_t sat_payload[64] = "";

	static bool sat_start = false;
	static int signal = 0;
	static bool msg_rdy = false;
	static bool msg_sent = false;

	// start uart
	uart_start(UART_2);

	while (!sat_start && cnt < max_retries_ESP)
	{
		uart_send(UART_2, "", strlen(""));
		uart_send(UART_2, "sat_start", strlen("sat_start"));
		LOG_INF("sent command satellite start to esp");
		k_msgq_get(&uart_msg_q, sat_response, K_FOREVER);
		LOG_INF("response: %s", log_strdup(sat_response));
		if (strstr(sat_response, "OK") != NULL)
		{
			LOG_INF("Received OK");
			sat_start = true;
		}
		cnt++;
	}
	if (sat_start)
	{

		cnt = 0;
		while (cnt < max_retries && !msg_sent)
		{ // finding signal
			signal = sat_check_signal();

			if (signal)
			{ // if signal found
				LOG_INF("got signal");
				break;

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

						strcpy(sat_payload, lat_JSON->valuestring);
						strcat(sat_payload, ",");
						strcat(sat_payload, lng_JSON->valuestring);
						msg_rdy = true;
					}
					else
					{
						LOG_ERR("error in gps data");
						ret = -1;
						break;
					}
					cJSON_Delete(gps_JSON);
				}

				LOG_INF("attempting send");
				ret = sat_send_msg(sat_payload);
				if (ret == 0)
				{
					ret = sat_sbd_session();
				}
				if (ret == 0 || ret == 1)
				{
					LOG_INF("satellite msg sent");
					msg_sent = true;
				}
				else if (ret >= 2)
				{
					LOG_INF("sbd session unsuccessful");
					cnt2++;
				}
				else if (ret < 0)
				{
					LOG_ERR("Error in serial communication");
					ret = -1;
					break;
				}

				if (ret == 1)
				{
					sat_rcv_msg();
				}

				if (cnt2 >= max_retries)
				{
					break;
				}

			} // if signal found end
			else
			{
				cnt++;
				k_sleep(K_SECONDS(20));
			}
		} // finding signal end
		if (!signal)
		{
			LOG_INF("Unable to get satellite fix");
			ret = 1;
		}

		uart_send(UART_2, "sat_end", strlen("sat_end"));
		LOG_INF("sent command satellite end to esp");
	}
	else
	{
		LOG_ERR("ESP communication unsuccessful");
		ret = -1;
	}

	uart_exit(UART_2);
	set_LED(30, 0);
	set_LED(31, 0);

	LOG_INF("Ending satellite module, ret: %d", ret);

	return ret;
}

static int glider_init()
{
	int ret = 0;
	/* Fetch serial number (uid) */
	uint8_t imei[18] = "";
	uint8_t uid[18] = "";
	at_cmd_write("AT+CGSN", imei, sizeof(imei), NULL);
	memcpy(uid, imei + 8, 6);
	strcpy(glider.uid, uid);

	LOG_INF("UID: %s", log_strdup(glider.uid));

	// initialize parameters, TODO: Add if there are any other important params
	glider.m_param.mission = 0;
	mission_state = MISSION_FINISHED;
	glider.m_param.wp_max = 1;
	glider.m_param.msg_max = 1;
	glider.m_state.wp_cur = 0;
	glider.m_state.msg_sent = 0;
	glider.m_state.surfaced = 1;

	time_CET->tm_year = 121;
	time_CET->tm_mon = 5;
	time_CET->tm_mday = 12;

	/* device inits and configurations */
	device_inits();

	// blink to signify init
	blink_LED();

	// message queues
	message_queue_init();

	// sd card thread
	sd_tid = k_thread_create(&sd_thread, sd_stack_area, K_THREAD_STACK_SIZEOF(sd_stack_area),
							 (k_thread_entry_t)app_sd_thread, NULL, NULL, NULL,
							 7, 0, K_NO_WAIT);

	ret = check_mission();
	if (ret == 0)
	{
		LOG_INF("Mission found");
		// temporary for testing
		// mission_state = MISSION_FINISHED;
		// glider.m_param.m_start_s = 1620724035;
	}
	else
	{
		LOG_INF("\nMission params not found, waiting for mission\n\n");
		mission_state = MISSION_FINISHED;
	}

	printk("\nInit complete. Press button 1 to start\n\n");

	return 0;
}

// --------------------------------------------------
// MAIN
// --------------------------------------------------

void main(void)
{
	printk("\n\n**** NordicOasys v0.99 - Started ****\n\n");

	static int ret = 0;
	static int wifi_on = 0;

	static int64_t unix_time_start = 1620810897;
	time_UTC = gmtime(&unix_time_start);

	date_time_set(time_UTC);
	date_time_now(&unix_time_ms);
	update_tm_all(unix_time_ms / 1000);

	LOG_INF("\nEvent: init");

	// initialize glider
	glider_init();

	// TODO: create check for finished missions
	if (mission_state == MISSION_WAIT_START)
	{
		event = EVT_IDLE;
		LOG_INF("Idling until mission start");
	}
	else if (mission_state == MISSION_ONGOING)
	{
		LOG_INF("Mission ongoing");
		if (glider.m_state.surfaced)
		{
			glider.m_state.wp_cur--;
			event = EVT_SURFACE;
		}
		else
		{
			event = EVT_DIVE;
		}
	}
	else if (mission_state == MISSION_FINISHED)
	{
		// LOG_INF("Running gps module");
		// button_wait();
		// gps_module();
		event = EVT_AWAIT_MISSION;
		LOG_INF("No mission ongoing, on standby for new mission");
	}

	while (1)
	{
		message_queue_reset();

		switch (event)
		{
		case EVT_AWAIT_MISSION:
			LOG_INF("\nEvent: await mission");

			set_button_config(3);

			set_LED(22, 1);
			set_LED(30, 1);
			set_LED(31, 1);
			LOG_INF("Waiting for wifi command");
			k_msgq_get(&button_msg_qr, &wifi_on, K_SECONDS(60));
			set_LED(22, 0);
			set_LED(30, 0);
			set_LED(31, 0);

			if (wifi_on == 1)
			{
				LOG_INF("Running wifi module");
				// button_wait();
				wifi_module();
				wifi_on = 0;
			}
			else
			{
				LOG_INF("Running gcloud module");
				// button_wait();
				ret = gcloud_module();
				if (ret == 1)
				{
					// if unsuccessful, check satellite for any commands
					LOG_INF("Running satellite module");
					// button_wait();
					satellite_module();
				}
			}

			if (mission_state == MISSION_WAIT_START)
			{
				event = EVT_IDLE;
			}
			else if (mission_state == MISSION_ONGOING)
			{
				event = EVT_DIVE;
			}
			else if (mission_state == MISSION_FINISHED)
			{
				event = EVT_AWAIT_MISSION;
			}

			break;
		case EVT_DIVE:
			LOG_INF("\nEvent: dive");
			glider.m_state.surfaced = 0;
			update_mission_status(MISSION_ONGOING);

			// function: send command to navigation

			LOG_INF("Running sensor module");
			// button_wait();
			// read sensor data
			date_time_now(&unix_time_ms);
			update_tm_all(unix_time_ms / 1000);
			sensor_module();

			glider.m_state.surfaced = 1;
			event = EVT_SURFACE;

			break;
		case EVT_SURFACE:
			LOG_INF("\nEvent: surface");

			glider.m_state.wp_cur++;
			LOG_INF("Current waypoint: %d", glider.m_state.wp_cur);
			update_mission_status(MISSION_ONGOING);

			publish_data_4G();

			LOG_INF("Running gps module");
			// button_wait();
			gps_module();

			LOG_INF("Running gcloud module");
			// button_wait();
			ret = gcloud_module();
			if (ret == 1)
			{
				// if unsuccessful, send data through satellite
				LOG_INF("Running satellite module");
				// button_wait();
				satellite_module();
			}
			if (mission_state == MISSION_WAIT_START)
			{
				event = EVT_IDLE;
			}
			else if (glider.m_state.wp_cur >= glider.m_param.wp_max)
			{
				LOG_INF("Last waypoint reached");
				event = EVT_AWAIT_MISSION;
				update_mission_status(MISSION_FINISHED);
			}
			else
			{
				event = EVT_DIVE;
			}

			break;
		case EVT_IDLE:
			LOG_INF("\nEvent: idle");

			date_time_now(&unix_time_ms);
			update_tm_all(unix_time_ms / 1000);
			LOG_INF("CET unix: %d, start time: %d", (int32_t)unix_time_CET, (int32_t)glider.m_param.m_start_s);
			if (unix_time_CET > glider.m_param.m_start_s)
			{
				LOG_INF("Start time OK, starting mission");
				event = EVT_DIVE;
			}
			else
			{
				update_mission_status(MISSION_WAIT_START);
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

				LOG_INF("Running gps module");
				// button_wait();
				// gps_module();

				event = EVT_IDLE;
			}

			break;
		default:
			LOG_ERR("Unknown event type");
			break;
		}
	}

	printk("**** NordicOasys test end ****\n");
}

/* Function ideas and TODOs

create function for mission updates, save to sd card
	- mission start
	- mission end
	CANDO: save updates to flash in case of reboot/ errors

CURRENT FLOW FOR HOW MISSION SEQUENCE WILL OPERATE

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
	uart_start(UART_1);

	printk("\nwifi test start\n");

	// send wifi command to wifi controller
	uart_send(UART_1, "wifi", strlen("wifi"));

	// wifi loop 
for (int i = 0;; i++)
{

	// test messages from wifi controller
	k_msgq_get(&uart_msg_q, &uart_msg, K_FOREVER);
	LOG_INF("%s", log_strdup(uart_msg));

	if (strcmp(uart_msg, "{exit}") == 0)
	{
		uart_exit(UART_1);
		break;
	}
	// wifi setup success, send further commands
	else if (strcmp(uart_msg, "{wifi ok}") == 0)
	{
		LOG_INF("\nReceived wifi ok");
		uart_send(UART_1, "{mqtt}\n", strlen("{mqtt}\n"));
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

	int ret;

	// mqtt init
	if (!mqtt_init_complete)
	{

		ret = app_mqtt_init_and_connect();
		if (ret != 0)
		{
			return ret;
		}

		mqtt_init_complete = true;
	}
	// reconnect
	else
	{

		ret = app_mqtt_connect();
		if (ret != 0)
		{
			return ret;
		}
	}

	// TODO: store mqtt commands into an array
	// MQTT loop
	while (1)
	{

		// mqtt function
		ret = app_mqtt_run();

		// test messages received from mqtt
		k_msgq_get(&mqtt_msg_q, &mqtt_msg, K_NO_WAIT);
		int mqtt_val = check_mqtt_msg(&mqtt_msg, sizeof(mqtt_msg));

		if (mqtt_val == 1 || ret != 0)
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

	ret = app_mqtt_disconnect();
	if (ret != 0)
	{
		printk("\nDisconnect unsuccessful: %d", ret);
		printk("\n\nClosing program, check for errors in code lol");
		return ret;
	}

	return 0;
}
*/