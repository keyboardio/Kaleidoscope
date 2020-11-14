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

# Set up an argument for passing to the simulator tests in docker
# but if the var isn't set, don't even pass the definition
# since this messes with downstream makefiles

ifneq ($(TEST_PATH),) 
 TEST_PATH_ARG="TEST_PATH='$(TEST_PATH)'"
endif



clean:
	$(MAKE) -C tests clean
	rm -rf testing/googletest/build/*


prepare-virtual:
	$(MAKE) -C $(BOARD_HARDWARE_PATH)/keyboardio prepare-virtual


simulator-tests: prepare-virtual
	$(MAKE) -C tests all

docker-simulator-tests:
	BOARD_HARDWARE_PATH="$(BOARD_HARDWARE_PATH)" ./bin/run-docker "make simulator-tests ${TEST_PATH_ARG}"

docker-bash:
	BOARD_HARDWARE_PATH="$(BOARD_HARDWARE_PATH)" ./bin/run-docker "bash"

run-tests: prepare-virtual build-gtest-gmock
	$(MAKE) -c tests
	@: # blah

build-gtest-gmock:
	(cd testing/googletest && cmake .)
	$(MAKE) -C testing/googletest

adjust-git-timestamps:
	bin/set-timestamps-from-git

find-filename-conflicts:
	@if [ -d "bin" ]; then \
		bin/find-filename-conflicts; \
	fi

#.PHONY: astyle test cpplint cpplint-noisy shellcheck smoke-examples find-filename-conflicts:

astyle:
	PATH="$(PLUGIN_TEST_BIN_DIR):$(PATH)" $(PLUGIN_TEST_SUPPORT_DIR)/quality/run-astyle

check-astyle: astyle
	PATH="$(PLUGIN_TEST_BIN_DIR):$(PATH)" $(PLUGIN_TEST_SUPPORT_DIR)/quality/astyle-check

cpplint-noisy:
	-$(PLUGIN_TEST_SUPPORT_DIR)/quality/cpplint.py  --filter=-legal/copyright,-build/include,-readability/namespace,-whitespace/line_length,-runtime/references  --recursive --extensions=cpp,h,ino --exclude=$(BOARD_HARDWARE_PATH) src examples


cpplint:
	$(PLUGIN_TEST_SUPPORT_DIR)/quality/cpplint.py  --quiet --filter=-whitespace,-legal/copyright,-build/include,-readability/namespace,-runtime/references  --recursive --extensions=cpp,h,ino src examples


SHELL_FILES = $(shell if [ -d bin ]; then egrep -n -r -l "(env (ba)?sh)|(/bin/(ba)?sh)" bin; fi)

shellcheck:
	@if [ -d "bin" ]; then \
		shellcheck ${SHELL_FILES}; \
	fi


check-docs:
	doxygen $(PLUGIN_TEST_SUPPORT_DIR)/quality/etc/check-docs.conf 2> /dev/null >/dev/null
	python $(PLUGIN_TEST_SUPPORT_DIR)/quality/doxy-coverage.py /tmp/undocced/xml


SMOKE_SKETCHES=$(shell if [ -d ./examples ]; then find ./examples -type f -name \*ino | xargs -n 1 dirname; fi)

smoke-sketches: $(SMOKE_SKETCHES)
	@echo "Smoke-tested all the sketches"

.PHONY: force


$(SMOKE_SKETCHES): force
	@BOARD_HARDWARE_PATH="$(BOARD_HARDWARE_PATH)" $(KALEIDOSCOPE_BUILDER_DIR)/kaleidoscope-builder $@ compile
