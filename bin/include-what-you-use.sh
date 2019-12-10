#!/bin/bash

# This script runs include-what-you-use on the Kaleidoscope source code.
# Suggestions made by IWYU can be used to speed up compilation times.

# author: noseglasses (shinynoseglasses@gmail.com)

# usage: BOARD_HARDWARE_PATH=<hardware path> CLANG_COMPILER=<path to clang excecutable> include-what-you-use.sh 

# Please not that this script requires include-what-you-use to be installed
# and the command iwyu to be found in the path.

# IMPORTANT: The list of source/header files undergoes changes 
#            and must be maintained manually!

kaleidoscope_headers=(
"key_defs_consumerctl.h"
"key_defs_keyboard.h"
"Kaleidoscope.h"
"keymaps.h"
"progmem_helpers.h"
"MatrixAddr.h"
"event_handler_result.h"
"macro_map.h"
"Hardware.h"
"plugin/EEPROM-Settings/crc.h"
"plugin/TriColor.h"
"plugin/MouseKeys/MouseWarpModes.h"
"plugin/MouseKeys/MouseKeyDefs.h"
"plugin/MouseKeys/MouseWrapper.h"
"plugin/CycleTimeReport.h"
"plugin/MagicCombo.h"
"plugin/Qukeys/KeyAddrEventQueue.h"
"plugin/LED-ActiveModColor.h"
"plugin/HardwareTestMode.h"
"plugin/Colormap.h"
"plugin/EEPROM-Keymap.h"
"plugin/Syster.h"
"plugin/Escape-OneShot.h"
"plugin/Miami.h"
"plugin/LED-Palette-Theme.h"
"plugin/LEDEffect-Breathe.h"
"plugin/DynamicMacros.h"
"plugin/LED-Wavepool.h"
"plugin/Leader.h"
"plugin/LEDModeInterface.h"
"plugin/SpaceCadet.h"
"plugin/LEDEffect-SolidColor.h"
"plugin/AccessTransientLEDMode.h"
"plugin/Redial.h"
"plugin/Qukeys.h"
"plugin/LED-AlphaSquare.h"
"plugin/LED-AlphaSquare/Font-3x4.h"
"plugin/LED-AlphaSquare/Effect.h"
"plugin/LED-AlphaSquare/Font-4x4.h"
"plugin/LED-AlphaSquare/Symbols.h"
"plugin/Cycle.h"
"plugin/LED-Stalker.h"
"plugin/HostOS.h"
"plugin/Macros.h"
"plugin/Model01-TestMode.h"
"plugin/EEPROM-Settings.h"
"plugin/LED-ActiveLayerColor.h"
"plugin/Jukebox.h"
"plugin/Unicode.h"
"plugin/FingerPainter.h"
"plugin/NumPad.h"
"plugin/LEDEffect-BootGreeting.h"
"plugin/Macros/MacroSteps.h"
"plugin/Macros/MacroKeyDefs.h"
"plugin/USB-Quirks.h"
"plugin/MouseKeys.h"
"plugin/LEDControl/LEDUtils.h"
"plugin/LEDControl/LED-Off.h"
"plugin/LEDMode.h"
"plugin/EEPROM-Keymap-Programmer.h"
"plugin/LEDEffect-Chase.h"
"plugin/LEDEffect-BootAnimation.h"
"plugin/WinKeyToggle.h"
"plugin/Heatmap.h"
"plugin/TapDance.h"
"plugin/LEDControl.h"
"plugin/GeminiPR.h"
"plugin/TopsyTurvy.h"
"plugin/LEDEffect-Rainbow.h"
"plugin/PersistentLEDMode.h"
"plugin/ShapeShifter.h"
"plugin/TypingBreaks.h"
"plugin/IdleLEDs.h"
"plugin/HostPowerManagement.h"
"plugin/FocusSerial.h"
"plugin/Turbo.h"
"plugin/GhostInTheFirmware.h"
"plugin/FirmwareDump.h"
"plugin/OneShot.h"
"plugin/HostOS-Focus.h"
"key_defs_sysctl.h"
"layers.h"
"hooks.h"
"driver/keyscanner/ATmega.h"
"driver/keyscanner/None.h"
"driver/keyscanner/Base.h"
"driver/bootloader/None.h"
"driver/bootloader/avr/Caterina.h"
"driver/bootloader/avr/FLIP.h"
"driver/bootloader/avr/HalfKay.h"
"driver/bootloader/Base.h"
"driver/storage/None.h"
"driver/storage/ATmega32U4EEPROMProps.h"
"driver/storage/Base.h"
"driver/storage/AVREEPROM.h"
"driver/mcu/None.h"
"driver/mcu/Base.h"
"driver/mcu/ATmega32U4.h"
"driver/led/WS2812.h"
"driver/led/None.h"
"driver/led/ws2812/config.h"
"driver/led/Color.h"
"driver/led/Base.h"
"key_defs.h"
"key_events.h"
"bitfields.h"
"plugin.h"
"device/device.h"
"device/softhruf/Splitography.h"
"device/kbdfans/KBD4x.h"
"device/key_indexes.h"
"device/keyboardio/Model01.h"
"device/keyboardio/twi.h"
"device/keyboardio/Imago.h"
"device/olkb/Planck.h"
"device/ATmega32U4Keyboard.h"
"device/avr/pins_and_ports.h"
"device/ez/ErgoDox.h"
"device/ez/ErgoDox/i2cmaster.h"
"device/ez/ErgoDox/ErgoDoxScanner.h"
"device/technomancy/Atreus2.h"
"device/technomancy/Atreus.h"
"device/Base.h"
"device/virtual/Logging.h"
"device/virtual/DefaultHIDReportConsumer.h"
"device/virtual/Virtual.h"
"macro_helpers.h"
"hid.h"
"HIDTables.h"
"KeyAddr.h"
"key_defs_aliases.h"
"key_defs_keymaps.h"
"keyswitch_state.h"
"event_handlers.h"
)

