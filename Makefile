# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

#BOARD_TAG    = keyboardio
BOARD_TAG    = micro
MONITOR_PORT = /dev/cu.usbmodem*
ARDUINO_LIBS = 
#ARDUINO_CORE_PATH = hardware/keyboardio/cores/keyboardio
#ALTERNATE_CORE = keyboardio
include build/arduino-mk/Arduino.mk

astyle:
	astyle --style=linux ArduinoKeyboard.ino *.h

generate-keymaps:
	-rm keymaps_generated.h
	cd layouts && ( find . -type f |xargs -n 1 -I % sh -c 'perl ../tools/generate_keymaps.pl < % >> ../keymaps_generated.h' )
	cat keymaps_h-template >> keymaps_generated.h
