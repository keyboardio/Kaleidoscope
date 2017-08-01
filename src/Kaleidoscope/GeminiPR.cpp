/* Kaleidoscope-Steno -- Steno protocols for Kaleidoscope
 * Copyright (C) 2017  Joseph Wasson
 * Copyright (C) 2017  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Kaleidoscope-Steno.h>

namespace kaleidoscope {
namespace steno {

  uint8_t GeminiPR::keys_held_;
  uint8_t GeminiPR::state_[6];

  void GeminiPR::begin(void) {
    Kaleidoscope.useEventHandlerHook(eventHandlerHook);
  }

  Key GeminiPR::eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state) {
    if (mapped_key < geminipr::START ||
        mapped_key > geminipr::END)
      return mapped_key;

    if (keyToggledOn(key_state)) {
      uint8_t key = mapped_key.raw - geminipr::START;
      ++keys_held_;

      state_[key / 7] |= 1 << (6 - (key % 7));
    } else if (keyToggledOff(key_state)) {
      --keys_held_;

      if (keys_held_ == 0) {
        state_[0] |= 0x80;
        Serial.write(state_, sizeof(state_));
        memset(state_, 0, sizeof(state_));
      }
    }

    return Key_NoKey;
  }
}
}

kaleidoscope::steno::GeminiPR GeminiPR;
