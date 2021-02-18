#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <kernel.h>
#include <string.h>
#include <dk_buttons_and_leds.h>

#include "app_mqtt.h"
#include "app_button.h"

bool led1_on = false;

#define MSG_1 "Pressed Button 1"
#define MSG_2 "Pressed Button 2"
#define MSG_3 "{\"health\":\"true\"}"
#define MSG_4 "{\"health\":\"false\"}"

// message queues
extern struct k_msgq button_msg_q;
extern struct k_msgq button_msg_qr;
int app_button_config;

extern struct k_msgq uart_msg_q;
extern struct k_msgq mqtt_msg_q;


// LOCAL
const struct device *dev_button;


static void button_handler(uint32_t button_states, uint32_t has_changed)
{
	// printk("button states: %d, has changed: %d \n", button_states, has_changed);
	// printk("button 1: %d\n", gpio_pin_get(dev_button, 6));
	// printk("button 2: %d\n", gpio_pin_get(dev_button, 7));

	int app_button_ret_val;

	k_msgq_get(&button_msg_q, &app_button_config, K_NO_WAIT);

	// TODO: create functions for the cases
	switch (app_button_config)
	{
		case 0: // for waiting
			if(has_changed == 1 && !gpio_pin_get(dev_button, 6)) {
				app_button_ret_val = 1;
				k_msgq_put(&button_msg_qr, &app_button_ret_val, K_NO_WAIT);
			}
			break;

		case 1: // UART
			if(has_changed == 1 && !gpio_pin_get(dev_button, 6)) {
				k_msgq_put(&uart_msg_q, "{exit}", K_NO_WAIT);
			}
			break;

		case 2: // MQTT
			if(has_changed == 1 && !gpio_pin_get(dev_button, 6)) {
				k_msgq_put(&mqtt_msg_q, "exit", K_NO_WAIT);
			}

			else if(has_changed == 2 && !gpio_pin_get(dev_button, 7)) {
				app_data_publish(MSG_2, sizeof(MSG_2));
			}

			else if(has_changed == 4) {
				app_data_publish(MSG_3, sizeof(MSG_3));
			}

			else if(has_changed == 8) {
				app_data_publish(MSG_4, sizeof(MSG_4));
			}
			break;
		
		case 999: // DO NOT USE, ONLY FOR TEMPLATE
			if(has_changed == 1 && !gpio_pin_get(dev_button, 6)) {
				// empty
			}

			else if(has_changed == 2 && !gpio_pin_get(dev_button, 7)) {
				// empty
			}

			else if(has_changed == 4) {
				// empty
			}

			else if(has_changed == 8) {
				// empty
			}
			break;

		default:
			printk("\nunregistered button configuration\n");
			break;
	}

}

// initialize buttons
void button_dev_init(void) 
{
	dev_button = device_get_binding("GPIO_0");
	dk_buttons_init(button_handler);
}

// toggle specified led
int toggle_LED(int led_val)
{
	switch (led_val)
	{
		case 1:
			led1_on = !led1_on;
			dk_set_led(DK_LED1, led1_on);
			break;

		default:
			printk("\nunknown led value\n");
			break;
	}

	return 0;
}