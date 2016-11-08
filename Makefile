# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

#BOARD_TAG    = keyboardio
BOARD    = model01
MCU 		= atmega32u4

DEVICE_PORT :=  `ls /dev/cu.usbmodemHID?? /dev/cu.usbmodem14*`
DEVICE_PORT_BOOTLOADER := `ls /dev/cu.usbmodem14*`
ARDUINO_LIBS = 
GIT_VERSION := $(shell git describe --abbrev=4 --dirty --always)

ARDUINO_PATH=/Applications/Arduino.app/Contents/Java/
ARDUINO_TOOLS_PATH=$(ARDUINO_PATH)/hardware/tools
FQBN=keyboardio:avr:model01
BUILD_PATH := $(shell mktemp -d 2>/dev/null || mktemp -d -t 'build')
OUTPUT_PATH=./output
ARDUINO_LOCAL_LIB_PATH=$(HOME)/Documents/Arduino
ARDUINO_IDE_VERSION=100607
VERBOSE= #-verbose
SKETCH=KeyboardioFirmware.ino

AVR_SIZE=$(ARDUINO_TOOLS_PATH)/avr/bin/avr-size


OUTPUT_FILE_PREFIX=$(SKETCH)-$(GIT_VERSION)

HEX_FILE_PATH=$(OUTPUT_PATH)/$(OUTPUT_FILE_PREFIX).hex
ELF_FILE_PATH=$(OUTPUT_PATH)/$(OUTPUT_FILE_PREFIX).elf

HEX_FILE_WITH_BOOTLOADER_PATH=$(OUTPUT_PATH)/$(OUTPUT_FILE_PREFIX)-with-bootloader.hex

# default action for `make` is `build`
build: compile size

astyle:
		find . -type f -name \*.cpp |xargs -n 1 astyle --style=google
		find . -type f -name \*.ino |xargs -n 1 astyle --style=google
		find . -type f -name \*.h |xargs -n 1 astyle --style=google

generate-keymaps:
	-rm generated/keymaps.h
	cd layouts && ( find . -type f |xargs -n 1 -I % sh -c 'perl ../tools/generate_keymaps.pl < % >> ../generated/keymaps.h' )

dirs:
	mkdir -p $(OUTPUT_PATH)

compile: dirs
	$(ARDUINO_PATH)/arduino-builder \
		-hardware $(ARDUINO_PATH)/hardware \
		-hardware $(ARDUINO_LOCAL_LIB_PATH)/hardware \
		-tools $(ARDUINO_TOOLS_PATH) \
		-tools $(ARDUINO_PATH)/tools-builder  \
		-fqbn $(FQBN) \
		-libraries $(ARDUINO_LOCAL_LIB_PATH) \
		-libraries $(ARDUINO_LOCAL_LIB_PATH)/Akela/lib \
		$(VERBOSE) \
		-build-path $(BUILD_PATH) \
		-ide-version $(ARDUINO_IDE_VERSION) \
		$(SKETCH)
	@cp $(BUILD_PATH)/$(SKETCH).hex $(HEX_FILE_PATH)
	@cp $(BUILD_PATH)/$(SKETCH).elf $(ELF_FILE_PATH)
	@echo "Firmware is available at $(HEX_FILE_PATH)"
	@echo "Have fun!\n"

size: compile
	$(AVR_SIZE) -C --mcu=$(MCU) $(ELF_FILE_PATH)


hex-with-bootloader: compile
	@cat $(HEX_FILE_PATH) | awk '/^:00000001FF/ == 0' > $(HEX_FILE_WITH_BOOTLOADER_PATH)
	@echo "Using $(ARDUINO_LOCAL_LIB_PATH)/hardware/keyboardio/avr/bootloaders/caterina/Caterina.hex"
	@md5 $(ARDUINO_LOCAL_LIB_PATH)/hardware/keyboardio/avr/bootloaders/caterina/Caterina.hex
	@cat $(ARDUINO_LOCAL_LIB_PATH)/hardware/keyboardio/avr/bootloaders/caterina/Caterina.hex >> $(HEX_FILE_WITH_BOOTLOADER_PATH)
	@echo "Combined firmware and bootloader are now at $(HEX_FILE_WITH_BOOTLOADER_PATH)"
	@echo "Make sure you have the bootloader version you expect."
	@echo "\n\nAnd TEST THIS ON REAL HARDWARE BEFORE YOU GIVE IT TO ANYONE\n\n"

reset-device: 
	stty -f $(DEVICE_PORT) 1200 ;

flash: compile reset-device
	sleep 3
	$(ARDUINO_TOOLS_PATH)/avr/bin/avrdude \
		-C$(ARDUINO_TOOLS_PATH)/avr/etc/avrdude.conf \
		-v \
		-p$(MCU) \
		-cavr109 \
		-P$(DEVICE_PORT_BOOTLOADER) \
		-b57600 \
		-D \
		-Uflash:w:$(HEX_FILE_PATH):i 

program:
	$(ARDUINO_TOOLS_PATH)/avr/bin/avrdude \
		-C$(ARDUINO_TOOLS_PATH)/avr/etc/avrdude.conf \
		-v \
		-p$(MCU) \
		-cusbtiny \
		-D \
		-B 1 \
		-Uflash:w:$(HEX_FILE_PATH):i 

