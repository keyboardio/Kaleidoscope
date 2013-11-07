# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

BOARD_TAG    = micro
MONITOR_PORT = /dev/cu.usbmodem14?1
ARDUINO_LIBS = 

include build/arduino-mk/Arduino.mk
