project(ControlTheory)

cmake_minimum_required(VERSION 2.8.6)

set(ARDUINO_PROTOCOL "wiring")    # change if bluetooth connected
set(ARDUINO_PORT "/dev/ttyACM0")    # TODO: find automatically?

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(ARDUINO REQUIRED)

# Arduino MEGA2560 specific settings
include(${CMAKE_SOURCE_DIR}/cmake/arduino_mega.cmake)

# include(${CMAKE_SOURCE_DIR}/cmake/fetch_libraries.cmake)
find_package(COMPONENTS_LIB REQUIRED)
find_package(MICROOS_LIB REQUIRED)

set(PROJECT_SOURCE_FILES
  src/main.cpp
  src/robot.cpp
  src/communicator.cpp
)

add_executable(firmware ${PROJECT_SOURCE_FILES} ${ARDUINO_CORE_SOURCES} ${LIBRARY_FILES})
