# default action for `make` is `build`
build: compile size

astyle:
		find . -type f -name \*.cpp |xargs -n 1 astyle --style=google
		find . -type f -name \*.ino |xargs -n 1 astyle --style=google
		find . -type f -name \*.h |xargs -n 1 astyle --style=google

generate-keymaps:
	tools/keyboardio-builder generate-keymaps

compile:
	tools/keyboardio-builder compile

size: compile
	tools/keyboardio-builder report-size

size-map: compile
	tools/keyboardio-builder size-map

decompile: compile
	tools/keyboardio-builder decompile

hex-with-bootloader: compile
	tools/keyboardio-builder hex-with-bootloader

reset-device:
	tools/keyboardio-builder reset-device

flash: compile reset-device
	tools/keyboardio-builder flash

program:
	tools/keyboardio-builder program
