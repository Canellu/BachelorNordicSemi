#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>

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



const struct device *dev;

static struct gpio_callback cb1;
static struct gpio_callback cb2;
static struct gpio_callback cb3;
static struct gpio_callback cb4;

bool button_1_state = true;
bool button_2_state = true;
bool switch_1_state = true;
bool switch_2_state = true;

// const struct device *dev, struct gpio_callback *cb, uint32_t pins

void state_change() {
	// button_1_state = gpio_pin_get(dev, button_1);
	// button_2_state = gpio_pin_get(dev, button_2);

	// switch_1_state = gpio_pin_get(dev, switch_1);
	// switch_2_state = gpio_pin_get(dev, switch_2);

	// gpio_pin_set(dev, LED_1, switch_1_state);
	// gpio_pin_set(dev, LED_2, switch_2_state);
	// gpio_pin_set(dev, LED_3, button_1_state);
	// gpio_pin_set(dev, LED_4, button_2_state);

	printk("Button 1: %d  Button 2: %d  Switch 1: %d  Switch 2: %d\n\n", button_1_state, button_2_state, switch_1_state, switch_2_state);
}
void get_state() {
	button_1_state = gpio_pin_get(dev, button_1);
	button_2_state = gpio_pin_get(dev, button_2);

	switch_1_state = gpio_pin_get(dev, switch_1);
	switch_2_state = gpio_pin_get(dev, switch_2);

	gpio_pin_set(dev, LED_1, switch_1_state);
	gpio_pin_set(dev, LED_2, switch_2_state);
	gpio_pin_set(dev, LED_3, button_1_state);
	gpio_pin_set(dev, LED_4, button_2_state);
}

void main(void)
{
	printk("\n\n\n\n\n\n\n*** Running Simple Sample ***\n");

	
	int count = 0;

	dev = device_get_binding("GPIO_0");

	/* Set LED pin as output */
	gpio_pin_configure(dev, LED_1, GPIO_OUTPUT);
	gpio_pin_configure(dev, LED_2, GPIO_OUTPUT);
	gpio_pin_configure(dev, LED_3, GPIO_OUTPUT);
	gpio_pin_configure(dev, LED_4, GPIO_OUTPUT);

	/* Set Button as input */
	gpio_pin_configure(dev, button_1, (GPIO_PULL_UP | GPIO_ACTIVE_LOW));
	gpio_pin_interrupt_configure(dev, button_1, GPIO_INT_EDGE_TO_ACTIVE);

	gpio_pin_configure(dev, button_2, (GPIO_PULL_UP | GPIO_ACTIVE_LOW));
	gpio_pin_interrupt_configure(dev, button_2, GPIO_INT_EDGE_TO_ACTIVE);

	/* Set Switch as input */
	gpio_pin_configure(dev, switch_1, (GPIO_PULL_UP | GPIO_ACTIVE_LOW));
	gpio_pin_interrupt_configure(dev, switch_1, GPIO_INT_EDGE_TO_ACTIVE);

	gpio_pin_configure(dev, switch_2, (GPIO_PULL_UP | GPIO_ACTIVE_LOW));
	gpio_pin_interrupt_configure(dev, switch_2, GPIO_INT_EDGE_TO_ACTIVE);


	gpio_init_callback(&cb1, state_change, BIT(switch_1));
	gpio_init_callback(&cb2, state_change, BIT(switch_2));
	gpio_init_callback(&cb3, state_change, BIT(button_1));
	gpio_init_callback(&cb4, state_change, BIT(button_2));

	gpio_add_callback(dev, &cb1);
	gpio_add_callback(dev, &cb2);
	gpio_add_callback(dev, &cb3);
	gpio_add_callback(dev, &cb4);

	printk("Set up button at pin %d\n", button_1);


	while (1) {

		/* Set pin to HIGH/LOW every 1 second */
        // if(count % 2 == 0)
        // {
        //     gpio_pin_toggle(dev, LED_2);	
		//     gpio_pin_toggle(dev, LED_3);

			

        // }

		// count++;
		get_state();
		k_msleep(SLEEP_TIME);


	}
}