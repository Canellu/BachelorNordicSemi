/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#include <logging/log.h>
LOG_MODULE_REGISTER(gcloud);

#include <zephyr.h>
#include <string.h>
#include <errno.h>

#include <net/socket.h>
#include <net/mqtt.h>

#include <data/jwt.h>
#include <modem/modem_key_mgmt.h>

#include "gcloud.h"
//COMEBACK
#include "ntp.h"
#include "path_module.h"

#include "globalsign.inc"
#include "gcloud_defines.h"

/** MQTT and cloud specifics */

enum gcloud_event_type
{
    CONNECT,
    INPUT_TIMEOUT,
    LIVE_TIMEOUT,
    PUBLISH_STATE,
    PUBLISH_GPS,
    RECONNECT_TIMEOUT,
    DISCONNECT,
    SUBSCRIBE
};

struct empty {};

union gcloud_event_param
{
    struct empty connect;
    //COMEBACK
    /*publish_state could be a whole MQTT message, thus allowing custom topics*/
    struct mqtt_publish_message publish_state;
    struct mqtt_publish_message publish_gps;
    struct empty reconnect_timeout;
    struct empty disconnect;
    /* subscribe is a list of topics to be subscribed to */
    struct empty subscribe;
};

struct gcloud_event
{
    enum gcloud_event_type type;
    union gcloud_event_param param;
};

static struct mqtt_client client;
static struct sockaddr_storage mqtt_broker;
static struct mqtt_sec_config tls_config;

char jwt_buffer[JWT_BUFFER_SIZE];
static struct mqtt_utf8 jwt_token = {
    .utf8 = jwt_buffer};

static struct mqtt_utf8 username = {
    .utf8 = "unused",
    .size = sizeof("unused")};

static struct mqtt_topic gcloud_topics_list[] = {
    {.topic = {
         .utf8 = GCLOUD_CONFIG_TOPIC,
         .size = sizeof(GCLOUD_CONFIG_TOPIC) - 1},
     .qos = MQTT_QOS_0_AT_MOST_ONCE}};

/** Private key information */
extern unsigned char zepfull_private_der[];
extern unsigned int zepfull_private_der_len;

//COMEBACK

/** Error information */
enum error_type
{
    ERROR_CLOUD,
    ERROR_BSD_RECOVERABLE,
    ERROR_BSD_IRRECOVERABLE,
    ERROR_LTE_LC,
    ERROR_THREAD
};

/** Thread specifics */
K_THREAD_STACK_DEFINE(gcloud_stack_area, GCLOUD_THREAD_STACK_SIZE);
static void gcloud_thread(void *, void *, void *);

static struct k_thread gcloud_thread_data;
static k_tid_t gcloud_thread_tid;

/** Forward Declarations */
extern void gcloud_error_handler(int err_code);

extern struct k_delayed_work gcloud_reboot_work;

extern bool gps_control_is_active(void);

static void mqtt_event_handler(struct mqtt_client *client,
                               const struct mqtt_evt *evt);

static void reconnect_timer_handler(struct k_timer *timer_id);

K_TIMER_DEFINE(reconnect_timer, reconnect_timer_handler, NULL);

/** Global Variables */
static atomic_t connected;
static atomic_t connecting;

/** Function Declarations */

