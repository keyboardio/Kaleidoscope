top_dir		:= $(dir $(lastword ${MAKEFILE_LIST}))../..
build_dir := ${top_dir}/_build

ifndef BOARD_HARDWARE_PATH
bundle_path = ${top_dir}/..
else
bundle_path = ${BOARD_HARDWARE_PATH}/keyboardio/avr/libraries
endif

LIB_DIR := ${build_dir}/lib
OBJ_DIR := ${build_dir}/obj

CXX_FILES := $(wildcard ${top_dir}/testing/*.cpp)
H_FILES		:= $(wildcard ${top_dir}/testing/*.h)
BARE_CXX_FILES := $(foreach path,${CXX_FILES},$(notdir ${path}))
OBJ_FILES	:= $(patsubst %.cpp,${OBJ_DIR}/%.o,$(BARE_CXX_FILES))
LIB_FILE	:= libcommon.a

.PHONY: all

all: ${OBJ_FILES} ${LIB_DIR}/${LIB_FILE}

${LIB_DIR}/${LIB_FILE}: ${OBJ_FILES}
	@install -d "${LIB_DIR}"
	ar rcs "${LIB_DIR}/${LIB_FILE}" ${OBJ_FILES}

${OBJ_DIR}/%.o: ${top_dir}/testing/%.cpp ${H_FILES}
	@echo "compile $@"
	@install -d "${OBJ_DIR}"
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
