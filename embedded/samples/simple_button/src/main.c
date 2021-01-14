#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME 	1000

void main(void)
{
	int cnt = 0;
	struct device *dev;

	dev = device_get_binding("GPIO_0");
	/* Set LED pin as output */
	gpio_pin_configure(dev, 3, GPIO_OUTPUT); //p0.03 == LED2
	gpio_pin_configure(dev, 4, GPIO_OUTPUT); //p0.04 == LED3
	gpio_pin_configure(dev, 17, GPIO_OUTPUT); //p0.17

	while (1) {
		/* Set pin to HIGH/LOW every 1 second */
        if(cnt % 2 == 0)
        {
            gpio_pin_toggle(dev, 3);	//p0.03 == LED2
		    gpio_pin_toggle(dev, 4);	//p0.04 == LED3
		    gpio_pin_toggle(dev, 17);
        }
			//p0.17 Toggling pin 17
		cnt++;
		k_msleep(SLEEP_TIME);
	}
}