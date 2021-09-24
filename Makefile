include $(dir $(abspath $(lastword $(MAKEFILE_LIST))))/etc/makefiles/arduino-cli.mk

# Set up an argument for passing to the simulator tests in docker
# but if the var isn't set, don't even pass the definition
# since this messes with downstream makefiles

ifneq ($(TEST_PATH),) 
TEST_PATH_ARG="TEST_PATH='$(TEST_PATH)'"
endif


DEFAULT_GOAL: smoke-sketches


setup: $(ARDUINO_CLI_PATH) $(ARDUINO_DIRECTORIES_DATA)/arduino-cli.yaml install-arduino-core-avr install-arduino-core-kaleidoscope $(ARDUINO_DIRECTORIES_USER)/hardware/keyboardio/avr/boards.txt $(ARDUINO_DIRECTORIES_USER)/hardware/keyboardio/virtual/boards.txt 
	@:


checkout-platform: $(ARDUINO_DIRECTORIES_USER)/hardware/keyboardio/avr/boards.txt
	@:

prepare-virtual: $(ARDUINO_DIRECTORIES_USER)/hardware/keyboardio/virtual/boards.txt
	@:

$(ARDUINO_DIRECTORIES_USER)/hardware/keyboardio/virtual/boards.txt:
	$(MAKE) -C $(ARDUINO_DIRECTORIES_USER)/hardware/keyboardio prepare-virtual


$(ARDUINO_DIRECTORIES_USER)/hardware/keyboardio/avr/boards.txt:
	git clone -c core.symlinks=true \
		--recurse-submodules=":(exclude)avr/libraries/Kaleidoscope" \
		--recurse-submodules=":(exclude)gd32/libraries/Kaleidoscope" \
		--recurse-submodules=avr/libraries/ \
		--recurse-submodules=gd32/ \
		--recurse-submodules=gd32/libraries/ \
		git://github.com/keyboardio/Kaleidoscope-Bundle-Keyboardio \
		$(ARDUINO_DIRECTORIES_USER)/hardware/keyboardio
	-rm -d $(ARDUINO_DIRECTORIES_USER)/hardware/keyboardio/avr/libraries/Kaleidoscope
	ln -s $(KALEIDOSCOPE_DIR) $(ARDUINO_DIRECTORIES_USER)/hardware/keyboardio/avr/libraries/Kaleidoscope
	git clone -c core.symlinks=true \
		--recurse-submodules=":(exclude)libraries/Kaleidoscope" \
		--recurse-submodules=libraries/ \
		git://github.com/keyboardio/ArduinoCore-GD32-Keyboardio $(ARDUINO_DIRECTORIES_USER)/hardware/keyboardio/gd32
	-rm -d $(ARDUINO_DIRECTORIES_USER)/hardware/keyboardio/gd32/libraries/Kaleidoscope
	ln -s $(KALEIDOSCOPE_DIR) $(ARDUINO_DIRECTORIES_USER)/hardware/keyboardio/gd32/libraries/Kaleidoscope

simulator-tests:
	$(MAKE) -C tests all

docker-simulator-tests:
	ARDUINO_DIRECTORIES_USER="$(ARDUINO_DIRECTORIES_USER)" ./bin/run-docker "make simulator-tests $(TEST_PATH_ARG)"

docker-clean:
	ARDUINO_DIRECTORIES_USER="$(ARDUINO_DIRECTORIES_USER)" ./bin/run-docker "make clean"

docker-bash:
	ARDUINO_DIRECTORIES_USER="$(ARDUINO_DIRECTORIES_USER)" ./bin/run-docker "bash"

run-tests: $(ARDUINO_DIRECTORIES_USER)/hardware/keyboardio/virtual/boards.txt build-gtest-gmock
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

.PHONY: astyle test cpplint cpplint-noisy shellcheck smoke-examples find-filename-conflicts prepare-virtual checkout-platform adjust-git-timestamps docker-bash docker-simulator-tests run-tests simulator-tests setup

astyle:
	find ./* -type f \( -name  '*.h' -o -name '*.cpp' -o -name '*.ino' \) | grep -v "testing/googletest" | xargs -n 1 astyle --project

check-astyle: astyle
	if ! git diff --exit-code; then \
  	 >&2 echo "'astyle' found code style differences. Please make astyle and commit your changes"; \
	 exit 1; \
	fi; \
	exit 0;

cpplint-noisy:
	-bin/cpplint.py  --filter=-legal/copyright,-build/include,-readability/namespace,-whitespace/line_length,-runtime/references  --recursive --extensions=cpp,h,ino src examples


cpplint:
	bin/cpplint.py  --quiet --filter=-whitespace,-legal/copyright,-build/include,-readability/namespace,-runtime/references  --recursive --extensions=cpp,h,ino src examples


SHELL_FILES = $(shell if [ -d bin ]; then egrep -n -r -l "(env (ba)?sh)|(/bin/(ba)?sh)" bin; fi)

shellcheck:
	@if [ -d "bin" ]; then \
		shellcheck ${SHELL_FILES}; \
	fi


SMOKE_SKETCHES=$(sort $(shell if [ -d ./examples ]; then find ./examples -type f -name \*ino | xargs -n 1 dirname; fi))

smoke-sketches: $(SMOKE_SKETCHES)
	@echo "Smoke-tested all the sketches"

.PHONY: force all clean test

clean: 
	$(MAKE) -C tests clean
	rm -rf -- "testing/googletest/build/*"
	rm -rf -- "_build/*"


$(SMOKE_SKETCHES): force
	$(MAKE) -C $@ -f $(KALEIDOSCOPE_ETC_DIR)/makefiles/sketch.mk compile


build-arduino-nightly-package:
	perl bin/build-arduino-package \
		--kaleidoscope-tag=master \
		--version `date +%Y.%-m.%-d%H%M%S` \
		--index-filename-slug=kaleidoscope_master \
		--only-one-platform-revision \
		--push \
		--output-repo=ssh://git@github.com/keyboardio/arduino-kaleidoscope-master
