#include <logging/log.h>

#include <zephyr.h>
#include <kernel.h>
#include <string.h>
#include <errno.h>

#include <date_time.h>

#include <net/mqtt.h>
#include <net/socket.h>
#include <net/tls_credentials.h>

#include <data/jwt.h>

#include <modem/lte_lc.h>
#include <modem/modem_key_mgmt.h>
#include <nrf_modem.h>

#include "gcloud.h"
#include <cJSON.h>

#include "globalsign.inc"

#define ROOT_CERT                                                        \
    "-----BEGIN CERTIFICATE-----\n"                                      \
    "MIIBdTCCARugAwIBAgIJAOvj4PVfAgqjMAoGCCqGSM49BAMCMBYxFDASBgNVBAMM\n" \
    "C25yZjkxLW1hcnRlMCAXDTE5MDQwNTExMTUxN1oYDzQ3NTcwMzAyMTExNTE3WjAW\n" \
    "MRQwEgYDVQQDDAtucmY5MS1tYXJ0ZTBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IA\n" \
    "BAq8otkWGnocpdAxlnpJdVn6EBb1WbQnZXLFaYzsMeXWGFIIO02hWTvaSUPI1xNs\n" \
    "0QB8c3IU9oBhDXkRNU2Nqi2jUDBOMB0GA1UdDgQWBBRLAhO335CAd280I/qVQmiM\n" \
    "07nd8TAfBgNVHSMEGDAWgBRLAhO335CAd280I/qVQmiM07nd8TAMBgNVHRMEBTAD\n" \
    "AQH/MAoGCCqGSM49BAMCA0gAMEUCIF2kQO7Cqn73ANmwBdgGA1y/KGOpOU4I/ywF\n" \
    "LEu8trOUAiEA8//UWttMRkmaylghw3mgsYGGKk17rY+4j+4mGUwvDnw=\n"         \
    "-----END CERTIFICATE-----"

#define GCLOUD_CA_CERTIFICATE ROOT_CERT

#define GCLOUD_ID "projects/" CONFIG_GCLOUD_PROJECT_NAME "/locations/" CONFIG_GCLOUD_REGION "/registries/" CONFIG_GCLOUD_REGISTRY_NAME "/devices/" CONFIG_GCLOUD_DEVICE_NAME

#define GCLOUD_HOSTNAME "mqtt.googleapis.com"
#define BROKER_ADDR GCLOUD_HOSTNAME
#define BROKER_PORT 8883

#define GCLOUD_SEC_TAG 16842752
#define SUBSCRIBE_ID 42

#define MQTT_INPUT_PERIOD K_SECONDS(2)
#define MQTT_LIVE_PERIOD K_SECONDS(55)
#define GCLOUD_DISCONNECT_INTERVAL K_HOURS(12)
#define INPUT_TIMEOUT_PERIOD K_SECONDS(10)

#define GCLOUD_CONFIG_TOPIC "/devices/" CONFIG_GCLOUD_DEVICE_NAME "/config"
#define GCLOUD_COMMANDS_TOPIC "/devices/" CONFIG_GCLOUD_DEVICE_NAME "/commands/#"
#define GCLOUD_STATE_TOPIC "/devices/" CONFIG_GCLOUD_DEVICE_NAME "/state"
#define GCLOUD_TOPIC "/devices/" CONFIG_GCLOUD_DEVICE_NAME "/events"

#define JWT_BUFFER_SIZE 256

LOG_MODULE_REGISTER(gcloud);

/* Buffers for MQTT client. */
static uint8_t rx_buffer[CONFIG_MQTT_MESSAGE_BUFFER_SIZE];
static uint8_t tx_buffer[CONFIG_MQTT_MESSAGE_BUFFER_SIZE];
static uint8_t payload_buf[CONFIG_MQTT_MESSAGE_BUFFER_SIZE];

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
     .qos = MQTT_QOS_1_AT_LEAST_ONCE},
    {.topic = {.utf8 = GCLOUD_COMMANDS_TOPIC, .size = sizeof(GCLOUD_COMMANDS_TOPIC) - 1}, .qos = MQTT_QOS_1_AT_LEAST_ONCE}

};

