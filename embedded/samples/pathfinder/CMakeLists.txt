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
set(spm_CONF_FILE
  prj.conf
  ${CMAKE_CURRENT_LIST_DIR}/child_secure_partition_manager.conf
  )

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(NONE)

if(NOT EXISTS ${APPLICATION_SOURCE_DIR}/src/private_info/key.c)
    message(FATAL_ERROR "!!!!!! Generate key file before continuing !!!!!!")
endif()


# NORDIC SDK APP START
target_sources(app PRIVATE src/main.c)
# NORDIC SDK APP END
zephyr_include_directories(src)

# Include application events and configuration headers
zephyr_library_include_directories(
  src/ui
  src/cJSON
  src/ntp
  src/gcloud
  src/micro_bit_compass
  src/path_module
  include
  )

# Application sources
add_subdirectory(src/ui)
add_subdirectory(src/gps_controller)
add_subdirectory(src/cJSON)
add_subdirectory(src/ntp)
add_subdirectory(src/gcloud)
add_subdirectory(src/micro_bit_compass)
add_subdirectory(src/path_module)

target_sources(app PRIVATE ${APPLICATION_SOURCE_DIR}/src/private_info/key.c)