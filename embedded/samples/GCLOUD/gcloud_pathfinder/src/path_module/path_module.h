/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#ifndef _PATH_MODULE_H_
#define _PATH_MODULE_H_
/* CODE FOR nRF9160 USING ZEPHYR */
/* Intended to handle a path */

#include <net/mqtt_socket.h>
#include <gps.h>

struct gps_marker
{
    char uniqueID[CONFIG_UNIQUE_ID_LEN];
    double lat;
    double lng;
};

/**
 * @brief Initialize the path module
 */
void path_module_init();

/**
 * @brief Return the ID of current path.
 * 
 * @retval "LL_XXXXXXX" Where LL will be the length of the path and XXXXXXX will be an unique stiring of characters.
 */
char *path_module_get_path_id();

/**
 * @brief Formatted printing of the whole path.
 */
void path_module_print_path();

/**@brief Handler for when receiving GPS data from Google Cloud.
 * 
 * @param[in] message MQTT message containing the path.
 * 
 * @note Calls the static function get_direction with the data received.
 */
void path_module_received_path_handler(const struct mqtt_publish_message *message);

/**@brief Send command to Google Cloud Functions to make them remove the first marker from firebase.
 * 
 * @note This will NOT edit the local path. Will have to receive the new path from the cloud to update this.
 */
void path_module_send_remove_marker_cmd();

/**
 * @brief sets the gps_data struct in the scope of the path_module so it's usable there.  
 * 
 * 
 * @param[in] gps_data struct to be written to the path module
 */
void path_module_provide_position(struct gps_data *gps_data_origin);

#endif /* _PATH_MODULE_H_ */
