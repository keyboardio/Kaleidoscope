#! /bin/sh
set -e

echo "* Building"
make OUTPUT_PATH=$(pwd)/_build >/dev/null 2>LOG

echo "* Jumping to bootloader..."
DEVICE=/dev/ttyACM1 focus device.reset
sleep 2s

echo "* Flashing..."
dfu-programmer atmega32u4 erase && dfu-programmer atmega32u4 flash _build/PloopyMini-latest.hex && dfu-programmer atmega32u4 start
