#pragma once
#ifndef _APP_SD_H_
#define _APP_SD_H_

typedef struct {
    uint16_t	year;
	uint8_t		month;
	uint8_t		day;
    
    uint8_t     json_string[256];
} oasys_data_t;

void app_sd(void);

#endif
