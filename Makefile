# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

#BOARD_TAG    = keyboardio
BOARD    = symmetric60
PORT =  /dev/cu.usbmodem1421
ARDUINO_LIBS = 
THIRD_PARTY_HARDWARE=hardware/keyboardio/avr
#ARDUINO_CORE_PATH = hardware/keyboardio/cores/keyboardio
#ALTERNATE_CORE = keyboardio
GIT_VERSION := $(shell git describe --abbrev=4 --dirty --always)
include ./_Makefile.Master
include ./blank
astyle:
	astyle --style=linux ArduinoKeyboard.ino *.h

generate-keymaps:
	-rm keymaps_generated.h
	cd layouts && ( find . -type f |xargs -n 1 -I % sh -c 'perl ../tools/generate_keymaps.pl < % >> ../keymaps_generated.h' )
	cat keymaps_h-template >> keymaps_generated.h
