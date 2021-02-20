#pragma once
#ifndef _APP_MQTT_H_
#define _APP_MQTT_H_

int app_mqtt_init_and_connect(void);
int app_mqtt_connect(void);
int app_mqtt_run(void);
int app_mqtt_disconnect(void);
int app_data_publish(uint8_t *data, size_t len);

#endif