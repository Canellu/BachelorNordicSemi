/*
 * Copyright (c) 2020 Safecility.com
 * Author: tad@safecility.com
 */

#include <zephyr.h>
#include <device.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include <bsd.h>
#include <modem/lte_lc.h>

#include <nrf_socket.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>

#include <dk_buttons_and_leds.h>

#include "gcloud.h"
#include "cJSON.h"

#define SLEEP_TIME 10010
#define READ_INTERVAL  2000

LOG_MODULE_REGISTER(gcloud, CONFIG_MQTT_SIMPLE_LOG_LEVEL);

// Forward declarations

void store_modem_configuration(void);

// Handlers

void button_handler(u32_t button_state, u32_t has_chandged)
{
    printk("Button event: %x    %x\n", button_state, has_chandged);
    if ( (has_chandged & 0x01) && (button_state & 0x01) ) {
        store_modem_configuration();
    }
}

/**@brief Handler for when receiving GPS data from Google Cloud.
 * Should also do something with the data when we know what to do.
 */ 

void received_config_handler(const struct mqtt_publish_message *message) {

	cJSON * data_json = cJSON_Parse(message->payload.data);

    printk("%s\n",cJSON_Print(data_json));

	cJSON_Delete(data_json);

}


// Initializers

/**@brief Configures modem to provide LTE link. Blocks until link is
 * successfully established.
 */
static int modem_configure(void)
{
#if defined(CONFIG_LTE_LINK_CONTROL)
	if (IS_ENABLED(CONFIG_LTE_AUTO_INIT_AND_CONNECT)) {
		/* Do nothing, modem is already turned on
		 * and connected.
		 */
	} else {
#if defined(CONFIG_LWM2M_CARRIER)
		/* Wait for the LWM2M_CARRIER to configure the modem and
		 * start the connection.
		 */
		LOG_INF("Waitng for carrier registration...");
		k_sem_take(&carrier_registered, K_FOREVER);
		LOG_INF("Registered!");
#else /* defined(CONFIG_LWM2M_CARRIER) */
		int err;

		LOG_INF("LTE Link Connecting...");
		err = lte_lc_init_and_connect();
		if (err) {
			LOG_INF("Failed to establish LTE connection: %d", err);
			return err;
		}
		LOG_INF("LTE Link Connected!");
#endif /* defined(CONFIG_LWM2M_CARRIER) */
	}
#endif /* defined(CONFIG_LTE_LINK_CONTROL) */

	/* Turn off power saving modes for a more responsive demo */
	LOG_INF("Disabling PSM and eDRX");
	lte_lc_psm_req(false);
	lte_lc_edrx_req(false);

	return 0;
}

/**@brief Configure LEDs and buttons. */
static void init_leds_and_buttons(void)
{
    int err;

    err = dk_leds_init();
    if (err) {
        printk("Could not initialize leds, err code: %d\n", err);
    }

    err = dk_buttons_init(button_handler);
    if (err) {
        printk("Could not initialize buttons, err code: %d\n", err);
    }
}

// TODO: FIX OR IGNORE
void store_modem_configuration(void)
{
    int err;

    printk("Storing modem configuration...");
    err = lte_lc_power_off();
    if (err) {
        printk("Fail turning off modem: %d\n", err);
    }
    printk("Modem offline, turning back on...");
    dk_set_led_off(1);

    err = lte_lc_normal();
    if (err) {
        printk("Fail turning on modem: %d\n", err);
    }
    dk_set_led_on(1);
    printk("Done\n");
}

void main(void)
{
	int err;
	init_leds_and_buttons();
	dk_set_led_on(0); // The app has started
	printk("Provisioning\n");
	err = gcloud_provision();
	if (err) {
        printk("Failed to provision: %d\n", err);
        k_sleep(K_FOREVER);
    } else {
        printk("Provisioning complete\n");
    }
	printk("Initializing modem\n");
    modem_configure();
    dk_set_led_on(1); // The modem is configured
    printk("Modem configured\n");

	err = gcloud_connect(received_config_handler);
    if (err) {
        printk("Failed to connect to Google Cloud, error: %d\n", err);
        k_sleep(K_FOREVER);
    }
    printk("Connecting to Google Cloud\n");

	u32_t cnt = 0;
    for(int i = 0; ; i++) {
        cnt++ % 2 == 0 ? dk_set_led_on(3) : dk_set_led_off(3);
        k_sleep(K_MSEC(SLEEP_TIME));

        double yrNum = 22019 - cnt;
        cJSON * yrObj = cJSON_CreateObject();
        cJSON * year = NULL;
        char * JSONString = NULL;
        year = cJSON_CreateNumber(yrNum);
        const char * yrStr = "Message";

        cJSON_AddItemToObject(yrObj, yrStr, year);
        JSONString = cJSON_Print(yrObj);
        cJSON_Delete(yrObj);

	    printk("Sending JSON data:\n%s\n", JSONString);
	    gcloud_publish(JSONString, strlens(JSONString), MQTT_QOS_1_AT_LEAST_ONCE);
    }
	gcloud_disconnect();

}

