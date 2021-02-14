/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 *
 * @brief   Defines for Google Cloud and MQTT
 */

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

#define GCLOUD_ID                              \
    "projects/" CONFIG_GCLOUD_PROJECT_NAME     \
    "/locations/" CONFIG_GCLOUD_REGION         \
    "/registries/" CONFIG_GCLOUD_REGISTRY_NAME \
    "/devices/" CONFIG_GCLOUD_DEVICE_NAME

#define GCLOUD_HOSTNAME "mqtt.googleapis.com"
#define BROKER_ADDR GCLOUD_HOSTNAME
#define BROKER_PORT 8883

#define GCLOUD_SEC_TAG 16842753

#define SUBSCRIBE_ID 42

#define CLOUD_CONNACK_WAIT_DURATION K_SECONDS(CONFIG_CLOUD_WAIT_DURATION)
#define GCLOUD_DISCONNECT_INTERVAL K_HOURS(12)

#define GCLOUD_CONFIG_TOPIC "/devices/" CONFIG_GCLOUD_DEVICE_NAME "/config"
#define GCLOUD_STATE_TOPIC "/devices/" CONFIG_GCLOUD_DEVICE_NAME "/state"
#define GCLOUD_TOPIC "/devices/" CONFIG_GCLOUD_DEVICE_NAME "/events"

#define JWT_BUFFER_SIZE 256

#define GCLOUD_THREAD_STACK_SIZE 2048
#define GCLOUD_THREAD_PRIORITY 8
