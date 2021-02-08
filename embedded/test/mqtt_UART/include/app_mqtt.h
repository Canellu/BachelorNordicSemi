#pragma once
#ifndef _APP_MQTT_H_
#define _APP_MQTT_H_


int modem_configure(void);

int app_data_publish(uint8_t *data, size_t len);

int app_mqtt_init(void);

int app_mqtt_run(void);

int app_mqtt_disconnect(void);

#endif