BIN_DIR=bin
LIB_DIR=lib
TEST_DIRS = $(dir $(wildcard */tests.h))

all: ${TEST_DIRS}
	@:

Makefile: ${TEST_DIRS}
	@:

%: FORCE 
	@if [ ! -f "$@/tests.h" ]; then \
		echo 'Unable to find tests file "$@/tests.h"'; \
	else \
		echo "Running test in $@"; \
		rm -rf "${BIN_DIR}" "${LIB_DIR}"; \
		mkdir -p "${BIN_DIR}" "${LIB_DIR}"; \
		env LIBONLY=yes LOCAL_CFLAGS='-DTESTING_INCLUDE_FILE="$@/tests.h" "-I$(PWD)/$@"' OUTPUT_PATH="${PWD}/${LIB_DIR}" VERBOSE=1 $(MAKE) -f delegate.mk; \
		g++ -o "${BIN_DIR}/sketch" -lpthread -g -w -lm -lXtst -lX11 "${LIB_DIR}/sketch-latest.a"; \
		"./${BIN_DIR}/sketch" -t; \
	fi

.PHONY: FORCE



