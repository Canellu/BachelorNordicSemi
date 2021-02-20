/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#include <logging/log.h>
LOG_MODULE_REGISTER(micro_bit);

#include <zephyr.h>
#include <bluetooth/gatt.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt_dm.h>
#include <bluetooth/scan.h>
#include <misc/byteorder.h>

#include "micro_bit_compass.h"

#define TRUE 1
#define FALSE 0

#define BT_UUID_MICRO_BIT_UART_SERVICE                                  \
	BT_UUID_DECLARE_128(0x9e, 0xca, 0xdc, 0x24, 0x0e, 0xe5, 0xa9, 0xe0, \
						0x93, 0xf3, 0xa3, 0xb5, 0x01, 0x00, 0x40, 0x6e)

#define BT_UUID_MICRO_BIT_TX_CHAR                                       \
	BT_UUID_DECLARE_128(0x9e, 0xca, 0xdc, 0x24, 0x0e, 0xe5, 0xa9, 0xe0, \
						0x93, 0xf3, 0xa3, 0xb5, 0x03, 0x00, 0x40, 0x6e)

#define MICROBIT_ID "BBC micro:bit [" CONFIG_MICROBIT_ID "]"

#define BLE_CONNECTION_NAME MICROBIT_ID

#ifdef BLE_CONNECTION_NAME
#define BT_SCAN_FILTER_TYPE BT_SCAN_FILTER_TYPE_NAME
#define BT_SCAN_FILTER BT_SCAN_NAME_FILTER
#endif

/*define BLE_CONNECTION_UUID some UUID here */
#ifdef BLE_CONNECTION_UUID
#define BT_SCAN_FILTER_TYPE BT_SCAN_FILTER_TYPE_UUID
#define BT_SCAN_FILTER BT_SCAN_UUID_FILTER
#endif

#define UUID_SERVICE BT_UUID_MICRO_BIT_UART_SERVICE
#define UUID_CHARACTERISTIC BT_UUID_MICRO_BIT_TX_CHAR

#define MICRO_BIT_DIRECION_STRING_LEN 3

/**Global variabels */
struct tec_attr ble_char;
static bool ble_connected = FALSE;
static char mb_last_dir[MICRO_BIT_DIRECION_STRING_LEN] = "NO:";

/**Function declarations*/
int micro_bit_get_ble_connected()
{
	return ble_connected;
}

static void discovery_completed(struct bt_gatt_dm *disc, void *ctx)
{
	int err;
	const struct bt_gatt_attr *chrc;
	const struct bt_gatt_attr *desc;

	LOG_DBG("Discovery completed.\n");

	chrc = bt_gatt_dm_char_by_uuid(disc, UUID_CHARACTERISTIC);
	if (!chrc)
	{
		LOG_ERR("Missing characteristic");
		goto release;
	}

	desc = bt_gatt_dm_desc_by_uuid(disc, chrc, UUID_CHARACTERISTIC);
	if (!desc)
	{
		LOG_ERR("Missing char value descriptor");
		goto release;
	}

	ble_char.value_handle = desc->handle;
	ble_char.uuid = UUID_CHARACTERISTIC;
	ble_char.conn = bt_gatt_dm_conn_get(disc);

release:
	err = bt_gatt_dm_data_release(disc);
	if (err)
	{
		LOG_ERR("Could not release discovery data, err: %d", err);
	}

	k_sleep(K_MSEC(100));
	ble_connected = TRUE;
	micro_bit_write_dir(mb_last_dir);
	LOG_DBG("Discovery completed and released.");
}

static void discovery_service_not_found(struct bt_conn *conn, void *ctx)
{
	LOG_ERR("Service not found!\n");
}

static void discovery_error_found(struct bt_conn *conn, int err, void *ctx)
{
	LOG_ERR("The discovery procedure failed\n");
	if (ble_char.conn != NULL)
	{
		LOG_DBG("Disconnecting.");
		bt_conn_disconnect(ble_char.conn,
						   BT_HCI_ERR_REMOTE_USER_TERM_CONN);
	}
}

