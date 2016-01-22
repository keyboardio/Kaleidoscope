# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

#BOARD_TAG    = keyboardio
BOARD    = model01
PORT =  /dev/cu.usbmodem1421
ARDUINO_LIBS = 
GIT_VERSION := $(shell git describe --abbrev=4 --dirty --always)

ARDUINO_PATH=/Applications/Arduino.app/Contents/Java/

FQBN=arduino:avr:leonardo
BUILD_PATH=./build
ABS_BUILD_PATH=`cd "$(BUILD_PATH)"; pwd` 

ARDUINO_LOCAL_LIB_PATH=~/Documents/Arduino/libraries
ARDUINO_IDE_VERSION=100607

SKETCH=KeyboardioFirmware.ino

astyle:
		find . -type f -name \*.cpp |xargs -n 1 astyle --style=google
		find . -type f -name \*.ino |xargs -n 1 astyle --style=google
		find . -type f -name \*.h |xargs -n 1 astyle --style=google

generate-keymaps:
	-rm keymaps_generated.h
	cd layouts && ( find . -type f |xargs -n 1 -I % sh -c 'perl ../tools/generate_keymaps.pl < % >> ../keymaps_generated.h' )
	cat keymaps_h-template >> keymaps_generated.h

compile:
	-mkdir $(ABS_BUILD_PATH)
	$(ARDUINO_PATH)/arduino-builder \
		-hardware $(ARDUINO_PATH)/hardware \
		-tools $(ARDUINO_PATH)/hardware/tools \
		-tools $(ARDUINO_PATH)/tools-builder  \
		-fqbn $(FQBN) \
		-libraries $(ARDUINO_LOCAL_LIB_PATH) \
		-verbose \
		-build-path $(ABS_BUILD_PATH) \
		-ide-version $(ARDUINO_IDE_VERSION) \
		$(SKETCH)

clean:
	rm -rf ./build/* # This should be BUILD_PATH, but without a decent guard, unsetting BUILD_PATH could wipe out /
