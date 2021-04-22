#pragma once
#ifndef _APP_GPS_H_
#define _APP_GPS_H_

typedef struct
{
	uint8_t gps_string[128];

	uint16_t year;
	uint8_t month;
	uint8_t day;

	uint8_t hour;
	uint8_t minute;
	uint8_t seconds;
} glider_gps_data_t;

int app_gps(glider_gps_data_t *app_gps_data, int64_t gps_timeout, int retry_interval);

#endif
