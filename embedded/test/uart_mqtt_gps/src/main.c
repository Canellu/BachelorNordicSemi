#include <zephyr.h>
#include <nrf_socket.h>
#include <dk_buttons_and_leds.h>
#include <kernel.h>
#include <string.h>
#include <stdio.h>

#include "app_button.h"
#include "app_uart.h"
#include "app_mqtt.h"
#include "app_gps.h"
#include "app_sd.h"


/* Stack definition for application */
static K_THREAD_STACK_DEFINE(sd_stack_area, 4096);

/* Threads */
static struct k_thread sd_thread;

k_tid_t sd_tid;


/* STRUCTS AND ENUMS */

// Enumerations
// CANDO: typedef everything

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
enum uart_device_type uart_dev1 = UART_1;

struct k_msgq uart_msg_q;
static uint8_t uart_msgq_buffer[3 * 128];
static uint8_t uart_msg[128];

// TODO: change to general data_array instead of just uart data
static uint8_t uart_data_array[16][128] = { "" };
int data_size = 0; // keeps track of no. of messages stored in data array. 
				   // UPDATE THIS VALUE WHEN SAVING INTO DATA ARRAY

// GPS variables
struct k_msgq gps_msg_q;
static uint8_t gps_msgq_buffer[2 * sizeof(oasys_gps_data_t)];
static oasys_gps_data_t gps_data;

// MQTT variables
struct k_msgq mqtt_msg_q;
static uint8_t mqtt_msgq_buffer[3 * 128];
static uint8_t mqtt_msg[128];

static bool mqtt_init_complete = false;
static bool data_available_to_send = false;

// SD Card variables
struct k_msgq sd_msg_q;
static uint8_t sd_msgq_buffer[5 * sizeof(oasys_data_t)];

// Time variables
static uint16_t current_year	= -1;
static uint8_t current_month 	= -1;
static uint8_t current_day   	= -1;


/* FUNCTION DECLARATIONS */

// publish all data that is currently saved from uart
// REMINDER: Double check after implementing SD card
static int publish_uart_data() 
{
	int i = 0;

	while(strcmp(uart_data_array[i], "") != 0) {
		app_data_publish(uart_data_array[i], strlen(uart_data_array[i])+1);
		k_sleep(K_MSEC(500));
		i++;
	}
	data_available_to_send = false;

	return 0;
}

// test messages received from mqtt
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

// fills out current date, sends string and event type
static int sd_msg_fill_send(void *data_string, enum sd_event_type event)
{
	oasys_data_t sd_msg;

	sd_msg.event = event;

	strcpy(sd_msg.json_string, data_string);

	sd_msg.year = current_year;
	sd_msg.month = current_month;
	sd_msg.day = current_day;

	// add to sd card message queue
	k_msgq_put(&sd_msg_q, &sd_msg, K_NO_WAIT);

	return 0;
}


// initialize message queues
//TODO: replace sizeof parameter with type instead of variable
static int message_queue_init()
{
	// button
	k_msgq_init(&button_msg_q, button_msgq_buffer, sizeof(int), 3);
	k_msgq_init(&button_msg_qr, button_msgq_buffer, sizeof(int), 3);

	// UART
	k_msgq_init(&uart_msg_q, uart_msgq_buffer, sizeof(uart_msg), 3);

	// GPS
	k_msgq_init(&gps_msg_q, gps_msgq_buffer, sizeof(gps_data), 2);

	// MQTT
	k_msgq_init(&mqtt_msg_q, mqtt_msgq_buffer, sizeof(mqtt_msg), 3);

	// SD
	k_msgq_init(&sd_msg_q, sd_msgq_buffer, sizeof(oasys_data_t), 5);

	return 0;
}

//TODO: pass devices to initialise
static int device_inits()
{	
	button_dev_init();
	dk_leds_init();
	uart_dev_init(uart_dev1);
	return 0;
}

// purge leftover messages from message queue
static int message_queue_reset()
{
	// NOTE: THIS IS ONLY HERE UNTIL SD CARD STORAGE IS OKAY
	data_size = 0;
	int i = 0;
	while(strcmp(uart_data_array[i], "") != 0) {
		strcpy(uart_data_array[i], "");
		i++;
	}
	
	k_msgq_purge(&button_msg_q);
	k_msgq_purge(&uart_msg_q);
	k_msgq_purge(&gps_msg_q);
	k_msgq_purge(&mqtt_msg_q);
	return 0;
}

static int uart_module()
{
	set_button_config(1);
	uart_start(uart_dev1);

	printk("\nuart test start\n");
	printk("button 1: exit \nbutton 2: send uart signal\n\n");

	/* UART loop */
	for (int i = 0;; i++) {
		k_msgq_get(&uart_msg_q, &uart_msg, K_FOREVER);

		// TODO: create function to check uart msg similar to mqtt

		if (strcmp(uart_msg, "{exit}") == 0) {
			uart_exit(uart_dev1);
			data_available_to_send = true;
			break;
		}
		else {

			// TODO: test timestamp to check if new day

			sd_msg_fill_send(uart_msg, WRITE_FILE);

			// printk("\nUART message: %s", uart_msg);
			// add to data array
			// strcpy(uart_data_array[i], uart_msg);
		}
	}
	/* UART loop end */

	// print all uart data
	if(strcmp(uart_data_array[0], "") != 0) {
		int i = 0;
		while(strcmp(uart_data_array[i], "") != 0){
			printk("\nArray %d: %s", i, uart_data_array[i]);
			i++;
			data_size++;
		}
	}
	else {
		printk("no data saved\n");
	}

	printk("\nuart test end\n\n");

	return 0;
}

