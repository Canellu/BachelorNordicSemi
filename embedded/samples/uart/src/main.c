
#include <zephyr.h>
#include <drivers/uart.h>

static void uart_cb(const struct device *dev, void *context)
{
	uint8_t buf[] = {"hi\n"};

	uart_irq_update(dev);

	if (uart_irq_tx_ready(dev))
	{
		(void)uart_fifo_fill(dev, buf, sizeof(buf));
		uart_irq_tx_disable(dev);
		// printk("sent\n");
	}

	if (uart_irq_rx_ready(dev))
	{
		uint8_t buf[10];
		int len = uart_fifo_read(dev, buf, sizeof(buf));
		// uart_buf[len] = 0;

		printk("%s", buf);
		// if (len) {
		// 	printk("read %d bytes\n", len);
		// }
	}
}

void main(void)
{
	const struct device *uart = device_get_binding("UART_1");
	__ASSERT(uart, "Failed to get the device\n\n");

	uart_irq_callback_set(uart, uart_cb);
	uart_irq_rx_enable(uart);
	printk("UART loopback start!\n");
	while (1)
	{
		uart_irq_tx_enable(uart);
		k_sleep(K_MSEC(2000));
	}
}