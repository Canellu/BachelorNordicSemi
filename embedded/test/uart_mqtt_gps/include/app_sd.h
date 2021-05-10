#pragma once
#ifndef _APP_SD_H_
#define _APP_SD_H_

enum sd_event_type
{
    READ_JSON,
    READ_JSON_4G,
    FIND_FILE,
    SEND_FILE_INFO,
    READ_FILE,
    OVERWRITE_FILE,
    WRITE_FILE
};

typedef struct
{
    enum sd_event_type event;
    uint8_t filename[32];
    uint8_t string[256];
} sd_msg_t;

void app_sd(void);
void app_sd_thread(void *unused1, void *unused2, void *unused3);

#endif
