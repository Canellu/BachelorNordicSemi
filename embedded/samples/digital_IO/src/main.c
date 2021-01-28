#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME 	1

#define LED_EXT 10
#define BUTTON_EXT 11

#define LED_1 2


const struct device *dev;

void main(void)
{
	printk("\n\n\n\n\n\n\n*** Running Simple Sample ***\n");

	dev = device_get_binding("GPIO_0");

	/* Set LED pin as output */

	gpio_pin_configure(dev, LED_1, GPIO_OUTPUT);

	gpio_pin_configure(dev, LED_EXT, GPIO_OUTPUT);
	gpio_pin_configure(dev, BUTTON_EXT, GPIO_INPUT);

	while (1) 
	{
		k_msleep(SLEEP_TIME);

		if(!gpio_pin_get(dev, BUTTON_EXT))
		{
			gpio_pin_set(dev, LED_EXT, 1);
			gpio_pin_set(dev, LED_1, 1);
		}
		else
		{
			gpio_pin_set(dev, LED_EXT, 0);
			gpio_pin_set(dev, LED_1, 0);
		}
		

	}
}