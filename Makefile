# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

BOARD_TAG    = micro
MONITOR_PORT = /dev/ttyACM*
ARDUINO_LIBS = 

include build/arduino-mk/Arduino.mk

astyle:
	astyle --style=linux ArduinoKeyboard.ino ArduinoKeyboard.h
