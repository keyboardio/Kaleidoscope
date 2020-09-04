LIB_DIR?=${PWD}/lib
TEST_DIRS=$(dir $(wildcard */*_test.cpp))

Makefile: build-libs ${TEST_DIRS}
	@:

build-libs: googletest common
	@:

googletest: FORCE
	cd googletest && $(MAKE)

common: FORCE
	cd common && env LIB_DIR="${LIB_DIR}" $(MAKE)

clean: FORCE
	rm -rf "${LIB_DIR}"
	cd common && make clean
	cd hello-simulator && make clean
	cd kaleidoscope && make clean

%: common googltest FORCE
	if [ -f "$@/Makefile" ]; then cd "$@"; $(MAKE); fi

.PHONY: FORCE googletest build-libs
