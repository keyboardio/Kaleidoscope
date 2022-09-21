/* DynamicTapDance -- Dynamic TapDance support for Kaleidoscope
 * Copyright (C) 2019, 2021  Keyboard.io, Inc
 * Copyright (C) 2019  Dygma Lab S.L.
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

#include "kaleidoscope/plugin/DynamicTapDance.h"

#include <Arduino.h>                       // for PSTR, F, __FlashStringHelper
#include <Kaleidoscope-EEPROM-Settings.h>  // for EEPROMSettings
#include <Kaleidoscope-FocusSerial.h>      // for Focus, FocusSerial
#include <stdint.h>                        // for uint16_t, uint8_t

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"         // for VirtualProps::Storage, Base<>::Storage
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/key_defs.h"              // for Key, Key_NoKey
#include "kaleidoscope/keyswitch_state.h"       // for IS_PRESSED, WAS_PRESSED
#include "kaleidoscope/plugin/TapDance.h"       // for TapDance, TapDance::ActionType, TapDance:...

namespace kaleidoscope {
namespace plugin {

void DynamicTapDance::updateDynamicTapDanceCache() {
  uint16_t pos              = storage_base_;
  uint8_t current_id        = 0;
  bool previous_dance_ended = false;

  dance_count_ = 0;
  map_[0]      = 0;

  while (pos < storage_base_ + storage_size_) {
    uint16_t raw_key = Runtime.storage().read(pos);
    pos += 2;
    Key key(raw_key);

    if (key == Key_NoKey) {
      map_[++current_id] = pos - storage_base_;

      if (previous_dance_ended)
        return;

      dance_count_++;
      previous_dance_ended = true;
    } else {
      previous_dance_ended = false;
    }
  }
}

bool DynamicTapDance::dance(uint8_t tap_dance_index, KeyAddr key_addr, uint8_t tap_count, TapDance::ActionType tap_dance_action) {
  uint16_t pos      = map_[tap_dance_index - offset_] + ((tap_count - 1) * 2);
  uint16_t next_pos = map_[tap_dance_index - offset_ + 1];
  if (next_pos <= pos || (tap_dance_index > offset_ + dance_count_))
    return false;

  Key key;
  Runtime.storage().get(storage_base_ + pos, key);

  switch (tap_dance_action) {
  case TapDance::Tap:
    break;
  case TapDance::Interrupt:
  case TapDance::Timeout:
    Runtime.handleKeyEvent(KeyEvent(key_addr, IS_PRESSED, key));
    break;
  case TapDance::Hold:
    Runtime.handleKeyEvent(KeyEvent(key_addr, IS_PRESSED | WAS_PRESSED, key));
    break;
  case TapDance::Release:
    //kaleidoscope::Runtime.hid().keyboard().sendReport();
    Runtime.handleKeyEvent(KeyEvent(key_addr, WAS_PRESSED, key));
    break;
  }

  return true;
}

EventHandlerResult DynamicTapDance::onNameQuery() {
  return ::Focus.sendName(F("DynamicTapDance"));
}

EventHandlerResult DynamicTapDance::onFocusEvent(const char *input) {
  const char *cmd_map = PSTR("tapdance.map");

  if (::Focus.inputMatchesHelp(input))
    return ::Focus.printHelp(cmd_map);

  if (!::Focus.inputMatchesCommand(input, cmd_map))
    return EventHandlerResult::OK;

  if (::Focus.isEOL()) {
    for (uint16_t i = 0; i < storage_size_; i += 2) {
      Key k;
      Runtime.storage().get(storage_base_ + i, k);
      ::Focus.send(k);
    }
  } else {
    uint16_t pos = 0;

    while (!::Focus.isEOL()) {
      Key k;
      ::Focus.read(k);

      Runtime.storage().put(storage_base_ + pos, k);
      pos += 2;
    }
    Runtime.storage().commit();
    updateDynamicTapDanceCache();
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

void DynamicTapDance::setup(uint8_t dynamic_offset, uint16_t size) {
  storage_base_ = ::EEPROMSettings.requestSlice(size);
  storage_size_ = size;
  offset_       = dynamic_offset;
  updateDynamicTapDanceCache();
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::DynamicTapDance DynamicTapDance;
