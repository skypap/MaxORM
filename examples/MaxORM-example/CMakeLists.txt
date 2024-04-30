# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)
add_definitions(-DBOARD_HAS_PSRAM)
#add_definitions(-DSPIRAM_SUPPORT=1)
add_definitions(-DESPPORT=/dev/ttyACM0)
#set(COMPONENTS main)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(MaxORM-example)
