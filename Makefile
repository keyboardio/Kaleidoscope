EXAMPLES   := $(shell find examples -type f -name '*.ino' -exec dirname {} \; | sort)
DESTDIR    := $(abspath ${CURDIR})/
export DESTDIR

BOARD_HARDWARE_PATH ?= ../../../../
BUILD_TOOLS_PATH     = ${BOARD_HARDWARE_PATH}/keyboardio/build-tools

include ${BUILD_TOOLS_PATH}/makefiles/travis.mk
include ${BUILD_TOOLS_PATH}/makefiles/tests.mk
