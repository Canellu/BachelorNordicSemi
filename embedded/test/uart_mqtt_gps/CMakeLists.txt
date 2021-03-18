#
# Copyright (c) 2018 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

cmake_minimum_required(VERSION 3.13.1)
list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(oasys_project)

if(NOT EXISTS ${APPLICATION_SOURCE_DIR}/src/private_info/key.c)
    message(FATAL_ERROR "!!!!!! Generate key file before continuing !!!!!!")
endif()

# NORDIC SDK APP START

file(GLOB SRC_FILES
    "src/*.c"
)
file(GLOB HEADER_FILES
    "include/*.h"
)

target_sources(app PRIVATE
    ${SRC_FILES}
    ${HEADER_FILES}
    src/private_info/key.c
)

zephyr_include_directories(include)

# NORDIC SDK APP END
