/* DynamicTapDance -- Dynamic TapDance support for Kaleidoscope
 * Copyright (C) 2019  Keyboard.io, Inc
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

#include "Kaleidoscope-DynamicTapDance.h"

#include <Kaleidoscope-EEPROM-Settings.h>
#include "Kaleidoscope-FocusSerial.h"

namespace kaleidoscope {
namespace plugin {

uint16_t DynamicTapDance::storage_base_;
uint16_t DynamicTapDance::storage_size_;
uint16_t DynamicTapDance::map_[];
uint8_t DynamicTapDance::offset_;
uint8_t DynamicTapDance::dance_count_;
constexpr uint8_t DynamicTapDance::reserved_tap_dance_key_count_;

void DynamicTapDance::updateDynamicTapDanceCache() {
  uint16_t pos = storage_base_;
  uint8_t current_id = 0;
  bool previous_dance_ended = false;

  dance_count_ = 0;
  map_[0] = 0;

  while (pos < storage_base_ + storage_size_) {
    uint16_t raw_key = Kaleidoscope.storage().read(pos);
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

bool DynamicTapDance::dance(uint8_t tap_dance_index, KeyAddr key_addr,
                            uint8_t tap_count, TapDance::ActionType tap_dance_action) {
  uint16_t pos = map_[tap_dance_index - offset_] + ((tap_count - 1) * 2);
  uint16_t next_pos = map_[tap_dance_index - offset_ + 1];
  if (next_pos <= pos || (tap_dance_index > offset_ + dance_count_))
    return false;

  Key key;
  Kaleidoscope.storage().get(storage_base_ + pos, key);

  switch (tap_dance_action) {
  case TapDance::Tap:
    break;
  case TapDance::Interrupt:
  case TapDance::Timeout:
    handleKeyswitchEvent(key, key_addr, IS_PRESSED | INJECTED);
    break;
  case TapDance::Hold:
    handleKeyswitchEvent(key, key_addr, IS_PRESSED | WAS_PRESSED | INJECTED);
    break;
  case TapDance::Release:
    kaleidoscope::Runtime.hid().keyboard().sendReport();
    handleKeyswitchEvent(key, key_addr, WAS_PRESSED | INJECTED);
    break;
  }

  return true;
}

EventHandlerResult DynamicTapDance::onFocusEvent(const char *command) {
  if (::Focus.handleHelp(command, PSTR("tapdance.map")))
    return EventHandlerResult::OK;

  if (strncmp_P(command, PSTR("tapdance."), 9) != 0)
    return EventHandlerResult::OK;

  if (strcmp_P(command + 9, PSTR("map")) == 0) {
    if (::Focus.isEOL()) {
      for (uint16_t i = 0; i < storage_size_; i += 2) {
        Key k;
        Kaleidoscope.storage().get(storage_base_ + i, k);
        ::Focus.send(k);
      }
    } else {
      uint16_t pos = 0;

      while (!::Focus.isEOL()) {
        Key k;
        ::Focus.read(k);

        Kaleidoscope.storage().put(storage_base_ + pos, k);
        pos += 2;
      }
      Kaleidoscope.storage().commit();
      updateDynamicTapDanceCache();
    }
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

void DynamicTapDance::setup(uint8_t dynamic_offset, uint16_t size) {
  storage_base_ = ::EEPROMSettings.requestSlice(size);
  storage_size_ = size;
  offset_ = dynamic_offset;
  updateDynamicTapDanceCache();
}

}
}

kaleidoscope::plugin::DynamicTapDance DynamicTapDance;
