
tests_dir	:= $(abspath $(dir $(lastword ${MAKEFILE_LIST})))

top_dir		:= $(abspath $(tests_dir)/..)

build_dir 	:= ${top_dir}/_build

LIB_DIR		:= ${build_dir}/lib

libcommon_a	:= ${top_dir}/_build/lib/libcommon.a

TEST_PATH 	?= ./

export FQBN	?= keyboardio:virtual:model01

TESTS		?= $(shell cd $(tests_dir); find ${TEST_PATH} -name '*.ino' -exec dirname {} \;)


# If we start off in tests to run make all, the sketch makefiles guess the wrong location for
# Kaliedoscope's makefiles

KALEIDOSCOPE_ETC_DIR ?= $(top_dir)/etc


.PHONY: clean cmake-clean all googletest generate-testcases


generate-testcases:
	@for test in ${TESTS}; do \
		${MAKE} -s -f ${top_dir}/testing/makefiles/testcase.mk -C $${test} testcase=$${test} generate-testcase; \
	done

all: ${TESTS} run-all

run-all: ${TESTS}
	@for test in ${TESTS}; do \
		${MAKE} -s -f ${top_dir}/testing/makefiles/testcase.mk -C $${test} testcase=$${test} run; \
	done || exit 1

cmake-clean:
	rm -rf "${top_dir}"/testing/googletest/build/*

clean: cmake-clean
	@for test in ${TESTS}; do \
		${MAKE} -s -f ${top_dir}/testing/makefiles/testcase.mk -C $${test} testcase=$${test} clean; \
	done
	rm -rf "${build_dir}"/*

googletest: ${top_dir}/testing/googletest/build/Makefile
	cd ${top_dir}/testing/googletest/build && $(MAKE)

${top_dir}/testing/googletest/build/Makefile:
	$(info googletest Makefile is being remade)
	install -d ${top_dir}/testing/googletest/build && cd ${top_dir}/testing/googletest/build && cmake ..

${libcommon_a}:
	${MAKE} -f ${top_dir}/testing/makefiles/libcommon.mk -C ${top_dir}/testing

Makefile:
	@:

${TESTS}: ${libcommon_a} googletest
	${MAKE} -f ${top_dir}/testing/makefiles/testcase.mk -C $@ testcase=$@ build

%+run:
	${MAKE} -s -f ${top_dir}/testing/makefiles/testcase.mk -C $* testcase=$* run