int gcloud_get_connection_status()
{
    return atomic_get(&connected);
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

    s32_t timestamp = ntp_get_unix_time();
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

static int gcloud_client_init(void)
{
    mqtt_client_init(&client);

    int err = make_jwt(jwt_buffer, JWT_BUFFER_SIZE);
    if (err != 0)
    {
        LOG_ERR("Unable to make jwt: %d %s", err, strerror(-err));
        return err;
    }

    LOG_DBG("JWT length: %d", strlen(jwt_buffer));

    jwt_token.size = strlen(jwt_buffer);

    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = GCLOUD_ID;
    client.client_id.size = strlen(GCLOUD_ID);

    client.password = &jwt_token;
    client.user_name = &username;

    client.protocol_version = MQTT_VERSION_3_1_1;

    /* MQTT buffers configuration */
	client.rx_buf = rx_buffer;
	// client.rx_buf_size = sizeof(rx_buffer);
    client.rx_buf_size = 512;

	client.tx_buf = tx_buffer;
	// client.tx_buf_size = sizeof(tx_buffer);
    client.tx_buf_size = 1024;

    client.transport.type = MQTT_TRANSPORT_SECURE;

    //COMEBACK
    struct mqtt_sec_config *client_tls_config = &client.transport.tls.config;
    memcpy(client_tls_config, &tls_config, sizeof(struct mqtt_sec_config));
    client.broker = (struct sockaddr *)&mqtt_broker;
    return 0;
}

static int gcloud_subscribe()
{
    const struct mqtt_subscription_list subscriptions = {
        .list = (struct mqtt_topic *)&gcloud_topics_list,
        .list_count = ARRAY_SIZE(gcloud_topics_list),
        .message_id = SUBSCRIBE_ID};

    return mqtt_subscribe(&client, &subscriptions);
}

int gcloud_provision(void)
{
    static sec_tag_t sec_tag_list[] = {GCLOUD_SEC_TAG};

    tls_config.peer_verify = 0;
    tls_config.cipher_count = 0;
    tls_config.cipher_list = NULL;
    tls_config.sec_tag_count = ARRAY_SIZE(sec_tag_list);
    tls_config.seg_tag_list = sec_tag_list;
    tls_config.hostname = GCLOUD_HOSTNAME;

    int err;

    /* Delete certificates with sec_tag GCLOUD_SEC_TAG*/
    nrf_sec_tag_t sec_tag = GCLOUD_SEC_TAG;

    for (modem_key_mgnt_cred_type type = 0; type < 5; type++)
    {
        modem_key_mgmt_delete(sec_tag, type);
    }
    /* Provision CA Certificate */
    err = modem_key_mgmt_write(
        GCLOUD_SEC_TAG,
        NRF_KEY_MGMT_CRED_TYPE_CA_CHAIN,
        GCLOUD_CA_CERTIFICATE,
        strlen(GCLOUD_CA_CERTIFICATE));
    if (err != 0)
    {
        LOG_ERR("GCLOUD_CA_CERTIFICATE err: %d %s\n", err, strerror(err));
        return err;
    }

    // This function should probably only be called once.
    err = mqtt_connect(&client);
    if (err != 0)
    {
        return err;
    }

    return 0;
}

//COMEBACK, CHECK CB
void gcloud_connect(void)
{
    LOG_DBG("Initiating broker");
    int err;
    struct addrinfo *result;
    struct addrinfo *addr;
    struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM};

    err = getaddrinfo(GCLOUD_HOSTNAME, NULL, &hints, &result);
    if (err)
    {
        LOG_ERR("ERROR: getaddrinfo failed %d", err);

        return;
    }

    addr = result;
    err = -ENOENT;

    /* Look for address of the broker. */
    while (addr != NULL)
    {
        /* IPv4 Address. */
        if (addr->ai_addrlen == sizeof(struct sockaddr_in))
        {
            struct sockaddr_in *broker4 =
                ((struct sockaddr_in *)&mqtt_broker);
            broker4->sin_addr.s_addr =
                ((struct sockaddr_in *)addr->ai_addr)
                    ->sin_addr.s_addr;
            broker4->sin_family = AF_INET;
            broker4->sin_port = htons(BROKER_PORT);
            LOG_DBG("IPv4 Address found 0x%08x",
                    broker4->sin_addr.s_addr);

            LOG_DBG("Initiating client");
            err = gcloud_client_init();
            if (err)
            {
                LOG_ERR("Could not initiate client: %d %s", err, strerror(-err));
            }

            LOG_DBG("Connecting mqtt");
            atomic_set(&connecting, 1);
            err = mqtt_connect(&client);
            if (err)
            {
                LOG_ERR("mqtt_connect failed: %d %s", err, strerror(-err));
            }
            break;
        }
        else
        {
            LOG_ERR("ai_addrlen = %u should be %u or %u",
                    (unsigned int)addr->ai_addrlen,
                    (unsigned int)sizeof(struct sockaddr_in),
                    (unsigned int)sizeof(struct sockaddr_in6));
        }

        addr = addr->ai_next;
        break;
    }

    /* Free the address. */
    freeaddrinfo(result);
}

