/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#include <zephyr.h>
#include <nrf_socket.h>
#include <net/socket.h>
#include <stdio.h>
#include <string.h>
#include <modem/at_cmd.h>
#include <modem/at_notif.h>
#include <cJSON.h>
#include <time.h>
#include <date_time.h>

#include "app_gps.h"

LOG_MODULE_REGISTER(app_gps);

#ifdef CONFIG_SUPL_CLIENT_LIB
#include <supl_os_client.h>
#include <supl_session.h>
#include "supl_support.h"
#endif

#define AT_XSYSTEMMODE "AT\%XSYSTEMMODE=1,0,1,0"
#define AT_DEACTIVATE_GPS "AT+CFUN=30"
#define AT_ACTIVATE_GPS "AT+CFUN=31"
#define AT_ACTIVATE_LTE "AT+CFUN=21"
#define AT_DEACTIVATE_LTE "AT+CFUN=20"

#define GNSS_INIT_AND_START 1
#define GNSS_STOP 2
#define GNSS_RESTART 3

#define AT_CMD_SIZE(x) (sizeof(x) - 1)

#ifdef CONFIG_BOARD_NRF9160DK_NRF9160NS
#define AT_MAGPIO "AT\%XMAGPIO=1,0,0,1,1,1574,1577"
#ifdef CONFIG_GPS_SAMPLE_ANTENNA_ONBOARD
#define AT_COEX0 "AT\%XCOEX0=1,1,1565,1586"
#elif CONFIG_GPS_SAMPLE_ANTENNA_EXTERNAL
#define AT_COEX0 "AT\%XCOEX0"
#endif
#endif /* CONFIG_BOARD_NRF9160DK_NRF9160NS */

static const char update_indicator[] = {'\\', '|', '/', '-'};
static const char *const at_commands[] = {
	AT_XSYSTEMMODE,
#if defined(CONFIG_BOARD_NRF9160DK_NRF9160NS)
	AT_MAGPIO,
	AT_COEX0,
#endif
	AT_ACTIVATE_GPS};

static int gnss_fd;
static char nmea_strings[10][NRF_GNSS_NMEA_MAX_LEN];
static uint32_t nmea_string_cnt;

static bool got_fix;
static uint64_t fix_timestamp;
static nrf_gnss_data_frame_t last_pvt;

K_SEM_DEFINE(lte_ready, 0, 1);

// CUSTOM DECLARATIONS

extern struct k_msgq gps_msg_q;
static bool init_complete = false;

static int setup_modem(void)
{
	for (int i = 0; i < ARRAY_SIZE(at_commands); i++)
	{

		if (at_cmd_write(at_commands[i], NULL, 0, NULL) != 0)
		{
			LOG_ERR("Unable to execute AT command gps start");
			return -1;
		}
	}

	return 0;
}

#ifdef CONFIG_SUPL_CLIENT_LIB
/* Accepted network statuses read from modem */
static const char status1[] = "+CEREG: 1";
static const char status2[] = "+CEREG:1";
static const char status3[] = "+CEREG: 5";
static const char status4[] = "+CEREG:5";

static void wait_for_lte(void *context, const char *response)
{
	if (!memcmp(status1, response, AT_CMD_SIZE(status1)) ||
		!memcmp(status2, response, AT_CMD_SIZE(status2)) ||
		!memcmp(status3, response, AT_CMD_SIZE(status3)) ||
		!memcmp(status4, response, AT_CMD_SIZE(status4)))
	{
		k_sem_give(&lte_ready);
	}
}

static int activate_lte(bool activate)
{
	if (activate)
	{
		if (at_cmd_write(AT_ACTIVATE_LTE, NULL, 0, NULL) != 0)
		{
			return -1;
		}

		at_notif_register_handler(NULL, wait_for_lte);
		if (at_cmd_write("AT+CEREG=2", NULL, 0, NULL) != 0)
		{
			return -1;
		}

		k_sem_take(&lte_ready, K_FOREVER);

		at_notif_deregister_handler(NULL, wait_for_lte);
		if (at_cmd_write("AT+CEREG=0", NULL, 0, NULL) != 0)
		{
			return -1;
		}
	}
	else
	{
		if (at_cmd_write(AT_DEACTIVATE_LTE, NULL, 0, NULL) != 0)
		{
			return -1;
		}
	}

	return 0;
}
#endif

