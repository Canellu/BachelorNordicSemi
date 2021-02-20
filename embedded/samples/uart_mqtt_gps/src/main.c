#include <zephyr.h>
#include <dk_buttons_and_leds.h>
#include <string.h>
#include <kernel.h>

#include "app_button.h"
#include "app_uart.h"
#include "app_mqtt.h"
#include "app_gps.h"


/* STRUCTS AND ENUMS */

// Enumerations
enum oasys_event_type { // currently not in use
	EVT_INIT,
	EVT_UART,
	EVT_MQTT
};

// Structs
struct prog_param_t { // currently not in use
	bool bttn_init;
	bool leds_init;
	bool uart_init;
	bool mqtt_init;
} prog_p;

struct sens_param_t { // currently not in use
	int freq_temp;
	int freq_pres;
	int freq_cond;
} sens_p;

struct oasys_param_t { // currently not in use
	int cycle_max;
	int cycle_cur;
	int depth_max;
	int depth_min;
	int wp_max;
	int wp_cur;
	struct prog_param_t prog_param;
	struct sens_param_t sens_param;
} oasys_p;


/* VARIABLE DECLARATIONS */

// button variables
struct k_msgq button_msg_q;
struct k_msgq button_msg_qr;
static uint8_t button_msgq_buffer[3 * sizeof(int)];
static int button_val; // value received from button, only used when button_config = 0


// UART variables
struct k_msgq uart_msg_q;
static uint8_t uart_msgq_buffer[128 * 3];
static uint8_t uart_msg[128];
static uint8_t uart_data_array[16][128] = { "" };

// MQTT variables
struct k_msgq mqtt_msg_q;
static uint8_t mqtt_msgq_buffer[128 * 3];
static uint8_t mqtt_msg[128];

static bool mqtt_init_complete = false;
static bool data_available_to_send = false;


/* FUNCTION DECLARATIONS */

// publish all data that is currently saved from uart
static int publish_uart_data() 
{
	int i = 0;

	while(strcmp(uart_data_array[i], "") != 0) {
		app_data_publish(uart_data_array[i], strlen(uart_data_array[i])+1);
		k_sleep(K_MSEC(500));
		i++;
	}
	return 0;
}

static int check_mqtt_msg(void *data, size_t len)
{
	int ret_value = 0;

    if(strcmp(data, "LED") == 0) {
		toggle_LED(1);
    }

	if (strcmp(data, "exit") == 0) {
		ret_value = 1;
	}

	else if (strcmp(data, "button 2") == 0) {
		ret_value = 2;
	}

	return ret_value;
}

// changes button configuration
static int set_button_config(int local_button_config)
{
	k_msgq_purge(&button_msg_q);
	k_msgq_put(&button_msg_q, &local_button_config, K_NO_WAIT);

	return 0;
}

// waiting function, waits for button 1 to be pressed
static int button_wait()
{
	set_button_config(0);
	while(1) {
		k_msgq_get(&button_msg_qr, &button_val, K_FOREVER);
		if(button_val == 1) {
			break;
		}
	}

	return 0;
}

/* MAIN */

void main(void)
{

	/* device inits and configurations */

	// buttons and leds
	button_dev_init();
	dk_leds_init();

	k_msgq_init(&button_msg_q, button_msgq_buffer, sizeof(int), 3);
	k_msgq_init(&button_msg_qr, button_msgq_buffer, sizeof(int), 3);

	// UART
	uart_dev_init();

	k_msgq_init(&uart_msg_q, uart_msgq_buffer, sizeof(uart_msg), 3);

	// MQTT
	k_msgq_init(&mqtt_msg_q, mqtt_msgq_buffer, sizeof(mqtt_msg), 3);

	
	/******* PROGRAM START *******/

	printk("\n\n**** NordicOasys v0.5 - Started ****\n\n");
	printk("press button 1 to start\n\n");

	button_wait();

	/******* MAIN LOOP START *******/

	while(1) {

		/******* UART START *******/

		int i = 0;
		while(strcmp(uart_data_array[i], "") != 0) {
			strcpy(uart_data_array[i], "");
			i++;
		}

		// CANDO: create function to purge and clear message queue / messages
		k_msgq_purge(&button_msg_q);
		k_msgq_purge(&uart_msg_q);
		k_msgq_purge(&mqtt_msg_q);

		set_button_config(1);

		uart_start();

		printk("\nuart test start\n");
		printk("button 1: exit \nbutton 2: send uart signal\n\n");

		/* UART loop */
		for (i = 0;; i++) {
			k_msgq_get(&uart_msg_q, &uart_msg, K_FOREVER);

			// TODO: create function to check uart msg similar to mqtt

			if (strcmp(uart_msg, "{exit}") == 0) {
				uart_exit();
				data_available_to_send = true;
				break;
			}
			else {
				// printk("\nUART message: %s", uart_msg);
				strcpy(uart_data_array[i], uart_msg);
			}
		}
		/* UART loop end */
		
		if(strcmp(uart_data_array[0], "") != 0) {
			i = 0;
			while(strcmp(uart_data_array[i], "") != 0){
				printk("\nArray %d: %s", i, uart_data_array[i]);
				i++;
			}
		}
		else {
			printk("no data saved\n");
		}

		printk("\nuart test end\n\n");		
		printk("\n\npress button 1 to start gps test\n\n");

		button_wait();

		/******* GPS START *******/
		
		// TODO: Test on separate thread

		printk("gps test start\n");

		app_gps(20, 500);

		printk("\ngps test end\n\n");
		printk("\n\npress button 1 to start mqtt test\n\n");

		button_wait();

		/******* MQTT START *******/

		set_button_config(2);

		printk("mqtt test start\n");

		int err;

		if(!mqtt_init_complete) {

			err = app_mqtt_init_and_connect();
			if (err != 0) {
				return;
			}

			mqtt_init_complete = true;
		}
		else {

			err = app_mqtt_connect();
			if (err != 0) {
				return;
			}
		}

		// TODO: store mqtt commands into an array
		/* MQTT loop */
		while (1) {
			err = app_mqtt_run();

			k_msgq_get(&mqtt_msg_q, &mqtt_msg, K_NO_WAIT);

			int mqtt_val = check_mqtt_msg(&mqtt_msg, sizeof(mqtt_msg));

			if (mqtt_val == 1 || err != 0) {
				strcpy(mqtt_msg, "");
				break;
			}
			else if (mqtt_val == 2) {

				printk("button choice: 2\n\n");
				strcpy(mqtt_msg, "");
			}

			if (data_available_to_send) {
				publish_uart_data();
				data_available_to_send = false;
			}
		} 
		/* MQTT loop end */

		err = app_mqtt_disconnect();
		if (err != 0) {
			printk("\nDisconnect unsuccessful: %d", err);
			printk("\n\nClosing program, check for errors in code lol");
			return;
		}

		printk("**** NordicOasys test end ****\n");

	}
	/******* MAIN LOOP END *******/
}

/* Function ideas and TODOs

- Allow option to choose between "main parts" 
  e.g. using switch case to choose between UART and MQTT.

- Fill up dev_param struct. Should contain parameters set by user.
  Should be changeable through MQTT.

*/