int gcloud_disconnect(bool RECONNECT)
{
    ARG_UNUSED(RECONNECT);

    int err = 0;
    k_timer_stop(&reconnect_timer);
    LOG_DBG("Disconnecting MQTT");
    err = mqtt_disconnect(&client);
    if (err)
    {
        LOG_ERR("mqtt_disconnect failed: %d", err);
    }
    atomic_set(&connected, 0);
    k_delayed_work_submit(&gcloud_reboot_work, K_NO_WAIT);
    return err;
}

int gcloud_publish_gps(u8_t *data, u32_t size, enum mqtt_qos qos)
{
    LOG_INF("Publishing GPS data to Google Cloud");
    int err;
    if (!atomic_get(&connected))
    {
        LOG_WRN("Cannot publish GPS data while not connected to Google Cloud");
        return -ENOTCONN;
    }

    struct mqtt_publish_param msg;

    struct mqtt_publish_message temp = {
        .topic = {
            .topic = {
                .utf8 = GCLOUD_GPS_TOPIC,
                .size = strlen(GCLOUD_GPS_TOPIC)},
            .qos = qos},
        .payload = {.data = data, .len = size}};
    msg.message = temp;
    msg.message_id = sys_rand32_get();
    msg.dup_flag = 0;
    msg.retain_flag = 0;
    err = mqtt_publish(&client, &msg);
    if (err)
    {
        LOG_ERR("mqtt_publish (gps) failed: %d", err);
    }

    return err;
}

int gcloud_publish_state(u8_t *data, u32_t size, enum mqtt_qos qos)
{
    LOG_INF("Publishing state to Google Cloud");
    int err;
    if (!atomic_get(&connected))
    {
        LOG_WRN("Cannot publish state while not connected to Google Cloud");
        return -ENOTCONN;
    }

    struct mqtt_publish_param msg;

    struct mqtt_publish_message temp = {
        .topic = {
            .topic = {
                .utf8 = GCLOUD_STATE_TOPIC,
                .size = strlen(GCLOUD_STATE_TOPIC)},
            .qos = qos},
        .payload = {.data = data, .len = size}};
    msg.message = temp;
    msg.message_id = sys_rand32_get();
    msg.dup_flag = 0;
    msg.retain_flag = 0;
    err = mqtt_publish(&client, &msg);

    if (err)
    {
        LOG_ERR("mqtt_publish (state) failed: %d", err);
    }
    return err;
}

void reconnect_timer_handler(struct k_timer *timer_id)
{
    gcloud_disconnect(true);
}

void gcloud_start_thread(void)
{
    gcloud_thread_tid =
        k_thread_create(&gcloud_thread_data,
                        gcloud_stack_area,
                        K_THREAD_STACK_SIZEOF(gcloud_stack_area),
                        gcloud_thread,
                        NULL, NULL, NULL,
                        GCLOUD_THREAD_PRIORITY, 0, K_NO_WAIT);
}

void gcloud_ping(void)
{
    int err;
    err = mqtt_live();
    if (err)
    {
        LOG_ERR("Error: MQTT live failed %d %s", err, strerror(err));
    }
}

void gcloud_process(void)
{
    int err;
    err = mqtt_input(&client);
    if (err)
    {
        LOG_ERR("Error: MQTT input failed %d %s", err, strerror(err));
    }

    err = mqtt_live();
    if (err)
    {
        LOG_ERR("Error: MQTT live (process) failed %d %s",
                err, strerror(err));
    }
}

