/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2018  Keyboard.io, Inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#define DEPRECATED(tag)                                                 \
  __attribute__((deprecated(_DEPRECATE(_DEPRECATED_MESSAGE_ ## tag))))

#define _DEPRECATE(message) "\n"                                               \
  "------------------------------------------------------------------------\n" \
  message                                                                      \
  "\n"                                                                         \
  "------------------------------------------------------------------------\n" \

/* Messages */

#define _DEPRECATED_MESSAGE_NAMED_HARDWARE              __NL__ \
  "Named hardware objects are deprecated, please use\n" __NL__ \
  "`Kaleidoscope.device()` instead."

#define _DEPRECATED_MESSAGE_KEYBOARDHARDWARE              \
  "`KeyboardHardware` is deprecated, please use\n" __NL__ \
  "`Kaleidoscope.device()` instead"
#define _DEPRECATED_MESSAGE_HARDWARE_RESETDEVICE                        \
  "`KeyboardHardware.resetDevice()` is deprecated, please use " __NL__  \
  "`Kaleidoscope.rebootBootloader()` instead."

#define _DEPRECATED_MESSAGE_ROWS                        \
  "The `ROWS` macro is deprecated, please use\n" __NL__ \
  "`Kaleidoscope.device().matrix_rows` instead."

#define _DEPRECATED_MESSAGE_COLS                          \
  "The `COLS` macro is deprecated, please use \n" __NL__  \
  "`Kaleidoscope.device().matrix_columns` instead."

#define _DEPRECATED_MESSAGE_LED_COUNT                         \
  "The `LED_COUNT` macro is deprecated, please use \n" __NL__ \
  "`Kaleidoscope.device().led_count` instead."

#define _DEPRECATED_MESSAGE_HARDWARE_BASE_CLASS                 \
  "The `Hardware` base class is deprecated. Please use\n" __NL__ \
  "the new APIs based on `kaleidoscope::device::Base`."

#define _DEPRECATED_MESSAGE_DIRECT_KEY_MEMBER_ACCESS                           \
  "Direct access to `Key` class' data members is deprecated.\n"                \
  "Please use `Key::setKeyCode()`/`Key::getKeyCode()` or\n"                    \
  "`Key::setFlags()`/`Key::getFlags()` instead.\n"                             \
  "\n"                                                                         \
  "For further information and examples on how to do that, \n"                 \
  "please see UPGRADING.md."

#define _DEPRECATED_MESSAGE_KEY_MEMBER_RAW_ACCESS                              \
  "The member variable `raw` of class Key had to be removed. Please \n"        \
  "use `Key::setRaw()`/`Key::getRaw()` to set and get raw data.\n"             \
  "\n"                                                                         \
  "Important: This is not a deprecation. Your code will compile but fail\n"    \
  "           to link until all access to `Key::raw` has been replaced.\n"     \
  "\n"                                                                         \
  "For further information and examples on how to do that, \n"                 \
  "please see UPGRADING.md."

#define _DEPRECATED_MESSAGE_GLOBAL_VARIABLE_KALEIDOSCOPE                       \
  "Variable `Kaleidoscope` in global namespace has been deprecated.\n"         \
  "Please use `kaleidoscope::Runtime` instead."

#define _DEPRECATED_MESSAGE_GLOBAL_TYPENAME_KALEIDOSCOPE                       \
  "Type `Kaleidoscope_` in global namespace has been deprecated.\n"            \
  "Please use `kaleidoscope::Runtime_` instead."