/* Private key information */
extern unsigned char zepfull_private_der[];
extern unsigned int zepfull_private_der_len;

/* Variables from Main */
extern struct k_msgq gcloud_msg_q;

/* Function declarations */
static received_config_handler_t received_config_handler = NULL;

static void mqtt_event_handler(struct mqtt_client *client, const struct mqtt_evt *evt);

int k_msgq_get_atomic(struct k_msgq *q, void *data, k_timeout_t timeout);
int k_msgq_put_atomic(struct k_msgq *q, void *data, k_timeout_t timeout);

static bool connected = false;
static bool connecting = false;

/* Semaphores */
static K_SEM_DEFINE(date_time_ok, 0, 1);

/* File descriptor */
static struct pollfd fds;

/* FUNCTIONS */

// Callback function for date_time_update
void date_time_handler(const struct date_time_evt *evt)
{

    switch (evt->type)
    {
    case DATE_TIME_OBTAINED_MODEM:
        LOG_DBG("DATE_TIME: got time from modem.\n");
        k_sem_give(&date_time_ok);
        break;
    case DATE_TIME_OBTAINED_NTP:
        LOG_DBG("DATE_TIME: got time from NTP.\n");
        k_sem_give(&date_time_ok);
        break;
    case DATE_TIME_OBTAINED_EXT:
        LOG_DBG("DATE_TIME: got time from external.\n");
        k_sem_give(&date_time_ok);
        break;
    case DATE_TIME_NOT_OBTAINED:
        LOG_DBG("DATE_TIME: failed to get time.\n");
        k_sem_give(&date_time_ok);
        break;
    default:
        break;
    }
}

/**@brief Resolves the configured hostname and
 * initializes the MQTT broker structure
 */
static void broker_init(void)
{
    int ret;
    struct addrinfo *result;
    struct addrinfo *addr;
    struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM};

    ret = getaddrinfo(GCLOUD_HOSTNAME, NULL, &hints, &result);
    if (ret)
    {
        LOG_ERR("ERROR: getaddrinfo failed [%d] %s", ret, strerror(ret));
        return;
    }

    addr = result;
    ret = -ENOENT;

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

static int make_jwt(char *buffer, size_t buffer_size)
{

    int ret;
    struct jwt_builder jb;

    ret = jwt_init_builder(&jb, buffer, buffer_size);
    if (ret != 0)
    {
        LOG_ERR("Unable to init JWT builder: %d", ret);
        return ret;
    }

    /* Get Unix time from modem thread and mask upper bits */
    int64_t ntp;
    date_time_now(&ntp);

    int64_t unixtime = (ntp / 1000); //TODO: Is unix time valid as jwt timestamp?
    int32_t y = (int32_t)unixtime;

    LOG_DBG("Unix Timestamp: %d\n", y);
    int32_t expiry_time = (y + (12 * 60 * 60));
    int32_t issue_time = y;

    ret = jwt_add_payload(&jb, expiry_time, issue_time, CONFIG_GCLOUD_PROJECT_NAME);
    if (ret != 0)
    {
        LOG_ERR("Unable to add JWT payload: %d", ret);
        return ret;
    }
    ret = jwt_sign(&jb, zepfull_private_der, zepfull_private_der_len);
    if (ret != 0)
    {
        LOG_ERR("Unable to add JWT payload: %d", ret);
        return ret;
    }
    if (jb.overflowed != 0)
    {
        LOG_ERR("JWT buffer overflowed");
        return -ENOMEM;
    }
    return ret;
}

static int gcloud_subscribe()
{
    const struct mqtt_subscription_list subscriptions = {
        .list = (struct mqtt_topic *)&gcloud_topics_list,
        .list_count = ARRAY_SIZE(gcloud_topics_list),
        .message_id = SUBSCRIBE_ID};

    return mqtt_subscribe(&client, &subscriptions);
}

