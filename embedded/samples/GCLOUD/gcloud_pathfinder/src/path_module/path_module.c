/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#include <logging/log.h>
LOG_MODULE_REGISTER(path_module);

#include "path_module.h"

#include <zephyr.h>
#include <zephyr/jwt.h>
#include <string.h>
#include <errno.h>
#include <cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <nrf_socket.h>
#include <math.h>

#include "ui.h"
#include "path_module.h"
#include "gcloud.h"
#if defined(CONFIG_BLE_CONNECT_TO_MICRO_BIT)
#include "micro_bit_compass.h"
#endif /* CONFIG_BLE_CONNECT_TO_MICRO_BIT */

#define PI 3.14159265
#define UNIQUE_PATH_ID_LENGTH 11

/* Global variables */
static char unique_path_id[UNIQUE_PATH_ID_LENGTH] = "00_0000000";
static struct gps_marker gps_path[CONFIG_GPS_PATH_MAX_LENGTH];
static struct gps_data gps_data;
static u8_t gps_data_is_set = 0;

/* Work variables */
static struct k_work remove_marker_work;

/* Forward declerations */
void path_module_send_remove_marker_cmd();

/* Function declerations */
inline double to_radians(double degrees)
{
    return degrees * (PI / 180.0);
}

void path_module_print_path()
{
    char latStr[20];
    char lngStr[20];

    printk("Path START\n");
    for (int i = 0; i < CONFIG_GPS_PATH_MAX_LENGTH; i++)
    {
        if (gps_path[i].uniqueID[0] != '\0')
        {
            sprintf(latStr, "%.5f", gps_path[i].lat);
            sprintf(lngStr, "%.5f", gps_path[i].lng);
            printk("ID: %s,\tlat: %s,\tlng: %s\n", gps_path[i].uniqueID, latStr, lngStr);
        }
    }
    printk("Path END\n");
}

static double gps_distance_measure(double lat1, double lng1, double lat2, double lng2)
{
    double rad_lat1 = to_radians(lat1);
    double rad_lng1 = to_radians(lng1);
    double rad_lat2 = to_radians(lat2);
    double rad_lng2 = to_radians(lng2);
    double R = 6371000;

    double x = (rad_lng2 - rad_lng1) * cos((rad_lat1 + rad_lat2) / 2);
    double y = (rad_lat2 - rad_lat1);
    double d = sqrt((x * x) + (y * y)) * R;

    return d;
}

/**@brief Compares received GPS data with user's GPS data to find direction.
 * Sends this direction to the Micro:Bit
 */
static void get_direction(struct gps_data *data)
{
    int err;
    char *direction = NULL;

    if (gps_path[0].uniqueID[0] != '\0')
    {
        double latitude_diff;
        double longitude_diff;
        double heading;

        double threshold = CONFIG_GPS_PATH_MARKER_REMOVAL_THRESHOLD;
        double distance = threshold + 1;

        path_module_print_path();
        distance = gps_distance_measure(gps_path[0].lat, gps_path[0].lng, data->pvt.latitude, data->pvt.longitude);

        latitude_diff = gps_path[0].lat - gps_data.pvt.latitude;
        longitude_diff = gps_path[0].lng - gps_data.pvt.longitude;

        heading = atan2(longitude_diff, latitude_diff) * 180.0 / PI;

        if (heading < 0)
        {
            heading += 360;
        }

        // Remove first marker if we have arrived at that marker
        if (distance < threshold)
        {
            direction = "OO:";
            k_work_submit(&remove_marker_work);
            LOG_INF("Marker reached.");
        }
        else if (gps_path[0].uniqueID[0] == '\0')
        {
            direction = "XX:";
        }
        else if ((heading >= 22.5) && (heading < 67.5))
        {
            direction = "NE:";
        }
        else if ((heading >= 67.5) && (heading < 112.5))
        {
            direction = "EE:";
        }
        else if ((heading >= 112.5) && (heading < 157.5))
        {
            direction = "SE:";
        }
        else if ((heading >= 157.5) && (heading < 202.5))
        {
            direction = "SS:";
        }
        else if ((heading >= 202.5) && (heading < 247.5))
        {
            direction = "SW:";
        }
        else if ((heading >= 247.5) && (heading < 292.5))
        {
            direction = "WW:";
        }
        else if ((heading >= 292.5) && (heading < 337.5))
        {
            direction = "NW:";
        }
        else
        {
            direction = "NN:";
        }
    }
    else
    {
        direction = "XX:";
    }

#if defined(CONFIG_BLE_CONNECT_TO_MICRO_BIT)
    err = micro_bit_write_dir(direction);
    if (err)
    {
        LOG_ERR("Could not write direction to micro:bit: %d", err);
    }
#endif /* CONFIG_BLE_CONNECT_TO_MICRO_BIT */
}

