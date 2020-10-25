top_dir		:= $(abspath $(dir $(lastword ${MAKEFILE_LIST}))..)
build_dir := ${top_dir}/_build
LIB_DIR		:= ${build_dir}/lib

libcommon_a	:= ${top_dir}/_build/lib/libcommon.a

TEST_PATH ?= tests/

TESTS			?= $(shell find ${top_dir}/${TEST_PATH} -name 'sketch.ino' -exec dirname {} \;)
.PHONY: clean cmake-clean all googletest

all: ${TESTS}
	@for test in ${TESTS}; do \
		${MAKE} -s -f ${top_dir}/testing/makefiles/testcase.mk -C $${test} top_dir=${top_dir} testcase=$${test} run; \
	done

cmake-clean:
	-${MAKE} clean
	rm -rf ../testing/googletest/build

clean: ../testing/googletest/build/Makefile
	rm -rf "${build_dir}"

googletest: ../testing/googletest/build/Makefile
	cd ../testing/googletest/build && $(MAKE)

../testing/googletest/build/Makefile:
	install -d ../testing/googletest/build && cd ../testing/googletest/build && cmake ..

${libcommon_a}:
	${MAKE} -f ${top_dir}/testing/makefiles/libcommon.mk -C ${top_dir}/testing

Makefile:
	@:

${TESTS}: ${libcommon_a} googletest
	${MAKE} -f ${top_dir}/testing/makefiles/testcase.mk -C $@ top_dir=${top_dir} testcase=$@

%+run:
	${MAKE} -s -f ${top_dir}/testing/makefiles/testcase.mk -C $* top_dir=${top_dir} testcase=$* run
