
#include <zephyr.h>
#include <drivers/uart.h>
#include <string.h>
#include "app_uart.h"

// VARIABLES FROM MAIN
extern struct k_msgq uart_msg_q;

// LOCAL
const struct device *dev_uart;
static uint8_t str_buf[128];
static uint8_t tx_buf[] = {"hi,"};

static void uart_cb(const struct device *dev_uart, void *context)
{

	uart_irq_update(dev_uart);

	if (uart_irq_tx_ready(dev_uart)) {
		(void)uart_fifo_fill(dev_uart, tx_buf, sizeof(tx_buf));
		uart_irq_tx_disable(dev_uart);
		printk("sent\n");
	}

	if (uart_irq_rx_ready(dev_uart)) {
		uint8_t buf[10];
		int len = uart_fifo_read(dev_uart, buf, sizeof(buf));
		buf[len] = 0;

		if (strcmp(buf, "{") == 0)
		{
			strcpy(str_buf, "");
			strcat(str_buf, buf);
		}
		else if (strcmp(buf, "}") == 0)
		{
			strcat(str_buf, buf);
			k_msgq_put(&uart_msg_q, &str_buf, K_NO_WAIT);
		}
		else
		{
			strcat(str_buf, buf);
		}

	}
}

// get device binding, only call once
void uart_dev_init(void)
{
	dev_uart = device_get_binding("UART_1");
	__ASSERT(dev_uart, "Failed to get the device\n\n");

	uart_irq_callback_set(dev_uart, uart_cb);

}

// start uart, run before using uart functions,
// add more here if other functions need to be enabled before uart use
void uart_start(void)
{
	uart_irq_rx_disable(dev_uart);
	uart_irq_rx_enable(dev_uart);
}

// turn off functions, might have to come back here and more things to disable
void uart_exit(void)
{
	uart_irq_rx_disable(dev_uart);
}

// send data to uart device
// TODO: add parameters to choose data to be sent, currently only sends "hi"
// should be useable in main
void uart_send(void)
{
	uart_irq_tx_enable(dev_uart);
}

/* Function ideas and TODOs

buffer read until a delimiter, save to array and start saving to array+1
- May not be the best idea to implement here, probably better to create this function in main
  to avoid creating too many variables that overlap.

- Overhaul saving of data. Ideal: just send buffer to main (e.g. mailbox), let main append data
  and split into array.

- Add timestamp to when the data was received.

*/