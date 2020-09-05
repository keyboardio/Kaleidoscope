COMMON_LIB_DIR?=${PWD}/lib
TEST_DIRS=$(shell dirname $(wildcard */Makefile))

DIRS=googletest hello-simulator kaleidoscope common issue_840
CLEANDIRS = $(DIRS:%=clean-%)

.PHONY: subdirs $(CLEANDIRS)

clean: googletest/Makefile $(CLEANDIRS)
	rm -rf "$(COMMON_LIB_DIR)"

$(CLEANDIRS): 
	cd $(@:clean-%=%) && $(MAKE) clean

all: ${TEST_DIRS}
	@echo TEST_DIRS="${TEST_DIRS}"

googletest: googletest/Makefile
	cd googletest && $(MAKE)

googletest/Makefile:
	cd googletest && cmake .

common: 
	cd common && COMMON_LIB_DIR="${COMMON_LIB_DIR}" $(MAKE)

hello-simulator:
	@:

Makefile:
	@:

%: common googletest 
	cd "$@" && $(MAKE)

.PHONY: googletest build-libs common
