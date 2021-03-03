/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#include <zephyr.h>
#include <kernel_structs.h>
#include <stdio.h>
#include <string.h>
#include <gps.h>
#include <sensor.h>
#include <console.h>
#include <misc/reboot.h>
#include <logging/log_ctrl.h>
#if defined(CONFIG_BSD_LIBRARY)
#include <net/bsdlib.h>
#include <lte_lc.h>
#endif /* CONFIG_BSD_LIBRARY */
#include <net/socket.h>

#include "ui.h"
#include "gps_controller.h"
#include "cJSON.h"
#include "ntp.h"
#if defined(CONFIG_BLE_CONNECT_TO_MICRO_BIT)
#include "micro_bit_compass.h"
#endif /* CONFIG_BLE_CONNECT_TO_MICRO_BIT */
#include "path_module.h"
#include "gcloud.h"

#define CALIBRATION_PRESS_DURATION K_SECONDS(5)

#if defined(CONFIG_BSD_LIBRARY) && \
	!defined(CONFIG_LTE_LINK_CONTROL)
#error  "Missing CONFIG_LTE_LINK_CONTROL"
#endif

#define UNIQUE_PATH_ID_LENGTH 11

/* Global variables */
static char uniquePathID[UNIQUE_PATH_ID_LENGTH] = "00_0000000";

/* Sensor data */
static struct gps_data gps_data;

static atomic_val_t send_data_enable = 0;

/* Structures for work */
static struct k_work send_gps_data_work;
static struct k_work toggle_gps_work;
struct k_delayed_work gcloud_reboot_work;

enum error_type
{
	ERROR_CLOUD,
	ERROR_BSD_RECOVERABLE,
	ERROR_BSD_IRRECOVERABLE,
	ERROR_LTE_LC,
	ERROR_THREAD
};

/* Forward declaration of functions */
static void sensors_init(void);
static void work_init(void);
static void sensor_data_send(struct gps_data *data);
char *create_JSON_string(struct gps_data *pvt_data);

#if defined(CONFIG_GPS_SPOOF)
static void gps_enable_spoofing(void);
#endif

/**@brief error handler. */
void error_handler(enum error_type err_type, int err_code)
{
	if (err_type == ERROR_CLOUD)
	{
		if (gps_control_is_enabled())
		{
			printk("Cloud error while GPS enabled. Rebooting...\n");
			sys_reboot(0);
		}
#if defined(CONFIG_LTE_LINK_CONTROL)
		/* Turn off and shutdown modem */
		printk("LTE link disconnect\n");
		int err = lte_lc_power_off();
		if (err)
		{
			printk("lte_lc_power_off failed: %d\n", err);
		}
#endif
#if defined(CONFIG_BSD_LIBRARY)
		printk("Shutdown modem\n");
		bsdlib_shutdown();
#endif
	}

#if !defined(CONFIG_DEBUG) && defined(CONFIG_REBOOT)
	LOG_PANIC();
	sys_reboot(0);
#else
	switch (err_type)
	{
	case ERROR_CLOUD:
		/* Blinking all LEDs ON/OFF in pairs (1 and 4, 2 and 3)
		 * if there is an application error.
		 */
		ui_led_set_pattern(UI_LED_ERROR_CLOUD);
		printk("Error of type ERROR_CLOUD: %d\n", err_code);
		break;
	case ERROR_BSD_RECOVERABLE:
		/* Blinking all LEDs ON/OFF in pairs (1 and 3, 2 and 4)
		 * if there is a recoverable error.
		 */
		ui_led_set_pattern(UI_LED_ERROR_BSD_REC);
		printk("Error of type ERROR_BSD_RECOVERABLE: %d\n", err_code);
		break;
	case ERROR_BSD_IRRECOVERABLE:
		/* Blinking all LEDs ON/OFF if there is an
		 * irrecoverable error.
		 */
		ui_led_set_pattern(UI_LED_ERROR_BSD_IRREC);
		printk("Error of type ERROR_BSD_IRRECOVERABLE: %d\n", err_code);
		break;
	default:
		/* Blinking all LEDs ON/OFF in pairs (1 and 2, 3 and 4)
		 * undefined error.
		 */
		ui_led_set_pattern(UI_LED_ERROR_UNKNOWN);
		printk("Unknown error type: %d, code: %d\n",
			   err_type, err_code);
		break;
	}

	while (true)
	{
		k_cpu_idle();
	}
#endif /* CONFIG_DEBUG */
}

