#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

cmake_minimum_required(VERSION 3.8.2)

set(BOARD nrf9160_pca10090ns)

# This sample runs as a non-secure application on nRF91. Therefore, it
# requires the secure_partition_manager that prepares the required
# peripherals to be available for the application.
#
# Configure the SPM image to enable the peripherals that this sample
# needs.

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
# find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(NONE)

if(NOT EXISTS ${APPLICATION_SOURCE_DIR}/src/private_info/key.c)
    message(FATAL_ERROR "!!!!!! Generate key file before continuing !!!!!!")
endif()


# NORDIC SDK APP START
target_sources(app PRIVATE
    src/main.c
    src/cJSON.c
    src/gcloud.c
    src/ntp.c

    
    include/cJSON.h
    include/gcloud.h
    include/gcloud_defines.h
    include/ntp.h
    )
# NORDIC SDK APP END

# Include application events and configuration headers
zephyr_include_directories(include)

# Application sources
target_sources(app PRIVATE ${APPLICATION_SOURCE_DIR}/src/private_info/key.c)