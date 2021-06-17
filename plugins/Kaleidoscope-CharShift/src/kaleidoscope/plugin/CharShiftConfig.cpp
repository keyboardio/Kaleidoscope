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

#include "kaleidoscope/plugin/CharShift.h"

#include <Kaleidoscope-FocusSerial.h>
#include <Kaleidoscope-EEPROM-Settings.h>

#include "kaleidoscope/Runtime.h"

namespace kaleidoscope {
namespace plugin {

// =============================================================================
// CharShiftConfig class variables

uint8_t CharShiftConfig::max_pairs_;
uint8_t CharShiftConfig::dynamic_offset_;
uint16_t CharShiftConfig::storage_base_;

// =============================================================================
// Event handlers

EventHandlerResult CharShiftConfig::onNameQuery() {
  return ::Focus.sendName(F("CharShiftConfig"));
}

EventHandlerResult CharShiftConfig::onFocusEvent(const char *command) {
  if (::Focus.handleHelp(command, PSTR("charshift.map")))
    return EventHandlerResult::OK;

  if (strcmp_P(command, PSTR("charshift.map")) != 0)
    return EventHandlerResult::OK;

  if (::Focus.isEOL()) {
    // We dump key by key, rather than pairs, because the end result is the
    // same, and dumping one by one is less code.
    for (uint16_t i = 0; i < max_pairs_ * 2; i += 2) {
      Key k;

      Runtime.storage().get(storage_base_ + i, k);
      ::Focus.send(k);
    }
  } else {
    uint16_t pos = 0;

    // We read one key at a time, rather than a keypair, to better handle
    // partials and failure, and to make the code simpler.
    while (!::Focus.isEOL()) {
      Key k;

      ::Focus.read(k);
      Runtime.storage().put(storage_base_ + pos, k);
      pos += 2;
    }
    Runtime.storage().commit();
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

// =============================================================================
// Support functions

void CharShiftConfig::setup(uint8_t dynamic_offset, uint8_t max_pairs) {
  dynamic_offset_ = dynamic_offset;
  max_pairs_ = max_pairs;

  storage_base_ = ::EEPROMSettings.requestSlice(max_pairs * 4);
  ::CharShift.setNumKeyPairsFunction(numPairs);
  ::CharShift.setReadKeyPairFunction(readKeyPair);
}

CharShift::KeyPair CharShiftConfig::readKeyPairFromEEPROM(uint8_t n) {
  uint16_t pos = storage_base_ + n * 4; // 4: Size of a keypair.
  uint16_t raw_lower = Runtime.storage().read(pos);
  uint16_t raw_upper = Runtime.storage().read(pos + 2);

  return CharShift::KeyPair(Key(raw_lower), Key(raw_upper));
}

uint8_t CharShiftConfig::numPairs() {
  return CharShift::numProgmemKeyPairs() + numEEPROMPairs();
}

CharShift::KeyPair CharShiftConfig::readKeyPair(uint8_t n) {
  if (n < dynamic_offset_) {
    return CharShift::readKeyPairFromProgmem(n);
  }
  return readKeyPairFromEEPROM(n - dynamic_offset_);
}

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::CharShiftConfig CharShiftConfig;
