/* -*- mode: c++ -*-
 * Kaleidoscope-Idle-LEDs -- Turn off the LEDs when the keyboard's idle
 * Copyright (C) 2018  Keyboard.io, Inc
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


// TODO: Including EEPROM-Settings should be eliminated by
//       adding additional methods to the storage controller API
//
#include <Kaleidoscope-EEPROM-Settings.h>

#include <Kaleidoscope-FocusSerial.h>

namespace kaleidoscope {

// TODO: Give the FocusEEPROMWrapper(s) an appropriate names.
//
template<typename _DatumType,
         typename _StoredType,
         typename _FocusType>
class BaseFocusEEPROMWrapper {
 public:

  typedef BaseFocusEEPROMWrapper<_DatumType, _StoredType, _FocusType> ThisType;

  typedef _DatumType DatumType;
  typedef _StoredType StoredType;
  typedef _FocusType FocusType;

  BaseFocusEEPROMWrapper(const StoredType datum,
                         const char *focus_string)
    :  focus_string_{focus_string},
       datum_{datum}
  {}

  void setup() {

    // TODO: Shouldn't EEPROMSettings be wrapped by
    //       KeyboardHardware.storage()?
    focus_address_ = ::EEPROMSettings.requestSlice(sizeof(FocusType));

    // Check if the value is initialized and initialize it if necessary
    // TODO: Have a storage command that enables this
    //       KeyboardHardware.storage().isInitialized(focus_address_);
    //       (checks for uninitialized EEPROM, e.g. FFFF...)
    //
    //if(!KeyboardHardware.storage().isInitialized(focus_address_)) {
    //   KeyboardHardware.storage().put(focus_address_, datum_);
    //}

    KeyboardHardware.storage().get(focus_address_, datum_);
  }

  bool focusRead(const char *command) {
    if (::Focus.handleHelp(command, focus_string_))
      return false;

    if (strcmp_P(command, focus_string_) != 0)
      return false;

    if (::Focus.isEOL()) {
      ::Focus.send((FocusType)datum_);
    } else {
      FocusType tmpDatum;
      ::Focus.read(tmpDatum);
      datum_ = tmpDatum;
    }

    return true;
  }

  ThisType &operator=(DatumType datum) {
    datum_ = datum;
    this->store();
    return *this;
  }

  void store() {
    KeyboardHardware.storage().put(focus_address_, datum_);
  }

  operator DatumType() {
    return datum_;
  }

 private:

  StoredType datum_;
  uint16_t focus_address_;
  const char *focus_string_;
};

// Use this template if you intent to add Focus/EEPROM capability
// to a member of a base class. In this case the wrapper stores a
// reference to the member.
//
template<typename _DatumType, typename _FocusType = _DatumType>
using ReferenceFocusEEPROMWrapper
  = BaseFocusEEPROMWrapper<_DatumType, _DatumType&, _FocusType>;

// Use this template if you want to have a variable that is directly
// wrapped (stored inside the wrapper). In this case the wrapper
// stores the datum (by value).
//
template<typename _DatumType, typename _FocusType = _DatumType>
using ValueFocusEEPROMWrapper
  = BaseFocusEEPROMWrapper<_DatumType, _DatumType, _FocusType>;

} // namespace kaleidoscope

// Due to the way the PSTR macro is designed, it does not allow
// for being used at global scope. That's why we have to
// add a temporary lambda function to define a macro that can be evaluated
// anywhere.
//
#define PSTR_ANYWHERE(S)                                                 \
   []() -> const char * {                                                \
      static const char tmp[] PROGMEM = (S);                             \
      return &tmp[0];                                                    \
   }()
