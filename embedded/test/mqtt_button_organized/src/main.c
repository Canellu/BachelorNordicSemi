#include <zephyr.h>
#include <dk_buttons_and_leds.h>
#include "app_mqtt.h"
#include "app_button.h"



void main(void)
{
	button_dev_init();
	dk_buttons_init(button_handler);
	dk_leds_init();


	int err;
    uint32_t connect_attempt = 0;



	printk("\n\n**** NordicOasys v0.1b - Started ****\n\n");
	do {
		err = modem_configure();
		if (err) {
			printk("Retrying in %d seconds",
				CONFIG_LTE_CONNECT_RETRY_DELAY_S);
			k_sleep(K_SECONDS(CONFIG_LTE_CONNECT_RETRY_DELAY_S));
		}
	} while (err);


do_connect:
	if (connect_attempt++ > 0) {
		printk("Reconnecting in %d seconds...",
			CONFIG_MQTT_RECONNECT_DELAY_S);
		k_sleep(K_SECONDS(CONFIG_MQTT_RECONNECT_DELAY_S));
	}

	err = app_mqtt_init();
	if (err != 0) {
		goto do_connect;
	}


	do {
		err = app_mqtt_run();
	}while(err == 0);


	err = app_mqtt_disconnect();
	if (err != 0) {
		goto do_connect;
	}


}