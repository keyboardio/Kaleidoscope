/* Kaleidoscope-LEDEffect-Chase - A Chase LED effect for Kaleidoscope.
 * Copyright (C) 2017-2018  Keyboard.io, Inc.
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

#include "Kaleidoscope-LEDControl.h"
#include "LEDUtils.h"

namespace kaleidoscope {
class LEDChaseEffect : public LEDMode {
 public:
  LEDChaseEffect(void) {}

 protected:
  void update(void) final;

 private:
  uint8_t pos = 0;
  int8_t chase_sign = 1; //negative values when it's going backwar
  uint8_t chase_pixels = 5;
  uint8_t current_chase_counter = 0;
  static const uint8_t chase_threshold = 150;
};
}

extern kaleidoscope::LEDChaseEffect LEDChaseEffect;
