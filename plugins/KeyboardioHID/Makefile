# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

#BOARD_TAG    = keyboardio
BOARD    = model01
MCU 		= atmega32u4

ARDUINO_LIBS = 

ARDUINO_PATH ?= /Applications/Arduino.app/Contents/Java/
ARDUINO_TOOLS_PATH=$(ARDUINO_PATH)/hardware/tools
FQBN=arduino:avr:leonardo
BUILD_PATH := $(shell mktemp -d 2>/dev/null || mktemp -d -t 'build')
ARDUINO_LOCAL_LIB_PATH=~/Documents/Arduino/libraries
ARDUINO_IDE_VERSION=100607
VERBOSE := #-verbose

EXAMPLES=$(shell find ./examples -type f -name \*.ino )
EXAMPLES_HEX := $(addsuffix .hex,${EXAMPLES})


astyle:
	astyle --project --recursive "src/*.cpp,*.h,*.ino"

smoke: ${EXAMPLES_HEX}

${EXAMPLES_HEX}: %.hex: 

	$(ARDUINO_PATH)/arduino-builder \
		-hardware $(ARDUINO_PATH)/hardware \
		-tools $(ARDUINO_TOOLS_PATH) \
		-tools $(ARDUINO_PATH)/tools-builder  \
		-fqbn $(FQBN) \
		-libraries $(ARDUINO_PATH)/libraries \
		-libraries $(ARDUINO_LOCAL_LIB_PATH) \
		$(VERBOSE) \
		-build-path $(BUILD_PATH) \
		-ide-version $(ARDUINO_IDE_VERSION) \
		$*
	$(ARDUINO_TOOLS_PATH)/avr/bin/avr-size -C --mcu=$(MCU) $(BUILD_PATH)/$(shell basename $*).elf
