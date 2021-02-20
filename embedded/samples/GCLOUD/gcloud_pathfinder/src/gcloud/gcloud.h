/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#ifndef _GCLOUD_H_
#define _GCLOUD_H_

#include <net/mqtt_socket.h>

/**
 * @brief Connects the device to Google Cloud, and subscribes to Google Cloud's configuration topic.
 * 
 * @note gcloud_provision must be called before this.
 * @note Shall be called before trying to send data to Google Cloud.
 * @note The modem must be turned on, and connected before this function can be called.
 */
void gcloud_connect(void);

/**
 * @brief Initializes the module and provisions the modem so that an TLS 
 * connection can be established to Google Cloud.
 * 
 * @note Shall be called before the modem is turned on.
 * @note Shall be called before gcloud_connect.
 * @note If module initialization fails, no module APIs shall be called.
 * 
 * @retval 0 or an error code indicating reason for failure.
 */
int gcloud_provision(void);

/**
 * @brief Disconnects the device from Google Cloud, then reboots.
 * 
 * @retval 0 or an error code indicating reason for failure.
 */
int gcloud_disconnect(bool RECONNECT);

/**
 * @brief Publishes device gps to Google Cloud.
 * 
 * @note Google Cloud only supports QoS 0 and QoS 1, not QoS 2
 * 
 * @param[in] data A pointer to the data to be sent.
 * @param[in] size The size of the data to be sent.
 * @param[in] qos The Quality of Serive level to be used when sending the
 *            telemetry.
 * 
 * @retval 0 or an error code indicating reason for failure.
 */
int gcloud_publish_gps(u8_t *data, u32_t size, enum mqtt_qos qos);

/**
 * @brief Publishes device state to Google Cloud.
 * 
 * @note Google Cloud does only support QoS 0 and QoS 1, not Qos 2.
 * @note Google Cloud limits state updates to 1 update per second, per device. 
 *       However, for best results, device state should be updated much less often 
 *       - at most, once every 10 second.
 * 
 * @param[in] data A pointer to the data to be sent.
 * @param[in] size The size of the data to be sent.
 * @param[in] qos The Quality of service level to be used when sending the state.
 * 
 * @retval 0 or an error code indicating reson for failure.
 */
int gcloud_publish_state(u8_t *data, u32_t size, enum mqtt_qos qos);


/**
 * @brief Returns current connection status.
 * 
 * @retval 0(disconnected) or 1(connected)
 */
int gcloud_get_connection_status();

/**
 * @brief Start the the gcloud thread.
 * 
 * @note The modem must be turned on, and connected before this function can be called.
 */
void gcloud_start_thread(void);

#endif /* _GCLOUD_H_ */