// TODO: Test on separate thread?
// Currently on hold until LTE and GPS can function at the same time
static int gps_module()
{
	printk("\n\npress button 1 to start gps test\n\n");

	button_wait();

	printk("gps test start\n");

	// CANDO: change parameter to timeout instead of no. of retries
	app_gps(5, 500);
	k_msgq_get(&gps_msg_q, &gps_data, K_NO_WAIT);

	// print to terminal
	// {"lng":"23.771611","lat":"61.491275"}
	printk("\n%s", gps_data.gps_string);

	// update date CANDO: create function?
	current_year = gps_data.year;
	current_month = gps_data.month;
	current_day = gps_data.day;

	// save gps data on sd card
	sd_msg_fill_send(gps_data.gps_string, WRITE_FILE);

	// send time to sensors
	uint8_t time_millis_str[16];
	uint32_t time_millis = ((gps_data.hour*60*60)+(gps_data.minute*60)+gps_data.seconds)*1000;
	snprintf(time_millis_str, sizeof(time_millis_str), "%02u", time_millis);

	uart_send(uart_dev1, time_millis_str);

	// add to data array
	strcpy(uart_data_array[data_size++], gps_data.gps_string);
	data_available_to_send = true;

	printk("\ngps test end\n\n");

	return 0;
}

static int wifi_module()
{
	set_button_config(1);
	uart_start(uart_dev1);

	printk("\nwifi test start\n");

	// send wifi command to wifi controller
	uart_send(uart_dev1, "{wifi}\n");

	/* wifi loop */
	for (int i = 0;; i++) {

		// test messages from wifi controller
		k_msgq_get(&uart_msg_q, &uart_msg, K_FOREVER);
		printk("\n%s", uart_msg);

		if (strcmp(uart_msg, "{exit}") == 0) {
			uart_exit(uart_dev1);
			break;
		}
		// wifi setup success, send further commands
		else if (strcmp(uart_msg, "{wifi ok}") == 0)
		{
			printk("\nReceived wifi ok");
			uart_send(uart_dev1, "{mqtt}\n");
		}
		// send data from uart_data_array
		else if (strcmp(uart_msg, "{mqtt ok}") == 0)
		{
			printk("\nReceived mqtt ok");
			publish_uart_data();
		}
		// else if (strcmp(uart_msg, "{keyword}") == 0)
		// {
		// 	//something
		// }
		// default print to terminal
		else
		{
			printk("\n%s", uart_msg);
		}

	}
	/* wifi loop end */
	return 0;
}

static int mqtt_module()
{

	printk("\n\npress button 1 to start mqtt test\n\n");
	button_wait();

	set_button_config(2);

	printk("mqtt test start\n");

	int err;

	// mqtt init
	if(!mqtt_init_complete) {

		err = app_mqtt_init_and_connect();
		if (err != 0) {
			return err;
		}

		mqtt_init_complete = true;
	}
	// reconnect
	else {

		err = app_mqtt_connect();
		if (err != 0) {
			return err;
		}
	}

	// TODO: store mqtt commands into an array
	/* MQTT loop */
	while (1) {

		// mqtt function
		err = app_mqtt_run();

		// test messages received from mqtt
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
		}
	} 
	/* MQTT loop end */

	err = app_mqtt_disconnect();
	if (err != 0) {
		printk("\nDisconnect unsuccessful: %d", err);
		printk("\n\nClosing program, check for errors in code lol");
		return err;
	}

	return 0;
}

/* MAIN */

void main(void)
{

	/* device inits and configurations */
	device_inits();

	// message queues
	message_queue_init();

	// sd card thread
	sd_tid = k_thread_create(&sd_thread, sd_stack_area, K_THREAD_STACK_SIZEOF(sd_stack_area),
							 (k_thread_entry_t)app_sd_thread, NULL, NULL, NULL,
							 7, 0, K_NO_WAIT);

	printk("\n\n**** NordicOasys v0.5 - Started ****\n\n");
	printk("press button 1 to start\n\n");

	button_wait();

	while(1) {

		message_queue_reset();

		gps_module();

		// TEMPORARY TEST PRINTING WHAT WILL BE SENT TO SD CARD FROM GPS

		// oasys_data_t test_data;

		// k_msgq_get(&sd_msg_q, &test_data, K_NO_WAIT);

		// printk("\n%02d-%02d-%02d", test_data.year, test_data.month, test_data.day);
		// printk("\n%s", test_data.json_string);


		uart_module();

		// for testing purposes, read JSON
		sd_msg_fill_send("", READ_JSON);
		sd_msg_fill_send("", READ_JSON);

		// read entire file
		k_sleep(K_MSEC(2000));
		sd_msg_fill_send("", READ_FILE);

		// app_sd();

		printk("press button 1 to start mqtt\n");
		printk("press button 2 to start wifi\n\n");
		set_button_config(3);
		k_msgq_get(&button_msg_qr, &button_val, K_FOREVER);

		if (button_val == 1) {
			mqtt_module();
		}
		else if (button_val == 2) {
			wifi_module();
		}


	}

	printk("**** NordicOasys test end ****\n");
}

/* Function ideas and TODOs

- Allow option to choose between "main parts" 
  e.g. using switch case to choose between UART and MQTT.

- Fill up dev_param struct. Should contain parameters set by user.
  Should be changeable through MQTT.

- Save messages received from MQTT into array similar to data.

*/