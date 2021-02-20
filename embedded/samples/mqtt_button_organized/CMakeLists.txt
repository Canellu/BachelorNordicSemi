#
# Copyright (c) 2018 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

cmake_minimum_required(VERSION 3.13.1)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(mqtt-simple)

# NORDIC SDK APP START

target_sources(app PRIVATE
    src/main.c
    src/app_button.c
    src/app_mqtt.c

    
    include/app_button.h
    include/app_mqtt.h)

zephyr_include_directories(include)

# NORDIC SDK APP END
