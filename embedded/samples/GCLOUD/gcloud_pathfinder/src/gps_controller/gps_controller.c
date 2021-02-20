/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#include <zephyr.h>
#include <misc/util.h>
#include <gps.h>
#include <lte_lc.h>

#include "ui.h"
#include "gps_controller.h"

#include <logging/log.h>
LOG_MODULE_REGISTER(gps_control, CONFIG_GPS_CONTROL_LOG_LEVEL);

/* Structure to hold GPS work information */
static struct
{
	enum
	{
		GPS_WORK_START,
		GPS_WORK_STOP
	} type;
	struct k_delayed_work work;
	struct device *dev;
} gps_work;

static atomic_t gps_is_active;
static atomic_t gps_is_enabled;

static int start(void)
{
	int err;

	if (IS_ENABLED(CONFIG_GPS_CONTROL_PSM_ENABLE_ON_START))
	{
		LOG_DBG("Enabling PSM");

		err = lte_lc_psm_req(true);
		if (err)
		{
			LOG_DBG("PSM mode could not be enabled");
			LOG_DBG(" or was already enabled.");
		}
		else
		{
			LOG_DBG("PSM enabled");
		}

		k_sleep(K_SECONDS(1));
	}

	err = gps_start(gps_work.dev);
	if (err)
	{
		LOG_DBG("Failed starting GPS!");
		return err;
	}

	atomic_set(&gps_is_active, 1);

	LOG_DBG("GPS started successfully. Searching for satellites ");
	LOG_DBG("to get position fix. This may take several minutes. ");
	LOG_DBG("The device will attempt to get a fix for %d seconds, ",
			CONFIG_GPS_CONTROL_FIX_TRY_TIME);
	LOG_DBG("before the GPS is stopped.");
	return 0;
}

static int stop(void)
{
	int err;

	if (IS_ENABLED(CONFIG_GPS_CONTROL_PSM_DISABLE_ON_STOP))
	{
		LOG_DBG("Disabling PSM");

		err = lte_lc_psm_req(false);
		if (err)
		{
			LOG_DBG("PSM mode could not be disabled");
		}
	}

	err = gps_stop(gps_work.dev);
	if (err)
	{
		return err;
	}

	return 0;
}

static void gps_work_handler(struct k_work *work)
{
	int err;

	if (gps_work.type == GPS_WORK_START)
	{
		err = start();
		if (err)
		{
			LOG_DBG("GPS could not be started, error: %d", err);
			return;
		}

		LOG_DBG("GPS operation started");

		atomic_set(&gps_is_active, 1);
		ui_led_set_pattern(UI_LED_GPS_SEARCHING);
		ui_led_set_state(UI_LED_3, 1);
		ui_led_set_state(UI_LED_4, 1);

		gps_work.type = GPS_WORK_STOP;

		k_delayed_work_submit(
			&gps_work.work,
			K_SECONDS(CONFIG_GPS_CONTROL_FIX_TRY_TIME));

		return;
	}
	else if (gps_work.type == GPS_WORK_STOP)
	{
		err = stop();
		if (err)
		{
			LOG_DBG("GPS could not be stopped, error: %d", err);
			return;
		}

		LOG_DBG("GPS operation was stopped");

		atomic_set(&gps_is_active, 0);

		if (atomic_get(&gps_is_enabled) == 0)
		{
			return;
		}

		gps_work.type = GPS_WORK_START;

		LOG_DBG("The device will try to get fix again in %d seconds",
				CONFIG_GPS_CONTROL_FIX_CHECK_INTERVAL);

		k_delayed_work_submit(
			&gps_work.work,
			K_SECONDS(CONFIG_GPS_CONTROL_FIX_CHECK_INTERVAL));
	}
}

bool gps_control_is_active(void)
{
	return atomic_get(&gps_is_active);
}

bool gps_control_is_enabled(void)
{
	return atomic_get(&gps_is_enabled);
}

void gps_control_enable(void)
{
	atomic_set(&gps_is_enabled, 1);
	gps_control_start(K_SECONDS(1));
}

void gps_control_disable(void)
{
	atomic_set(&gps_is_enabled, 0);
	gps_control_stop(K_NO_WAIT);
	ui_led_set_pattern(UI_CLOUD_CONNECTED);
}

void gps_control_stop(u32_t delay_ms)
{
	k_delayed_work_cancel(&gps_work.work);
	gps_work.type = GPS_WORK_STOP;
	k_delayed_work_submit(&gps_work.work, delay_ms);
}

void gps_control_start(u32_t delay_ms)
{
	k_delayed_work_cancel(&gps_work.work);
	gps_work.type = GPS_WORK_START;
	k_delayed_work_submit(&gps_work.work, delay_ms);
}

void gps_control_on_trigger(void)
{
}

/** @brief Configures and starts the GPS device. */
int gps_control_init(gps_trigger_handler_t handler)
{
	int err;
	struct device *gps_dev;
#if defined(CONFIG_GPS_CONTROL_USE_NMEA)
	struct gps_trigger gps_trig = {.type = GPS_TRIG_FIX,
								   .chan = GPS_CHAN_NMEA};
#else
	struct gps_trigger gps_trig = {.type = GPS_TRIG_FIX,
								   .chan = GPS_CHAN_PVT};
#endif
	gps_dev = device_get_binding(CONFIG_GPS_DEV_NAME);
	if (gps_dev == NULL)
	{
		LOG_DBG("Could not get %s device", CONFIG_GPS_DEV_NAME);
		return -ENODEV;
	}

	err = gps_trigger_set(gps_dev, &gps_trig, handler);
	if (err)
	{
		LOG_DBG("Could not set trigger, error code: %d", err);
		return err;
	}

	k_delayed_work_init(&gps_work.work, gps_work_handler);

	gps_work.dev = gps_dev;
	LOG_DBG("GPS initialized");

	return 0;
}
