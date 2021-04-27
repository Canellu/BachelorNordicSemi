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

#include "app_gps.h"
#include "cJSON.h"

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
			printk("GPS Socket created\n");
		}
		else
		{
			printk("Could not init socket (err: %d)\n", gnss_fd);
			return -1;
		}

		retval = nrf_setsockopt(gnss_fd,
								NRF_SOL_GNSS,
								NRF_SO_GNSS_FIX_RETRY,
								&fix_retry,
								sizeof(fix_retry));
		if (retval != 0)
		{
			printk("Failed to set fix retry value\n");
			return -1;
		}

		retval = nrf_setsockopt(gnss_fd,
								NRF_SOL_GNSS,
								NRF_SO_GNSS_FIX_INTERVAL,
								&fix_interval,
								sizeof(fix_interval));
		if (retval != 0)
		{
			printk("Failed to set fix interval value\n");
			return -1;
		}

		retval = nrf_setsockopt(gnss_fd,
								NRF_SOL_GNSS,
								NRF_SO_GNSS_NMEA_MASK,
								&nmea_mask,
								sizeof(nmea_mask));
		if (retval != 0)
		{
			printk("Failed to set nmea mask\n");
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
			printk("Failed to start GPS\n");
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
			printk("Failed to stop GPS\n");
			return -1;
		}
	}

	return 0;
}

static int init_app(void)
{
	int retval;

	if (setup_modem() != 0)
	{
		printk("Failed to initialize modem\n");
		return -1;
	}

	retval = gnss_ctrl(GNSS_INIT_AND_START);

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

	printk("Tracking: %d Using: %d Unhealthy: %d\n", tracked,
		   in_fix,
		   unhealthy);
}

static void print_gnss_stats(nrf_gnss_data_frame_t *pvt_data)
{
	if (pvt_data->pvt.flags & NRF_GNSS_PVT_FLAG_DEADLINE_MISSED)
	{
		printk("GNSS notification deadline missed\n");
	}
	if (pvt_data->pvt.flags & NRF_GNSS_PVT_FLAG_NOT_ENOUGH_WINDOW_TIME)
	{
		printk("GNSS operation blocked by insufficient time windows\n");
	}
}

static void print_fix_data(nrf_gnss_data_frame_t *pvt_data)
{
	printf("Longitude:  %f\n", pvt_data->pvt.longitude);
	printf("Latitude:   %f\n", pvt_data->pvt.latitude);
	printk("Date:       %02u-%02u-%02u\n", pvt_data->pvt.datetime.year,
		   pvt_data->pvt.datetime.month,
		   pvt_data->pvt.datetime.day);
	printk("Time (UTC): %02u:%02u:%02u\n", pvt_data->pvt.datetime.hour,
		   pvt_data->pvt.datetime.minute,
		   pvt_data->pvt.datetime.seconds);
}

