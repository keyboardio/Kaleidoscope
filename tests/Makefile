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

tests_dir	:= $(abspath $(dir $(lastword ${MAKEFILE_LIST})))

top_dir		:= $(abspath $(tests_dir)/..)

build_dir 	:= ${top_dir}/_build

LIB_DIR		:= ${build_dir}/lib

libcommon_a	:= ${top_dir}/_build/lib/libcommon.a

TEST_PATH 	?= ./

export FQBN	?= keyboardio:virtual:model01

TESTS		:= $(shell cd $(tests_dir); find ${TEST_PATH} -name '*.ino' -exec dirname {} \;)

# The clutter up the test output on Make 4.0 and newer
MAKEFLAGS += --no-print-directory

include $(top_dir)/etc/makefiles/arduino-cli.mk

.DEFAULT_GOAL := all

# If we start off in tests to run make all, the sketch makefiles guess the wrong location for
# Kaleidoscope's makefiles
KALEIDOSCOPE_ETC_DIR ?= $(top_dir)/etc

.PHONY: all
all: ${libcommon_a} googletest ${TESTS} 
	@:

.PHONY: cmake-clean
cmake-clean:
	rm -rf "${top_dir}"/testing/googletest/build/*

run-all: ${TESTS}
	@for test in ${TESTS}; do \
		${MAKE} -s -f ${top_dir}/testing/makefiles/testcase.mk \
			-C $${test} \
			testcase=$${test} run || ERROR=$$?; \
	done; \
	if [ -n $${ERROR} ]; then exit $${ERROR}; fi

.PHONY: clean
clean: cmake-clean
	@for test in ${TESTS}; do \
		${MAKE} -s -f ${top_dir}/testing/makefiles/testcase.mk \
			-C $${test} \
			testcase=$${test} clean; \
	done
	rm -rf "${build_dir}"/*

.PHONY: googletest
googletest: ${top_dir}/testing/googletest/build/Makefile
	${MAKE} -C ${top_dir}/testing/googletest/build

${top_dir}/testing/googletest/build/Makefile:
	$(info googletest Makefile is being remade)
	# This can fail if we're running in parallel, but that'd be harmless
	-install -d ${top_dir}/testing/googletest/build
	$(QUIET) cmake \
	  -S ${top_dir}/testing/googletest \
	  -B ${top_dir}/testing/googletest/build \
	  -DCMAKE_C_COMPILER=$(call _arduino_prop,compiler.path)$(call _arduino_prop,compiler.c.cmd) \
	  -DCMAKE_CXX_COMPILER=$(call _arduino_prop,compiler.path)$(call _arduino_prop,compiler.cpp.cmd) \
	  -DCMAKE_C_COMPILER_LAUNCHER=ccache \
	  -DCMAKE_CXX_COMPILER_LAUNCHER=ccache

${libcommon_a}:
	$(QUIET) ${MAKE} -f ${top_dir}/testing/makefiles/libcommon.mk -C ${top_dir}/testing

Makefile:
	@:

${TESTS}: ${libcommon_a} googletest
	$(QUIET) ${MAKE} -f ${top_dir}/testing/makefiles/testcase.mk -C $@ testcase=$@ build run
