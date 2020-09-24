top_dir		:= $(abspath $(dir $(lastword ${MAKEFILE_LIST}))..)
build_dir := ${top_dir}/_build
LIB_DIR		:= ${build_dir}/lib

libcommon_a	:= ${top_dir}/_build/lib/libcommon.a

TESTS			?= $(shell find . -name 'sketch.ino' -exec dirname {} \;)
.PHONY: clean cmake-clean all googletest

all: ${TESTS}

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

%: ${libcommon_a} googletest
	${MAKE} -f ${top_dir}/testing/makefiles/testcase.mk -C $@ top_dir=${top_dir} testcase=$@
