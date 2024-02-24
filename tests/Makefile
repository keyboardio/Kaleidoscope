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

# Hardocde an FQBN that gets the virtual core here so that when arduino_prop 
# gets called, it picks up the virtual platform compiler settings

export FQBN := keyboardio:virtual:model01

build_dir 	:= ${top_dir}/_build

TEST_PATH 	?= .

TESTS		:= $(shell cd $(tests_dir); find ${TEST_PATH} -name '*.ino' -exec dirname {} \;)

# The clutter up the test output on Make 4.0 and newer
MAKEFLAGS += --no-print-directory

include $(top_dir)/etc/makefiles/arduino-cli.mk



ifneq ($(KALEIDOSCOPE_CCACHE),)
_compiler_wrappers :=  -DCMAKE_C_COMPILER_LAUNCHER=ccache -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
endif

.DEFAULT_GOAL := all

# If we start off in tests to run make all, the sketch makefiles guess the wrong location for
# Kaleidoscope's makefiles
KALEIDOSCOPE_ETC_DIR ?= $(top_dir)/etc

.PHONY: all
all:   googletest run-all
	@:

.PHONY: cmake-clean
cmake-clean:
	rm -rf "${top_dir}"/testing/googletest/build/*

.PHONY: run-all
run-all: run-cache-warmer run-other-tests aggregate-test-results

.PHONY: run-cache-warmer
run-cache-warmer: generate-all-plugins-header $(filter ./_cache-warmer/%,${TESTS})

.PHONY: run-other-tests
run-other-tests: run-cache-warmer
	@$(MAKE) $(filter-out ./_cache-warmer/%,${TESTS})


${TESTS}:  googletest
	$(QUIET) { \
		$(MAKE) -s -f ${top_dir}/testing/makefiles/testcase.mk -C $@ testcase=$@ build run; \
		if [ $$? -ne 0 ]; then \
			hash=$$(echo "$$@" | shasum | cut -f1 -d' '); \
			echo "$@ failed" > ${build_dir}/test_failures_$$hash.log; \
		fi; \
	}


.PHONY: aggregate-test-results
aggregate-test-results: run-other-tests
	$(QUIET) TEST_ERRORS=0; \
	for file in ${build_dir}/test_failures_*.log; do \
		if [ -f "$$file" ]; then \
			TEST_ERRORS=$$((TEST_ERRORS + 1)); \
			cat "$$file" >> ${build_dir}/combined_test_failures.log; \
			rm -f "$$file"; \
		fi; \
	done; \
	if [ -f ${build_dir}/combined_test_failures.log ]; then \
		echo "Test failures: $$TEST_ERRORS"; \
		cat ${build_dir}/combined_test_failures.log; \
		rm -f ${build_dir}/combined_test_failures.log; \
		exit 1; \
	else \
		echo "All tests passed."; \
	fi





.PHONY: clean
clean: cmake-clean
	$(QUIET) for test in ${TESTS}; do \
		${MAKE} -s -f ${top_dir}/testing/makefiles/testcase.mk \
			-C $${test} \
			testcase=$${test} clean; \
	done
	$(QUIET)  rm -rf "${build_dir}"/*

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
	  $(_compiler_wrappers)

Makefile:
	@:



.PHONY: generate-all-plugins-header
generate-all-plugins-header:
	$(QUIET) echo "// This file is generated automatically. Do not edit." > _cache-warmer/warm-cache/generated-all-plugins.h
	$(QUIET) for dir in ../plugins/*; do \
		if [ -d "$$dir" ]; then \
			header_path="$$dir/$$(basename $$dir).h"; \
			header_src_path="$$dir/src/$$(basename $$dir).h"; \
			header="$$(basename $$dir).h"; \
			if [ -f "$$header_path" -o -f "$$header_src_path" ]; then \
				echo '#include "'$$header'"' >> _cache-warmer/warm-cache/generated-all-plugins.h; \
			fi; \
		fi; \
	done

