#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <dk_buttons_and_leds.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME 	1

#define LED_1 2
#define LED_2 3
#define LED_3 4
#define LED_4 5
#define button_1 6
#define button_2 7
#define switch_1 8
#define switch_2 9

#define MSG_1 "this"

const struct device *dev;


static void button_handler(uint32_t button_states, uint32_t has_changed)
{
	printk("button states: %d, has changed: %d \n", button_states, has_changed);
	printk("button 1: %d\n", gpio_pin_get(dev, button_1));
	printk("button 2: %d\n", gpio_pin_get(dev, button_2));

	if(button_states == 1)
	{
		printk(MSG_1 "\n");
	}
	else if(button_states == 5)
	{
		printk("is \n");
	}
	else if(button_states == 9)
	{
		printk("completely \n");
	}
	else if(button_states == 13)
	{
		printk("unnecessary \n");
	}
	else if(button_states == 2)
	{
		printk("lol \n");
	}
}

void main(void)
{
	printk("\n\n\n\n\n\n\n*** Running Simple Sample ***\n");

	dev = device_get_binding("GPIO_0");

	/* Set LED pin as output */

	gpio_pin_configure(dev, LED_3, GPIO_OUTPUT);
	gpio_pin_configure(dev, LED_4, GPIO_OUTPUT);

	dk_buttons_init(button_handler);

	while (1) 
	{
	    // get_state();
		k_msleep(SLEEP_TIME);

	}
}