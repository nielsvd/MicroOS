# define needed Arduino libraries
set(ARDUINO_LIB_NAMES
  Wire
  SPI
  EEPROM
  SoftwareSerial
)


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
    ${ARDUINO_CORE_DIR}/HID.cpp
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
  Wire/Wire.h
  HINTS $ENV{ARDUINO_ROOT}/hardware/arduino/avr/libraries
)

if(ARDUINO_LIB_DIR)
  message(STATUS "Found Arduino library dir: ${ARDUINO_LIB_DIR}")
  foreach(subdir ${ARDUINO_LIB_NAMES})
    set(LIB_FILE "${ARDUINO_LIB_DIR}/${subdir}/${subdir}.cpp")
    if(EXISTS ${LIB_FILE})
      set(ARDUINO_LIB_FILES ${ARDUINO_LIB_FILES} ${LIB_FILE})
      include_directories("${ARDUINO_LIB_DIR}/${subdir}")
    else()
      message(SEND_ERROR "Could not find header file of Arduino library: ${subdir}")
    endif()
  endforeach()

  # TODO: workaround for twi.h
  set(LIB_FILE "${ARDUINO_LIB_DIR}/Wire/utility/twi.c")
  set(ARDUINO_LIB_FILES ${ARDUINO_LIB_FILES} ${LIB_FILE})
  include_directories("${ARDUINO_LIB_DIR}/Wire/utility")

else()
  message(SEND_ERROR "Could not find Arduino library dir.")
endif()
set(LIBRARY_FILES ${LIBRARY_FILES} ${ARDUINO_LIB_FILES})
