/* Kaleidoscope-Steno -- Steno protocols for Kaleidoscope
 * Copyright (C) 2017  Joseph Wasson
 * Copyright (C) 2017, 2018  Keyboard.io, Inc.
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

#include <Kaleidoscope-Steno.h>
#include "kaleidoscope/keyswitch_state.h"

namespace kaleidoscope {
namespace plugin {
namespace steno {

uint8_t GeminiPR::keys_held_;
uint8_t GeminiPR::state_[6];

EventHandlerResult GeminiPR::onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t keyState) {
  if (mapped_key < geminipr::START ||
      mapped_key > geminipr::END)
    return EventHandlerResult::OK;

  if (keyToggledOn(keyState)) {
    uint8_t key = mapped_key.getRaw() - geminipr::START;
    ++keys_held_;

    state_[key / 7] |= 1 << (6 - (key % 7));
  } else if (keyToggledOff(keyState)) {
    --keys_held_;

    if (keys_held_ == 0) {
      state_[0] |= 0x80;
      Runtime.serialPort().write(state_, sizeof(state_));
      memset(state_, 0, sizeof(state_));
    }
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

}
}
}

kaleidoscope::plugin::steno::GeminiPR GeminiPR;
