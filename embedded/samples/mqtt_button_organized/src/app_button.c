#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <string.h>
#include <dk_buttons_and_leds.h>
#include "app_mqtt.h"
#include "app_button.h"

bool ledOn = 0;

#define MSG_1 "Pressed Button 1"
#define MSG_2 "Pressed Button 2"
#define MSG_3 "Changed Switch 1"
#define MSG_4 "Changed Switch 2"

const struct device *dev;

void button_dev_init(void) {
	dev = device_get_binding("GPIO_0");
}


void button_handler(uint32_t button_states, uint32_t has_changed)
{
	printk("button states: %d, has changed: %d \n", button_states, has_changed);
	printk("button 1: %d\n", gpio_pin_get(dev, 6));
	printk("button 2: %d\n", gpio_pin_get(dev, 7));

	if((has_changed == 1) && (!gpio_pin_get(dev, 6)))
	{
		app_data_publish(MSG_1, sizeof(MSG_1));
	}
	else if(has_changed == 2 && !gpio_pin_get(dev, 7))
	{
		app_data_publish(MSG_2, sizeof(MSG_2));
	}
	else if(has_changed == 4)
	{
		app_data_publish(MSG_3, sizeof(MSG_3));
	}
	else if(has_changed == 8)
	{
		app_data_publish(MSG_4, sizeof(MSG_4));
	}


}


void toggleLED(uint8_t *data, size_t len) {
    if(strcmp(data, "LED") == 0) {
            ledOn = !ledOn;
            dk_set_led(DK_LED1, ledOn);
    }
}