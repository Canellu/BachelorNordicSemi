# Install script for directory: C:/Users/Einar/ncs/v1.4.99-dev1/zephyr

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Zephyr-Kernel")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Users/Einar/ncs/v1.4.1/toolchain/opt/bin/arm-none-eabi-objdump.exe")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/zephyr/arch/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/zephyr/lib/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/zephyr/soc/arm/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/zephyr/boards/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/zephyr/subsys/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/zephyr/drivers/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/modules/nrf/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/modules/mcuboot/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/modules/mcumgr/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/modules/nrfxlib/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/modules/cmsis/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/modules/canopennode/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/modules/civetweb/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/modules/fatfs/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/modules/nordic/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/modules/st/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/modules/libmetal/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/modules/lvgl/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/modules/mbedtls/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/modules/open-amp/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/modules/loramac-node/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/modules/openthread/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/modules/segger/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/modules/tinycbor/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/modules/tinycrypt/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/modules/littlefs/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/modules/mipi-sys-t/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/modules/nrf_hw_models/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/modules/tfm/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/zephyr/kernel/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/zephyr/cmake/flash/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/zephyr/cmake/usage/cmake_install.cmake")
  include("C:/Users/Einar/Desktop/Bachelor_Project/github/BachelorNordicSemi/embedded/samples/nrfgcloud_new/build/zephyr/cmake/reports/cmake_install.cmake")

endif()