static int client_init(void)
{

    mqtt_client_init(&client);

    int ret = make_jwt(jwt_buffer, JWT_BUFFER_SIZE);
    if (ret != 0)
    {
        LOG_ERR("Unable to make jwt: [%d] %s", ret, strerror(-ret));
        return ret;
    }

    LOG_DBG("JWT:\n%s\n", log_strdup(jwt_buffer));
    LOG_DBG("JWT length: %d\n", strlen(jwt_buffer));
    jwt_token.size = strlen(jwt_buffer);

    client.broker = &mqtt_broker;

    /* MQTT client configuration */
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = (uint8_t *)GCLOUD_ID;
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

    /* MQTT transport configuration */
    client.transport.type = MQTT_TRANSPORT_SECURE;

    /* Session */
    client.clean_session = 1;
    client.keepalive = 120;

    return 0;
}

// CANDO: Merge publish functions, and only have one publish command
// (the public API can still have two separate functions)
int gcloud_publish(uint8_t *data, uint32_t len, enum mqtt_qos qos)
{

    if (!connected)
    {
        LOG_WRN("Cannot publish data while not connected to Google Cloud");
        return -ENOTCONN;
    }

    LOG_INF("Publishing to GCloud");

    struct mqtt_publish_param param;

    param.message.topic.qos = qos;
    param.message.topic.topic.utf8 = GCLOUD_TOPIC;
    param.message.topic.topic.size = strlen(GCLOUD_TOPIC);
    param.message.payload.data = data;
    param.message.payload.len = len;
    param.message_id = k_uptime_get_32();
    param.dup_flag = 0;
    param.retain_flag = 0;

    LOG_INF("Publishing: %s", log_strdup(param.message.payload.data));

    return mqtt_publish(&client, &param);
}

/*
int gcloud_publish_state(uint8_t *data, uint32_t size, enum mqtt_qos qos)
{
    int ret;
    if (!connected)
    {
        LOG_WRN("Cannot publish state while not connected to Google Cloud");
        return -ENOTCONN;
    }
    struct gcloud_event state_cmd = {
        .type = PUBLISH_STATE,
        .param.publish_state = {
            .topic = {
                .topic = {
                    .utf8 = GCLOUD_STATE_TOPIC,
                    .size = strlen(GCLOUD_STATE_TOPIC)},
                .qos = qos},
            .payload = {.data = data, .len = size}}};
    ret = k_msgq_put_atomic(&gcloud_msgq, &state_cmd, K_FOREVER);
    return ret;
}
*/

int k_msgq_get_atomic(struct k_msgq *q, void *data, k_timeout_t timeout)
{
    unsigned int key = irq_lock();
    int temp = k_msgq_get(q, data, timeout);
    irq_unlock(key);
    return temp;
}

int k_msgq_put_atomic(struct k_msgq *q, void *data, k_timeout_t timeout)
{
    unsigned int key = irq_lock();
    int temp = k_msgq_put(q, data, timeout);
    irq_unlock(key);
    return temp;
}

/**@brief Handler for when receiving data from Google Cloud */
static void gcloud_received_data_handler(const struct mqtt_publish_message *message)
{

    cJSON *data_json = cJSON_Parse(message->payload.data);
    cJSON_Print(data_json);
    cJSON_Delete(data_json);
}

/**@brief Initialize the file descriptor structure used by poll */
static int fds_init(struct mqtt_client *c)
{
    if (c->transport.type == MQTT_TRANSPORT_NON_SECURE)
    {
        fds.fd = c->transport.tcp.sock;
    }
    else
    {
#if defined(CONFIG_MQTT_LIB_TLS)
        fds.fd = c->transport.tls.sock;
#else
        return -ENOTSUP;
#endif
    }

    fds.events = POLLIN;

    return 0;
}

static int gcloud_connect(received_config_handler_t received_config_cb)
{
    int ret = 0;
    received_config_handler = received_config_cb;

    // send connect command
    LOG_DBG("Initiating broker");
    broker_init();

    LOG_DBG("Making JWT");
    make_jwt(jwt_buffer, JWT_BUFFER_SIZE);

    LOG_DBG("Initiating client");
    client_init();

    LOG_DBG("Connecting mqtt");
    connecting = true;
    ret = mqtt_connect(&client);
    if (ret)
    {
        LOG_ERR("mqtt_connect failed: [%d] %s", ret, strerror(-ret));
        // TODO: Find a way to report this error to the application.
    }

    printk("Mqtt connected\n");

    return ret;
}

