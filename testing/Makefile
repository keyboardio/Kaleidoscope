COMMON_LIB_DIR?=${PWD}/lib
TEST_DIRS=$(shell dirname $(wildcard */Makefile))

Makefile: ${TEST_DIRS} FORCE
	@echo TEST_DIRS="${TEST_DIRS}"

googletest: FORCE
	cd googletest && $(MAKE)

common: FORCE
	cd common && env COMMON_LIB_DIR="${COMMON_LIB_DIR}" $(MAKE)

clean:
	rm -rf "${COMMON_LIB_DIR}"
	cd googletest && make clean
	cd common && make clean
	cd hello-simulator && make clean
	cd kaleidoscope && make clean

hello-simulator:
	@:

%: common googletest FORCE
	cd "$@" && $(MAKE)

.PHONY: FORCE googletest build-libs
