BIN_DIR=bin
LIB_DIR=lib
OBJ_DIR=obj

SKETCH_FILE=$(wildcard *.ino)
BIN_FILE=$(subst .ino,,$(SKETCH_FILE))
LIB_FILE=${BIN_FILE}-latest.a

TEST_FILES=$(wildcard *_test.cpp)
TEST_OBJS=$(patsubst %.cpp,${OBJ_DIR}/%.o,$(TEST_FILES))

run: ${BIN_DIR}/${BIN_FILE}
	@echo "run"
	"./${BIN_DIR}/${BIN_FILE}" -t -q

${BIN_DIR}/${BIN_FILE}: ${TEST_OBJS} FORCE
	@echo "link"
	mkdir -p "${BIN_DIR}" "${LIB_DIR}"
	env LIBONLY=yes LOCAL_CFLAGS='"-I$(PWD)"' OUTPUT_PATH="$(PWD)/$(LIB_DIR)" VERBOSE=1 $(MAKE) -f delegate.mk
	g++ -o "${BIN_DIR}/${BIN_FILE}" -lpthread -g -w -lm -lXtst -lX11 ${TEST_OBJS} "${LIB_DIR}/${LIB_FILE}" -L"$(PWD)/../googletest/lib" -lgtest -lgmock

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
		-DARDUINO_ARCH_VIRTUAL \
		-DARDUINO_AVR_MODEL01 \
		'-DKALEIDOSCOPE_HARDWARE_H="Kaleidoscope-Hardware-Model01.h"' \
		-DKALEIDOSCOPE_VIRTUAL_BUILD=1 \
		-DKEYBOARDIOHID_BUILD_WITHOUT_HID=1 \
		-DUSBCON=dummy \
		-DARDUINO_ARCH_AVR=1 \
		'-DUSB_PRODUCT="Model 01"' \
		$<

clean: FORCE
	rm -rf "${BIN_DIR}" "${LIB_DIR}" "${OBJ_DIR}"

.PHONY: FORCE

