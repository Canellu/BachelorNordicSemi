#pragma once
#ifndef _APP_UART_H_
#define _APP_UART_H_

enum uart_device_type {
	UART_1,
	UART_2
};


void uart_dev_init(enum uart_device_type uart_dev_no);
void uart_start(enum uart_device_type uart_dev_no);
void uart_exit(enum uart_device_type uart_dev_no);
void uart_send(enum uart_device_type uart_dev_no, uint8_t *msg);

#endif
