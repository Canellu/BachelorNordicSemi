/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#include <zephyr.h>
#include <stdio.h>
#include <drivers/uart.h>
#include <string.h>
#include <random/rand32.h>

#include <net/mqtt.h>
#include <net/socket.h>

#include <data/jwt.h>

#include <modem/lte_lc.h>
#include <logging/log.h>
#if defined(CONFIG_MODEM_KEY_MGMT)
#include <modem/modem_key_mgmt.h>
#endif
#if defined(CONFIG_LWM2M_CARRIER)
#include <lwm2m_carrier.h>
#endif
#include <dk_buttons_and_leds.h>

#include "cJSON.h"
#include "ntp.h"

LOG_MODULE_REGISTER(mqtt_simple, CONFIG_MQTT_SIMPLE_LOG_LEVEL);

/* Buffers for MQTT client. */
static uint8_t rx_buffer[CONFIG_MQTT_MESSAGE_BUFFER_SIZE];
static uint8_t tx_buffer[CONFIG_MQTT_MESSAGE_BUFFER_SIZE];
static uint8_t payload_buf[CONFIG_MQTT_PAYLOAD_BUFFER_SIZE];

/* The mqtt client struct */
static struct mqtt_client client;

/* MQTT Broker details. */
static struct sockaddr_storage broker;

/* File descriptor */
static struct pollfd fds;


/* GCLOUD FUNCTIONS */

#define ROOT_CERT \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIDoTCCAomgAwIBAgILBAAAAAABD4WqLUgwDQYJKoZIhvcNAQEFBQAwOzEYMBYG\n" \
	"A1UEChMPQ3liZXJ0cnVzdCwgSW5jMR8wHQYDVQQDExZDeWJlcnRydXN0IEdsb2Jh\n" \
	"bCBSb290MB4XDTA2MTIxNTA4MDAwMFoXDTIxMTIxNTA4MDAwMFowOzEYMBYGA1UE\n" \
	"ChMPQ3liZXJ0cnVzdCwgSW5jMR8wHQYDVQQDExZDeWJlcnRydXN0IEdsb2JhbCBS\n" \
	"b290MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA+Mi8vRRQZhP/8NN5\n" \
	"7CPytxrHjoXxEnOmGaoQ25yiZXRadz5RfVb23CO21O1fWLE3TdVJDm71aofW0ozS\n" \
	"J8bi/zafmGWgE07GKmSb1ZASzxQG9Dvj1Ci+6A74q05IlG2OlTEQXO2iLb3VOm2y\n" \
	"HLtgwEZLAfVJrn5GitB0jaEMAs7u/OePuGtm839EAL9mJRQr3RAwHQeWP032a7iP\n" \
	"t3sMpTjr3kfb1V05/Iin89cqdPHoWqI7n1C6poxFNcJQZZXcY4Lv3b93TZxiyWNz\n" \
	"FtApD0mpSPCzqrdsxacwOUBdrsTiXSZT8M4cIwhhqJQZugRiQOwfOHB3EgZxpzAY\n" \
	"XSUnpQIDAQABo4GlMIGiMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8EBTADAQH/\n" \
	"MB0GA1UdDgQWBBS2CHsNesysIEyGVjJez6tuhS1wVzA/BgNVHR8EODA2MDSgMqAw\n" \
	"hi5odHRwOi8vd3d3Mi5wdWJsaWMtdHJ1c3QuY29tL2NybC9jdC9jdHJvb3QuY3Js\n" \
	"MB8GA1UdIwQYMBaAFLYIew16zKwgTIZWMl7Pq26FLXBXMA0GCSqGSIb3DQEBBQUA\n" \
	"A4IBAQBW7wojoFROlZfJ+InaRcHUowAl9B8Tq7ejhVhpwjCt2BWKLePJzYFa+HMj\n" \
	"Wqd8BfP9IjsO0QbE2zZMcwSO5bAi5MXzLqXZI+O4Tkogp24CJJ8iYGd7ix1yCcUx\n" \
	"XOl5n4BHPa2hCwcUPUf/A2kaDAtE52Mlp3+yybh2hO0j9n0Hq0V+09+zv+mKts2o\n" \
	"omcrUtW3ZfA5TGOgkXmTUg9U3YO7n9GPp1Nzw8v/MOx8BLjYRB+TX3EJIrduPuoc\n" \
	"A06dGiBh+4E37F78CkWr1+cXVdCg6mCbpvbjjFspwgZgFJ0tl0ypkxWdYcQBX0jW\n" \
	"WL1WMRJOEcgh4LMRkWXbtKaIOM5V\n" \
    "-----END CERTIFICATE-----"

