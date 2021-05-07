
#include <zephyr.h>
#include <drivers/uart.h>
#include <string.h>
#include "app_uart.h"
#include <logging/log.h>

LOG_MODULE_REGISTER(uart_module);

// VARIABLES FROM MAIN
extern struct k_msgq uart_msg_q;

// LOCAL
const struct device *dev_uart1;
const struct device *dev_uart2;

static uint8_t rx_buf[256];
static uint8_t tx_buf[40];

static void uart_cb(const struct device *dev_uart, void *context)
{

	uart_irq_update(dev_uart);

	// send uart msg, runs when uart_irq_tx_enable is run
	if (uart_irq_tx_ready(dev_uart))
	{
		// LOG_DBG("tx: %s", log_strdup(tx_buf));
		unsigned int key = irq_lock();
		(void)uart_fifo_fill(dev_uart, tx_buf, strlen(tx_buf));
		uart_irq_tx_disable(dev_uart);
		memset(tx_buf, 0, sizeof(tx_buf));
		irq_unlock(key);
	}

	// read uart msg, only reads in JSON format
	// adds to message queue when entire JSON is received
	if (uart_irq_rx_ready(dev_uart))
	{
		uint8_t buf[4] = "";
		int len = uart_fifo_read(dev_uart, buf, sizeof(buf));
		buf[len] = 0;

		// handling overflow
		if (strlen(rx_buf) >= sizeof(rx_buf) - 1)
		{
			printk("overflow, deleting str\n");
			memset(rx_buf, 0, sizeof(rx_buf));
		}
		// delimiter
		else if (buf[0] == '\r')
		{
			LOG_DBG("rx: %s", log_strdup(rx_buf));
			k_msgq_put(&uart_msg_q, &rx_buf, K_NO_WAIT);
			memset(rx_buf, 0, sizeof(rx_buf));
		}
		// filter for unwanted characters
		else if (buf[0] == '\n')
		{
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
	switch (uart_dev_no)
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
	switch (uart_dev_no)
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
	switch (uart_dev_no)
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
void uart_send(enum uart_device_type uart_dev_no, char *msg, size_t len)
{
	if (len > 254)
	{
		LOG_INF("max UART string is 254 characters");
	}
	else
	{
		int char_cnt = 0;

		for (int i = 0; i < len; i++)
		{
			// add characters to uart buffer
			tx_buf[char_cnt++] = msg[i];

			// if uart buffer filled
			if ((char_cnt == 32) || (i == (len - 1)))
			{
				char_cnt = 0;

				switch (uart_dev_no)
				{
				case UART_1:
					uart_irq_tx_enable(dev_uart1);
					break;

				case UART_2:
					uart_irq_tx_enable(dev_uart2);
					break;
				}
				k_sleep(K_MSEC(5));
			}
		}

		strcat(tx_buf, "\r");
		strcat(tx_buf, "\0");

		switch (uart_dev_no)
		{
		case UART_1:
			uart_irq_tx_enable(dev_uart1);
			break;

		case UART_2:
			uart_irq_tx_enable(dev_uart2);
			break;
		}
		k_sleep(K_MSEC(5));
	}
}
/* Function ideas and TODOs

- specify UART to use
  currently uses switch case to find out which uart device to use.
  Horrible, need a switch case in each function.
  Possible fix: create device in main and pass device to functions, requires big overhaul.
*/