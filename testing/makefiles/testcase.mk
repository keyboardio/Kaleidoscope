# Reset a bunch of historical GNU make implicit rules that we never
# use, but which have a disastrous impact on performance
#
# --no-builtin-rules in MAKEFLAGS apparently came in with GNU Make 4,
# which is newer than what Apple ships
MAKEFLAGS += --no-builtin-rules

# These lines reset the implicit rules we really care about
%:: %,v

%:: RCS/%,v

%:: RCS/%

%:: s.%

%:: SCCS/s.%

.SUFFIXES:


mkfile_dir 	:= $(dir $(lastword ${MAKEFILE_LIST}))
top_dir         := $(abspath $(mkfile_dir)../..)


# need to set this before we get the FQBN
SKETCH_FILE	:= $(wildcard *.ino)
FQBN     	= $(shell cat sketch.yaml|grep default_fqbn | cut -d " " -f 2)

pathsafe_fqbn   := $(subst :,_,${FQBN})

build_root       := ${top_dir}/_build/$(pathsafe_fqbn)


export KALEIDOSCOPE_TEMP_PATH := ${build_root}/kaleidoscope

include_plugins_dir := -I${top_dir}/plugins \

build_dir := ${build_root}/${testcase}

LIB_DIR := ${build_dir}/lib
OBJ_DIR := ${build_dir}/obj
BIN_DIR	:= ${build_dir}/bin

COMMON_LIB_DIR	:= ${build_root}/lib
libcommon_a     := ${COMMON_LIB_DIR}/libcommon.a

shared_mk := $(mkfile_dir)/shared.mk
include $(top_dir)/etc/makefiles/arduino-cli.mk
include $(shared_mk)

ifneq ($(KALEIDOSCOPE_CCACHE),)
COMPILER_WRAPPER := ccache
endif


SRC_DIR	:= test

BIN_FILE=$(subst .ino,,$(SKETCH_FILE))
LIB_FILE=${BIN_FILE}-latest.a

# Immediate assignment prevents duplicates after append from HAS_KTEST_FILE
TEST_FILES:=$(sort $(wildcard $(SRC_DIR)/*.cpp))

# If we have a ktest file and no generated testcase, 
# we want to turn it into a generated testcase
# and add it to the list of possible test files

ifneq (,$(wildcard test.ktest))
HAS_KTEST_FILE=1
ifeq (,$(findstring $(SRC_DIR)/generated-testcase.cpp, $(TEST_FILES)))
TEST_FILES += $(SRC_DIR)/generated-testcase.cpp
endif
endif

.DEFAULT_GOAL := build

TEST_OBJS=$(patsubst $(SRC_DIR)/%.cpp,${OBJ_DIR}/%.o,$(TEST_FILES))

build: $(if $(HAS_KTEST_FILE), generate-testcase) compile-sketch

all: run

run: ${BIN_DIR}/${BIN_FILE}
	$(info )
	$(info Running test $(testcase))
	$(QUIET) "${BIN_DIR}/${BIN_FILE}" -t -q

${BIN_DIR}/${BIN_FILE}: compile-sketch

# We force sketch recompiliation because otherwise, make won't pick up changes to...anything on the arduino side
.PHONY: compile-sketch
compile-sketch: ${libcommon_a} ${TEST_OBJS}
	-@install -d "${BIN_DIR}" "${LIB_DIR}"
	$(QUIET) env LIBONLY=yes VERBOSE=${VERBOSE}  \
		OUTPUT_PATH="${LIB_DIR}" \
		_ARDUINO_CLI_COMPILE_CUSTOM_FLAGS='--build-property upload.maximum_size=""' \
		$(MAKE) -f ${top_dir}/etc/makefiles/sketch.mk compile
	$(QUIET) $(COMPILER_WRAPPER) $(call _arduino_prop,compiler.cpp.cmd) $(call _arduino_prop,compiler.cpp.elf.flags) -o "${BIN_DIR}/${BIN_FILE}" \
		-lpthread -g -w ${TEST_OBJS} \
		-L"${COMMON_LIB_DIR}" -lcommon \
		"${LIB_DIR}/${LIB_FILE}" \
		-L"${top_dir}/testing/googletest/build/lib" \
		-lgtest -lgmock -lpthread -lm

${libcommon_a}:
	$(QUIET) ${MAKE} -f ${top_dir}/testing/makefiles/libcommon.mk -C ${top_dir}/testing


# If we have a test.ktest file, it should be processed into a c++ testcase
.PHONY: generate-testcase
generate-testcase: $(if $(HAS_KTEST_FILE), ${SRC_DIR}/generated-testcase.cpp)

${SRC_DIR}/generated-testcase.cpp: test.ktest
ifneq (,$(wildcard test.ktest))
ifdef VERBOSE
	$(QUIET) $(info Compiling ${testcase} ktest script into ${SRC_DIR}/generated-testcase.cpp)
endif
	-$(QUIET) install -d "${SRC_DIR}"
	$(QUIET) perl ${top_dir}/testing/bin/ktest-to-cxx \
		--ktest=test.ktest \
		--cxx=${SRC_DIR}/generated-testcase.cpp
endif

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	-$(QUIET) install -d "${OBJ_DIR}"
	$(QUIET) $(COMPILER_WRAPPER) $(call _arduino_prop,compiler.cpp.cmd) -o "$@" -c -std=c++14 \
		${shared_includes} ${include_plugins_dir} ${shared_defines} $<

clean:
	$(QUIET) rm -f -- "${SRC_DIR}/generated-testcase.cpp"
	$(QUIET) rm -rf -- "${build_dir}"

.PHONY: clean run all build