#define GCLOUD_CA_CERTIFICATE ROOT_CERT
#define GCLOUD_SEC_TAG 16842752
#define SUBSCRIBE_ID 42

#define GCLOUD_ID "projects/" CONFIG_GCLOUD_PROJECT_NAME "/locations/" CONFIG_GCLOUD_REGION "/registries/" CONFIG_GCLOUD_REGISTRY_NAME "/devices/" CONFIG_GCLOUD_DEVICE_NAME
#define GCLOUD_HOSTNAME "mqtt.googleapis.com" 
#define BROKER_ADDR GCLOUD_HOSTNAME

#define GCLOUD_CONFIG_TOPIC "/devices/" CONFIG_GCLOUD_DEVICE_NAME "/config"
#define GCLOUD_STATE_TOPIC "/devices/" CONFIG_GCLOUD_DEVICE_NAME "/state"
#define GCLOUD_TOPIC "/devices/" CONFIG_GCLOUD_DEVICE_NAME "/events"

#define JWT_BUFFER_SIZE 256

/* Private key information */
extern unsigned char zepfull_private_der[];
extern unsigned int zepfull_private_der_len;

static struct mqtt_utf8 username = {
    .utf8 = "unused",
    .size = sizeof("unused")
};
char jwt_buffer[JWT_BUFFER_SIZE];
char jwt_buffer2[JWT_BUFFER_SIZE] = "hitest";
static struct mqtt_utf8 jwt_token = {.utf8 = jwt_buffer};
static struct mqtt_utf8 jwt_tokentest = {.utf8 = jwt_buffer2};

static struct mqtt_topic gcloud_topics_list[] = {
    {
        .topic = {
            .utf8 = GCLOUD_CONFIG_TOPIC,
            .size = sizeof(GCLOUD_CONFIG_TOPIC) - 1
        },
        .qos = MQTT_QOS_1_AT_LEAST_ONCE
    }
};

static int gcloud_provision(void)
{
	int err = 0;

	LOG_INF("Provisioning google cloud certificates");

	// nrf_sec_tag_t sec_tag = GCLOUD_SEC_TAG;

    // for(enum modem_key_mgmt_cred_type type = 0; type < 5; type++) {
    //     err = modem_key_mgmt_delete(sec_tag, type);
    //     if (err) {
    //         LOG_ERR("key delete err: [%d] %s", err, strerror(err));
    //     }
    // }

	err = modem_key_mgmt_write(GCLOUD_SEC_TAG,
				   MODEM_KEY_MGMT_CRED_TYPE_CA_CHAIN,
				   GCLOUD_CA_CERTIFICATE,
				   strlen(GCLOUD_CA_CERTIFICATE));
	if (err) {
		LOG_ERR("Failed to provision CA certificate: %d", err);
	}
	return err;
}

static int make_jwt(char *buffer, size_t buffer_size)
{
    int err;
    struct jwt_builder jb;

    err = jwt_init_builder(&jb, buffer, buffer_size);
    if (err != 0)
    {
        LOG_ERR("Unable to init JWT builder: %d", err);
        return err;
    }

    uint32_t timestamp = ntp_get_unix_time();
    if (timestamp < 0)
    {
        LOG_ERR("Unable to get Unix time: %d", err);
        return err;
    }

    err = jwt_add_payload(&jb, timestamp + 12 * 60 * 60, timestamp,
                          CONFIG_GCLOUD_PROJECT_NAME);
    if (err != 0)
    {
        LOG_ERR("Unable to add JWT payload: %d", err);
        return err;
    }

    err = jwt_sign(&jb, zepfull_private_der, zepfull_private_der_len);
    if (err != 0)
    {
        LOG_ERR("Unable to sign JWT token: %d", err);
        return err;
    }

    if (jb.overflowed != 0)
    {
        LOG_ERR("JWT buffer overflowed");
        return -ENOMEM;
    }
    return err;
}







#if defined(CONFIG_BSD_LIBRARY)

/**@brief Recoverable BSD library error. */
void bsd_recoverable_error_handler(uint32_t err)
{
	LOG_ERR("bsdlib recoverable error: %u", (unsigned int)err);
}

#endif /* defined(CONFIG_BSD_LIBRARY) */

/**@brief Function to print strings without null-termination
 */
static void data_print(uint8_t *prefix, uint8_t *data, size_t len)
{
	char buf[len + 1];

	memcpy(buf, data, len);
	buf[len] = 0;
	LOG_INF("%s%s", log_strdup(prefix), log_strdup(buf));
}

/**@brief Function to publish data on the configured topic
 */
