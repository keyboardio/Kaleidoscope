# This stub makefile for a Kaleidoscope plugin pulls in 
# all targets from the Kaleidoscope-Plugin library

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
SKETCHBOOK_DIR ?= $(HOME)/Documents/Arduino
PACKAGE_DIR ?= $(HOME)/Library/Arduino15
else
SKETCHBOOK_DIR ?= $(HOME)/Arduino
PACKAGE_DIR ?= $(HOME)/.arduino15
endif


ARDUINO_INSTALLED_ENV=$(shell ls -dt $(PACKAGE_DIR)/packages/keyboardio/hardware/avr 2>/dev/null |head -n 1)
MANUALLY_INSTALLED_ENV=$(shell ls -dt $(SKETCHBOOK_DIR)/hardware/keyboardio/avr 2>/dev/null |head -n 1)



ifneq ("$(wildcard $(ARDUINO_INSTALLED_ENV)/boards.txt)","")

ifneq ("$(wildcard $(MANUALLY_INSTALLED_ENV)/boards.txt)","")

$(info ***************************************************************************)
$(info It appears that you have installed two copies of Kaleidoscope. One copy was)
$(info installed using Arduino's "Board Manager", while the other was installed by)
$(info hand, probably using "git".)
$(info )
$(info This will likely cause some trouble as you try to build keyboard firmware)
$(info using Kaleidoscope. You may want to remove either: )
$(info )
$(info $(PACKAGE_DIR)/packages/keyboardio/ which was installed using Arduino)
$(info )
$(info or)
$(info )
$(info $(SKETCHBOOK_DIR)/hardware/keyboardio/ which was installed by hand.)
$(info )
$(info ***************************************************************************)
$(info )
	
endif

BOARD_HARDWARE_PATH = $(ARDUINO_INSTALLED_ENV)
KALEIDOSCOPE_PLUGIN_MAKEFILE_DIR ?= build-tools/makefiles/
KALEIDOSCOPE_BUILDER_DIR ?= $(ARDUINO_INSTALLED_ENV)/libraries/Kaleidoscope/bin/



endif


BOARD_HARDWARE_PATH ?= $(SKETCHBOOK_DIR)/hardware
KALEIDOSCOPE_PLUGIN_MAKEFILE_DIR ?= keyboardio/build-tools/makefiles/

# If Kaleidoscope's Arduino libraries cannot be found, e.g. because 
# they reside outside of SKETCHBOOK_DIR, we fall back to assuming that 
# the hardware directory can be determined in relation to the position of 
# this Makefile.
ifeq ("$(wildcard $(BOARD_HARDWARE_PATH)/keyboardio/build-tools/makefiles/rules.mk)","")
   # Determine the path of this Makefile
   MKFILE_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
   BOARD_HARDWARE_PATH = $(MKFILE_DIR)/../../../..
endif

ifeq ("$(wildcard $(BOARD_HARDWARE_PATH)/keyboardio/build-tools/makefiles/rules.mk)","")
$(info ***************************************************************************)
$(info Unable to autodetect a proper BOARD_HARDWARE_PATH. Please define it manually.)
$(info ***************************************************************************)
$(info )
endif

include $(BOARD_HARDWARE_PATH)/$(KALEIDOSCOPE_PLUGIN_MAKEFILE_DIR)/rules.mk