kaleidoscope_sources=(
"hooks.cpp"
"Kaleidoscope.cpp"
"plugin/TriColor.cpp"
"plugin/EEPROM-Settings/crc.cpp"
"plugin/MouseKeys/MouseWrapper.cpp"
"plugin/FocusSerial.cpp"
"plugin/Qukeys.cpp"
"plugin/GhostInTheFirmware.cpp"
"plugin/IdleLEDs.cpp"
"plugin/HostPowerManagement.cpp"
"plugin/Redial.cpp"
"plugin/Unicode.cpp"
"plugin/TypingBreaks.cpp"
"plugin/DynamicMacros.cpp"
"plugin/CycleTimeReport.cpp"
"plugin/MouseKeys.cpp"
"plugin/LED-Palette-Theme.cpp"
"plugin/LEDEffect-BootAnimation.cpp"
"plugin/LEDEffect-SolidColor.cpp"
"plugin/EEPROM-Keymap-Programmer.cpp"
"plugin/EEPROM-Keymap.cpp"
"plugin/HostOS.cpp"
"plugin/LEDModeInterface.cpp"
"plugin/LEDControl.cpp"
"plugin/LED-ActiveModColor.cpp"
"plugin/Escape-OneShot.cpp"
"plugin/LEDEffect-Chase.cpp"
"plugin/LED-AlphaSquare/Effect.cpp"
"plugin/TopsyTurvy.cpp"
"plugin/WinKeyToggle.cpp"
"plugin/LED-AlphaSquare.cpp"
"plugin/NumPad.cpp"
"plugin/OneShot.cpp"
"plugin/TapDance.cpp"
"plugin/Heatmap.cpp"
"plugin/LED-Stalker.cpp"
"plugin/Cycle.cpp"
"plugin/HardwareTestMode.cpp"
"plugin/Colormap.cpp"
"plugin/GeminiPR.cpp"
"plugin/Leader.cpp"
"plugin/EEPROM-Settings.cpp"
"plugin/LEDControl/LED-Off.cpp"
"plugin/LEDControl/LEDUtils.cpp"
"plugin/Macros.cpp"
"plugin/LED-Wavepool.cpp"
"plugin/LEDEffect-Rainbow.cpp"
"plugin/FirmwareDump.cpp"
"plugin/Syster.cpp"
"plugin/HostOS-Focus.cpp"
"plugin/ShapeShifter.cpp"
"plugin/Turbo.cpp"
"plugin/Jukebox.cpp"
"plugin/LEDEffect-Breathe.cpp"
"plugin/PersistentLEDMode.cpp"
"plugin/MagicCombo.cpp"
"plugin/Miami.cpp"
"plugin/SpaceCadet.cpp"
"plugin/FingerPainter.cpp"
"plugin/LEDEffect-BootGreeting.cpp"
"plugin/LED-ActiveLayerColor.cpp"
"plugin/USB-Quirks.cpp"
"bitfields.cpp"
"driver/bootloader/avr/FLIP.cpp"
"layers.cpp"
"key_events.cpp"
"device/softhruf/Splitography.cpp"
"device/kbdfans/KBD4x.cpp"
"device/keyboardio/Model01.cpp"
"device/keyboardio/Imago.cpp"
"device/olkb/Planck.cpp"
"device/ez/ErgoDox.cpp"
"device/ez/ErgoDox/ErgoDoxScanner.cpp"
"device/ez/ErgoDox/i2cmaster.cpp"
"device/technomancy/Atreus.cpp"
"device/technomancy/Atreus2.cpp"
"device/virtual/DefaultHIDReportConsumer.cpp"
"device/virtual/Logging.cpp"
"device/virtual/Virtual.cpp"
"device/virtual/HID.cpp"
"device/keyboardio/twi.c"
)

