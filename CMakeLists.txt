# I don't know what minim is actually required.
# Setting this to the version I'm using while
# writing these files. We can figure out which
# versions we want to support later. -- epan
cmake_minimum_required(VERSION 3.18.2)

project(kaleidoscope NONE)

set(KALEIDOSCOPE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
set(VIRTUAL_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../virtual")

add_subdirectory( src )