void z_SysFatalErrorHandler(unsigned int reason, const NANO_ESF *pEsf)
{
	ARG_UNUSED(pEsf);

#if !defined(CONFIG_SIMPLE_FATAL_ERROR_HANDLER)
#if defined(CONFIG_STACK_SENTINEL)
	if (reason == _NANO_ERR_STACK_CHK_FAIL)
	{
		goto error_handler;
	}
#endif
	if (reason == _NANO_ERR_KERNEL_PANIC)
	{
		goto error_handler;
	}

	if (z_is_thread_essential())
	{
		printk("Fatal fault in essential thread! Spinning...\n");
		goto error_handler;
	}

	printk("Fatal fault in thread %p! Aborting.\n", _current);
	k_thread_abort(_current);

	return;
#endif

error_handler:
	error_handler(ERROR_THREAD, reason);
}

void gcloud_error_handler(int err)
{
	error_handler(ERROR_CLOUD, err);
}

/**@brief Recoverable BSD library error. */
void bsd_recoverable_error_handler(uint32_t err)
{
	error_handler(ERROR_BSD_RECOVERABLE, (int)err);
}

/**@brief Irrecoverable BSD library error. */
void bsd_irrecoverable_error_handler(uint32_t err)
{
	error_handler(ERROR_BSD_IRRECOVERABLE, (int)err);
}

static void send_gps_data_work_fn(struct k_work *work)
{
	sensor_data_send(&gps_data);
}

/**@brief Callback for GPS trigger events */
static void gps_trigger_handler(struct device *dev, struct gps_trigger *trigger)
{
	static u32_t fix_count;

	ARG_UNUSED(trigger);

	if (!atomic_get(&send_data_enable))
	{
		printk("Error: Sending of data has not been enabled.");
		return;
	}

	if (++fix_count < CONFIG_GPS_CONTROL_FIX_COUNT)
	{
		return;
	}

	fix_count = 0;

	ui_led_set_state(UI_LED_1, 1);

	gps_channel_get(dev, GPS_CHAN_PVT, &gps_data);

	gps_control_stop(K_NO_WAIT);
	k_work_submit(&send_gps_data_work);
}