void path_module_received_path_handler(
    const struct mqtt_publish_message *message)
{
    if ((message == NULL) || (message->payload.data == NULL))
    {
        LOG_ERR("Error: empty message");
        return;
    }

    cJSON *data_json = cJSON_Parse(message->payload.data);
    if (data_json == NULL)
    {
        LOG_ERR("Error: cJSON_Parse failed");
        return;
    }
    const cJSON *ID = cJSON_GetObjectItem(data_json, "pathID");
    if (ID == NULL)
    {
        LOG_ERR("Error: empty ID");
        return;
    }

    memcpy(unique_path_id, ID->valuestring, UNIQUE_PATH_ID_LENGTH);
    cJSON *child = data_json->child;

    int counter = 0;
    for (int i = 0; i < CONFIG_GPS_PATH_MAX_LENGTH; i++)
    {
        if ((child != NULL) && (strcmp(child->string, "pathID") != 0))
        {
            double temp;
            counter++;
            const cJSON *lat = cJSON_GetObjectItem(child, "lat");
            const cJSON *lng = cJSON_GetObjectItem(child, "lng");
            if ((lat == NULL) || (lng == NULL))
            {
                LOG_ERR("Error: empty lat/lng");
                return;
            }
            memcpy(gps_path[i].uniqueID, child->string, CONFIG_UNIQUE_ID_LEN);
            sscanf(lat->valuestring, "%lf", &temp);
            gps_path[i].lat = temp;
            sscanf(lng->valuestring, "%lf", &temp);
            gps_path[i].lng = temp;
            child = child->next;
        }
        else
        {
            gps_path[i].uniqueID[0] = '\0';
        }
    }

    if (gps_data_is_set == 1)
    {
        get_direction(&gps_data);
    }

    cJSON_Delete(data_json);
}

char *path_module_get_path_id()
{
    return unique_path_id;
}

/**@brief Removes first marker from the path at firebase
 * when the nRF9160 DK has arrived at that marker
 */
void path_module_send_remove_marker_cmd()
{
    int err;

    if ((gps_path[0].uniqueID) && (gps_path[0].uniqueID[0] != '\0'))
    {
        LOG_DBG("Publishing marker removal to Google Cloud\n");
        cJSON *pvt_JSON = cJSON_CreateObject();
        if (pvt_JSON == NULL)
        {
            LOG_ERR("path_module_send_remove_marker_cmd failed: %s\n", strerror(ENOMEM));
            return;
        }

        cJSON *data = cJSON_CreateObject();
        if (data == NULL)
        {
            LOG_ERR("path_module_send_remove_marker_cmd failed: %s\n", strerror(ENOMEM));
            return;
        }

        cJSON *ID = cJSON_CreateString(gps_path[0].uniqueID);
        if (ID == NULL)
        {
            LOG_ERR("path_module_send_remove_marker_cmd failed: %s\n", strerror(ENOMEM));
            return;
        }

        cJSON_AddItemToObject(data, "uniqueMarkerID", ID);
        cJSON_AddItemToObject(pvt_JSON, "data", data);
        char *JSONString = cJSON_Print(pvt_JSON);
        cJSON_Delete(pvt_JSON);
        if (JSONString == NULL)
        {
            LOG_ERR("path_module_send_remove_marker_cmd failed: %s\n", strerror(ENOMEM));
            return;
        }

        err = gcloud_publish_gps(JSONString, strlen(JSONString), MQTT_QOS_1_AT_LEAST_ONCE);
        free(JSONString);
        if (err)
        {
            LOG_ERR("Could not send remove marker command: %s\n", strerror(err));
        }
        // Sleep, so as not to publish messages too often
        k_sleep(K_SECONDS(5));
    }
}

static void remove_marker_work_fn(struct k_work *work)
{
    path_module_send_remove_marker_cmd();
}

void path_module_init()
{
    k_work_init(&remove_marker_work, remove_marker_work_fn);
    for (int i = 0; i < CONFIG_GPS_PATH_MAX_LENGTH; i++)
    {
        gps_path[i].lat = 0;
        gps_path[i].lng = 0;
        gps_path[i].uniqueID[0] = '\0';
    }
}

void path_module_provide_position(struct gps_data *gps_data_origin)
{
    gps_data = *gps_data_origin;
    gps_data_is_set = 1;

    get_direction(&gps_data);
}
