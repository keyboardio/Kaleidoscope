/* -*- mode: c++ -*-
 * Kaleidoscope-TopsyTurvy -- Turn the effect of Shift upside down for certain keys
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

#pragma once

#include <Kaleidoscope.h>
#include <Kaleidoscope-Ranges.h>

#define TOPSY(k) (Key) { .raw = kaleidoscope::ranges::TT_FIRST + (Key_ ## k).keyCode }

namespace kaleidoscope {

class TopsyTurvy: public KaleidoscopePlugin {
 public:
  TopsyTurvy(void);

  void begin(void) final;
 private:
  static uint8_t mod_state_;
  static uint8_t last_pressed_position_;

  static Key eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state);
};

}

extern kaleidoscope::TopsyTurvy TopsyTurvy;