/**@brief Reads GPS data and returns the data in a JSON string format. */
char *create_JSON_string(struct gps_data *pvt_data)
{
	cJSON *pvt_JSON = cJSON_CreateObject();
	char *JSONString = NULL;
	if (pvt_JSON == NULL)
	{
		goto end;
	}

	cJSON *data = NULL;
	cJSON *latitude = NULL;
	cJSON *longitude = NULL;
	cJSON *altitude = NULL;
	cJSON *speed = NULL;
	cJSON *heading = NULL;
	cJSON *datetime_year = NULL;
	cJSON *datetime_month = NULL;
	cJSON *datetime_day = NULL;
	cJSON *datetime_hour = NULL;
	cJSON *datetime_minute = NULL;
	cJSON *datetime_seconds = NULL;
	cJSON *path_id = NULL;

	memcpy(uniquePathID, path_module_get_path_id(), UNIQUE_PATH_ID_LENGTH);

	data = cJSON_CreateObject();
	if (data == NULL)
	{
		goto end;
	}

	latitude = cJSON_CreateNumber(pvt_data->pvt.latitude);
	if (latitude == NULL)
	{
		goto end;
	}
	cJSON_AddItemToObject(data, "latitude", latitude);

	longitude = cJSON_CreateNumber(pvt_data->pvt.longitude);
	if (longitude == NULL)
	{
		goto end;
	}
	cJSON_AddItemToObject(data, "longitude", longitude);

	altitude = cJSON_CreateNumber(pvt_data->pvt.altitude);
	if (altitude == NULL)
	{
		goto end;
	}
	cJSON_AddItemToObject(data, "altitude", altitude);

	speed = cJSON_CreateNumber(pvt_data->pvt.speed);
	if (speed == NULL)
	{
		goto end;
	}
	cJSON_AddItemToObject(data, "speed", speed);

	heading = cJSON_CreateNumber(pvt_data->pvt.heading);
	if (heading == NULL)
	{
		goto end;
	}
	cJSON_AddItemToObject(data, "heading", heading);

	datetime_year = cJSON_CreateNumber(pvt_data->pvt.datetime.year);
	if (datetime_year == NULL)
	{
		goto end;
	}
	cJSON_AddItemToObject(data, "datetime_year", datetime_year);

	datetime_month = cJSON_CreateNumber(pvt_data->pvt.datetime.month);
	if (datetime_month == NULL)
	{
		goto end;
	}
	cJSON_AddItemToObject(data, "datetime_month", datetime_month);

	datetime_day = cJSON_CreateNumber(pvt_data->pvt.datetime.day);
	if (datetime_day == NULL)
	{
		goto end;
	}
	cJSON_AddItemToObject(data, "datetime_day", datetime_day);

	datetime_hour = cJSON_CreateNumber(pvt_data->pvt.datetime.hour);
	if (datetime_hour == NULL)
	{
		goto end;
	}
	cJSON_AddItemToObject(data, "datetime_hour", datetime_hour);

	datetime_minute = cJSON_CreateNumber(pvt_data->pvt.datetime.minute);
	if (datetime_minute == NULL)
	{
		goto end;
	}
	cJSON_AddItemToObject(data, "datetime_minute", datetime_minute);

	datetime_seconds = cJSON_CreateNumber(pvt_data->pvt.datetime.seconds);
	if (datetime_seconds == NULL)
	{
		goto end;
	}
	cJSON_AddItemToObject(data, "datetime_seconds", datetime_seconds);

	path_id = cJSON_CreateString(uniquePathID);
	if (path_id == NULL)
	{
		goto end;
	}
	cJSON_AddItemToObject(data, "path_id", path_id);

	cJSON_AddItemToObject(pvt_JSON, "data", data);
	JSONString = cJSON_Print(pvt_JSON);

end:
	cJSON_Delete(pvt_JSON);
	return JSONString;
}

/**@brief Send sensor data to gcloud. **/
static void sensor_data_send(struct gps_data *data)
{
	int err = 0;
	char latStr[20];
	char lngStr[20];

	if (!atomic_get(&send_data_enable) || gps_control_is_active())
	{
		return;
	}

	sprintf(latStr, "%.5f", data->pvt.latitude);
	sprintf(lngStr, "%.5f", data->pvt.longitude);
	printk("Publishing new position: latitude: %s | longitude: %s\n", latStr, lngStr);

	char *JSONString = create_JSON_string(data);
	if (JSONString == NULL)
	{
		printk("sensor_data_send failed: %s\n", strerror(ENOMEM));
		gcloud_error_handler(-ENOMEM);
		return;
	}

	path_module_provide_position(data);
	err = gcloud_publish_gps(JSONString, strlen(JSONString), MQTT_QOS_1_AT_LEAST_ONCE);
	free(JSONString);
	if (err)
	{
		printk("sensor_data_send failed: %s\n", strerror(err));
		gcloud_error_handler(err);
	}
}

#if defined(CONFIG_GPS_SPOOF)
/**@brief Spoofs GPS data, and sends it to gcloud. **/
static void gps_enable_spoofing(void)
{
	gps_data.pvt.latitude = 63.421100;
	gps_data.pvt.longitude = 10.437060;
	gps_data.pvt.altitude = 49;
	gps_data.pvt.speed = 10;
	gps_data.pvt.heading = 3;
	gps_data.pvt.datetime.year = 1066;
	gps_data.pvt.datetime.month = 9;
	gps_data.pvt.datetime.day = 25;
	gps_data.pvt.datetime.hour = 12;
	gps_data.pvt.datetime.minute = 19;
	gps_data.pvt.datetime.seconds = 37;

	atomic_set(&send_data_enable, 1);
}
#endif

