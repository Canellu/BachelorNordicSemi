#include <zephyr.h>
#include <stdio.h>
#include <device.h>
#include <drivers/gpio.h>
#include <dk_buttons_and_leds.h>
#include "AnalogIn.h"

/* 1000 msec = 1 sec */
#define SLEEP_TIME 	1000

#define LED_1 2
#define button_1 6


#define MSG_1 "TEST SEND"

const struct device *dev;

float temperature = 0.0;

static void button_handler(uint32_t button_states, uint32_t has_changed)
{
	if(button_states == 1)
	{
		temperature = AnalogRead(0);
		printf("Temp1: %.3f\n", temperature);
		temperature = AnalogRead(1);
		printf("Temp2: %.3f\n", temperature);
		temperature = AnalogRead(2);
		printf("Temp3: %.3f\n", temperature);
		temperature = AnalogRead(3);
		printf("Temp4: %.3f\n", temperature);
		temperature = AnalogRead(4);
		printf("Temp5: %.3f\n", temperature);
		temperature = AnalogRead(5);
		printf("Temp6: %.3f\n", temperature);
		temperature = AnalogRead(6);
		printf("Temp7: %.3f\n", temperature);
		temperature = AnalogRead(7);
		printf("Temp8: %.3f\n\n", temperature);
	}	
}

void main(void)
{
	printk("\n\n\n\n\n\n\n*** RUNNING ADC SHIT Sample ***\n");

	dev = device_get_binding("GPIO_0");

	/* Set LED pin as output */

	dk_buttons_init(button_handler);

	while (1) 
	{

		temperature = AnalogRead(0);
		printf("Temp1: %.3f\n", temperature);
		temperature = AnalogRead(1);
		printf("Temp2: %.3f\n", temperature);
		temperature = AnalogRead(2);
		printf("Temp3: %.3f\n", temperature);
		temperature = AnalogRead(3);
		printf("Temp4: %.3f\n", temperature);
		temperature = AnalogRead(4);
		printf("Temp5: %.3f\n", temperature);
		temperature = AnalogRead(5);
		printf("Temp6: %.3f\n", temperature);
		temperature = AnalogRead(6);
		printf("Temp7: %.3f\n", temperature);
		temperature = AnalogRead(7);
		printf("Temp8: %.3f\n\n", temperature);
		k_msleep(SLEEP_TIME);

	}
}