#pragma once
#ifndef _APP_BUTTON_H_
#define _APP_BUTTON_H_


void button_dev_init(void);
void button_handler(uint32_t button_states, uint32_t has_changed);
int toggleLED(uint8_t *data, size_t len);

#endif
