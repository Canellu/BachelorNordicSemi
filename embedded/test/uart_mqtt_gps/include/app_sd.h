#pragma once
#ifndef _APP_SD_H_
#define _APP_SD_H_

enum sd_event_type
{
    READ_JSON,
    SEND_FILE_INFO,
    READ_FILE,
    WRITE_FILE
};

typedef struct
{
    enum sd_event_type event;

    uint16_t year;
    uint8_t month;
    uint8_t day;

    uint8_t json_string[256];
} oasys_data_t;

void app_sd(void);
void app_sd_thread(void *unused1, void *unused2, void *unused3);

#endif
