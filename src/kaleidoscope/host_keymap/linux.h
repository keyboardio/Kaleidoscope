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

#include "kaleidoscope/host_keymap/ascii.h"
#include "kaleidoscope/host_keymap/unicode.h"
#include "kaleidoscope/host_keymap/non_printable.h"

#define HOST_KEYMAP_LINUX( ASCII_KEYMAP,                       \
                           UNICODE_KEYMAP,                     \
                           NON_PRINTABLE_KEYMAP)               \
   HOST_KEYMAP_ASCII_CONVERTER(                                \
      ASCII_KEYMAP,                                            \
      ascii::CharParsingStandardFallback                       \
   )                                                           \
   HOST_KEYMAP_UNICODE_CONVERTER(                              \
      UNICODE_KEYMAP,                                          \
      unicode::CharParsingStandardFallback                     \
   )                                                           \
   HOST_KEYMAP_NON_PRINTABLE_CONVERTER(                        \
      NON_PRINTABLE_KEYMAP                                     \
   )                                                           \
   constexpr Key convertToKey(Key k) { return k; }
