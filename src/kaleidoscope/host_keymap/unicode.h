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
namespace unicode {

struct CharProcessor {
  static constexpr bool isEscapeChar(wchar_t c) {
    return c == L'#';
  }

  static constexpr bool isSeparator(wchar_t c) {
    return (c == L' ')
           || (c == L'\t')
           || (c == L'+');
  }

  static constexpr bool isMirrorChar(wchar_t c) {
    return c == L'r';
  }
};

#define _HOST_KEYMAP_CAST_ON_MODIFIERS_UNICODE(OP)                             \
                                                                               \
   OP(L's', LSHIFT(k))                                                         \
   OP(L'S', LSHIFT(k))                                                         \
   OP(L'⇧', LSHIFT(k))                                                         \
                                                                               \
   OP(L'c', LCTRL(k))                                                          \
   OP(L'C', LCTRL(k))                                                          \
   OP(L'⌃', LCTRL(k))                                                          \
                                                                               \
   OP(L'a', LALT(k))                                                           \
   OP(L'A', RALT(k))                                                           \
   OP(L'⌥', LALT(k))                                                           \
                                                                               \
   OP(L'm', LGUI(k))                                                           \
   OP(L'M', LGUI(k))                                                           \
   OP(L'g', LGUI(k))                                                           \
   OP(L'G', LGUI(k))                                                           \
   OP(L'⌘', LGUI(k))                                                           \

// Define a UnicodeConverterBase template base class that any unicode keymap converters
// are derived from by means of invoking the HOST_KEYMAP_UNICODE_LANGUAGE_CONVERTER
// function macro.
//
_HOST_KEYMAP_DEFINE_CHAR_CONVERTER(
  UnicodeConverterBase, wchar_t, _HOST_KEYMAP_CAST_ON_MODIFIERS_UNICODE)

#undef _HOST_KEYMAP_CAST_ON_MODIFIERS_UNICODE

} // namespace unicode
} // namespace host_keymap
} // namespace kaleidoscope

#define HOST_KEYMAP_UNICODE_CONVERTER(LANGUAGE_KEYMAP,                         \
                                               CHAR_PARSING_FALLBACK)          \
   struct UnicodeConverter                                                     \
      : public unicode::UnicodeConverterBase<UnicodeConverter, unicode::CharProcessor>  \
   {                                                                           \
      typedef unicode::UnicodeConverterBase<UnicodeConverter, unicode::CharProcessor> Parent; \
                                                                               \
      using typename Parent::StringMemberType;                                 \
      using typename Parent::CharType;                                         \
                                                                               \
      static constexpr bool isKeyChar(wchar_t c) {                             \
         return LANGUAGE_KEYMAP(_HOST_KEYMAP_IS_KEY_CHAR)                      \
               CHAR_PARSING_FALLBACK::isKeyChar(c);                            \
      }                                                                        \
                                                                               \
      static constexpr Key charToKey(wchar_t c) {                              \
         return LANGUAGE_KEYMAP(                                               \
                  _HOST_KEYMAP_MAP_KEY_CHAR_TO_KALEIDOSCOPE_KEY                \
               )                                                               \
               CHAR_PARSING_FALLBACK::charToKey(c);                            \
      }                                                                        \
   };                                                                          \
                                                                               \
   constexpr Key convertToKey(wchar_t c) {                                     \
      return UnicodeConverter::convertToKey(c);                                \
   }                                                                           \
   template<int _Size>                                                         \
   constexpr Key convertToKey(wchar_t const(&string) [_Size]) {                \
      return UnicodeConverter::convertToKey(string);                           \
   }

namespace kaleidoscope {
namespace host_keymap {
namespace unicode {
namespace control_keys {

#define _HOST_KEYMAP_UNICODE_CONTROL_KEYS(OP)                                  \
  OP(L'⎋', Key_Escape)                                                         \
  OP(L'⌫', Key_Backspace)                                                      \
  OP(L'↹', Key_Tab)                                                            \
  OP(L'␣', Key_Spacebar)                                                       \
  OP(L'⇪', Key_CapsLock)                                                       \
  OP(L'↩', Key_Return)                                                         \
  OP(L'⎆', Key_Enter)                                                          \
  OP(L'⎙', Key_PrintScreen)                                                    \
  OP(L'⤓', Key_ScrollLock)                                                     \
  OP(L'⇳', Key_ScrollLock)                                                     \
  OP(L'⎉', Key_Pause)                                                          \
  OP(L'⎀', Key_Insert)                                                         \
  OP(L'↖', Key_Home)                                                           \
  OP(L'⇞', Key_PageUp)                                                         \
  OP(L'⌦', Key_Delete)                                                         \
  OP(L'↘', Key_End)                                                            \
  OP(L'⇟', Key_PageDown)                                                       \
  OP(L'→', Key_RightArrow)                                                     \
  OP(L'←', Key_LeftArrow)                                                      \
  OP(L'↓', Key_DownArrow)                                                      \
  OP(L'↑', Key_UpArrow)                                                        \
  OP(L'⇭', Key_KeypadNumLock)                                                  \
  OP(L'⌽', Key_Power)                                                          \
  OP(L'⌃', Key_LeftControl)                                                    \
  OP(L'⌘', Key_LeftGui)                                                        \
  OP(L'⇧', Key_LeftShift)                                                      \
  OP(L'⌥', Key_LeftAlt)

HOST_KEYMAP_UNICODE_CONVERTER(
  _HOST_KEYMAP_UNICODE_CONTROL_KEYS,
  _CharParsingStandardFallback<wchar_t>
)

#undef _HOST_KEYMAP_UNICODE_CONTROL_KEYS

} // namespace control_keys

// If you do not want to use unicode control keys, define
// macro HOST_KEYMAP_NO_UNICODE_CONTROL_CHARS before including
// anything host_keymap related.
//
#ifndef HOST_KEYMAP_NO_UNICODE_CONTROL_CHARS
typedef control_keys::UnicodeConverter CharParsingStandardFallback;
#else
typedef _CharParsingStandardFallback<wchar_t> CharParsingStandardFallback;
#endif

} // namespace unicode
} // namespace host_keymap
} // namespace kaleidoscope