static int gnss_ctrl(uint32_t ctrl)
{
	int retval;

	nrf_gnss_fix_retry_t fix_retry = 0;
	nrf_gnss_fix_interval_t fix_interval = 1;
	nrf_gnss_delete_mask_t delete_mask = 0;
	nrf_gnss_nmea_mask_t nmea_mask = NRF_GNSS_NMEA_GSV_MASK |
									 NRF_GNSS_NMEA_GSA_MASK |
									 NRF_GNSS_NMEA_GLL_MASK |
									 NRF_GNSS_NMEA_GGA_MASK |
									 NRF_GNSS_NMEA_RMC_MASK;

	if (ctrl == GNSS_INIT_AND_START)
	{
		gnss_fd = nrf_socket(NRF_AF_LOCAL,
							 NRF_SOCK_DGRAM,
							 NRF_PROTO_GNSS);

		if (gnss_fd >= 0)
		{
			LOG_INF("GPS Socket created");
		}
		else
		{
			LOG_ERR("Could not init socket (err: %d)", gnss_fd);
			return -1;
		}

		retval = nrf_setsockopt(gnss_fd,
								NRF_SOL_GNSS,
								NRF_SO_GNSS_FIX_RETRY,
								&fix_retry,
								sizeof(fix_retry));
		if (retval != 0)
		{
			LOG_ERR("Failed to set fix retry value");
			return -1;
		}

		retval = nrf_setsockopt(gnss_fd,
								NRF_SOL_GNSS,
								NRF_SO_GNSS_FIX_INTERVAL,
								&fix_interval,
								sizeof(fix_interval));
		if (retval != 0)
		{
			LOG_ERR("Failed to set fix interval value");
			return -1;
		}

		retval = nrf_setsockopt(gnss_fd,
								NRF_SOL_GNSS,
								NRF_SO_GNSS_NMEA_MASK,
								&nmea_mask,
								sizeof(nmea_mask));
		if (retval != 0)
		{
			LOG_ERR("Failed to set nmea mask");
			return -1;
		}
	}

	if ((ctrl == GNSS_INIT_AND_START) ||
		(ctrl == GNSS_RESTART))
	{
		retval = nrf_setsockopt(gnss_fd,
								NRF_SOL_GNSS,
								NRF_SO_GNSS_START,
								&delete_mask,
								sizeof(delete_mask));
		if (retval != 0)
		{
			LOG_ERR("Failed to start GPS");
			return -1;
		}
	}

	if (ctrl == GNSS_STOP)
	{
		retval = nrf_setsockopt(gnss_fd,
								NRF_SOL_GNSS,
								NRF_SO_GNSS_STOP,
								&delete_mask,
								sizeof(delete_mask));
		if (retval != 0)
		{
			LOG_ERR("Failed to stop GPS");
			return -1;
		}
	}

	return 0;
}

static int gps_init()
{
	int retval;

	retval = setup_modem();
	if (retval != 0)
	{
		return retval;
	}
	retval = gnss_ctrl(GNSS_INIT_AND_START);
	if (retval != 0)
	{
		return retval;
	}

	return 0;
}

static int gps_reinit()
{
	int retval;

	retval = setup_modem();
	if (retval != 0)
	{
		return retval;
	}
	retval = gnss_ctrl(GNSS_RESTART);
	if (retval != 0)
	{
		return retval;
	}

	return retval;
}

static void print_satellite_stats(nrf_gnss_data_frame_t *pvt_data)
{
	uint8_t tracked = 0;
	uint8_t in_fix = 0;
	uint8_t unhealthy = 0;

	for (int i = 0; i < NRF_GNSS_MAX_SATELLITES; ++i)
	{

		if ((pvt_data->pvt.sv[i].sv > 0) &&
			(pvt_data->pvt.sv[i].sv < 33))
		{

			tracked++;

			if (pvt_data->pvt.sv[i].flags &
				NRF_GNSS_SV_FLAG_USED_IN_FIX)
			{
				in_fix++;
			}

			if (pvt_data->pvt.sv[i].flags &
				NRF_GNSS_SV_FLAG_UNHEALTHY)
			{
				unhealthy++;
			}
		}
	}

	LOG_INF("Tracking: %d Using: %d Unhealthy: %d", tracked,
			in_fix,
			unhealthy);
}

static void print_gnss_stats(nrf_gnss_data_frame_t *pvt_data)
{
	if (pvt_data->pvt.flags & NRF_GNSS_PVT_FLAG_DEADLINE_MISSED)
	{
		LOG_INF("GNSS notification deadline missed");
	}
	if (pvt_data->pvt.flags & NRF_GNSS_PVT_FLAG_NOT_ENOUGH_WINDOW_TIME)
	{
		LOG_INF("GNSS operation blocked by insufficient time windows");
	}
}

static void print_fix_data(nrf_gnss_data_frame_t *pvt_data)
{
	LOG_INF("Longitude:  %f", pvt_data->pvt.longitude);
	LOG_INF("Latitude:   %f", pvt_data->pvt.latitude);
	LOG_INF("Date:       %02u-%02u-%02u", pvt_data->pvt.datetime.year,
			pvt_data->pvt.datetime.month,
			pvt_data->pvt.datetime.day);
	LOG_INF("Time (UTC): %02u:%02u:%02u", pvt_data->pvt.datetime.hour,
			pvt_data->pvt.datetime.minute,
			pvt_data->pvt.datetime.seconds);
}

