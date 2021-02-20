#
# Copyright (c) 2018 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

cmake_minimum_required(VERSION 3.13.1)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(mqtt-gcloud)

if(NOT EXISTS ${APPLICATION_SOURCE_DIR}/src/private_info/key.c)
    message(FATAL_ERROR "!!!!!! Generate key file before continuing !!!!!!")
endif()

# NORDIC SDK APP START
target_sources(app PRIVATE 
  src/main.c
  src/cJSON.c
  src/ntp.c
  src/private_info/key.c

  include/cJSON.h
  include/ntp.h
  )
# NORDIC SDK APP END
zephyr_include_directories(include)