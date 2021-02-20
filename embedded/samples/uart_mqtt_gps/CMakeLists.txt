#
# Copyright (c) 2018 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

cmake_minimum_required(VERSION 3.13.1)
list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(mqtt-simple)

# NORDIC SDK APP START

target_sources(app PRIVATE
    src/main.c
    src/app_button.c
    src/app_uart.c
    src/app_mqtt.c
    src/app_gps.c

    
    include/app_button.h
    include/app_uart.h
    include/app_mqtt.h
    include/app_gps.h
    )

zephyr_include_directories(include)

# NORDIC SDK APP END
