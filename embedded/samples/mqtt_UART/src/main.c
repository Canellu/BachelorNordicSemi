#include <zephyr.h>
#include <dk_buttons_and_leds.h>
#include <string.h>
#include "app_mqtt.h"
#include "app_button.h"
#include "app_uart.h"

// VARIABLES USED EXTERNALLY
int button_choice = 0;

int new_data = 0;
uint8_t uart_data[512];
uint8_t uart_data_array[128][16] = { "" };
int data_available_to_send = 0;

//LOCAL


void app_data_publish_uart()
{
	int i = 0;

	while(strcmp(uart_data_array[i], "") != 0)
	{
		app_data_publish(uart_data_array[i], strlen(uart_data_array[i])+1);
		k_sleep(K_MSEC(1000));
		i++;
	}
	data_available_to_send = 0;
}

void main(void)
{

	printk("\n\n**** NordicOasys v0.1b - Started ****\n\n");

// ************************ BUTTONS AND LEDS ************************

	button_dev_init();
	dk_buttons_init(button_handler);
	dk_leds_init();

	printk("press button 1 to start\n\n");
	while(1)
	{
		if(button_choice != 0)
		{
			button_choice = 0;
			break;
		}
		k_sleep(K_MSEC(100));
	}



// ************************ UART START ************************

	uart_init();
	printk("uart test start\n");
	printk("button 1: exit \nbutton 2: send uart signal\n\n");

	while(1)
	{
		if(button_choice == 1)
		{
			button_choice = 0;
			uart_exit();
			break;
		}
		else if(button_choice == 2)
		{
			button_choice = 0;
			uart_send();
		}
		k_sleep(K_MSEC(100));
	}

	if(uart_data[0] != '\0')
	{

		int i = 0;
		uint8_t delim[] = ",";
		uint8_t *str_p;
		char *uart_data_p = uart_data;

		while((str_p = strtok_r(uart_data_p, delim, &uart_data_p)))
		{
			strcpy(uart_data_array[i], str_p);
			//printk("%s\n", uart_data_array[i]);
			i++;
		}

		// i = 0;
		// while(strcmp(uart_data_array[i], "") != 0)
		// {
		// 	printk("%s %d \n",uart_data_array[i], strlen(uart_data_array[i])+1);
		// 	k_sleep(K_MSEC(100));
		// 	i++;
		// }

		data_available_to_send = 1;
	}

	else
	{
		printk("no data saved\n");
	}
	
	
	printk("\nuart test end\n\n");


// ************************ UART END ************************
// ************************ MQTT START ************************

	int err;
    uint32_t connect_attempt = 0;

	
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



	while (1) {
		err = app_mqtt_run();

		if(button_choice == 1 || err != 0)
		{
			button_choice = 0;
			break;
		}
		else if(button_choice == 2)
		{
			button_choice = 0;
			printk("button choice: 2\n\n");
		}
		k_sleep(K_MSEC(100));

		if (data_available_to_send)
		{
			app_data_publish_uart();
		}
	}



	err = app_mqtt_disconnect();
	if (err != 0) {
		goto do_connect;
	}

	printk("**** NordicOasys test end ****\n");


// ************************ MQTT END ************************

}