static int data_publish(struct mqtt_client *c, enum mqtt_qos qos,
	uint8_t *data, size_t len)
{
	struct mqtt_publish_param param;

	param.message.topic.qos = qos;
	param.message.topic.topic.utf8 = GCLOUD_TOPIC;
	param.message.topic.topic.size = strlen(GCLOUD_TOPIC);
	param.message.payload.data = data;
	param.message.payload.len = len;
	param.message_id = sys_rand32_get();
	param.dup_flag = 0;
	param.retain_flag = 0;

	data_print("Publishing: ", data, len);
	LOG_INF("to topic: %s len: %u",
		GCLOUD_TOPIC,
		(unsigned int)strlen(GCLOUD_TOPIC));

	return mqtt_publish(c, &param);
}

/**@brief Function to subscribe to the configured topic
 */
static int subscribe(void)
{
	const struct mqtt_subscription_list subscription_list = {
		.list = (struct mqtt_topic *)&gcloud_topics_list,
		.list_count = ARRAY_SIZE(gcloud_topics_list),
		.message_id = SUBSCRIBE_ID
	};

	return mqtt_subscribe(&client, &subscription_list);
}

/**@brief Function to read the published payload.
 */
static int publish_get_payload(struct mqtt_client *c, size_t length)
{
	if (length > sizeof(payload_buf)) {
		return -EMSGSIZE;
	}

	return mqtt_readall_publish_payload(c, payload_buf, length);
}

/**@brief MQTT client event handler
 */
void mqtt_evt_handler(struct mqtt_client *const c,
		      const struct mqtt_evt *evt)
{
	int err;

	switch (evt->type) {
	case MQTT_EVT_CONNACK:
		if (evt->result != 0) {
			LOG_ERR("MQTT connect failed: %d", evt->result);
			break;
		}

		LOG_INF("MQTT client connected");
		subscribe();
		break;

	case MQTT_EVT_DISCONNECT:
		LOG_INF("MQTT client disconnected: %d", evt->result);
		break;

	case MQTT_EVT_PUBLISH: {
		const struct mqtt_publish_param *p = &evt->param.publish;

		LOG_INF("MQTT PUBLISH result=%d len=%d",
			evt->result, p->message.payload.len);
		err = publish_get_payload(c, p->message.payload.len);

		if (p->message.topic.qos == MQTT_QOS_1_AT_LEAST_ONCE) {
			const struct mqtt_puback_param ack = {
				.message_id = p->message_id
			};

			/* Send acknowledgment. */
			mqtt_publish_qos1_ack(&client, &ack);
		}

		if (err >= 0) {
			data_print("Received: ", payload_buf,
				p->message.payload.len);
			/* Echo back received data */
			data_publish(&client, MQTT_QOS_1_AT_LEAST_ONCE,
				payload_buf, p->message.payload.len);
		} else {
			LOG_ERR("publish_get_payload failed: %d", err);
			LOG_INF("Disconnecting MQTT client...");

			err = mqtt_disconnect(c);
			if (err) {
				LOG_ERR("Could not disconnect: %d", err);
			}
		}
	} break;

	case MQTT_EVT_PUBACK:
		if (evt->result != 0) {
			LOG_ERR("MQTT PUBACK error: %d", evt->result);
			break;
		}

		LOG_INF("PUBACK packet id: %u", evt->param.puback.message_id);
		break;

	case MQTT_EVT_SUBACK:
		if (evt->result != 0) {
			LOG_ERR("MQTT SUBACK error: %d", evt->result);
			break;
		}

		LOG_INF("SUBACK packet id: %u", evt->param.suback.message_id);
		break;

	case MQTT_EVT_PINGRESP:
		if (evt->result != 0) {
			LOG_ERR("MQTT PINGRESP error: %d", evt->result);
		}
		break;

	default:
		LOG_INF("Unhandled MQTT event type: %d", evt->type);
		break;
	}
}

/**@brief Resolves the configured hostname and
 * initializes the MQTT broker structure
 */
