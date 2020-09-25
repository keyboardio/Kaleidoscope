build_dir := ${top_dir}/_build/${testcase}

LIB_DIR := ${build_dir}/lib
OBJ_DIR := ${build_dir}/obj
BIN_DIR	:= ${build_dir}/bin

COMMON_LIB_DIR	:= ${top_dir}/_build/lib

SRC_DIR	:= test

SKETCH_FILE=$(wildcard *.ino)
BIN_FILE=$(subst .ino,,$(SKETCH_FILE))
LIB_FILE=${BIN_FILE}-latest.a

TEST_FILES=$(wildcard $(SRC_DIR)/*.cpp)
TEST_OBJS=$(patsubst $(SRC_DIR)/%.cpp,${OBJ_DIR}/%.o,$(TEST_FILES))

ifndef BOARD_HARDWARE_PATH
bundle_path = ${top_dir}/..
else
bundle_path = ${BOARD_HARDWARE_PATH}/keyboardio/avr/libraries
endif

all: run

run: ${BIN_DIR}/${BIN_FILE}
	@echo "run"
	"${BIN_DIR}/${BIN_FILE}" -t -q

${BIN_DIR}/${BIN_FILE}: ${TEST_OBJS}
	@echo "link"
	install -d "${BIN_DIR}" "${LIB_DIR}"
	env LIBONLY=yes \
		  LOCAL_CFLAGS='"-I$(shell pwd)"' \
		  OUTPUT_PATH="${LIB_DIR}" \
			VERBOSE=1 \
			ARCH=virtual DEFAULT_SKETCH=sketch \
			BOARD_HARDWARE_PATH="${BOARD_HARDWARE_PATH}" \
		$(MAKE) -f ${top_dir}/testing/makefiles/delegate.mk
	g++ -o "${BIN_DIR}/${BIN_FILE}" \
		-lpthread \
		-g \
		-w \
		${TEST_OBJS} \
		-L"${COMMON_LIB_DIR}" \
		-lcommon \
		"${LIB_DIR}/${LIB_FILE}" \
		-L"${top_dir}/testing/googletest/build/lib" \
		-lgtest \
		-lgmock \
		-lpthread \
		-lm

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@echo "compile $@"
	install -d "${OBJ_DIR}"
	g++ -o "$@" -c \
	  -std=c++14 \
		-I${top_dir} \
		-I${top_dir}/src \
		-I${bundle_path}/../../virtual/cores/arduino \
		-I${bundle_path}/Kaleidoscope-HIDAdaptor-KeyboardioHID/src \
		-I${bundle_path}/KeyboardioHID/src \
		-I${top_dir}/testing/googletest/googlemock/include \
		-I${top_dir}/testing/googletest/googletest/include \
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
	rm -rf "${build_dir}"

.PHONY: clean run all
