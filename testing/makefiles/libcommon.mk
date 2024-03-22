mkfile_dir      := $(dir $(lastword ${MAKEFILE_LIST}))
top_dir         := $(abspath $(mkfile_dir)../..)
shared_mk := $(mkfile_dir)/shared.mk

pathsafe_fqbn := $(subst :,_,${FQBN})


build_dir := ${top_dir}/_build/$(pathsafe_fqbn)

LIB_DIR := ${build_dir}/lib
OBJ_DIR := ${build_dir}/obj

CXX_FILES := $(sort $(wildcard ${top_dir}/testing/*.cpp))
H_FILES		:= $(sort $(wildcard ${top_dir}/testing/*.h))
BARE_CXX_FILES := $(foreach path,${CXX_FILES},$(notdir ${path}))
OBJ_FILES	:= $(patsubst %.cpp,${OBJ_DIR}/%.o,$(BARE_CXX_FILES))
LIB_FILE	:= libcommon.a

ifneq ($(KALEIDOSCOPE_CCACHE),)
COMPILER_WRAPPER := ccache
endif


.PHONY: all

DEFAULT_GOAL: all

all: ${OBJ_FILES} ${LIB_DIR}/${LIB_FILE}

${LIB_DIR}/${LIB_FILE}: ${OBJ_FILES}
	-$(QUIET) install -d "${LIB_DIR}"
	$(QUIET) $(call _arduino_prop,compiler.ar.cmd) $(call _arduino_prop,compiler.ar.flags) "${LIB_DIR}/${LIB_FILE}" ${OBJ_FILES}

${OBJ_DIR}/%.o: ${top_dir}/testing/%.cpp ${H_FILES}
	$(info compile $@)
	-$(QUIET) install -d "${OBJ_DIR}"
	$(QUIET) $(COMPILER_WRAPPER) $(call _arduino_prop,compiler.cpp.cmd) -o "$@" -c -std=c++14 ${shared_includes} ${shared_defines} $<

clean:
	$(QUIET) rm -rf -- "${build_dir}"

include $(top_dir)/etc/makefiles/arduino-cli.mk
include $(shared_mk)
