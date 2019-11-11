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

#include "kaleidoscope/key_defs.h"
#include "kaleidoscope/key_defs_keyboard.h"
#include "kaleidoscope/host_keymap/xkbcommon_keysyms_printable.h"
#include "kaleidoscope/host_keymap/xkbcommon_keysyms_non_printable.h"

//##############################################################################

// Use the USE_HOST_KEYMAP macro in you sketch to make a host_keymap available.
//
#define USE_HOST_KEYMAP(SYSTEM, LAYOUT, VARIANT)                               \
   namespace kaleidoscope {                                                    \
                                                                               \
      template<typename _T>                                                    \
      constexpr Key convertToKey(const _T &t) {                                \
         return kaleidoscope::host_keymap::SYSTEM::LAYOUT::VARIANT             \
            ::convertToKey(t);                                                 \
      }                                                                        \
   }                                                                           \
   using namespace kaleidoscope::host_keymap::SYSTEM;

// Use the following macro e.g. to define convenience macros that help
// to work with different keymaps in the same sketch.
//
#define MAP_WITH_HOST_KEYMAP(SYSTEM, LAYOUT, VARIANT, ...)                     \
   kaleidoscope::host_keymap::SYSTEM::LAYOUT::VARIANT                          \
            ::convertToKey(__VA_ARGS__)

namespace kaleidoscope {
namespace host_keymap {

// A helper class for compile time keymap string parsing.
//
template<typename _CharType, typename _CharProcessor>
class StringMember {
 public:

  typedef _CharType CharType;
  typedef _CharProcessor CharProcessor;
  typedef StringMember<_CharType, _CharProcessor> ThisType;

  template< unsigned _Size >
  constexpr StringMember(const _CharType(&string)[_Size], int pos = 0)
    :  string_{string},
       size_{_Size - 1},
       pos_{pos} {
    static_assert(_Size >= 1, "Not a string literal");
  }

  constexpr StringMember(const _CharType * const string, int size, int pos)
    :  string_{string},
       size_{size},
       pos_{pos}
  {}

  constexpr _CharType toChar() const {
    return string_[pos_];
  }
  constexpr bool isValid() const {
    return pos_ < size_;
  }
  constexpr ThisType next() const {
    return ThisType{string_, size_, pos_ + 1};
  }
  constexpr bool is(_CharType c) {
    return string_[pos_] == c;
  }
  constexpr int length() const {
    return size_;
  }

  constexpr bool isEscapeChar() const {
    return CharProcessor::isEscapeChar(string_[pos_]);
  }

  constexpr bool isSeparator() const {
    return CharProcessor::isSeparator(string_[pos_]);
  }

  constexpr bool isMirrorChar() const {
    return CharProcessor::isMirrorChar(string_[pos_]);
  }

  constexpr bool isPanultimate() const {
    return (pos_ + 2) == size_;
  }
  constexpr bool onlySeparatorsFollow() const {
    return this->isValid()
           ? (this->isSeparator() && this->next().onlySeparatorsFollow())
           :  true;
  }
  constexpr bool isLast() const {
    return (pos_ + 1) == size_;
  }
  constexpr bool isLastNonSeparator() const {
    return this->isValid() && this->next().onlySeparatorsFollow();
  }

 private:

