/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#include <zephyr.h>
#include <kernel_structs.h>
#include <stdio.h>
#include <string.h>
#include <console/console.h>
#include <power/reboot.h>
#include <logging/log_ctrl.h>

#if defined(CONFIG_BSD_LIBRARY)
#include <bsd.h>
#include <modem/lte_lc.h>
#endif /* CONFIG_BSD_LIBRARY */
#include <net/socket.h>
#include <net/mqtt.h>

#include "cJSON.h"
#include "ntp.h"
#include "gcloud.h"

#define CALIBRATION_PRESS_DURATION K_SECONDS(5)
#define SLEEP_TIME 10010

#if defined(CONFIG_BSD_LIBRARY) && \
	!defined(CONFIG_LTE_LINK_CONTROL)
#error  "Missing CONFIG_LTE_LINK_CONTROL"
#endif

#define UNIQUE_PATH_ID_LENGTH 11

/* Global variables */
static char uniquePathID[UNIQUE_PATH_ID_LENGTH] = "00_0000000";

extern atomic_t connected;
extern atomic_t connecting;

enum error_type
{
	ERROR_CLOUD,
	ERROR_BSD_RECOVERABLE,
	ERROR_BSD_IRRECOVERABLE,
	ERROR_LTE_LC,
	ERROR_THREAD
};

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
		printk("Waitng for carrier registration...\n");
		k_sem_take(&carrier_registered, K_FOREVER);
		printk("Registered!\n");
#else /* defined(CONFIG_LWM2M_CARRIER) */
		int err;

		printk("LTE Link Connecting...\n");
		err = lte_lc_init_and_connect();
		if (err) {
			printk("Failed to establish LTE connection: %d", err);
			return err;
		}
		printk("LTE Link Connected!\n");
#endif /* defined(CONFIG_LWM2M_CARRIER) */
	}
#endif /* defined(CONFIG_LTE_LINK_CONTROL) */

	/* Turn off power saving modes for a more responsive demo */
	printk("Disabling PSM and eDRX\n");
	lte_lc_psm_req(false);
	lte_lc_edrx_req(false);
	printk("disabling finished\n\n");

	return 0;
}

void main(void)
{
	int ret;

	printk("Program started\n");

	ret = gcloud_provision();
	if (ret)
	{
		printk("Failed to provision: %d\n", ret);
	}
	else
	{
		printk("Provisioning complete\n");
	}

	modem_configure();

    // k_timer_init(&reconnect_timer, reconnect_timer_handler, NULL);

    gcloud_connect();
    
    /* Waiting for client socket to be initialized */
    for (int i = 200;
         (!atomic_get(&connecting)) && (!atomic_get(&connected)) && (i > 0); i--)
    {
        k_sleep(K_MSEC(50));
    }

    struct pollfd fds[] = {
        {.fd = client.transport.tcp.sock,
         .events = POLLIN}};

	u32_t cnt = 0;

    while (true)
    {
        ret = poll(fds, ARRAY_SIZE(fds),
                   mqtt_keepalive_time_left(&client));

        if (ret < 0)
        {
            printk("poll() returned an error: %d", ret);
            continue;
        }

        if (ret == 0)
        {
            printk("GCLOUD: ping");
            gcloud_ping();
            continue;
        }

        if ((fds[0].revents & POLLIN) == POLLIN)
        {
            printk("GCLOUD: process");
            gcloud_process();
        }

        if ((fds[0].revents & POLLNVAL) == POLLNVAL)
        {
            printk("Socket error: POLLNVAL");
            return;
        }

        if ((fds[0].revents & POLLHUP) == POLLHUP)
        {
            printk("Socket error: POLLHUP");
            return;
        }

        if ((fds[0].revents & POLLERR) == POLLERR)
        {
            printk("Socket error: POLLERR");
            return;
        }

		double yrNum = 22021 - cnt;
        cJSON * yrObj = cJSON_CreateObject();
        cJSON * year = NULL;
        char * JSONString = NULL;
        year = cJSON_CreateNumber(yrNum);
        const char * yrStr = "Message";

        cJSON_AddItemToObject(yrObj, yrStr, year);
        JSONString = cJSON_Print(yrObj);
        cJSON_Delete(yrObj);

	    printk("Sending JSON data:\n%s\n", JSONString);
	    gcloud_publish(JSONString, strlen(JSONString), MQTT_QOS_1_AT_LEAST_ONCE);
		cnt++;
    }

	while (!gcloud_get_connection_status())
	{
		k_sleep(K_SECONDS(1));
	}

	u32_t cnt = 0;
	for(int i = 0; ; i++) {
        k_sleep(K_MSEC(SLEEP_TIME));

        double yrNum = 22021 - cnt;
        cJSON * yrObj = cJSON_CreateObject();
        cJSON * year = NULL;
        char * JSONString = NULL;
        year = cJSON_CreateNumber(yrNum);
        const char * yrStr = "Message";

        cJSON_AddItemToObject(yrObj, yrStr, year);
        JSONString = cJSON_Print(yrObj);
        cJSON_Delete(yrObj);

	    printk("Sending JSON data:\n%s\n", JSONString);
	    gcloud_publish(JSONString, strlen(JSONString), MQTT_QOS_1_AT_LEAST_ONCE);
	}
}