static void print_nmea_data(void)
{
	for (int i = 0; i < nmea_string_cnt; ++i)
	{
		printk("%s", nmea_strings[i]);
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
			printk("\033[1;1H");
			printk("\033[2J");
			printk("New AGPS data requested, contacting SUPL server, flags %d\n",
				   gps_data->agps.data_flags);
			gnss_ctrl(GNSS_STOP);
			activate_lte(true);
			printk("Established LTE link\n");
			if (open_supl_socket() == 0)
			{
				printf("Starting SUPL session\n");
				supl_session(&gps_data->agps);
				printk("Done\n");
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
		printk("Failed to send AGNSS data, type: %d (err: %d)\n",
			   type,
			   errno);
		return -1;
	}

	printk("Injected AGPS data, flags: %d, size: %d\n", type, agps_size);

	return 0;
}
#endif

// Creates JSON version of desired gps data, converts back to string in JSON format
static int gps_struct_to_JSON(void *gps_str, nrf_gnss_data_frame_t *pvt_data)
{
	cJSON *gps_JSON = cJSON_CreateObject();
	cJSON *gps_data = cJSON_CreateObject();

	cJSON_AddNumberToObject(gps_data, "lat", pvt_data->pvt.latitude);
	cJSON_AddNumberToObject(gps_data, "lng", pvt_data->pvt.longitude);

	uint8_t ts_string[64] = "";
	uint8_t temp_str[16] = "";

	snprintf(temp_str, sizeof(temp_str), "%02u:", pvt_data->pvt.datetime.hour);
	strcat(ts_string, temp_str);
	snprintf(temp_str, sizeof(temp_str), "%02u:", pvt_data->pvt.datetime.minute);
	strcat(ts_string, temp_str);
	snprintf(temp_str, sizeof(temp_str), "%02u", pvt_data->pvt.datetime.seconds);
	strcat(ts_string, temp_str);

	cJSON_AddItemToObject(gps_JSON, "data", gps_data);
	cJSON_AddStringToObject(gps_JSON, "ts", ts_string);

	strcpy(gps_str, cJSON_Print(gps_JSON));
	cJSON_Minify(gps_str);

	cJSON_Delete(gps_data);
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

	printk("\n\nCreated dummy gps data");

	return 0;
}

static int glider_gps_fill(glider_gps_data_t *app_gps_data, nrf_gnss_data_frame_t *pvt_data, void *gps_string)
{
	strcpy(app_gps_data->gps_string, gps_string);

	app_gps_data->year = pvt_data->pvt.datetime.year;
	app_gps_data->month = pvt_data->pvt.datetime.month;
	app_gps_data->day = pvt_data->pvt.datetime.day;
	app_gps_data->hour = pvt_data->pvt.datetime.hour;
	app_gps_data->minute = pvt_data->pvt.datetime.minute;
	app_gps_data->seconds = pvt_data->pvt.datetime.seconds;

	return 0;
}

int app_gps(glider_gps_data_t *app_gps_data, int64_t gps_timeout, int retry_interval)
{
	nrf_gnss_data_frame_t gps_data;
	got_fix = false;

	uint8_t gps_string[128] = "";
	int64_t current_time = 0;
	int64_t start_time = k_uptime_get();

	int cnt = 0;

	printk("Starting GPS application\n");

	// Runs first time init
	// CANDO: create separate function and run in main?
	if (init_complete == false)
	{

		if (init_app() != 0)
		{
			return -1;
		}

		init_complete = true;
	}

	// Reconnect
	else
	{
		setup_modem();
		gnss_ctrl(GNSS_RESTART);
	}

	printk("Getting GPS data...\n");
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
			// printk("\033[1;1H");
			// printk("\033[2J");
			print_satellite_stats(&last_pvt);
			print_gnss_stats(&last_pvt);
			printk("---------------------------------\n");

			if (!got_fix)
			{
				printk("Searching [%c] ---- Elapsed time: %lld s\n",
					   update_indicator[cnt++ % 4], (current_time) / 1000);
			}
			else
			{
				printk("\n\nGot fix\n");
				print_fix_data(&last_pvt);
				break;
			}

			// printk("\nNMEA strings:\n\n");
			// print_nmea_data();
			// printk("---------------------------------");
		}

		k_sleep(K_MSEC(retry_interval));
	}

	if (!got_fix)
	{
		strcpy(gps_string, "no fix");

		// create dummy data, REMINDER: COMMENT WHEN RUNNING PROPER TESTS
		create_dummy_gps_data(&last_pvt);
		gps_struct_to_JSON(&gps_string, &last_pvt);
	}
	else
	{
		gps_struct_to_JSON(&gps_string, &last_pvt);
		got_fix = 0;
	}

	// fill out struct to add to message queue
	glider_gps_fill(app_gps_data, &last_pvt, &gps_string);

	printk("\nStopping GPS module");

	// AT command to turn off GPS
	gnss_ctrl(GNSS_STOP);
	if (at_cmd_write(AT_DEACTIVATE_GPS, NULL, 0, NULL) != 0)
	{
		printk("\nunable to deactivate GPS\n");
	}

	return 0;
}
