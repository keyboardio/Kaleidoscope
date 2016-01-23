# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

#BOARD_TAG    = keyboardio
BOARD    = model01
MCU 		= atmega32u4

DEVICE_PORT =  /dev/cu.usbmodemHIDO1
DEVICE_PORT_BOOTLOADER = /dev/cu.usbmodem1421
ARDUINO_LIBS = 
GIT_VERSION := $(shell git describe --abbrev=4 --dirty --always)

ARDUINO_PATH=/Applications/Arduino.app/Contents/Java/
ARDUINO_TOOLS_PATH=$(ARDUINO_PATH)/hardware/tools
FQBN=arduino:avr:leonardo
BUILD_PATH := $(shell mktemp -d 2>/dev/null || mktemp -d -t 'build')
OUTPUT_PATH=./output
ARDUINO_LOCAL_LIB_PATH=~/Documents/Arduino/libraries
ARDUINO_IDE_VERSION=100607

SKETCH=KeyboardioFirmware.ino



OUTPUT_FILE_PREFIX=$(SKETCH)-$(GIT_VERSION)





astyle:
		find . -type f -name \*.cpp |xargs -n 1 astyle --style=google
		find . -type f -name \*.ino |xargs -n 1 astyle --style=google
		find . -type f -name \*.h |xargs -n 1 astyle --style=google

generate-keymaps:
	-rm keymaps_generated.h
	cd layouts && ( find . -type f |xargs -n 1 -I % sh -c 'perl ../tools/generate_keymaps.pl < % >> ../keymaps_generated.h' )
	cat keymaps_h-template >> keymaps_generated.h

dirs:
	mkdir -p $(OUTPUT_PATH)

build: compile size

compile: dirs
	$(ARDUINO_PATH)/arduino-builder \
		-hardware $(ARDUINO_PATH)/hardware \
		-tools $(ARDUINO_TOOLS_PATH) \
		-tools $(ARDUINO_PATH)/tools-builder  \
		-fqbn $(FQBN) \
		-libraries $(ARDUINO_LOCAL_LIB_PATH) \
		-verbose \
		-build-path $(BUILD_PATH) \
		-ide-version $(ARDUINO_IDE_VERSION) \
		$(SKETCH)
	cp $(BUILD_PATH)/$(SKETCH).hex $(OUTPUT_PATH)/$(OUTPUT_FILE_PREFIX).hex
	cp $(BUILD_PATH)/$(SKETCH).elf $(OUTPUT_PATH)/$(OUTPUT_FILE_PREFIX).elf

size:
	avr-size -C --mcu=$(MCU) $(OUTPUT_PATH)/$(OUTPUT_FILE_PREFIX).elf

reset-device: 
	stty -f $(DEVICE_PORT) 1200 ;

flash: compile reset-device
	sleep 3
	$(ARDUINO_TOOLS_PATH)/avr/bin/avrdude -C/Applications/Arduino.app/Contents/Java/hardware/tools/avr/etc/avrdude.conf -v -p$(MCU) -cavr109 -P$(DEVICE_PORT_BOOTLOADER) -b57600 -D -Uflash:w:$(HEXFILE_PATH):i 

