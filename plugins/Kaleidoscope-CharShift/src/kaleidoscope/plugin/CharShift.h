/* -*- mode: c++ -*-
 * Kaleidoscope-CharShift -- Independently assign shifted and unshifted symbols
 * Copyright (C) 2021  Keyboard.io, Inc
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

#include "kaleidoscope/Runtime.h"

#include <Kaleidoscope-Ranges.h>

namespace kaleidoscope {
namespace plugin {

namespace charshift {
class Storage;
}

// =============================================================================
/// Kaleidoscope plugin for independently assigning shifted symbols
///
/// This plugin provides a way to define keys that, when `shift` is held, will
/// produce different symbols than they normally would.  Either, both, or
/// neither of the symbols assigned to a key in this way can be ones that are
/// normally produced with the `shift` modifier.  Crucially, when `shift` is
/// held, and the `Key` to be output does not have the `shift` modifier flag
/// applied to it, the held `shift` modifier will be suppressed.
class CharShift : public Plugin {

 public:
  EventHandlerResult onSetup();
  EventHandlerResult onNameQuery();
  EventHandlerResult onKeyEvent(KeyEvent &event);
  EventHandlerResult beforeReportingState(const KeyEvent &event);
  EventHandlerResult onFocusEvent(const char *command);

  // ---------------------------------------------------------------------------
  /// A structure that stores CharShift key pair values
  ///
  /// This structure stores two separate `Key` values: `lower` (to be used when
  /// a key is pressed without `shift` held), and `upper` (to be used when
  /// `shift` is held).
  struct KeyPair {
    Key lower;
    Key upper;

    /// Constructor for use in PROGMEM
    ///
    /// This constructor is used when defining an array of `KeyPair` objects in
    /// PROGMEM (though it can also be used elsewhere).
    constexpr KeyPair(Key lower, Key upper) : lower(lower), upper(upper) {}

    /// Constructor for reading from PROGMEM
    ///
    /// This constructor is used by the helper function that copies values from
    /// PROGMEM so that they can be used normally.
    KeyPair() = default;
  };

  charshift::Storage &storage() {
    return *storage_;
  }
  void setStorage(charshift::Storage *st) {
    storage_ = st;
  }

 private:
  static charshift::Storage *storage_;

  // If a `shift` key needs to be suppressed in `beforeReportingState()`
  static bool reverse_shift_state_;

  /// Test for keys that should be handled by CharShift
  static bool isCharShiftKey(Key key);

  /// Look up the `KeyPair` specified by the given keymap entry
  static KeyPair decodeCharShiftKey(Key key);
};

namespace charshift {

class Storage {
 public:
  Storage() {}

  static uint8_t numKeyPairs() {
    return num_keypairs_;
  }
  static CharShift::KeyPair readKeyPair(uint8_t n) {
    return CharShift::KeyPair(Key_NoKey, Key_NoKey);
  }

  EventHandlerResult onFocusEvent(const char *command) {
    return EventHandlerResult::OK;
  }

 protected:
  static uint8_t num_keypairs_;
};

class ProgmemStorage: public Storage {
 public:

  static CharShift::KeyPair readKeyPair(uint8_t n);

  /// Configure the KeyPairs array in PROGMEM
  ///
  /// This function configures the PROGMEM array of `KeyPair` objects,
  /// automatically setting the internal count variable from the size of the
  /// `keypairs` array given, which must be a fixed-sized array, not a pointer.
  /// Generally, it will be called via the `KEYPAIRS()` preprocessor macro, not
  /// directly by user code.
  template <uint8_t _num_keypairs>
  static void setKeyPairs(CharShift::KeyPair const(&keypairs)[_num_keypairs]) {
    keypairs_ = keypairs;
    num_keypairs_ = _num_keypairs;
  }

 private:
  // A pointer to an array of `KeyPair` objects in PROGMEM
  static CharShift::KeyPair const *keypairs_;
};

class EEPROMStorage: public Storage {
 public:
  void setup(uint8_t num_pairs);
  static CharShift::KeyPair readKeyPair(uint8_t n);

  EventHandlerResult onFocusEvent(const char *command);
 private:
  static uint16_t storage_base_;
};

}

} // namespace plugin
} // namespace kaleidoscope

extern kaleidoscope::plugin::CharShift CharShift;

/// Define an array of `KeyPair` objects in PROGMEM
///
/// This preprocessor macro takes a list of `KeyPair` objects as arguments, and
/// defines them as an array in PROGMEM, and configures `CharShift` to use that
/// array, automatically setting the count correctly to prevent out-of-bounds
/// lookups.
#define CS_KEYS(keypairs...) {                                          \
    static kaleidoscope::plugin::CharShift::KeyPair const kp_table[] PROGMEM = { \
      keypairs                                                          \
    };                                                                  \
    CharShift.storage().setKeyPairs(kp_table);                          \
}

/// Define an `KeyPair` entry in a keymap
///
/// This defines a `Key` object that will be handled by the CharShift plugin.
/// The argument `n` is the index number of the `KeyPair` in the array (starting
/// at zero).
constexpr kaleidoscope::Key CS(uint8_t n) {
  return kaleidoscope::Key(kaleidoscope::ranges::CS_FIRST + n);
}
