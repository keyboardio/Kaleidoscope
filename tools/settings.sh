## NEEDS: LIBRARY, SKETCH, ROOT, SOURCEDIR
## Should be included when the current directory is the dir of the Sketch.

if [ -z "${SKETCH}" ] || [ -z "${LIBRARY}" ] || [ -z "${ROOT}" ] || [ -z "${SOURCEDIR}" ]; then
    echo "SKETCH, LIBRARY, SOURCEDIR, and ROOT need to be set before including this file!" >&2
    exit 1
fi

case "$0" in
    */settings.sh)
        echo "This file must be included, never run directly!" >&2
        exit 1
        ;;
esac

if [ -e "${SOURCEDIR}/settings.sh" ]; then
   . "${SOURCEDIR}/settings.sh"
fi

BOARD="model01"
MCU="atmega32u4"
FQBN="keyboardio:avr:model01"

ARDUINO_PATH="${ARDUINO_PATH:-${HOME}/install/arduino}"
ARDUINO_TOOLS_PATH="${ARDUINO_TOOLS_PATH:-${ARDUINO_PATH}/hardware/tools}"
ARDUINO_BUILDER="${ARDUINO_BUILDER:-${ARDUINO_PATH}/arduino-builder}"
ARDUINO_IDE_VERSION="100607"

BOARD_HARDWARE_PATH="${BOARD_HARDWARE_PATH:-${ROOT}/hardware}"

AVR_SIZE="${AVR_SIZE:-${ARDUINO_TOOLS_PATH}/avr/bin/avr-size}"
AVR_NM="${AVR_NM:-${ARDUINO_TOOLS_PATH}/avr/bin/avr-nm}"

BUILD_PATH="${BUILD_PATH:-$(mktemp -d 2>/dev/null || mktemp -d -t 'build')}"
OUTPUT_DIR="${OUTPUT_DIR:-output/${LIBRARY}}"
OUTPUT_PATH="${OUTPUT_PATH:-${SOURCEDIR}/${OUTPUT_DIR}}"

GIT_VERSION="$(git describe --abbrev=4 --dirty --always)"
LIB_VERSION="$( (grep version= ../../library.properties 2>/dev/null || echo version=0.0.0) | cut -d= -f2)-g${GIT_VERSION}"

OUTPUT_FILE_PREFIX="${SKETCH}-${LIB_VERSION}"

HEX_FILE_PATH="${OUTPUT_PATH}/${OUTPUT_FILE_PREFIX}.hex"
ELF_FILE_PATH="${OUTPUT_PATH}/${OUTPUT_FILE_PREFIX}.elf"

ARDUINO_TOOLS_PARAM="-tools ${ARDUINO_TOOLS_PATH}"
if [ -z "${ARDUINO_TOOLS_PATH}" ]; then
    ARDUINO_TOOLS_PARAM=""
fi

if [ ! -z "${AVR_GCC_PREFIX}" ]; then
    ARDUINO_AVR_GCC_PREFIX_PARAM="-prefs \"runtime.tools.avr-gcc.path=${AVR_GCC_PREFIX}\""
fi

if [ ! -z "${VERBOSE}" ] && [ "${VERBOSE}" -gt 0 ]; then
    ARDUINO_VERBOSE="-verbose"
else
    ARDUINO_VERBOSE="-quiet"
fi