  const _CharType *const  string_;
  int                     size_;
  int                     pos_;
};

#define _IS_MOD(TYPE, HAND) (k.getKeyCode() == Key_##HAND##TYPE.getKeyCode())
#define _IS_ANY_MOD(TYPE) (_IS_MOD(TYPE, Left) || (_IS_MOD(TYPE, Right)))

// TODO: Move this to key_defs.h?
//
constexpr bool isModifierKey(Key k) {
  return    _IS_ANY_MOD(Shift)
            || _IS_ANY_MOD(Control)
            || _IS_ANY_MOD(Alt)
            || _IS_ANY_MOD(Gui);
}

#undef _IS_MOD
#undef _IS_ANY_MOD

#define _MIRROR_MOD(NAME)                                                      \
   (k.getKeyCode() == Key_Left##NAME.getKeyCode())                             \
      ? Key{Key_Right##NAME.getKeyCode(), k.getFlags()}                        \
 : (k.getKeyCode() == Key_Right##NAME.getKeyCode())                            \
      ? Key{Key_Left##NAME.getKeyCode(), k.getFlags()}

// TODO: Move this to key_defs.h?
//
constexpr Key mirrorModifier(Key k) {
  return isModifierKey(k)
         ? (
           _MIRROR_MOD(Shift)
           :  _MIRROR_MOD(Control)
           :  _MIRROR_MOD(Alt)
           :  _MIRROR_MOD(Gui)
           :  bad_keymap_key
         )
         : bad_keymap_key;
}

#undef MIRROR_MOD

template<typename _CharType,
         typename _DerivedConverter,
         typename _CharProcessor>
struct BaseConverter {

  typedef _CharType CharType;
  typedef StringMember<_CharType, _CharProcessor> StringMemberType;

  template<typename _StringMember>
  static constexpr Key charToKey(_StringMember s) {
    return _DerivedConverter::charToKey(s.toChar());
  }

  // This implements the parser logic that is used to parse keymap
  // strings like "c+a".
  //
  template<typename _StringMember>
  static constexpr Key parseKeyDefinitionString(_StringMember s) {

    // C++11 constexpr functions do not allow using if/else clauses.
    // Because of that, we use the ternary operator to express
    // conditionals instead.

    return
      (s.length() == 0)
      ? bad_keymap_key
      : (s.length() == 1)
      ? charToKey(s) // Consider the only character as ascii key
      :  s.isEscapeChar() // Is this an escape character?
      ? (
        s.next().isLastNonSeparator()
        ? charToKey(s.next())
        : bad_keymap_key
      )
      :  s.isSeparator()
      ?  parseKeyDefinitionString(s.next()) // Ignore separator keys
      :  s.isLastNonSeparator()
      ?  charToKey(s) // The last key is treated as ASCII key
      :  s.isMirrorChar()
      ? (
        !s.isLastNonSeparator()
        ? mirrorModifier(parseKeyDefinitionString(s.next()))
        : bad_keymap_key
      )
      :  _DerivedConverter::addModifier( // s must be a modifier key
        parseKeyDefinitionString(s.next()), // Recurse (continue parsing the remaining string)
        s // This character must be a modifier token
      );
  }

  // This function allows to use character strings to define keys.
  //
  template<int _Size>
  static constexpr Key convertToKey(_CharType const(&string) [_Size]) {
    return _DerivedConverter
           ::parseKeyDefinitionString(StringMemberType{string});
  }

  static constexpr Key convertToKey(_CharType c) {
    return _DerivedConverter::charToKey(c);
  }
};

template<typename _CharType>
struct _CharParsingStandardFallback {
  static constexpr bool isKeyChar(_CharType c) {
    return false;
  }
  static constexpr Key charToKey(_CharType c) {
    return bad_keymap_key;
  }
};

namespace linux {

enum class XKBKeySym {
#define _HOST_KEYMAP_INIT_NON_PRINTABLE_SYMBOLS_ENUM(XKB_KEY_SYM, KEY_EVENT, DESC) \
   XKB_KEY_SYM,
#define _HOST_KEYMAP_INIT_PRINTABLE_SYMBOLS_ENUM(XKB_KEY_SYM, KEY_EVENT, UNICODE, DESC) \
   XKB_KEY_SYM,

  XKB_KEY_SYM_NAME__KEY_SYM__DESCRIPTION(_HOST_KEYMAP_INIT_NON_PRINTABLE_SYMBOLS_ENUM)
  XKB_KEY_NAME__KEY_SYM__UNICODE__DESC(_HOST_KEYMAP_INIT_PRINTABLE_SYMBOLS_ENUM)
  None
};
} // namespace linux

} // namespace host_keymap
} // namespace kaleidoscope

#define _HOST_KEYMAP_CHECK_IF_MODIFIER(CHAR, OPERATION) s.is((CHAR)) ? true :
#define _HOST_KEYMAP_ADD_MODIFIER(CHAR, OPERATION) s.is((CHAR)) ? (OPERATION) :

#define _HOST_KEYMAP_IS_KEY_CHAR(KI, KO) (c == (KI)) ? true :
#define _HOST_KEYMAP_MAP_KEY_CHAR_TO_KALEIDOSCOPE_KEY(KI, KO) (c == (KI)) ? (KO) :

#define _HOST_KEYMAP_DEFINE_CHAR_CONVERTER(NAME, CHAR_TYPE, MODIFIER_MAP)      \
   template<typename _DerivedConverter, typename _CharProcessor>               \
   struct NAME                                                                 \
      : public BaseConverter<CHAR_TYPE, _DerivedConverter, _CharProcessor>     \
   {                                                                           \
      typedef BaseConverter<CHAR_TYPE, _DerivedConverter, _CharProcessor>      \
         Parent;                                                               \
                                                                               \
      using typename Parent::StringMemberType;                                 \
      using typename Parent::CharType;                                         \
                                                                               \
      static constexpr bool isModifierKey(StringMemberType s) {                \
         return MODIFIER_MAP(_HOST_KEYMAP_CHECK_IF_MODIFIER)                   \
               false;                                                          \
      }                                                                        \
                                                                               \
      static constexpr Key addModifier(Key k, StringMemberType s) {            \
         return (k == bad_keymap_key)                                          \
                  ? bad_keymap_key /* If the input key is already */           \
                                   /* the bad_keymap_key we do not */          \
                                   /* touch it here. */                        \
               : MODIFIER_MAP(_HOST_KEYMAP_ADD_MODIFIER)                       \
                 bad_keymap_key;                                               \
      }                                                                        \
   };