int gcloud_provision(void)
{

    int ret;

    /* Security configuration */
    static sec_tag_t sec_tag_list[] = {GCLOUD_SEC_TAG};
    tls_config = (client.transport).tls.config;

    tls_config.peer_verify = 0;
    tls_config.hostname = NULL;

    tls_config.cipher_list = NULL;

    tls_config.cipher_count = 0;
    tls_config.sec_tag_count = ARRAY_SIZE(sec_tag_list);
    tls_config.sec_tag_list = sec_tag_list;
    tls_config.hostname = GCLOUD_HOSTNAME;

    /* Delete certificates */
    nrf_sec_tag_t sec_tag = GCLOUD_SEC_TAG;

    for (enum modem_key_mgmt_cred_type type = 0; type < 5; type++)
    {
        ret = modem_key_mgmt_delete(sec_tag, type);
        if (ret)
        {
            LOG_ERR("key delete err: [%d] %s", ret, strerror(ret));
        }
    }
    // LOG_DBG("CERT:\n%s\n", GCLOUD_CA_CERTIFICATE);
    /* Provision CA Certificate */
    ret = modem_key_mgmt_write(
        GCLOUD_SEC_TAG,
        MODEM_KEY_MGMT_CRED_TYPE_CA_CHAIN,
        GCLOUD_CA_CERTIFICATE,
        strlen(GCLOUD_CA_CERTIFICATE));
    if (ret != 0)
    {
        LOG_ERR("GCLOUD_CA_CERTIFICATE err: [%d] %s\n", ret, strerror(ret));
        return ret;
    }

    // This function should probably only be called once.
    // connecting = true;
    // ret = mqtt_connect(&client);

    return 0;
}

