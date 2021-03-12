#include <zephyr.h>
#include <device.h>
#include <misc/printk.h>
#include <lte_lc.h>
#include <net/mqtt_socket.h>
#include <string.h>
#include <errno.h>
#include <bsd.h>
#include <nrf_socket.h>
#include <net/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <dk_buttons_and_leds.h>

#include <ntp.h>
#include <gcloud.h>
#include <cJSON.h>

#define SLEEP_TIME 10010
#define READ_INTERVAL  2000

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
static void modem_configure(void)
{
    if (IS_ENABLED(CONFIG_LTE_AUTO_INIT_AND_CONNECT)) {
        /* Do nothing, modem is already turned on and connected. */
    } else {
        int err;

        printk("Establishing LTE link (this may take some time) ...\n");
        err = lte_lc_init_and_connect();
        __ASSERT(err == 0, "LTE link could not be established.");
    }
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

    err = dk_set_leds_state(0x00, DK_ALL_LEDS_MSK);
    if (err) {
        printk("Could not set led state, err code: %d\n", err);
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
        k_sleep(SLEEP_TIME);

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

