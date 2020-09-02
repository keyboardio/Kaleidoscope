# I don't know what minim is actually required.
# Setting this to the version I'm using while
# writing these files. We can figure out which
# versions we want to support later. -- epan
cmake_minimum_required(VERSION 3.18.2)

add_library(bitfields bitfields.cpp)
target_include_directories(bitfields
  PRIVATE
  ${KALEIDOSCOPE_SRC}
  "${VIRTUAL_ROOT}/cores/arduino" )

add_library(hooks hooks.cpp)
target_include_directories(hooks
  PRIVATE
  ${KALEIDOSCOPE_SRC}
  "${VIRTUAL_ROOT}/cores/arduino" )

#add_subdirectory(
#  "${KALEIDOSCOPE_SRC}/kaleidoscope_internal"
#  "${kaleidoscope_BINARY_DIR}/src/kaleidoscope_internal" )

add_library(key_events key_events.cpp)
target_include_directories(key_events
  PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}/device"
  "${KALEIDOSCOPE_SRC}/kaleidocope_internal"
  "${VIRTUAL_ROOT}/cores/arduino" )

#add_subdirectory( device )
#add_subdirectory( driver )
#add_subdirectory( plugin )
#add_subdirectory( sim )
#add_subdirectory( simulator )
#add_subdirectory( testing )
#add_subdirectory( testing )
#add_subdirectory( util )
