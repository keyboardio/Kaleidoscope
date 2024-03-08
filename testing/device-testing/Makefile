# Makefile for running Python tests

# Shell to use with Make
SHELL := /bin/bash

# Find all Python test scripts
TEST_SCRIPTS := $(shell find . -type f -name 'test.py' )

.PHONY: test

test-on-hardware-wiping-your-device:
	@for script in $(TEST_SCRIPTS) ; do \
		echo "Running $$script..." ; \
		python -m unittest $$script ; \
		if [ $$? -ne 0 ]; then \
			echo "Test failed: $$script" ; \
			exit 1 ; \
		fi ; \
	done

