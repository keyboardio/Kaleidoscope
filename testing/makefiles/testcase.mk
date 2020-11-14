top_dir         := $(dir $(lastword ${MAKEFILE_LIST}))../..


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

# If we have a ktest file and no generated testcase, 
# we want to turn it into a generated testcase
# and add it to the list of possible test files

ifneq (,$(wildcard test.ktest))
HAS_KTEST_FILE=1
ifeq (,$(findstring $(SRC_DIR)/generated-testcase.cpp, $(TEST_FILES)))
TEST_FILES += $(SRC_DIR)/generated-testcase.cpp
endif
endif


TEST_OBJS=$(patsubst $(SRC_DIR)/%.cpp,${OBJ_DIR}/%.o,$(TEST_FILES))

ifndef BOARD_HARDWARE_PATH
bundle_path = ${top_dir}/..
else
bundle_path = ${BOARD_HARDWARE_PATH}/keyboardio/avr/libraries
endif

build: ${BIN_DIR}/${BIN_FILE} compile-sketch

all: run

run: ${BIN_DIR}/${BIN_FILE}
	@echo "run"
	"${BIN_DIR}/${BIN_FILE}" -t -q

${BIN_DIR}/${BIN_FILE}: ${TEST_OBJS} 

# We force sketch recompiliation because otherwise, make won't pick up changes to...anything on the arduino side
compile-sketch:
	@echo "link"
	install -d "${BIN_DIR}" "${LIB_DIR}"
	env LIBONLY=yes \
		  LOCAL_CFLAGS='"-I$(shell pwd)"' \
		  OUTPUT_PATH="${LIB_DIR}" \
			VERBOSE=${VERBOSE} \
			ARCH=virtual DEFAULT_SKETCH=sketch \
		$(MAKE) -f ${top_dir}/testing/makefiles/delegate.mk compile
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


# If we have a test.ktest file, it should be processed into a c++ testcase

generate-testcase: $(if $(HAS_KTEST_FILE), ${SRC_DIR}/generated-testcase.cpp)


${SRC_DIR}/generated-testcase.cpp: test.ktest
ifneq (,$(wildcard test.ktest))
	@echo "Compiling ${testcase} ktest script into ${SRC_DIR}/generated-testcase.cpp"
	install -d "${SRC_DIR}"
	perl ${top_dir}/testing/bin/ktest-to-cxx \
		--ktest=test.ktest \
		--cxx=${SRC_DIR}/generated-testcase.cpp
endif

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@echo "compile $@"
	install -d "${OBJ_DIR}"
	g++ -o "$@" -c \
	  -std=c++14 \
		-I${top_dir} \
		-I${top_dir}/src \
		-I${bundle_path}/../../virtual/cores/arduino \
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
	rm -f "${SRC_DIR}/generated-testcase.cpp"
	rm -rf "${build_dir}"

.PHONY: clean run all build
