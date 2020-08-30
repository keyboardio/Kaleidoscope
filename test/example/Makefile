all:
	@:
Makefile:
	@:

%: FORCE 
	@if [ ! -f "$@/tests.h" ]; then \
		echo 'Unable to find tests file "$@/tests.h"'; \
	else \
		echo "Running test in $@"; \
		env LOCAL_CFLAGS='-DTESTING_INCLUDE_FILE="$@/tests.h" "-I$(PWD)/$@"' VERBOSE=1 $(MAKE) -f delegate.mk; \
	fi

.PHONY: FORCE



