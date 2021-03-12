#
# Copyright (c) 2018 Nordic Semiconductor
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
#set(spm_CONF_FILE
#  prj.conf
#  ${CMAKE_CURRENT_LIST_DIR}/child_secure_partition_manager.conf
#  )


include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)

project(NONE)

if(NOT EXISTS ${APPLICATION_SOURCE_DIR}/src/private_info/key.c)
    message(FATAL_ERROR "!!!!!! Generate key file before continuing !!!!!!")
endif()

FILE(GLOB_RECURSE app_sources src/*.c)

target_sources(app PRIVATE ${app_sources})

zephyr_library_include_directories(include)
