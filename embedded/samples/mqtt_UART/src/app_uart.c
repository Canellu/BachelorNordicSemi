
#include <zephyr.h>
#include <drivers/uart.h>
#include <string.h>
#include "app_uart.h"

// VARIABLES FROM MAIN

extern int new_data;
extern uint8_t uart_data[128];

// LOCAL
const struct device *uart;
static uint8_t buf[] = {"hi,"};

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
		// uart_buf[len] = 0;
		strcat(uart_data, buf);

		printk("%s", buf);
		// if (len) {
		// 	printk("read %d bytes\n", len);
		// }
	}

}

void uart_init(void)
{
	uart = device_get_binding("UART_1");
	__ASSERT(uart, "Failed to get the device\n\n");

	uart_irq_callback_set(uart, uart_cb);
	uart_irq_rx_enable(uart);
}

// turn off functions, might have to come back here and more things to disable
void uart_exit(void)
{
	uart_irq_rx_disable(uart);
}

void uart_send(void)
{
	uart_irq_tx_enable(uart);
}

/* Function ideas

buffer read until a delimiter, save to array and start saving to array+1

*/