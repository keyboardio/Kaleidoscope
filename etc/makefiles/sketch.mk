mkfile_dir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# If the sketch is defined
ifneq ($(SKETCH),) 
# If the sketch isn't a directory, we want to get the directory the sketch is in
ifeq ($(wildcard $(SKETCH)/.),)
SKETCH_DIR	:= $(dir $(SKETCH))
endif
else
# If the sketch wasn't defined as we came in, assume the current directory
# is where we're looking
SKETCH_DIR	:= $(realpath $(CURDIR))
endif

SKETCH_BASE_NAME	:=	$(notdir $(SKETCH_DIR))
SKETCH_FILE_NAME	:= 	$(addsuffix .ino, $(SKETCH_BASE_NAME))

# Find the path of the sketch file 
SKETCH_DIR_CANDIDATES = $(sketch_dir) src/ .
sketch_exists_p = $(realpath $(wildcard $(dir)/$(SKETCH_FILE_NAME)))
SKETCH_FILE_PATH := $(firstword $(foreach dir,$(SKETCH_DIR_CANDIDATES),$(sketch_exists_p)))

include $(mkfile_dir)/arduino-cli.mk

ifeq ($(FQBN),)
export FQBN = $(call _arduino_prop,build.fqbn)
ifneq ($(VERBOSE),)
$(info Arduino provided FQBN $(call _arduino_prop,build.fqbn))
endif
endif

# We -could- check to see if sketch-dir is in git before running this command 
# but since we'd just return an empty value in that case, why bother?
GIT_VERSION := $(shell git -C "$(SKETCH_DIR)" describe --abbrev=6 --dirty --alway  2>/dev/null || echo 'unknown')

ifeq ($(SKETCH_IDENTIFIER),)
SKETCH_IDENTIFIER := $(shell echo "${SKETCH_FILE_PATH}" | cksum | cut -d ' ' -f 1)-$(SKETCH_FILE_NAME)
endif

BUILD_PATH ?= $(KALEIDOSCOPE_BUILD_PATH)/$(SKETCH_IDENTIFIER)
OUTPUT_PATH ?= $(KALEIDOSCOPE_OUTPUT_PATH)/$(SKETCH_IDENTIFIER)


OUTPUT_FILE_PREFIX 		:= $(SKETCH_BASE_NAME)-$(GIT_VERSION)
HEX_FILE_PATH 			:= $(OUTPUT_PATH)/$(OUTPUT_FILE_PREFIX).hex
HEX_FILE_WITH_BOOTLOADER_PATH 	:= $(OUTPUT_PATH)/$(OUTPUT_FILE_PREFIX)-with-bootloader.hex
ELF_FILE_PATH 			:= $(OUTPUT_PATH)/$(OUTPUT_FILE_PREFIX).elf
LIB_FILE_PATH 			:= $(OUTPUT_PATH)/$(OUTPUT_FILE_PREFIX).a


KALEIDOSCOPE_PLATFORM_LIB_DIR := $(abspath $(KALEIDOSCOPE_DIR)/..)




ifeq ($(FQBN),)
possible_fqbns = $(shell $(ARDUINO_CLI) board list --format=json |grep FQBN| grep -v "keyboardio:virtual"|cut -d: -f 2-)

possible_fqbn = $(firstword $(possible_fqbns))

