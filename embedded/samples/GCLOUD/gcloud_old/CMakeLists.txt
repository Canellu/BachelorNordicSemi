#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

cmake_minimum_required(VERSION 3.13.1)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(gcloud_test)

if(NOT EXISTS ${APPLICATION_SOURCE_DIR}/src/private_info/key.c)
    message(FATAL_ERROR "!!!!!! Generate key file before continuing !!!!!!")
endif()

# NORDIC SDK APP START
target_sources(app PRIVATE 
  src/main.c
  src/gcloud.c
  src/cJSON.c
  src/private_info/key.c

  include/gcloud.h
  include/cJSON.h
  )

# NORDIC SDK APP END
zephyr_include_directories(include)