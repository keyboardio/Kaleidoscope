top_dir		:= $(abspath $(dir $(lastword ${MAKEFILE_LIST}))..)
build_dir := ${top_dir}/_build
LIB_DIR		:= ${build_dir}/lib

libcommon_a	:= ${top_dir}/_build/lib/libcommon.a

TEST_PATH ?= tests/

TESTS			?= $(shell find ${top_dir}/${TEST_PATH} -name '*.ino' -exec dirname {} \;)
.PHONY: clean cmake-clean all googletest

generate-testcases:
	@for test in ${TESTS}; do \
		${MAKE} -s -f ${top_dir}/testing/makefiles/testcase.mk -C $${test} top_dir=${top_dir} testcase=$${test} generate-testcase; \
	done

all: ${TESTS}
	@for test in ${TESTS}; do \
		${MAKE} -s -f ${top_dir}/testing/makefiles/testcase.mk -C $${test} top_dir=${top_dir} testcase=$${test} run; \
	done || exit 1

cmake-clean:
	rm -rf "${top_dir}/testing/googletest/build/*"

clean: cmake-clean
# ${top_dir}/testing/googletest/build/Makefile
	@for test in ${TESTS}; do \
		${MAKE} -s -f ${top_dir}/testing/makefiles/testcase.mk -C $${test} top_dir=${top_dir} testcase=$${test} clean; \
	done

	rm -rf "${build_dir}/*"

googletest: ${top_dir}/testing/googletest/build/Makefile
	cd ${top_dir}/testing/googletest/build && $(MAKE)

${top_dir}/testing/googletest/build/Makefile:
	install -d ${top_dir}/testing/googletest/build && cd ${top_dir}/testing/googletest/build && cmake ..

${libcommon_a}:
	${MAKE} -f ${top_dir}/testing/makefiles/libcommon.mk -C ${top_dir}/testing

Makefile:
	@:

${TESTS}: ${libcommon_a} googletest
	${MAKE} -f ${top_dir}/testing/makefiles/testcase.mk -C $@ top_dir=${top_dir} testcase=$@

%+run:
	${MAKE} -s -f ${top_dir}/testing/makefiles/testcase.mk -C $* top_dir=${top_dir} testcase=$* run