static void print_nmea_data(void)
{
	for (int i = 0; i < nmea_string_cnt; ++i)
	{
		LOG_INF("%s", nmea_strings[i]);
	}
}

int process_gps_data(nrf_gnss_data_frame_t *gps_data)
{
	int retval;

	retval = nrf_recv(gnss_fd,
					  gps_data,
					  sizeof(nrf_gnss_data_frame_t),
					  NRF_MSG_DONTWAIT);

	if (retval > 0)
	{

		switch (gps_data->data_id)
		{
		case NRF_GNSS_PVT_DATA_ID:
			memcpy(&last_pvt,
				   gps_data,
				   sizeof(nrf_gnss_data_frame_t));
			nmea_string_cnt = 0;
			got_fix = false;

			if ((gps_data->pvt.flags &
				 NRF_GNSS_PVT_FLAG_FIX_VALID_BIT) == NRF_GNSS_PVT_FLAG_FIX_VALID_BIT)
			{

				got_fix = true;
				fix_timestamp = k_uptime_get();
			}
			break;

		case NRF_GNSS_NMEA_DATA_ID:
			if (nmea_string_cnt < 10)
			{
				memcpy(nmea_strings[nmea_string_cnt++],
					   gps_data->nmea,
					   retval);
			}
			break;

		case NRF_GNSS_AGPS_DATA_ID:
#ifdef CONFIG_SUPL_CLIENT_LIB
			LOG_INF("\033[1;1H");
			LOG_INF("\033[2J");
			LOG_INF("New AGPS data requested, contacting SUPL server, flags %d",
					gps_data->agps.data_flags);
			gnss_ctrl(GNSS_STOP);
			activate_lte(true);
			LOG_INF("Established LTE link");
			if (open_supl_socket() == 0)
			{
				LOG_INF("Starting SUPL session");
				supl_session(&gps_data->agps);
				LOG_INF("Done");
				close_supl_socket();
			}
			activate_lte(false);
			gnss_ctrl(GNSS_RESTART);
			k_sleep(K_MSEC(2000));
#endif
			break;

		default:
			break;
		}
	}

	return retval;
}

#ifdef CONFIG_SUPL_CLIENT_LIB
int inject_agps_type(void *agps,
					 size_t agps_size,
					 nrf_gnss_agps_data_type_t type,
					 void *user_data)
{
	ARG_UNUSED(user_data);
	int retval = nrf_sendto(gnss_fd,
							agps,
							agps_size,
							0,
							&type,
							sizeof(type));

	if (retval != 0)
	{
		LOG_ERR("Failed to send AGNSS data, type: %d (err: %d)",
				type,
				errno);
		return -1;
	}

	LOG_INF("Injected AGPS data, flags: %d, size: %d\n", type, agps_size);

	return 0;
}
#endif

// Creates JSON version of desired gps data, converts back to string in JSON format
static int gnss_data_to_JSON(void *gps_str, nrf_gnss_data_frame_t *pvt_data)
{
	cJSON *gps_JSON = cJSON_CreateObject();
	cJSON *gps_data = cJSON_CreateObject();

	uint8_t ts_string[64] = "";
	uint8_t date_string[64] = "";
	uint8_t tmp_str[16] = "";

	snprintf(tmp_str, sizeof(tmp_str), "%.4f", pvt_data->pvt.latitude);
	cJSON_AddStringToObject(gps_data, "lat", tmp_str);

	snprintf(tmp_str, sizeof(tmp_str), "%.4f", pvt_data->pvt.longitude);
	cJSON_AddStringToObject(gps_data, "lng", tmp_str);

	// adding date to JSON
	snprintf(tmp_str, sizeof(tmp_str), "%02u-", pvt_data->pvt.datetime.year);
	strcat(date_string, tmp_str);
	snprintf(tmp_str, sizeof(tmp_str), "%02u-", pvt_data->pvt.datetime.month);
	strcat(date_string, tmp_str);
	snprintf(tmp_str, sizeof(tmp_str), "%02u", pvt_data->pvt.datetime.day);
	strcat(date_string, tmp_str);

	// adding timestamp to JSON
	snprintf(tmp_str, sizeof(tmp_str), "%02u:", pvt_data->pvt.datetime.hour);
	strcat(ts_string, tmp_str);
	snprintf(tmp_str, sizeof(tmp_str), "%02u:", pvt_data->pvt.datetime.minute);
	strcat(ts_string, tmp_str);
	snprintf(tmp_str, sizeof(tmp_str), "%02u", pvt_data->pvt.datetime.seconds);
	strcat(ts_string, tmp_str);

	cJSON_AddItemToObject(gps_JSON, "data", gps_data);
	cJSON_AddStringToObject(gps_JSON, "dt", date_string);
	cJSON_AddStringToObject(gps_JSON, "ts", ts_string);

	strcpy(gps_str, cJSON_Print(gps_JSON));
	cJSON_Minify(gps_str);

	// cJSON_Delete(gps_data);
	cJSON_Delete(gps_JSON);

	return 0;
}