static int broker_init(void)
{
	int err;
	struct addrinfo *result;
	struct addrinfo *addr;
	struct addrinfo hints = {
		.ai_family = AF_INET,
		.ai_socktype = SOCK_STREAM
	};

	err = getaddrinfo(GCLOUD_HOSTNAME, NULL, &hints, &result);
	if (err) {
		LOG_ERR("getaddrinfo failed: %d", err);
		return -ECHILD;
	}

	addr = result;

	/* Look for address of the broker. */
	while (addr != NULL) {
		/* IPv4 Address. */
		if (addr->ai_addrlen == sizeof(struct sockaddr_in)) {
			struct sockaddr_in *broker4 =
				((struct sockaddr_in *)&broker);
			char ipv4_addr[NET_IPV4_ADDR_LEN];

			broker4->sin_addr.s_addr =
				((struct sockaddr_in *)addr->ai_addr)
				->sin_addr.s_addr;
			broker4->sin_family = AF_INET;
			broker4->sin_port = htons(CONFIG_MQTT_BROKER_PORT);

			inet_ntop(AF_INET, &broker4->sin_addr.s_addr,
				  ipv4_addr, sizeof(ipv4_addr));
			LOG_INF("IPv4 Address found %s", log_strdup(ipv4_addr));

			break;
		} else {
			LOG_ERR("ai_addrlen = %u should be %u or %u",
				(unsigned int)addr->ai_addrlen,
				(unsigned int)sizeof(struct sockaddr_in),
				(unsigned int)sizeof(struct sockaddr_in6));
		}

		addr = addr->ai_next;
	}

	/* Free the address. */
	freeaddrinfo(result);

	return err;
}

/**@brief Initialize the MQTT client structure
 */
static int client_init(struct mqtt_client *client)
{
	int err;

	mqtt_client_init(client);

	err = make_jwt(jwt_buffer, JWT_BUFFER_SIZE);
    if (err != 0) {
        LOG_ERR("Unable to make jwt: %d", err);
        return err;
    }
	
	LOG_DBG("\nJWT length: %d\n", strlen(jwt_buffer));

    jwt_token.size = strlen(jwt_buffer);
	jwt_tokentest.size = strlen(jwt_buffer2);


	err = broker_init();
	if (err) {
		LOG_ERR("Failed to initialize broker connection");
		return err;
	}

	//COMEBACK

	/* MQTT client configuration */
	client->broker = &broker;
	client->evt_cb = mqtt_evt_handler;
	client->client_id.utf8 = GCLOUD_ID;
	client->client_id.size = strlen(GCLOUD_ID);
	client->password = &jwt_token;
	client->user_name = &username;
	client->protocol_version = MQTT_VERSION_3_1_1;

	/* MQTT buffers configuration */
	client->rx_buf = rx_buffer;
	client->rx_buf_size = sizeof(rx_buffer);
	client->tx_buf = tx_buffer;
	client->tx_buf_size = sizeof(tx_buffer);

	/* MQTT transport configuration */
#if defined(CONFIG_MQTT_LIB_TLS)
	struct mqtt_sec_config *tls_cfg = &(client->transport).tls.config;
	static sec_tag_t sec_tag_list[] = { GCLOUD_SEC_TAG };

	LOG_INF("TLS enabled");
	client->transport.type = MQTT_TRANSPORT_SECURE;

	tls_cfg->peer_verify = 0;
	tls_cfg->cipher_count = 0;
	tls_cfg->cipher_list = NULL;
	tls_cfg->sec_tag_count = ARRAY_SIZE(sec_tag_list);
	tls_cfg->sec_tag_list = sec_tag_list;
	tls_cfg->hostname = GCLOUD_HOSTNAME;

#if defined(CONFIG_BSD_LIBRARY)
	tls_cfg->session_cache = IS_ENABLED(CONFIG_MQTT_TLS_SESSION_CACHING) ?
					    TLS_SESSION_CACHE_ENABLED :
					    TLS_SESSION_CACHE_DISABLED;
#else
	/* TLS session caching is not supported by the Zephyr network stack */
	tls_cfg->session_cache = TLS_SESSION_CACHE_DISABLED;

#endif

#else
	client->transport.type = MQTT_TRANSPORT_NON_SECURE;
#endif

	return err;
}

/**@brief Initialize the file descriptor structure used by poll.
 */
static int fds_init(struct mqtt_client *c)
{
	if (c->transport.type == MQTT_TRANSPORT_NON_SECURE) {
		fds.fd = c->transport.tcp.sock;
	} else {
#if defined(CONFIG_MQTT_LIB_TLS)
		fds.fd = c->transport.tls.sock;
#else
		return -ENOTSUP;
#endif
	}

	fds.events = POLLIN;

	return 0;
}

static void button_handler(uint32_t button_states, uint32_t has_changed)
{
	if (has_changed & button_states &
	    BIT(CONFIG_BUTTON_EVENT_BTN_NUM - 1)) {
		int ret;

		ret = data_publish(&client,
				   MQTT_QOS_1_AT_LEAST_ONCE,
				   CONFIG_BUTTON_EVENT_PUBLISH_MSG,
				   sizeof(CONFIG_BUTTON_EVENT_PUBLISH_MSG)-1);
		if (ret) {
			LOG_ERR("Publish failed: %d", ret);
		}
	}
}

