/* -*- mode: c++ -*-
 * Kaleidoscope-LED-AlphaSquare -- 4x4 pixel LED alphabet
 * Copyright (C) 2017, 2018  Keyboard.io, Inc
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

#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>

namespace kaleidoscope {
namespace plugin {
class AlphaSquareEffect : public LEDMode {
 public:
  AlphaSquareEffect(void) {}

  static uint16_t length;

  EventHandlerResult onKeyswitchEvent2(Key &mappedKey, KeyAddr key_addr, uint8_t keyState);

 protected:
  void update(void) final;
  void refreshAt(KeyLEDAddr led_addr) final;
  KS_ROW_COL_FUNC void refreshAt(byte row, byte col) final {
    refreshAt(KeyLEDAddr(row, col));
  }

 private:
  static uint32_t end_time_left_, end_time_right_;
  static Key last_key_left_, last_key_right_;
};
}
}

extern kaleidoscope::plugin::AlphaSquareEffect AlphaSquareEffect;
