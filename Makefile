
# Shamelessly stolen from git's Makefile
uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')


DEVICE_PORT := `ls /dev/ttyACM*`
DEVICE_PORT_BOOTLOADER := `ls /dev/ttyACM*`
ARDUINO_PATH=/usr/local/arduino
ARDUINO_LOCAL_LIB_PATH=$(HOME)/Arduino

MD5 = md5sum

RESET_BOARD=stty -F $(DEVICE_PORT) 1200

ifeq ($(uname_S),Darwin)

# Port locations

DEVICE_PORT :=  `ls /dev/cu.usbmodemHID?? /dev/cu.usbmodem14*`
DEVICE_PORT_BOOTLOADER := `ls /dev/cu.usbmodem14*`

# Tools

ARDUINO_PATH=/Applications/Arduino.app/Contents/Java/
ARDUINO_LOCAL_LIB_PATH=$(HOME)/Documents/Arduino

MD5 = md5

RESET_BOARD=stty -f $(DEVICE_PORT) 1200

endif



ARDUINO_TOOLS_PATH=$(ARDUINO_PATH)/hardware/tools
ARDUINO_BUILDER_PATH=$(ARDUINO_PATH)/arduino-builder
AVRDUDE_PATH=$(ARDUINO_TOOLS_PATH)/avr/bin/avrdude
AVRDUDE_CONF_PATH=$(ARDUINO_TOOLS_PATH)/avr/etc/avrdude.conf
AVR_SIZE_PATH=$(ARDUINO_TOOLS_PATH)/avr/bin/avr-size



#
#
# Device and sketch info
#
#

BOARD    = model01
MCU 		= atmega32u4
FQBN=keyboardio:avr:model01
SKETCH=KeyboardioFirmware.ino
BOOTLOADER_PATH = $(ARDUINO_LOCAL_LIB_PATH)/hardware/keyboardio/avr/bootloaders/caterina/Caterina.hex
VERBOSE= #-verbose

#
#
# Build
#
#

BUILD_PATH := $(shell mktemp -d 2>/dev/null || mktemp -d -t 'build')
OUTPUT_PATH=./output
ARDUINO_IDE_VERSION=100607

#
#
# Output 
#
#

GIT_VERSION := $(shell git describe --abbrev=4 --dirty --always)
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
	-rm src/generated/keymaps.h
	cd layouts && ( find . -type f |xargs -n 1 -I % sh -c 'perl ../tools/generate_keymaps.pl < % >> ../src/generated/keymaps.h' )

dirs:
	mkdir -p $(OUTPUT_PATH)

compile: dirs
	$(ARDUINO_BUILDER_PATH) \
		-hardware $(ARDUINO_PATH)/hardware \
		-hardware $(ARDUINO_LOCAL_LIB_PATH)/hardware \
		-tools $(ARDUINO_TOOLS_PATH) \
		-tools $(ARDUINO_PATH)/tools-builder  \
		-fqbn $(FQBN) \
		-libraries $(ARDUINO_LOCAL_LIB_PATH) \
		-libraries . \
		$(VERBOSE) \
		-build-path $(BUILD_PATH) \
		-ide-version $(ARDUINO_IDE_VERSION) \
		examples/KeyboardioFirmware/$(SKETCH)
	@cp $(BUILD_PATH)/$(SKETCH).hex $(HEX_FILE_PATH)
	@cp $(BUILD_PATH)/$(SKETCH).elf $(ELF_FILE_PATH)
	@echo "Firmware is available at $(HEX_FILE_PATH)"
	@echo "Have fun!\n"

size: compile
	$(AVR_SIZE_PATH) -C --mcu=$(MCU) $(ELF_FILE_PATH)


hex-with-bootloader: compile
	@cat $(HEX_FILE_PATH) | awk '/^:00000001FF/ == 0' > $(HEX_FILE_WITH_BOOTLOADER_PATH)
	@echo "Using $(BOOTLOADER_PATH)"
	@$(MD5) $(BOOTLOADER_PATH)
	@cat $(BOOTLOADER_PATH) >> $(HEX_FILE_WITH_BOOTLOADER_PATH)
	@echo "Combined firmware and bootloader are now at $(HEX_FILE_WITH_BOOTLOADER_PATH)"
	@echo "Make sure you have the bootloader version you expect."
	@echo "\n\nAnd TEST THIS ON REAL HARDWARE BEFORE YOU GIVE IT TO ANYONE\n\n"

reset-device: 
	$(RESET_BOARD)

flash: compile reset-device
	sleep 3
	$(AVRDUDE_PATH) \
		-C$(AVRDUDE_CONF_PATH) \
		-v \
		-p$(MCU) \
		-cavr109 \
		-P$(DEVICE_PORT_BOOTLOADER) \
		-b57600 \
		-D \
		-Uflash:w:$(HEX_FILE_PATH):i 

program:
	$(AVRDUDE_PATH) \
		-C$(AVRDUDE_CONF_PATH) \
		-v \
		-p$(MCU) \
		-cusbtiny \
		-D \
		-B 1 \
		-Uflash:w:$(HEX_FILE_PATH):i 

