set(ARDUINO_MCU "atmega2560")
set(AVRDUDE_MCU "m2560")
set(ARDUINO_FCPU "16000000")
set(ARDUINO_UPLOAD_SPEED "115200")

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER ${ARDUINO_TOOLS_DIR}/avr-gcc)
set(CMAKE_CXX_COMPILER ${ARDUINO_TOOLS_DIR}/avr-g++)
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
# enable_language(ASM)

set(SPEC_FLAGS "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums")

# TODO: why do we have to define ARDUINO manually??
# set(CMAKE_CXX_FLAGS "-mmcu=${ARDUINO_MCU} -DF_CPU=${ARDUINO_FCPU} -Os")
set(CMAKE_CXX_FLAGS "-mmcu=${ARDUINO_MCU} -DF_CPU=${ARDUINO_FCPU} -DARDUINO=103 -Os")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} ${SPEC_FLAGS} -w -std=gnu99")

set(AVRDUDE_CONFIG "$ENV{ARDUINO_ROOT}/hardware/tools/avr/etc/avrdude.conf")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

find_program(AVROBJCOPY
  avr-objcopy
  HINTS ${ARDUINO_TOOLS_DIR}
)
find_program(AVRDUDE
  avrdude
  HINTS ${ARDUINO_TOOLS_DIR}
)

# FIXME: Forcing target name to be "firmware"
if(AVROBJCOPY AND AVRDUDE)
  add_custom_target(hex)
  add_dependencies(hex firmware)

  add_custom_command(TARGET hex POST_BUILD
    COMMAND ${AVROBJCOPY} -O ihex -R .eeprom ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firmware ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firmware.hex
  )

  add_custom_target(flash)
  add_dependencies(flash hex)

  add_custom_command(TARGET flash POST_BUILD
    COMMAND ${AVRDUDE} -C${AVRDUDE_CONFIG} -p${ARDUINO_MCU} -c${ARDUINO_PROTOCOL} -P${ARDUINO_PORT} -b${ARDUINO_UPLOAD_SPEED} -D -Uflash:w:${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firmware.hex:i
  )
else()
  message(SEND_ERROR "Couldn't find AVROBJCOPY and AVRDUDE!")
endif()