static void mqtt_event_handler(struct mqtt_client *client,
                               const struct mqtt_evt *evt)
{
    int err;
    LOG_DBG("MQTT event: %d", evt->type);

    switch (evt->type)
    {

    /* Response to conneck request */
    case MQTT_EVT_CONNACK:
        LOG_DBG("MQTT CONNACK");
        LOG_INF("Connected to Google Cloud");
        if (evt->result != 0)
        {
            LOG_ERR("MQTT connect failed: %d %s", evt->result,
                    strerror(evt->result));
            break;
        }

        k_delayed_work_cancel(&gcloud_reboot_work);
        if (evt->param.connack.return_code != MQTT_CONNECTION_ACCEPTED)
        {
            LOG_ERR("CONNACK ERROR. Return Code: %d", evt->param.connack.return_code);
        }

        atomic_set(&connected, 1);
        atomic_set(&connecting, 0);
        k_timer_start(&reconnect_timer, GCLOUD_DISCONNECT_INTERVAL,
                      GCLOUD_DISCONNECT_INTERVAL);

        LOG_DBG("SUBSCRIBING");
        err = gcloud_subscribe();
        if (err)
        {
            LOG_ERR("gcloud_subscribe failed: %d",
                    err);
            gcloud_error_handler(err);
        }
        break;

    /* A message has been published on a topic we subscribe to. */
    case MQTT_EVT_PUBLISH:
        LOG_DBG("MQTT PUBLISH");
        LOG_INF("Received a message from Google Cloud");
        if (evt->result != 0)
        {
            LOG_ERR("Publish event error: %d %s", evt->result,
                    strerror(-evt->result));
        }

        if (evt->param.publish.message.topic.qos == MQTT_QOS_1_AT_LEAST_ONCE)
        {
            const struct mqtt_puback_param ack = {
                .message_id = evt->param.publish.message_id};

            /* Send acknowledgement. */
            err = mqtt_publish_qos1_ack(client, &ack);
            if (err)
            {
                LOG_ERR("Publish acknowledgement error %d %s", err, strerror(err));
            }
            LOG_DBG("MSG ACKNOWLEDGED");
        }

        if ((evt->param.publish.message.payload.data == NULL) ||
            (evt->param.publish.message.payload.len <= 2))
        {
            LOG_WRN("Received an empty message");
            break;
        }

        LOG_DBG("Length of payload: %d", evt->param.publish.message.payload.len);
        LOG_DBG("Topic: %s", log_strdup(evt->param.publish.message.topic.topic.utf8));
        LOG_DBG("QoS: %d", evt->param.publish.message.topic.qos);
        LOG_DBG("Message:\n%s\n", log_strdup(evt->param.publish.message.payload.data));

        struct mqtt_evt event = *evt;

        char *str = calloc(evt->param.publish.message.payload.len, sizeof(char) + 1);
        memcpy(str, evt->param.publish.message.payload.data, evt->param.publish.message.payload.len);
        event.param.publish.message.payload.data = str;
        path_module_received_path_handler(&event.param.publish.message);

        free(str);
        break;

    /* Disconnect event. MQTT Client Reference is no longer valid. */
    case MQTT_EVT_DISCONNECT:
        LOG_DBG("MQTT DISCONNECT");
        LOG_INF("MQTT client disconnected: %d %s", evt->result, strerror(-evt->result));
        k_timer_stop(&reconnect_timer);

        k_delayed_work_submit(&gcloud_reboot_work, K_NO_WAIT);

        break;

    /* Acknowledgement for QoS 1 messages. */
    case MQTT_EVT_PUBACK:
        LOG_DBG("MQTT PUBACK");
        if (evt->result != 0)
        {
            LOG_ERR("MQTT PUBACK error: %d", evt->result);
            break;
        }

        LOG_DBG("PUBACK packet id: %d\n", evt->param.puback.message_id);

        break;

    /* Reception confirmation for messages with QoS 2. */
    case MQTT_EVT_PUBREC:
        LOG_DBG("MQTT PUBREC");
        if (evt->result != 0)
        {
            LOG_ERR("MQTT PUBREC error: %d", evt->result);
            break;
        }

        LOG_DBG("PUBREC packet id: %u", evt->param.pubrec.message_id);

        const struct mqtt_pubrel_param rel_param = {
            .message_id = evt->param.pubrec.message_id};

        err = mqtt_publish_qos2_release(client, &rel_param);
        if (err != 0)
        {
            LOG_ERR("Failed to send MQTT PUBREL: %d", err);
        }

        break;

    /* Release of messages with QoS 2. */
    case MQTT_EVT_PUBREL:
        LOG_DBG("MQTT PUBREL");
        if (evt->result != 0)
        {
            LOG_ERR("PUBREL error: %d", evt->result);
            break;
        }
        break;

    /* Confirmation to publish */
    case MQTT_EVT_PUBCOMP:
        if (evt->result != 0)
        {
            LOG_ERR("MQTT PUBCOMP error: %d", evt->result);
            break;
        }

        LOG_DBG("PUBCOMP packet id: %u", evt->param.pubcomp.message_id);
        break;

    /* Acknowledgment of a subscription request. */
    case MQTT_EVT_SUBACK:
        LOG_DBG("MQTT SUBACK");
        if (evt->result != 0)
        {
            LOG_ERR("MQTT SUBACK error: %d", evt->result);
            break;
        }
        switch (*evt->param.suback.return_codes.data)
        {

        /* Subscription with QoS 0 succeeded. */
        case MQTT_SUBACK_SUCCESS_QoS_0:
            LOG_INF("Subscribed with QoS 0");
            break;

        /* Subscription with QoS 1 succeeded. */
        case MQTT_SUBACK_SUCCESS_QoS_1:
            LOG_INF("Subscribed with QoS 1");
            break;

        /* Subscription with QoS 2 succeeded. */
        case MQTT_SUBACK_SUCCESS_QoS_2:
            LOG_INF("Subscribed with QoS 2");
            break;

        /* Subscription for a topic failed. */
        case MQTT_SUBACK_FAILURE:
            LOG_ERR("SUBACK FAILURE");
            break;

        default:
            LOG_ERR("Unknown suback return code");
            break;
        }

        break;

    /* Acknowledgment of a unsubscribe request. */
    case MQTT_EVT_UNSUBACK:
        LOG_DBG("MQTT UNSUBACK");
        if (evt->result != 0)
        {
            LOG_ERR("MQTT UNSUBACK error: %d", evt->result);
            break;
        }

        break;

    default:
        LOG_ERR("Unknown mqtt event type");
        break;
    }
}

