# define and include header files of MicroOS library
set(MICROOS_LIB_FILES
  channel
  communicator_interface
  hal_base
  mavlink
  microOS
  protocol_interface
  protocol_mavlink
  thread
)

find_path(MICROOS_LIB_DIR
  microOS.h
  HINTS ${CMAKE_SOURCE_DIR}/lib/microOS
)

if(MICROOS_LIB_DIR)
  message(STATUS "Found microOS library dir: ${MICROOS_LIB_DIR}")
  foreach(file ${MICROOS_LIB_FILES})
    set(MICRO_FILE "${MICROOS_LIB_DIR}/${file}.cpp")
    if(EXISTS ${MICRO_FILE})
      set(LIBRARY_FILES ${LIBRARY_FILES} ${MICRO_FILE})
    endif()
  endforeach()
  include_directories(${MICROOS_LIB_DIR})
else()
  message(SEND_ERROR "Could not find microOS library dir.")
endif()