static void mqtt_event_handler(struct mqtt_client *client,
                               const struct mqtt_evt *evt)
{
    int ret = 0;
    LOG_INF("MQTT event: %d", evt->type);

    switch (evt->type)
    {
    /* Response to connack request */
    case MQTT_EVT_CONNACK:
        LOG_DBG("Got CONNACK");
        if (evt->result != 0)
        {
            LOG_ERR("MQTT connect failed: [%d] %s", evt->result, strerror(evt->result));
            break;
        }

        connected = true;
        connecting = false;

        ret = gcloud_subscribe();
        if (ret)
        {
            LOG_ERR("gcloud_subscribe failed: [%d] %s", ret, strerror(-ret));
            // TODO: Find a way to report this error to the application.
        }
        else
        {
            printk("Subscribe successful\n");
        }

        break;

    /* Message has been published to a topic we subscribe to */
    case MQTT_EVT_PUBLISH:
        LOG_DBG("MQTT PUBLISH event");
        if (evt->result != 0)
        {
            LOG_ERR("Publish event error: [%d] %s", evt->result, strerror(-evt->result));
        }

        if (evt->param.publish.message.topic.qos == MQTT_QOS_1_AT_LEAST_ONCE)
        {
            const struct mqtt_puback_param ack = {
                .message_id = evt->param.publish.message_id};

            /* Send ack */
            mqtt_publish_qos1_ack(client, &ack);
            LOG_INF("Send acknowledgement");
        }

        if (evt->param.publish.message.payload.len <= 2)
        {
            LOG_WRN("Received empty payload");
            break;
        }

        LOG_DBG("Length of payload: %d", evt->param.publish.message.payload.len);
        LOG_DBG("Topic: %s", log_strdup(evt->param.publish.message.topic.topic.utf8));
        LOG_DBG("QoS: %d", evt->param.publish.message.topic.qos);

        mqtt_readall_publish_payload(client, payload_buf, evt->param.publish.message.payload.len);
        LOG_DBG("Message:\n%s\n", log_strdup(payload_buf));

        char *gcloud_msg = calloc(evt->param.publish.message.payload.len, sizeof(char) + 1);
        memcpy(gcloud_msg, payload_buf, evt->param.publish.message.payload.len);
        memset(payload_buf, 0, sizeof(payload_buf));

        LOG_DBG("Adding to message queue");
        k_msgq_put(&gcloud_msg_q, gcloud_msg, K_NO_WAIT);

        free(gcloud_msg);

        break;

    /* Disconnect event. MQTT Client Reference is no longer valid. */
    case MQTT_EVT_DISCONNECT:

        LOG_DBG("MQTT client disconnected: [%d] %s", evt->result, strerror(-evt->result));
        // If we are connected, something has gone wrong, and we would like to reconnect

        if (connected || connecting)
        {
            ret = gcloud_connect(received_config_handler);
            if (ret)
            {
                LOG_ERR("event handler reconnect failed [%d] %s", ret, strerror(ret));
            }
        }
        break;

    /* Acknowledgement for QoS 1 messages */
    case MQTT_EVT_PUBACK:
        if (evt->result != 0)
        {
            LOG_ERR("MQTT PUBACK error: %d", evt->result);
            break;
        }

        LOG_DBG("PUBACK packet id: %d\n", evt->param.puback.message_id);

        break;

    /* Reception confirmation for messages with QoS 2 */
    case MQTT_EVT_PUBREC:
        if (evt->result != 0)
        {
            LOG_ERR("MQTT PUBREC error: %d", evt->result);
            break;
        }

        LOG_DBG("PUBREC packet id: %u", evt->param.pubrec.message_id);

        const struct mqtt_pubrel_param rel_param = {
            .message_id = evt->param.pubrec.message_id};

        ret = mqtt_publish_qos2_release(client, &rel_param);
        if (ret != 0)
        {
            LOG_ERR("Failed to send MQTT PUBREL: [%d] %s", ret, strerror(ret));
        }

        break;

    /* Release of messages with QoS 2 */
    case MQTT_EVT_PUBREL:
        LOG_DBG("MQTT PUBREL event");
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

    /* Ack of a subscription request */
    case MQTT_EVT_SUBACK:
        LOG_DBG("Got SUBACK");
        if (evt->result != 0)
        {
            LOG_ERR("MQTT SUBACK error: %d", evt->result);
            break;
        }

        break;

    /* Ack of an unsubscribe request */
    case MQTT_EVT_UNSUBACK:
        LOG_DBG("MQTT UNSUBACK event");
        if (evt->result != 0)
        {
            LOG_ERR("MQTT UNSUBACK error: %d", evt->result);
            break;
        }

        break;

    default:
        LOG_INF("Unhandled MQTT event type: %d", evt->type);
        break;
    }
}

/**@brief Configures modem to provide LTE link. Blocks until link is
 * successfully established
 */
int modem_configure(void)
{
    int ret;

    if (IS_ENABLED(CONFIG_LTE_AUTO_INIT_AND_CONNECT))
    {
        /* Do nothing, modem is already turned on and connected */
    }
    else
    {
        //LOG_INF("Enabling PSM\n");
        /* Enable PSM mode */
        //lte_lc_psm_req(true);

        LOG_INF("Establishing LTE link\n");
        ret = lte_lc_init_and_connect();
        if (ret)
        {
            LOG_INF("Failed to establish LTE connection: %d", ret);
            return ret;
        }
    }
    LOG_INF("LTE Link Connected!");

    return ret;
}

static int modem_reconnect()
{
    int ret;

    LOG_INF("LTE Link Connecting...");
    ret = lte_lc_connect();
    if (ret)
    {
        LOG_INF("Failed to establish LTE connection: %d", ret);
        return ret;
    }
    LOG_INF("LTE Link Connected!");

    return ret;
}

