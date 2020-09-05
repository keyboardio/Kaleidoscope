COMMON_LIB_DIR ?= ${PWD}/lib
OBJ_DIR ?= ${PWD}/obj

CXX_FILES=$(wildcard *.cpp)
LIB_FILE=libcommon.a
OBJ_FILES=$(patsubst %.cpp,${OBJ_DIR}/%.o,$(CXX_FILES))

all: ${OBJ_FILES} ${COMMON_LIB_DIR}/${LIB_FILE}
	@:

${COMMON_LIB_DIR}/${LIB_FILE}: ${OBJ_FILES}
	mkdir -p "${COMMON_LIB_DIR}"
	ar rcs "${COMMON_LIB_DIR}/${LIB_FILE}" ${OBJ_FILES}

${OBJ_DIR}/%.o: %.cpp $(wildcard *.h)
	@echo "compile $@"
	mkdir -p "${OBJ_DIR}"
	g++ -o "$@" -c \
		-I${PWD}/../.. \
		-I${PWD}/../../src \
		-I${PWD}/../../../../../virtual/cores/arduino \
		-I${PWD}/../../../Kaleidoscope-HIDAdaptor-KeyboardioHID/src \
		-I${PWD}/../../../KeyboardioHID/src \
		-I${PWD}/../../testing/googletest/googlemock/include \
		-I${PWD}/../../testing/googletest/googletest/include \
		-DARDUINO=10607 \
		-DARDUINO_ARCH_VIRTUAL \
		-DARDUINO_AVR_MODEL01 \
		'-DKALEIDOSCOPE_HARDWARE_H="Kaleidoscope-Hardware-Model01.h"' \
		-DKALEIDOSCOPE_VIRTUAL_BUILD=1 \
		-DKEYBOARDIOHID_BUILD_WITHOUT_HID=1 \
		-DUSBCON=dummy \
		-DARDUINO_ARCH_AVR=1 \
		'-DUSB_PRODUCT="Model 01"' \
		$<

clean:
	rm -rf "${COMMON_LIB_DIR}" "${OBJ_DIR}"