void gcloud_thread(void *unused1, void *unused2, void *unused3)
{
    int ret;
    k_timer_init(&reconnect_timer, reconnect_timer_handler, NULL);

    gcloud_connect();
    k_delayed_work_submit(&gcloud_reboot_work,
                          CLOUD_CONNACK_WAIT_DURATION);

    /* Waiting for client socket to be initialized */
    for (int i = 200;
         (!atomic_get(&connecting)) && (!atomic_get(&connected)) && (i > 0); i--)
    {
        k_sleep(K_MSEC(50));
    }

    if ((!atomic_get(&connecting)) && (!atomic_get(&connected)))
    {
        gcloud_error_handler(-ETIMEDOUT);
    }

    struct pollfd fds[] = {
        {.fd = client.transport.tls.sock,
         .events = POLLIN}};

    while (true)
    {
        ret = poll(fds, ARRAY_SIZE(fds),
                   K_SECONDS(CONFIG_MQTT_KEEPALIVE));

        if (ret < 0)
        {
            LOG_ERR("poll() returned an error: %d", ret);
            gcloud_error_handler(ret);
            continue;
        }

        if (ret == 0)
        {
            LOG_DBG("GCLOUD: ping");
            gcloud_ping();
            continue;
        }

        if ((fds[0].revents & POLLIN) == POLLIN)
        {
            LOG_DBG("GCLOUD: process");
            gcloud_process();
        }

        if ((fds[0].revents & POLLNVAL) == POLLNVAL)
        {
            LOG_ERR("Socket error: POLLNVAL");
            gcloud_error_handler(-EIO);
            return;
        }

        if ((fds[0].revents & POLLHUP) == POLLHUP)
        {
            LOG_ERR("Socket error: POLLHUP");
            gcloud_error_handler(-EIO);
            return;
        }

        if ((fds[0].revents & POLLERR) == POLLERR)
        {
            LOG_ERR("Socket error: POLLERR");
            gcloud_error_handler(-EIO);
            return;
        }
    }
}
