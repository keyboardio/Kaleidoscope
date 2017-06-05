/* -*- mode: c++ -*-
 * Kaleidoscope-SpaceCadet -- Space Cadet Shift
 * Copyright (C) 2016, 2017  Gergely Nagy
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

namespace kaleidoscope {

class SpaceCadetShift : public KaleidoscopePlugin {
 public:
  SpaceCadetShift(void);

  void begin(void) final;

  static uint16_t time_out;
  static Key opening_paren, closing_paren;

 private:
  static uint8_t paren_needed_;
  static uint32_t start_time_;

  static Key eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state);
};
};

extern kaleidoscope::SpaceCadetShift SpaceCadetShift;
