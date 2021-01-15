#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME 	1000

#define LED_2 3
#define LED_3 4
#define button_1 6
#define button_2 7
#define switch_1 8
#define switch_2 9




static struct gpio_callback cb;
const struct device *dev;

int button_1_state = 2;
int button_2_state = 2;
int switch_1_state = 2;
int switch_2_state = 2;

void button1_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins) {
	button_1_state = gpio_pin_get(dev, button_1);
	button_2_state = gpio_pin_get(dev, button_2);

	switch_1_state = gpio_pin_get(dev, switch_1);
	switch_2_state = gpio_pin_get(dev, switch_2);

	printk("Button 1: %d  Button 2: %d  Switch 1: %d  Switch 2: %d\n\n", button_1_state, button_2_state, switch_1_state, switch_2_state);
}

void main(void)
{
	printk("\n\n\n\n\n\n\n*** Running Simple Sample ***");


	int count = 0;

	

	dev = device_get_binding("GPIO_0");

	/* Set LED pin as output */
	gpio_pin_configure(dev, LED_2, GPIO_OUTPUT);
	gpio_pin_configure(dev, LED_3, GPIO_OUTPUT);

	/* Set Button as input */
	gpio_pin_configure(dev, button_1, GPIO_INPUT);
	gpio_pin_interrupt_configure(dev, button_1, GPIO_INT_EDGE_TO_ACTIVE);
	gpio_init_callback(&cb, button1_pressed, BIT(button_1));
	gpio_add_callback(dev, &cb);

	gpio_pin_configure(dev, button_2, GPIO_INPUT);
	gpio_pin_interrupt_configure(dev, button_2, GPIO_INT_EDGE_TO_ACTIVE);

	/* Set Switch as input */
	gpio_pin_configure(dev, switch_1, GPIO_INPUT);
	gpio_pin_configure(dev, switch_2, GPIO_INPUT);


	while (1) {

		/* Set pin to HIGH/LOW every 1 second */
        if(count % 2 == 0)
        {
            gpio_pin_toggle(dev, LED_2);	
		    gpio_pin_toggle(dev, LED_3);

			

        }

		count++;
		k_msleep(SLEEP_TIME);


	}
}