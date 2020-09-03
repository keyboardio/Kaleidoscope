TEST_DIRS=$(dir $(wildcard */*_test.cpp))

Makefile: ${TEST_DIRS}
	@:

%: FORCE
	cd "$@" && $(MAKE)

.PHONY: FORCE
