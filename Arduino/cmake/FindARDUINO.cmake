# define needed Arduino libraries
set(ARDUINO_LIB_NAMES
  Wire
  SPI
  SoftwareSerial
)

# find ARDUINO version
find_file(ARDUINO_REVISIONS_PATH
  NAMES revisions.txt
  PATHS $ENV{ARDUINO_ROOT}
)
if(ARDUINO_REVISIONS_PATH)
    file(READ ${ARDUINO_REVISIONS_PATH} ARD_VERSION)
    if("${ARD_VERSION}" MATCHES "ARDUINO ([0-9]+).([0-9]+).([0-9]+)")
        set(ARDUINO_SDK_VERSION_WORLD ${CMAKE_MATCH_1})
        set(ARDUINO_SDK_VERSION_MAJOR ${CMAKE_MATCH_2})
        set(ARDUINO_SDK_VERSION_MINOR ${CMAKE_MATCH_3})
        message(STATUS "Found Arduino SDK version: ${ARDUINO_SDK_VERSION_WORLD}.${ARDUINO_SDK_VERSION_MAJOR}.${ARDUINO_SDK_VERSION_MINOR}")
    endif()
elseif()
    message(SEND_ERROR "Could not find revisions.txt in order to detect Arduino SDK version.")
endif()

# find and include Arduino tools
find_path(ARDUINO_TOOLS_DIR
  avr-g++
  HINTS $ENV{ARDUINO_ROOT}/hardware/tools/avr/bin
)

if(ARDUINO_TOOLS_DIR)
  message(STATUS "Found Arduino tools include dir: ${ARDUINO_TOOLS_DIR}")
else()
  message(SEND_ERROR "Could not find Arduino tools include dir.")
endif()


# find and include Arduino core directory
find_path(ARDUINO_CORE_DIR
  HardwareSerial.cpp
  HINTS $ENV{ARDUINO_ROOT}/hardware/arduino/avr/cores/arduino
)

if(ARDUINO_CORE_DIR)
  message(STATUS "Found Arduino AVR cores include dir: ${ARDUINO_CORE_DIR}")

  include_directories(${ARDUINO_CORE_DIR})
  set(ARDUINO_CORE_SOURCES
    # ${ARDUINO_CORE_DIR}/wiring_pulse.S
    ${ARDUINO_CORE_DIR}/wiring_digital.c
    ${ARDUINO_CORE_DIR}/wiring.c
    ${ARDUINO_CORE_DIR}/WInterrupts.c
    ${ARDUINO_CORE_DIR}/wiring_pulse.c
    ${ARDUINO_CORE_DIR}/wiring_shift.c
    ${ARDUINO_CORE_DIR}/hooks.c
    ${ARDUINO_CORE_DIR}/wiring_analog.c
    ${ARDUINO_CORE_DIR}/WMath.cpp
    ${ARDUINO_CORE_DIR}/IPAddress.cpp
    ${ARDUINO_CORE_DIR}/Tone.cpp
    ${ARDUINO_CORE_DIR}/HardwareSerial2.cpp
    ${ARDUINO_CORE_DIR}/Print.cpp
    ${ARDUINO_CORE_DIR}/new.cpp
    ${ARDUINO_CORE_DIR}/HardwareSerial0.cpp
    ${ARDUINO_CORE_DIR}/HardwareSerial.cpp
    ${ARDUINO_CORE_DIR}/WString.cpp
    ${ARDUINO_CORE_DIR}/abi.cpp
    ${ARDUINO_CORE_DIR}/USBCore.cpp
    ${ARDUINO_CORE_DIR}/Stream.cpp
    ${ARDUINO_CORE_DIR}/CDC.cpp
  )
  if((${ARDUINO_SDK_VERSION_WORLD} EQUAL 1) AND (${ARDUINO_SDK_VERSION_MAJOR} EQUAL 6))
    set(ARDUINO_CORE_SOURCES
      ${ARDUINO_CORE_SOURCES}
      ${ARDUINO_CORE_DIR}/wiring_pulse.S
    )
  endif()
  if((${ARDUINO_SDK_VERSION_WORLD} EQUAL 1) AND (${ARDUINO_SDK_VERSION_MAJOR} EQUAL 7))
    set(ARDUINO_CORE_SOURCES ${ARDUINO_CORE_SOURCES} ${ARDUINO_CORE_DIR}/HID.cpp)
  endif()
else()
  message(SEND_ERROR "Could not find Arduino AVR cores include dir.")
endif()


# find and include Arduino MEGA specifics
find_path(ARDUINO_MEGA_DIR
  pins_arduino.h
  HINTS $ENV{ARDUINO_ROOT}/hardware/arduino/avr/variants/mega
)

if(ARDUINO_MEGA_DIR)
  message(STATUS "Found Arduino MEGA include dir: ${ARDUINO_MEGA_DIR}")

  include_directories(${ARDUINO_MEGA_DIR})
else()
  message(SEND_ERROR "Could not find Arduino MEGA include dir.")
endif()


# include Arduino libraries that are needed
find_path(ARDUINO_LIB_DIR
  NAMES Wire/Wire.h Wire/src/Wire.h
  HINTS $ENV{ARDUINO_ROOT}/hardware/arduino/avr/libraries
)

if(ARDUINO_LIB_DIR)
  message(STATUS "Found Arduino library dir: ${ARDUINO_LIB_DIR}")
  if((${ARDUINO_SDK_VERSION_WORLD} EQUAL 1) AND (${ARDUINO_SDK_VERSION_MAJOR} EQUAL 6))
    set(ARDUINO_LIB_PATH_SRC "/src")
  endif()
  foreach(subdir ${ARDUINO_LIB_NAMES})
    set(LIB_FILE "${ARDUINO_LIB_DIR}/${subdir}${ARDUINO_LIB_PATH_SRC}/${subdir}.cpp")
    if(EXISTS ${LIB_FILE})
      set(ARDUINO_LIB_FILES ${ARDUINO_LIB_FILES} ${LIB_FILE})
      include_directories("${ARDUINO_LIB_DIR}/${subdir}${ARDUINO_LIB_PATH_SRC}")
    else()
      message(SEND_ERROR "Could not find header file of Arduino library: ${subdir}")
    endif()
  endforeach()

  # TODO: workaround for twi.h
  set(LIB_FILE "${ARDUINO_LIB_DIR}/Wire${ARDUINO_LIB_PATH_SRC}/utility/twi.c")
  set(ARDUINO_LIB_FILES ${ARDUINO_LIB_FILES} ${LIB_FILE})
  include_directories("${ARDUINO_LIB_DIR}/Wire${ARDUINO_LIB_PATH_SRC}/utility")

else()
  message(SEND_ERROR "Could not find Arduino library dir.")
endif()
set(LIBRARY_FILES ${LIBRARY_FILES} ${ARDUINO_LIB_FILES})
