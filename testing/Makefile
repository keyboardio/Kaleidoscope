TEST_DIRS = $(dir $(wildcard */tests.h))

all: ${TEST_DIRS}
	@:

Makefile: ${TEST_DIRS}
	@:

%: FORCE 
	@if [ ! -f "$@/tests.h" ]; then \
		echo 'Unable to find test file "$@/tests.h"'; \
	else \
		echo "Running tests in $@"; \
		echo "env LOCAL_CFLAGS='-DTESTING_INCLUDE_FILE="$@/tests.h" "-I$(PWD)/$@"' VERBOSE=1 $(MAKE) -f delegate.mk"; \
		env LOCAL_CFLAGS='-DTESTING_INCLUDE_FILE="$@/tests.h" "-I$(PWD)/$@"' VERBOSE=1 $(MAKE) -f delegate.mk; \
	fi

.PHONY: FORCE



