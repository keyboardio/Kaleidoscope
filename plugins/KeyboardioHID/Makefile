# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

#BOARD_TAG    = keyboardio
BOARD    = model01
MCU 		= atmega32u4

ARDUINO_LIBS = 

ARDUINO_PATH=/Applications/Arduino.app/Contents/Java/
ARDUINO_TOOLS_PATH=$(ARDUINO_PATH)/hardware/tools
FQBN=arduino:avr:leonardo
BUILD_PATH := $(shell mktemp -d 2>/dev/null || mktemp -d -t 'build')
ARDUINO_LOCAL_LIB_PATH=~/Documents/Arduino/libraries
ARDUINO_IDE_VERSION=100607
VERBOSE := #-verbose
SKETCH_BASENAME = `basename "${SKETCH}"`

smoke: compile size

astyle:
		find . -type f -name \*.cpp |xargs -n 1 astyle --style=google
		find . -type f -name \*.ino |xargs -n 1 astyle --style=google
		find . -type f -name \*.h |xargs -n 1 astyle --style=google


compile:
	$(ARDUINO_PATH)/arduino-builder \
		-hardware $(ARDUINO_PATH)/hardware \
		-tools $(ARDUINO_TOOLS_PATH) \
		-tools $(ARDUINO_PATH)/tools-builder  \
		-fqbn $(FQBN) \
		-libraries $(ARDUINO_LOCAL_LIB_PATH) \
		$(VERBOSE) \
		-build-path $(BUILD_PATH) \
		-ide-version $(ARDUINO_IDE_VERSION) \
		$(SKETCH)

size: compile
	$(ARDUINO_TOOLS_PATH)/avr/bin/avr-size -C --mcu=$(MCU) $(BUILD_PATH)/$(SKETCH_BASENAME).elf
