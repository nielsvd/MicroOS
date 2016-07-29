# define and include header files of components library
set(COMPONENTS_LIB_FILES
  AF_motorshield
  analog_sensor
  bournsEMS22A
  bournsEMS22D
  component_interface
  difference_sensor
  easydriver_stepper
  encoder
  encoder_sensor
  fast_stepper
  hbridge_interface
  hbridge_stepper
  joystick
  led
  pololuMC33926
  pololuMD08A
  rate_limiter
  sensor1D
  sensor3D
  sharp41S
  stepper
  vishay157
)

find_path(COMPONENTS_LIB_DIR
  encoder.h
  HINTS ${CMAKE_SOURCE_DIR}/lib/components
)

if(COMPONENTS_LIB_DIR)
  message(STATUS "Found components library dir: ${COMPONENTS_LIB_DIR}")
  foreach(file ${COMPONENTS_LIB_FILES})
    set(COMP_FILE "${COMPONENTS_LIB_DIR}/${file}.cpp")
    if(EXISTS ${COMP_FILE})
      set(LIBRARY_FILES ${LIBRARY_FILES} ${COMP_FILE})
    endif()
  endforeach()
  include_directories(${COMPONENTS_LIB_DIR})
else()
  message(SEND_ERROR "Could not find components library dir.")
endif()