kaleidoscope_internal_headers=(
"event_dispatch.h"
"type_traits/has_member.h"
"type_traits/has_method.h"
"device.h"
"eventhandler_signature_check.h"
"LEDModeManager.h"
"array_like_storage.h"
"compiler_warnings.h"
"deprecations.h"
)

kaleidoscope_internal_sources=(
"LEDModeManager.cpp"
"device.cpp"
)

clang_resource_dir=$(${CLANG_COMPILER} -print-resource-dir)

iwyu_flags=(
   -x c++
)

compiler_flags=(
   -c 
   -g 
   -Wall 
   -Wextra 
   "-std=gnu++11" 
   -ffunction-sections 
   -fdata-sections 
   -fno-threadsafe-statics 
   "-I${clang_resource_dir}/include/" 
   -MMD 
   "-DKALEIDOSCOPE_VIRTUAL_BUILD=1"
   "-DKEYBOARDIOHID_BUILD_WITHOUT_HID=1"
   "-DUSBCON=dummy"
   "-DARDUINO_ARCH_AVR=1"
   "-DARDUINO=10607"
   -DARDUINO_AVR_MODEL01
   -DARDUINO_ARCH_VIRTUAL
   "-DUSB_VID=0x1209"
   "-DUSB_PID=0x2301"
   "-DUSB_MANUFACTURER=\"Keyboardio\"" 
   "-DUSB_PRODUCT=\"Model 01\"" 
   -Woverloaded-virtual 
   -Wno-unused-parameter 
   -Wno-unused-variable 
   -Wno-ignored-qualifiers 
   -Wno-pragma-once-outside-header
   "-DKALEIDOSCOPE_HARDWARE_H=\"Kaleidoscope-Hardware-Model01.h\"" 
   "-I${BOARD_HARDWARE_PATH}/keyboardio/virtual/cores/arduino" 
   "-I${BOARD_HARDWARE_PATH}/keyboardio/virtual/variants/model01" 
   "-I${BOARD_HARDWARE_PATH}/keyboardio/virtual/libraries/Kaleidoscope/src" 
   "-I${BOARD_HARDWARE_PATH}/keyboardio/virtual/libraries/KeyboardioHID/src" 
   "-I${BOARD_HARDWARE_PATH}/keyboardio/virtual/libraries/Kaleidoscope-HIDAdaptor-KeyboardioHID/src"
)

function run_iwyu {

   src_abs="$1"
   
   echo "********************************************************************************"
   echo "${src_abs}"
   echo "********************************************************************************"
   echo iwyu "${iwyu_flags[@]}" "${compiler_flags[@]}" "${src_abs}"
   iwyu "${iwyu_flags[@]}" "${compiler_flags[@]}" "${src_abs}"
}

kaleidoscope_path="${BOARD_HARDWARE_PATH}/keyboardio/avr/libraries/Kaleidoscope"

for src in "${kaleidoscope_headers[@]}"
do 
   src_abs="${kaleidoscope_path}/src/kaleidoscope/${src}"
   run_iwyu "${src_abs}"
done

for src in "${kaleidoscope_sources[@]}"
do 
   src_abs="${kaleidoscope_path}/src/kaleidoscope/${src}"
   run_iwyu "${src_abs}"
done

for src in "${kaleidoscope_internal_headers[@]}"
do 
   src_abs="${kaleidoscope_path}/src/kaleidoscope_internal/${src}"
   run_iwyu "${src_abs}"
done

for src in "${kaleidoscope_internal_sources[@]}"
do 
   src_abs="${kaleidoscope_path}/src/kaleidoscope_internal/${src}"
   run_iwyu "${src_abs}"
done