/**@brief Reboot the device if CONNACK has not arrived. */
static void gcloud_reboot_handler(struct k_work *work)
{
	error_handler(ERROR_CLOUD, 0);
}

/**@brief Enable sending of data and init sensors. */
void sensors_start(void)
{
	sensors_init();
	atomic_set(&send_data_enable, 1);
}

/**@brief Start or Stop the GPS, if the GPS controller is initialized. */
static void toggle_gps_work_fn(struct k_work *work)
{
#if !defined(CONFIG_GPS_SPOOF)

	if (!atomic_get(&send_data_enable))
	{
		printk("Link not ready, gps toggle disregarded.\n");
		return;
	}

	if (gps_control_is_enabled())
	{
		printk("Stopping GPS\n");
		gps_control_disable();
	}
	else
	{
		printk("Starting GPS\n");
		gps_control_enable();
	}
#endif
}

/**@brief Initializes and submits delayed work. */
static void work_init(void)
{
	k_work_init(&send_gps_data_work, send_gps_data_work_fn);
	k_work_init(&toggle_gps_work, toggle_gps_work_fn);
	k_delayed_work_init(&gcloud_reboot_work, gcloud_reboot_handler);
}

/**@brief Configures modem to provide LTE link. Blocks until link is
 * successfully established.
 */
static void modem_configure(void)
{
#if defined(CONFIG_BSD_LIBRARY)
	if (IS_ENABLED(CONFIG_LTE_AUTO_INIT_AND_CONNECT))
	{
		/* Do nothing, modem is already turned on
		 * and connected.
		 */
	}
	else
	{
		int err;

		printk("Connecting to LTE network. ");
		printk("This may take several minutes.\n");
		ui_led_set_pattern(UI_LTE_CONNECTING);

		err = lte_lc_init_and_connect();
		if (err)
		{
			printk("LTE link could not be established.\n");
			error_handler(ERROR_LTE_LC, err);
			return;
		}

		printk("Connected to LTE network\n");
		ui_led_set_pattern(UI_LTE_CONNECTED);
	}
#endif
}

/**@brief Initializes the sensors that are used by the application. */
static void sensors_init(void)
{
	gps_control_init(gps_trigger_handler);
}

/**@brief User interface event handler. */
static void ui_evt_handler(struct ui_evt evt)
{
	if (IS_ENABLED(CONFIG_GPS_CONTROL_ON_SWITCH_1_TOGGLE) &&
		(evt.button == UI_SWITCH_1))
	{
		k_work_submit(&toggle_gps_work);
	}
}

void main(void)
{
	int ret;

	printk("Program started\n");
#if defined(CONFIG_BLE_CONNECT_TO_MICRO_BIT)
	micro_bit_compass_init();
	while (!micro_bit_get_ble_connected())
	{
		k_sleep(K_MSEC(100));
	}
#endif /* CONFIG_BLE_CONNECT_TO_MICRO_BIT */

	ret = gcloud_provision();
	if (ret)
	{
		printk("Failed to provision: %d\n", ret);
		gcloud_error_handler(ret);
	}
	else
	{
		printk("Provisioning complete\n");
	}

	ui_init(ui_evt_handler);
	path_module_init();
	work_init();
	modem_configure();

	gcloud_start_thread();

	ui_led_set_pattern(UI_CLOUD_CONNECTING);
	ui_led_set_state(UI_LED_3, 1);

	while (!gcloud_get_connection_status())
	{
		k_sleep(K_SECONDS(1));
	}

#if defined(CONFIG_GPS_SPOOF)
	gps_enable_spoofing();
	while (1)
	{
		k_work_submit(&send_gps_data_work);
		k_sleep(K_SECONDS(5));
	}
#else
	//TODO: fix LED signalling here
	printk("GPS is waiting for PSM...\n");
	k_sleep(K_SECONDS(15));
	sensors_start();
	ui_led_set_pattern(UI_CLOUD_CONNECTED);
	printk("GPS is ready to go!\n");
#if defined(CONFIG_GPS_AUTO_START)
	k_work_submit(&toggle_gps_work);
#endif /* CONFIG_GPS_AUTO_START */
#endif /* CONFIG_GPS_SPOOF */
}
