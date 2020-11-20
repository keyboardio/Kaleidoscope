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

#include "kaleidoscope/host_keymap/host_keymap.h"

namespace kaleidoscope {
namespace host_keymap {
namespace ascii {

struct CharProcessor {
  static constexpr bool isEscapeChar(char c) {
    return c == '#';
  }

  static constexpr bool isSeparator(char c) {
    return (c == ' ')
           || (c == '\t')
           || (c == '+');
  }

  static constexpr bool isMirrorChar(char c) {
    return c == 'r';
  }
};

#define _HOST_KEYMAP_CAST_ON_MODIFIERS_ASCII(OP)                               \
   OP('s', LSHIFT(k))                                                          \
   OP('S', LSHIFT(k))                                                          \
   OP('c', LCTRL(k))                                                           \
   OP('C', LCTRL(k))                                                           \
   OP('a', LALT(k))                                                            \
   OP('A', RALT(k))                                                            \
   OP('m', LGUI(k))                                                            \
   OP('M', LGUI(k))                                                            \
   OP('g', LGUI(k))                                                            \
   OP('G', LGUI(k))

// Define a AsciiConverterBase template base class that any ascii keymap converters
// are derived from by means of invoking the HOST_KEYMAP_ASCII_LANGUAGE_CONVERTER
// function macro.
//
_HOST_KEYMAP_DEFINE_CHAR_CONVERTER(
  ConverterBase, char, _HOST_KEYMAP_CAST_ON_MODIFIERS_ASCII)

#undef _HOST_KEYMAP_CAST_ON_MODIFIERS_ASCII

typedef _CharParsingStandardFallback<char> CharParsingStandardFallback;

} // namespace ascii
} // namespace host_keymap
} // namespace kaleidoscope

#define HOST_KEYMAP_ASCII_CONVERTER(LANGUAGE_KEYMAP, CHAR_PARSING_FALLBACK)    \
   struct AsciiConverter                                                       \
      : public ascii::ConverterBase<AsciiConverter, ascii::CharProcessor>      \
   {                                                                           \
      typedef ascii::ConverterBase<AsciiConverter, ascii::CharProcessor> Parent; \
                                                                               \
      using typename Parent::StringMemberType;                                 \
      using typename Parent::CharType;                                         \
                                                                               \
      static constexpr bool isKeyChar(char c) {                                \
         return LANGUAGE_KEYMAP(_HOST_KEYMAP_IS_KEY_CHAR)                      \
               CHAR_PARSING_FALLBACK::isKeyChar(c);                            \
      }                                                                        \
                                                                               \
      static constexpr Key charToKey(char c) {                                 \
         return LANGUAGE_KEYMAP(                                               \
                  _HOST_KEYMAP_MAP_KEY_CHAR_TO_KALEIDOSCOPE_KEY                \
               )                                                               \
               CHAR_PARSING_FALLBACK::charToKey(c);                            \
      }                                                                        \
   };                                                                          \
                                                                               \
   constexpr Key convertToKey(char c) {                                        \
      return AsciiConverter::convertToKey(c);                                  \
   }                                                                           \
   template<int _Size>                                                         \
   constexpr Key convertToKey(char const(&string) [_Size]) {                   \
      return AsciiConverter::convertToKey(string);                             \
   }
