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
uint8_t uart_data_array[32][64] = { "" };
int data_available_to_send = 0;

//LOCAL

bool mqtt_init_complete = false;

void app_data_publish_uart()
{
	int i = 0;

	while(strcmp(uart_data_array[i], "") != 0)
	{
		app_data_publish(uart_data_array[i], strlen(uart_data_array[i])+1);
		k_sleep(K_MSEC(500));
		i++;
	}
	data_available_to_send = 0;
}

void main(void)
{

	printk("\n\n**** NordicOasys v0.1b - Started ****\n\n");

	// device inits and configurations

	// button and leds
	button_dev_init();
	dk_buttons_init(button_handler);
	dk_leds_init();

	// UART
	uart_dev_init();

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

	/******* MAIN LOOP START *******/
	while(1)
	{
		/******* UART START *******/

		uart_start();
		printk("uart test start\n");
		printk("button 1: exit \nbutton 2: send uart signal\n\n");

		int i = 0;
		while(strcmp(uart_data_array[i], "") != 0)
		{
			strcpy(uart_data_array[i], "");
			i++;
		}
		strcpy(uart_data, "");

		/* UART loop */
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
		/* UART loop end */

		printk("\n\n%s", uart_data);

		if(uart_data[0] != '\0')
		{

			i = 0;
			uint8_t delim[] = ",";
			uint8_t *str_p;
			char *uart_data_p = uart_data;

			while((str_p = strtok_r(uart_data_p, delim, &uart_data_p)))
			{
				strcpy(uart_data_array[i], str_p);
				//printk("%s\n", uart_data_array[i]);
				i++;
			}

			data_available_to_send = 1;
		}

		else
		{
			printk("no data saved\n");
		}

		i = 0;
		printk("\n");

		while(strcmp(uart_data_array[i], "") != 0)
		{
			printk("\n%d: %s", i, uart_data_array[i]);
			i++;
		}

		printk("\nuart test end\n\n");		
		printk("\n\npress button 1 to start mqtt test\n\n");

		while(1)
		{
			if(button_choice != 0)
			{
				button_choice = 0;
				break;
			}
			k_sleep(K_MSEC(100));
		}
		

		/******* MQTT START *******/

		printk("mqtt test start\n");

		int err;

		if(!mqtt_init_complete)
		{
			do {
				err = modem_configure();
				if (err) {
					printk("Retrying in %d seconds", CONFIG_LTE_CONNECT_RETRY_DELAY_S);
					k_sleep(K_SECONDS(CONFIG_LTE_CONNECT_RETRY_DELAY_S));
				}
			} while (err);

			err = app_mqtt_init();
			if (err != 0)
			{
				return;
			}

			err = app_mqtt_connect();
			if (err != 0)
			{
				printk("Reconnecting in %d seconds...", CONFIG_MQTT_RECONNECT_DELAY_S);
				k_sleep(K_SECONDS(CONFIG_MQTT_RECONNECT_DELAY_S));
			}

			err = app_fds_init();
			if (err != 0)
			{
				return;
			}

			mqtt_init_complete = true;		
		}
		else
		{
			do {
				err = modem_reconnect();
				if (err) {
					printk("Retrying in %d seconds", CONFIG_LTE_CONNECT_RETRY_DELAY_S);
					k_sleep(K_SECONDS(CONFIG_LTE_CONNECT_RETRY_DELAY_S));
				}
			} while (err);

			err = app_mqtt_connect();
			if (err != 0)
			{
				printk("Reconnecting in %d seconds...", CONFIG_MQTT_RECONNECT_DELAY_S);
				k_sleep(K_SECONDS(CONFIG_MQTT_RECONNECT_DELAY_S));
			}
		}

		/* MQTT loop */
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
		/* MQTT loop end */

		err = app_mqtt_disconnect();
		if (err != 0) {
			printk("\nDisconnect unsuccessful: %d", err);
			printk("\n\nClosing program, check for errors in code lol");
		}

		printk("**** NordicOasys test end ****\n");

	}
	/******* MAIN LOOP END *******/
}

/* Function ideas and TODOs

- Overhaul externs and global variables.
  Currently hard to track where all the variables are being used across headers.
  Mailbox function seems to be the way to go.
  Where to use: 
  	buttons, use mailbox to wait for and receive button choices.
  	uart, fetch buffer from mailbox.
	mqtt, nothing currently, may be able to create mailbox in mqtt to append to fds,
		mailbox will wait for input from either buttons or fds, this is only useful
		for testing purposes, as a more complete program will eventually have to run 
		based on mqtt commands.

- Allow option to choose between "main parts" 
  e.g. using switch case to choose between UART and MQTT.

*/