/**@brief Configures modem to provide LTE link. Blocks until link is
 * successfully established.
 */
static int modem_configure(void)
{
#if defined(CONFIG_LTE_LINK_CONTROL)
	if (IS_ENABLED(CONFIG_LTE_AUTO_INIT_AND_CONNECT)) {
		/* Do nothing, modem is already turned on
		 * and connected.
		 */
	} else {
#if defined(CONFIG_LWM2M_CARRIER)
		/* Wait for the LWM2M_CARRIER to configure the modem and
		 * start the connection.
		 */
		LOG_INF("Waitng for carrier registration...");
		k_sem_take(&carrier_registered, K_FOREVER);
		LOG_INF("Registered!");
#else /* defined(CONFIG_LWM2M_CARRIER) */
		int err;

		LOG_INF("LTE Link Connecting...");
		err = lte_lc_init_and_connect();
		if (err) {
			LOG_INF("Failed to establish LTE connection: %d", err);
			return err;
		}
		LOG_INF("LTE Link Connected!");
#endif /* defined(CONFIG_LWM2M_CARRIER) */
	}
#endif /* defined(CONFIG_LTE_LINK_CONTROL) */

	/* Turn off power saving modes for a more responsive demo */
	LOG_INF("Disabling PSM and eDRX");
	lte_lc_psm_req(false);
	lte_lc_edrx_req(false);

	return 0;
}

void main(void)
{
	int err;
	uint32_t connect_attempt = 0;

	LOG_INF("The MQTT simple sample started");

#if defined(CONFIG_MQTT_LIB_TLS)
	err = gcloud_provision();
	if (err != 0) {
		LOG_ERR("Failed to provision google cloud certificates");
		return;
	}
#endif /* defined(CONFIG_MQTT_LIB_TLS) */

	do {
		err = modem_configure();
		if (err) {
			LOG_INF("Retrying in %d seconds",
				CONFIG_LTE_CONNECT_RETRY_DELAY_S);
			k_sleep(K_SECONDS(CONFIG_LTE_CONNECT_RETRY_DELAY_S));
		}
	} while (err);

	

	err = client_init(&client);
	if (err != 0) {
		LOG_ERR("client_init: %d", err);
		return;
	}

	 dk_buttons_init(button_handler);

do_connect:
	if (connect_attempt++ > 0) {
		LOG_INF("Reconnecting in %d seconds...",
			CONFIG_MQTT_RECONNECT_DELAY_S);
		k_sleep(K_SECONDS(CONFIG_MQTT_RECONNECT_DELAY_S));
	}
	err = mqtt_connect(&client);
	if (err != 0) {
		LOG_ERR("mqtt_connect %d", err);
		goto do_connect;
	}

	err = fds_init(&client);
	if (err != 0) {
		LOG_ERR("fds_init: %d", err);
		return;
	}

	uint32_t cnt = 0;
	while (1) {
		err = poll(&fds, 1, mqtt_keepalive_time_left(&client));
		if (err < 0) {
			LOG_ERR("poll: %d", errno);
			break;
		}

		err = mqtt_live(&client);
		if ((err != 0) && (err != -EAGAIN)) {
			LOG_ERR("ERROR: mqtt_live: %d", err);
			break;
		}

		if ((fds.revents & POLLIN) == POLLIN) {
			err = mqtt_input(&client);
			if (err != 0) {
				LOG_ERR("mqtt_input: %d", err);
				break;
			}
		}

		if ((fds.revents & POLLERR) == POLLERR) {
			LOG_ERR("POLLERR");
			break;
		}

		if ((fds.revents & POLLNVAL) == POLLNVAL) {
			LOG_ERR("POLLNVAL");
			break;
		}

        double yrNum = 22021 - cnt;
        cJSON * yrObj = cJSON_CreateObject();
        cJSON * year = NULL;
        char * JSONString = NULL;
        year = cJSON_CreateNumber(yrNum);
        const char * yrStr = "Message";

        cJSON_AddItemToObject(yrObj, yrStr, year);
        JSONString = cJSON_Print(yrObj);
        cJSON_Delete(yrObj);

	    printk("Sending JSON data:\n%s\n", JSONString);
	    data_publish(&client, MQTT_QOS_1_AT_LEAST_ONCE, JSONString, strlen(JSONString));
		cnt++;

	}

	LOG_INF("Disconnecting MQTT client...");

	err = mqtt_disconnect(&client);
	if (err) {
		LOG_ERR("Could not disconnect MQTT client: %d", err);
	}
	goto do_connect;
}
