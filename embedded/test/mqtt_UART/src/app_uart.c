
#include <zephyr.h>
#include <drivers/uart.h>
#include <string.h>
#include "app_uart.h"

// VARIABLES FROM MAIN

extern int new_data;
extern int button_choice;
extern uint8_t uart_data[128];

uint8_t uart_data2[128];

// LOCAL
const struct device *uart;
static uint8_t buf[] = {"hi,"};
int i = 0;

static void uart_cb(const struct device *dev, void *context)
{

	uart_irq_update(dev);

	if (uart_irq_tx_ready(dev)) {
		(void)uart_fifo_fill(dev, buf, sizeof(buf));
		uart_irq_tx_disable(dev);
		printk("sent\n");
	}

	if (uart_irq_rx_ready(dev)) {
		uint8_t buf[10];
		int len = uart_fifo_read(dev, buf, sizeof(buf));
		buf[len] = 0;
		// check for newline due to arduino sending newline, only used for testing purposes
		// MOVE TO MAIN AND CLEAN UP
		if((strcmp(buf, ",") == 0 || strcmp(buf, "\n") == 0) && !uart_irq_is_pending(dev))
		{
			strcat(uart_data, buf);
			//printk("\n%s", uart_data2);
			// int test = uart_irq_is_pending(dev);
			// printk(" %d\n", test);
			if(strcmp(uart_data2, "nice") == 0)
			{
				printk("   yay");
				button_choice = 1;
			}

			strcpy(uart_data2, "");

		}
		else
		{
			strcat(uart_data, buf);
			strcat(uart_data2, buf);
		}
		printk("%s", buf);

	}

}

// get device binding, only call once
void uart_dev_init(void)
{
	uart = device_get_binding("UART_1");
	__ASSERT(uart, "Failed to get the device\n\n");

	uart_irq_callback_set(uart, uart_cb);

}

// start uart, run before using uart functions,
// add more here if other functions need to be enabled before uart use
void uart_start(void)
{
	uart_irq_rx_enable(uart);
}

// turn off functions, might have to come back here and more things to disable
void uart_exit(void)
{
	uart_irq_rx_disable(uart);
}

// send data to uart device
// TODO: add parameters to choose data to be sent, currently only sends "hi"
// should be useable in main
void uart_send(void)
{
	uart_irq_tx_enable(uart);
}

/* Function ideas and TODOs

buffer read until a delimiter, save to array and start saving to array+1
- May not be the best idea to implement here, probably better to create this function in main
  to avoid creating too many variables that overlap.

- Overhaul saving of data. Ideal: just send buffer to main (e.g. mailbox), let main append data
  and split into array.

- Add timestamp to when the data was received.

*/