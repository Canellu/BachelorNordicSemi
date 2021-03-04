
#include <zephyr.h>
#include <drivers/uart.h>
#include <string.h>
#include "app_uart.h"

// VARIABLES FROM MAIN
extern struct k_msgq uart_msg_q;

// LOCAL
const struct device *dev_uart1;
const struct device *dev_uart2;

static uint8_t rx_buf[128];
static uint8_t tx_buf[128];

static void uart_cb(const struct device *dev_uart, void *context)
{

	uart_irq_update(dev_uart);

	// send uart msg, runs when uart_irq_tx_enable is run
	if (uart_irq_tx_ready(dev_uart)) {
		(void)uart_fifo_fill(dev_uart, tx_buf, sizeof(tx_buf));
		uart_irq_tx_disable(dev_uart);
		printk("\nsent");
	}

	// read uart msg, only reads in JSON format
	// adds to message queue when entire JSON is received
	if (uart_irq_rx_ready(dev_uart)) {
		uint8_t buf[10];
		int len = uart_fifo_read(dev_uart, buf, sizeof(buf));
		buf[len] = 0;

		if (strcmp(buf, "{") == 0)
		{
			strcpy(rx_buf, "");
			strcat(rx_buf, buf);
		}
		else if (strcmp(buf, "}") == 0)
		{
			strcat(rx_buf, buf);
			printk("\nuart msg received");
			k_msgq_put(&uart_msg_q, &rx_buf, K_NO_WAIT);		
		}
		else
		{
			strcat(rx_buf, buf);
		}

	}
}

// get device binding, only call once
void uart_dev_init(enum uart_device_type uart_dev_no)
{
	switch(uart_dev_no)
	{
		case UART_1:
			dev_uart1 = device_get_binding("UART_1");
			__ASSERT(dev_uart1, "Failed to get the device\n\n");
			uart_irq_callback_set(dev_uart1, uart_cb);
			break;

		case UART_2:
			dev_uart2 = device_get_binding("UART_2");
			__ASSERT(dev_uart2, "Failed to get the device\n\n");
			uart_irq_callback_set(dev_uart2, uart_cb);
			break;

		default:
			printk("\n\nUnknown UART device");
			break;
	}

}

// start uart, run before using uart functions,
// add more here if other functions need to be enabled before uart use
void uart_start(enum uart_device_type uart_dev_no)
{
	switch(uart_dev_no)
	{
		case UART_1:
			uart_irq_rx_disable(dev_uart1);
			uart_irq_rx_enable(dev_uart1);
			break;

		case UART_2:
			uart_irq_rx_disable(dev_uart2);
			uart_irq_rx_enable(dev_uart2);
			break;
	}
}

// turn off functions, might have to come back here and more things to disable
void uart_exit(enum uart_device_type uart_dev_no)
{
	switch(uart_dev_no)
	{
		case UART_1:
			uart_irq_rx_disable(dev_uart1);
			break;

		case UART_2:
			uart_irq_rx_disable(dev_uart2);
			break;
	}
}

// send data to uart device
void uart_send(enum uart_device_type uart_dev_no, uint8_t *msg)
{
	strcpy(tx_buf, msg);

	switch(uart_dev_no)
	{
		case UART_1:
			uart_irq_tx_enable(dev_uart1);
			break;

		case UART_2:
			uart_irq_tx_enable(dev_uart2);
			break;
	}
}

/* Function ideas and TODOs

- specify UART to use
  currently uses switch case to find out which uart device to use.
  Horrible, need a switch case in each function.
  Possible fix: create device in main and pass device to functions, requires big overhaul.
*/