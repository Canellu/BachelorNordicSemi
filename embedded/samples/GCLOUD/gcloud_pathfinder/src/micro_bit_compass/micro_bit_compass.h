/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#ifndef _MICRO_BIT_COMPASS_H_
#define _MICRO_BIT_COMPASS_H_

#include <bluetooth/gatt.h>
#include <bluetooth/uuid.h>

/* CODE FOR nRF9160 USING ZEPHYR */
/* Intended to communicate with Micro:bit */

struct tec_attr
{
    struct bt_conn * conn;
    const struct bt_uuid *uuid; 
    u16_t value_handle;
    u16_t ccc_handle;
};

struct tec_param
{
    struct bt_conn * conn;
};

/**
 * @brief Initialize Micro:bit bluetooth connection. Enables uart sending
 *        over bluetooth.
 * 
 */
void micro_bit_compass_init(void);

/**
 * @brief Send a direction to the Micro:bit over bluetooth to make 
 *        create an arrow on its screen.
 * 
 * @param[in] direction A string which spesify what direction the on the 
 *            Micro:bit will point.
 *            Must be one of the following strings:
 *            "NN:", "NW:", "WW:", "SW:", "SS:", "SE:", "EE:", "NE:".
 * 
 * @note North is up from the perspective of the Micro:bit
 * 
 * @note: Crashes if called more frequently than once per K_MSEC(500).
 * 
 * @note Actually just send a string over bluetooth uart tx. Micro:bit must
 *       be set up to display the arrows.
 * 
 * @retval 0 or an error code indicating reason for failure.
 */
int micro_bit_write_dir(char * direction);

/**
 * @brief Check if Micro:Bit is is finished connecting and discovering.
 * 
 * @retval 1 if finished, and 0 if not.
 */
int micro_bit_get_ble_connected();

#endif /* _MICRO_BIT_COMPASS_H_ */
