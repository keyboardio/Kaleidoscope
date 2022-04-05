arduino_platform_path := ${ARDUINO_DIRECTORIES_USER}/hardware/keyboardio/virtual/

shared_defines := \
                -DARDUINO=10607 \
                -DARDUINO_ARCH_VIRTUAL \
                -DKALEIDOSCOPE_VIRTUAL_BUILD=1 \
                -DKEYBOARDIOHID_BUILD_WITHOUT_HID=1 \
                -DUSBCON=dummy \
                -DARDUINO_ARCH_AVR=1 \
                -DARDUINO_AVR_MODEL01 \
                '-DKALEIDOSCOPE_HARDWARE_H="Kaleidoscope-Hardware-Keyboardio-Model01.h"' \
                '-DUSB_PRODUCT="Model 01"'

shared_includes := \
		-I${top_dir} \
		-I${top_dir}/src \
		-I${top_dir}/plugins/Kaleidoscope-Hardware-Keyboardio-Model01/src \
		-I${arduino_platform_path}/cores/arduino \
		-I${arduino_platform_path}/libraries/KeyboardioHID/src \
		-I${top_dir}/testing/googletest/googlemock/include \
		-I${top_dir}/testing/googletest/googletest/include \

