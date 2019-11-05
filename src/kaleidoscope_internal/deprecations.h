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

#define _DEPRECATED_MESSAGE_ROWS                        \
  "The `ROWS` macro is deprecated, please use\n" __NL__ \
  "`Kaleidoscope.device().matrix_rows` instead."

#define _DEPRECATED_MESSAGE_COLS                          \
  "The `COLS` macro is deprecated, please use \n" __NL__  \
  "`Kaleidoscope.device().matrix_columns` instead."

#define _DEPRECATED_MESSAGE_LED_COUNT                         \
  "The `LED_COUNT` macro is deprecated, please use \n" __NL__ \
  "`Kaleidoscope.device().led_count` instead."