// Creates dummy data. Values are constant
static int create_dummy_gps_data(nrf_gnss_data_frame_t *pvt_data)
{
	got_fix = 1;

	pvt_data->pvt.longitude = 23.771611;
	pvt_data->pvt.latitude = 61.491278;
	pvt_data->pvt.altitude = 116.274658;
	pvt_data->pvt.speed = 0.039595;
	pvt_data->pvt.heading = 0.000000;

	pvt_data->pvt.datetime.year = 2021;
	pvt_data->pvt.datetime.month = 04;
	pvt_data->pvt.datetime.day = 22;
	pvt_data->pvt.datetime.hour = 19;
	pvt_data->pvt.datetime.minute = 07;
	pvt_data->pvt.datetime.seconds = 34;

	LOG_INF("Created dummy gps data");

	return 0;
}

static int gps_fill_struct(glider_gps_data_t *app_gps_data, nrf_gnss_data_frame_t *pvt_data, void *gps_string)
{
	strcpy(app_gps_data->gps_string, gps_string);

	app_gps_data->time.tm_year = pvt_data->pvt.datetime.year - 1900;
	app_gps_data->time.tm_mon = pvt_data->pvt.datetime.month - 1;
	app_gps_data->time.tm_mday = pvt_data->pvt.datetime.day;
	app_gps_data->time.tm_hour = pvt_data->pvt.datetime.hour;
	app_gps_data->time.tm_min = pvt_data->pvt.datetime.minute;
	app_gps_data->time.tm_sec = pvt_data->pvt.datetime.seconds;

	return 0;
}

int app_gps(glider_gps_data_t *app_gps_data, int64_t gps_timeout, int retry_interval)
{
	int retval = 0;

	nrf_gnss_data_frame_t gps_data;
	got_fix = false;

	uint8_t gps_string[128] = "";
	int64_t start_time = k_uptime_get();
	int64_t current_time = 0;

	int cnt = 0;

	LOG_INF("Starting GPS application");

	// first time gps runs
	if (init_complete == false)
	{

		if (gps_init() != 0)
		{
			LOG_ERR("Error in gps initialization");
			return -1;
		}

		init_complete = true;
	}

	// Reconnect
	else
	{
		if (gps_reinit() != 0)
		{
			LOG_ERR("Error in gps reinitialization");
			return -1;
		}
	}

	LOG_INF("Getting GPS data...");
	while ((current_time) <= gps_timeout)
	{

		current_time = k_uptime_get() - start_time;

		do
		{
			/* Loop until we don't have more
			 * data to read
			 */
		} while (process_gps_data(&gps_data) > 0);

		if (IS_ENABLED(CONFIG_GPS_SAMPLE_NMEA_ONLY))
		{
			print_nmea_data();
			nmea_string_cnt = 0;
		}
		else
		{
			// LOG_INF("\033[1;1H");
			// LOG_INF("\033[2J");
			print_satellite_stats(&last_pvt);
			print_gnss_stats(&last_pvt);
			LOG_INF("---------------------------------");

			if (!got_fix)
			{
				LOG_INF("Searching [%c] ---- Elapsed time: %d s",
						update_indicator[cnt++ % 4], (int32_t)(current_time) / 1000);
			}
			else
			{
				LOG_INF("Got fix");
				print_fix_data(&last_pvt);
				break;
			}

			// LOG_INF("\nNMEA strings:\n\n");
			// print_nmea_data();
			// LOG_INF("---------------------------------");
		}

		k_sleep(K_MSEC(retry_interval));
	}

	if (!got_fix)
	{
		// create dummy data, REMINDER: COMMENT WHEN RUNNING PROPER TESTS
		create_dummy_gps_data(&last_pvt);
		gnss_data_to_JSON(&gps_string, &last_pvt);
		gps_fill_struct(app_gps_data, &last_pvt, &gps_string);

		// retval = 1;
	}
	else
	{
		// fill out struct
		gnss_data_to_JSON(&gps_string, &last_pvt);
		gps_fill_struct(app_gps_data, &last_pvt, &gps_string);
	}

	LOG_INF("Stopping GPS module");

	// AT command to turn off GPS
	retval = gnss_ctrl(GNSS_STOP);
	if (at_cmd_write(AT_DEACTIVATE_GPS, NULL, 0, NULL) != 0)
	{
		LOG_ERR("\nunable to deactivate GPS");
	}

	return retval;
}
