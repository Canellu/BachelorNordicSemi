#pragma once
#ifndef _APP_GPS_H_
#define _APP_GPS_H_

#include <time.h>

typedef struct
{
	uint8_t gps_string[128];

	struct tm time;

} glider_gps_data_t;

int app_gps(glider_gps_data_t *app_gps_data, int64_t gps_timeout, int retry_interval);

#endif
