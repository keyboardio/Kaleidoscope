LIB_DIR ?= ${PWD}/lib
OBJ_DIR ?= ${PWD}/obj


CXX_FILES=$(wildcard *.cpp)
LIB_FILE=common.a
OBJ_FILES=$(patsubst %.cpp,${OBJ_DIR}/%.o,$(CXX_FILES))

Makefile: ${LIB_DIR}/${LIB_FILE}
	@:

${LIB_DIR}/${LIB_FILE}: ${OBJ_FILES}
	mkdir -p "${LIB_DIR}"
	ar rcs "${LIB_DIR}/${LIB_FILE}" ${OBJ_FILES}

${OBJ_DIR}/%.o: %.cpp
	@echo "compile $@"
	mkdir -p "${OBJ_DIR}"
	g++ -o "$@" -c \
		-I../.. \
		-I../../src \
		-I../../../../../virtual/cores/arduino \
		-I../../../Kaleidoscope-HIDAdaptor-KeyboardioHID/src \
		-I../../../KeyboardioHID/src \
		-I../../testing/googletest/googlemock/include \
		-I../../testing/googletest/googletest/include \
		-DARDUINO=10607 \
		-DARDUINO_AVR_MODEL01 \
		'-DKALEIDOSCOPE_HARDWARE_H="Kaleidoscope-Hardware-Model01.h"' \
		-DKALEIDOSCOPE_VIRTUAL_BUILD=1 \
		-DKEYBOARDIOHID_BUILD_WITHOUT_HID=1 \
		-DUSBCON=dummy \
		-DARDUINO_ARCH_AVR=1 \
		'-DUSB_PRODUCT="Model 01"' \
		$<
