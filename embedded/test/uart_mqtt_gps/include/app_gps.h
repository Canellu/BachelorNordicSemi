#pragma once
#ifndef _APP_GPS_H_
#define _APP_GPS_H_

typedef struct {
	uint8_t 	gps_string[128];

    double		longitude;
	double		latitude;

	uint16_t	year;
	uint8_t		month;
	uint8_t		day;

	uint8_t		hour;
	uint8_t		minute;
	uint8_t		seconds;
} oasys_gps_data_t;

int app_gps(int fix_retries, int retry_interval);

#endif