# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

BOARD_TAG    = micro
MONITOR_PORT = /dev/ttyACM0
ARDUINO_LIBS = 

include build/arduino-mk/Arduino.mk