static struct bt_gatt_dm_cb discovery_cb = {
	.completed = discovery_completed,
	.service_not_found = discovery_service_not_found,
	.error_found = discovery_error_found,
};

static void connected(struct bt_conn *conn, u8_t conn_err)
{
	int err;
	if (conn_err)
	{
		LOG_ERR("Failed to connect to (%u)", conn_err);
		return;
	}
	ble_char.conn = conn;
	LOG_INF("Connected.");
	err = bt_gatt_dm_start(conn, UUID_SERVICE, &discovery_cb, conn);
	if (err)
	{
		LOG_ERR("Could not start service discovery, err %d", err);
	}
}

static void disconnected(struct bt_conn *conn, u8_t reason)
{
	int err = bt_scan_start(BT_SCAN_TYPE_SCAN_ACTIVE);
	if (err)
	{
		LOG_ERR("Scanning failed to start, err %d", err);
	}

	LOG_INF("Scanning...");
}

static struct bt_conn_cb conn_callbacks = {
	.connected = connected,
	.disconnected = disconnected,
};

static void scan_filter_match(struct bt_scan_device_info *device_info,
							  struct bt_scan_filter_match *filter_match,
							  bool connectable)
{
	LOG_INF("Device found.");
}

static void scan_connecting_error(struct bt_scan_device_info *device_info)
{
	LOG_ERR("Connection to peer failed!");
}

static struct bt_scan_cb scan_cb = {
	.filter_match = scan_filter_match,
	.connecting_error = scan_connecting_error,
};

static void scan_start(void)
{
	int err;

	struct bt_le_scan_param scan_param = {
		.type = BT_HCI_LE_SCAN_ACTIVE,
		.filter_dup = BT_HCI_LE_SCAN_FILTER_DUP_ENABLE,
		.interval = 0x0010,
		.window = 0x0010,
	};

	struct bt_scan_init_param scan_init = {
		.connect_if_match = 1,
		.scan_param = &scan_param,
		.conn_param = BT_LE_CONN_PARAM_DEFAULT,
	};

	bt_scan_init(&scan_init);
	bt_scan_cb_register(&scan_cb);

	err = bt_scan_filter_add(BT_SCAN_FILTER_TYPE, MICROBIT_ID);
	if (err)
	{
		LOG_ERR("Scanning filters cannot be set");
		return;
	}

	err = bt_scan_filter_enable(BT_SCAN_FILTER, false);
	if (err)
	{
		LOG_ERR("Filters cannot be turned on");
	}

	err = bt_scan_start(BT_SCAN_TYPE_SCAN_ACTIVE);
	if (err)
	{
		LOG_ERR("Scanning failed to start, err %d", err);
	}

	LOG_INF("Scanning...");
}

static void ble_ready(int err)
{
	LOG_DBG("Bluetooth ready");

	bt_conn_cb_register(&conn_callbacks);
	scan_start();
}

static void ble_init(void)
{
	int err;
	/** Sleep to give Micro:Bit time to restart */
	k_sleep(K_SECONDS(4));
	ble_connected = FALSE;
	LOG_DBG("Initializing Bluetooth..");
	err = bt_enable(ble_ready);
	if (err)
	{
		LOG_ERR("Bluetooth init failed (err %d)", err);
		return;
	}
}

static int write_over_ble(struct bt_conn *conn, u16_t value_handle, u8_t *data,
						  u16_t len)
{
	if (!ble_connected)
	{
		return -ENOTCONN;
	}
	return bt_gatt_write_without_response_cb(conn, value_handle, data, len,
											 FALSE, NULL, NULL);
}

void micro_bit_compass_init(void)
{
	ble_init();
}

int micro_bit_write_dir(char *direction)
{
	int err = write_over_ble(ble_char.conn, ble_char.value_handle,
							 direction, sizeof(direction) - 1);
	if (err)
	{
		LOG_ERR("Sending direction failed.");
	}
	else
	{
		memcpy(mb_last_dir, direction, MICRO_BIT_DIRECION_STRING_LEN);
	}
	return err;
}