$(info *************************************************************** )
$(info )
$(info  Arduino couldn't figure out what kind of device this sketch )
$(info  is for. Usually, Arduino looks in a file called `sketch.json` )
$(info  to figure this out. )
ifneq ($(possible_fqbn),)

fake_var_to_run_shell := $(shell  $(ARDUINO_CLI) board attach $(possible_fqbn))

$(info )
$(info I have detected a connected device supported by Kaleidoscope and) 
$(info attepted to automatically resolve this issue by running the)
$(info following command:)
$(info )
$(info  $(ARDUINO_CLI) board attach $(possible_fqbn))
$(info ) 
$(info If the build fails or $(possible_fqbn) doesn't)
$(info look like your keyboard, you may need to manually edit your)
$(info `sketch.json` file or run )
$(info )
$(info  $(ARDUINO_CLI) board attach )
$(info )
$(info manually, specifying the FQBN for your keyboard. )
$(info )
$(info *************************************************************** )

else

$(info )
$(info I'm unable to detect your keyboard, you may need to manually )
$(info edit your `sketch.json` file or run )
$(info )
$(info  $(ARDUINO_CLI) board attach )
$(info )
$(info manually, specifying the FQBN for your keyboard. )
$(info )
$(info *************************************************************** )
$(error )

endif
endif






# Flashing related config
ifneq ($(FQBN),)
ifeq ($(KALEIDOSCOPE_DEVICE_PORT),)
KALEIDOSCOPE_DEVICE_PORT = $(shell $(ARDUINO_CLI) board list --format=text | grep $(FQBN) |cut -d' ' -f 1)
endif
endif

flashing_instructions	:= $(call _arduino_prop,build.flashing_instructions)
ifeq ($(flashing_instructions),)
flashing_instructions	:= "If your keyboard needs you to do something to put it in flashing mode, do that now."
endif

unescaped_flashing_instructions = $(shell printf $(flashing_instructions) )

DEFAULT_GOAL: compile


#$(SKETCH_FILE_PATH):
#	@: # dummy recipe for the sketch file


.PHONY: compile configure-arduino-cli install-arduino-core-kaleidoscope install-arduino-core-avr 
.PHONY: disassemble decompile size-map flash clean all test

all: compile
	@: ## Do not remove this line, otherwise `make all` will trigger the `%` rule too.


disassemble: ${ELF_FILE_PATH}
	$(call _arduino_prop,compiler.objdump.cmd) \
		$(call _arduino_prop,compiler.objdump.flags) \
		"${ELF_FILE_PATH}"

size-map: ${ELF_FILE_PATH}
	$(call _arduino_prop,compiler.size-map.cmd) \
		$(call _arduino_prop,compiler.size-map.flags) \
		"${ELF_FILE_PATH}"

flash: ${HEX_FILE_PATH}

${ELF_FILE_PATH}: compile
${HEX_FILE_PATH}: compile
	

BOOTLOADER_PATH := $(call _arduino_prop,runtime.platform.path)/bootloaders/$(call _arduino_prop,bootloader.file)

hex-with-bootloader: ${HEX_FILE_PATH}  
	$(QUIET) awk '/^:00000001FF/ == 0' "${HEX_FILE_PATH}" >"${HEX_FILE_WITH_BOOTLOADER_PATH}"
	$(QUIET) cat "${BOOTLOADER_PATH}" >>"${HEX_FILE_WITH_BOOTLOADER_PATH}"
	$(QUIET) ln -sf -- "${OUTPUT_FILE_PREFIX}-with-bootloader.hex" "${OUTPUT_PATH}/${SKETCH_BASE_NAME}-latest-with-bootloader.hex"
	$(info Combined firmware and bootloader are now at)
	$(info  ${HEX_FILE_WITH_BOOTLOADER_PATH})
	$(info )
	$(info Make sure you have the bootloader version you expect.)
	$(info )
	$(info )
	$(info And TEST THIS ON REAL HARDWARE BEFORE YOU GIVE IT TO ANYONE.)

clean:
	$(QUIET) rm -rf -- "${OUTPUT_PATH}"/*


ifneq ($(LOCAL_CFLAGS),)
local_cflags_property = --build-properties "compiler.cpp.extra_flags=${LOCAL_CFLAGS}"
else
local_cflags_property =
endif

compile:
	$(QUIET) install -d "${OUTPUT_PATH}"
	$(QUIET) $(ARDUINO_CLI) compile --fqbn "${FQBN}" ${ARDUINO_VERBOSE} --warnings all ${ccache_wrapper_property} ${local_cflags_property} \
	  --libraries "${KALEIDOSCOPE_PLATFORM_LIB_DIR}" \
	  --libraries "${KALEIDOSCOPE_DIR}/plugins/" \
	  --build-path "${BUILD_PATH}" \
	  --output-dir "${OUTPUT_PATH}" \
	  --build-cache-path "${CORE_CACHE_PATH}" \
	  "${SKETCH_FILE_PATH}"
ifeq ($(LIBONLY),)
	$(QUIET) cp "${BUILD_PATH}/${SKETCH_FILE_NAME}.hex" "${HEX_FILE_PATH}"
	$(QUIET) cp "${BUILD_PATH}/${SKETCH_FILE_NAME}.elf" "${ELF_FILE_PATH}"
	$(QUIET) ln -sf "${OUTPUT_FILE_PREFIX}.hex" "${OUTPUT_PATH}/${SKETCH_BASE_NAME}-latest.hex"
	$(QUIET) ln -sf "${OUTPUT_FILE_PREFIX}.elf" "${OUTPUT_PATH}/${SKETCH_BASE_NAME}-latest.elf"
else    
	$(QUIET) cp "${BUILD_PATH}/${SKETCH_FILE_NAME}.a" "${LIB_FILE_PATH}"
	$(QUIET) ln -sf "${OUTPUT_FILE_PREFIX}.a" "${OUTPUT_PATH}/${SKETCH_BASE_NAME}-latest.a"
endif
ifneq ($(VERBOSE),)
	$(info Build artifacts can be found in ${BUILD_PATH})
endif

#TODO (arduino team) I'd love to do this with their json output
#but it's short some of the data we kind of need

.PHONY: ensure-device-port-defined

ensure-device-port-defined:  
	@if [ -z $(KALEIDOSCOPE_DEVICE_PORT) ]; then \
	echo "ERROR: Unable to detect keyboard serial port.";\
	echo ;\
	echo "Arduino should autodetect it, but you could also set";\
	echo "KALEIDOSCOPE_DEVICE_PORT to your keyboard's serial port.";\
	echo ;\
	exit -1;fi

ifneq ($(FQBN),)
fqbn_arg = --fqbn $(FQBN)
endif

ifneq ($(KALEIDOSCOPE_DEVICE_PORT),)
port_arg = --port $(KALEIDOSCOPE_DEVICE_PORT)
endif


flash: 
	$(info $(unescaped_flashing_instructions))
	$(info )
	$(info When you're ready to proceed, press 'Enter'.)
	$(info )
	@$(shell read _)
	$(QUIET) $(ARDUINO_CLI) upload $(fqbn_arg) \
	  --input-dir "${OUTPUT_PATH}" \
	  $(port_arg) $(ARDUINO_VERBOSE)