int app_gcloud_init_and_connect(int retries)
{
    int cnt = 0;
    int ret = 0;

    // modem configure
    LOG_INF("Initializing modem\n");
    while (cnt < retries)
    {
        ret = modem_configure();
        if (ret == 0)
        {
            break;
        }
        else
        {
            LOG_INF("Retrying in %d seconds", CONFIG_LTE_CONNECT_RETRY_DELAY_S);
            k_sleep(K_SECONDS(CONFIG_LTE_CONNECT_RETRY_DELAY_S));
            cnt++;
        }
    }
    if (ret)
    {
        LOG_ERR("Unable to configure modem");
        return ret;
    }

    // synchronize time
    date_time_clear();
    date_time_update_async(date_time_handler);
    k_sem_take(&date_time_ok, K_FOREVER);

    ret = gcloud_provision();
    if (ret != 0)
    {
        LOG_ERR("Provisioning failed, error: %d\n", ret);
        return ret;
    }

    /* Connect to Google Cloud */
    cnt = 0;
    while (cnt < retries)
    {
        ret = gcloud_connect(gcloud_received_data_handler);
        if (ret == 0)
        {
            break;
        }
        else
        {
            LOG_ERR("Failed to connect to Google Cloud, error: %d\n", ret);
            cnt++;
        }
    }
    if (ret)
    {
        LOG_ERR("Unable to connect to Google Cloud, max retries reached");
        return ret;
    }

    // fds init
    ret = fds_init(&client);
    if (ret != 0)
    {
        LOG_ERR("fds_init: %d", ret);
        return ret;
    }

    LOG_INF("Connect successful");
    return 0;
}

int app_gcloud_reconnect(int retries)
{
    int cnt = 0;
    int ret = 0;

    // modem configure
    LOG_INF("Reinitializing modem\n");
    while (cnt < retries)
    {
        ret = modem_reconnect();
        if (ret == 0)
        {
            break;
        }
        else
        {
            LOG_INF("Retrying in %d seconds", CONFIG_LTE_CONNECT_RETRY_DELAY_S);
            k_sleep(K_SECONDS(CONFIG_LTE_CONNECT_RETRY_DELAY_S));
            cnt++;
        }
    }
    if (ret)
    {
        LOG_ERR("Unable to configure modem");
        return ret;
    }

    // synchronize time
    date_time_update_async(date_time_handler);
    k_sem_take(&date_time_ok, K_FOREVER);

    // mqtt connect
    LOG_DBG("Connecting mqtt");
    connecting = true;
    cnt = 0;
    while (cnt < retries)
    {
        ret = mqtt_connect(&client);
        if (ret == 0)
        {
            break;
        }
        else
        {
            LOG_ERR("mqtt_connect failed: [%d] %s", ret, strerror(-ret));
            cnt++;
        }
    }
    if (ret)
    {
        LOG_ERR("Unable to connect to Google Cloud, max retries reached");
        return ret;
    }

    LOG_INF("Reconnect successful");
    return 0;
}

int app_gcloud(void)
{
    int ret;

    ret = poll(&fds, 1, mqtt_keepalive_time_left(&client));
    if (ret < 0)
    {
        LOG_ERR("poll: %d", errno);
        return ret;
    }

    ret = mqtt_live(&client);
    if ((ret != 0) && (ret != -EAGAIN))
    {
        LOG_ERR("ERROR: mqtt_live: %d", ret);
        return ret;
    }

    if ((fds.revents & POLLIN) == POLLIN)
    {
        ret = mqtt_input(&client);
        if (ret != 0)
        {
            LOG_ERR("mqtt_input: %d", ret);
            return ret;
        }
    }

    if ((fds.revents & POLLERR) == POLLERR)
    {
        LOG_ERR("POLLERR");
        return ret;
    }

    if ((fds.revents & POLLNVAL) == POLLNVAL)
    {
        LOG_ERR("POLLNVAL");
        return ret;
    }

    return 0;
}

int app_gcloud_disconnect(int retries)
{
    int cnt = 0;
    int ret = 0;

    LOG_INF("Disconnecting GCloud MQTT client...");

    connected = false;

    // MQTT disconnect client
    ret = mqtt_disconnect(&client);
    if (ret != 0)
    {
        LOG_ERR("Could not disconnect GCloud MQTT client: %d", ret);
        cnt++;
    }

    // lte off
    while (cnt < retries)
    {
        ret = lte_lc_offline();
        if (ret == 0)
        {
            break;
        }
        else
        {
            LOG_ERR("Could not set modem to offline: %d", ret);
            cnt++;
        }
    }
    if (ret)
    {
        LOG_ERR("Unable to properly turn off modem, max retries reached");
        return ret;
    }

    return 0;
}
