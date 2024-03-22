arduino_platform_path := ${ARDUINO_DIRECTORIES_USER}/hardware/keyboardio/virtual/

shared_defines := \
                -DARDUINO=10607 \
                -DARDUINO_ARCH_VIRTUAL \
                -DKALEIDOSCOPE_VIRTUAL_BUILD=1 \
                -DKEYBOARDIOHID_BUILD_WITHOUT_HID=1 \
                -DUSBCON=dummy \
                -DARDUINO_ARCH_AVR=1 \
		$(call _arduino_prop,build.extra_flags) \
                -DARDUINO_$(call _arduino_prop,build.board) \
                -DUSB_PRODUCT='$(call _arduino_prop,build.usb_product)'

# Extract the kaleidoscope_hardware_h from shared defines
_hardware_h := $(shell echo $(shared_defines) | sed -n 's/.*-DKALEIDOSCOPE_HARDWARE_H="\([^"]*\)".*/\1/p')

# Find the directory containing the file
_hardware_plugin_path := $(shell find $(top_dir)/plugins/ -type f -name "$(_hardware_h)" -exec dirname {} \;)

shared_includes := \
		-I${top_dir} \
		-I${top_dir}/src \
		-I${_hardware_plugin_path} \
		-I${arduino_platform_path}/cores/keyboardio \
		-I${top_dir}/plugins/KeyboardioHID/src \
		-I${top_dir}/testing/googletest/googlemock/include \
		-I${top_dir}/testing/googletest/googletest/include \

