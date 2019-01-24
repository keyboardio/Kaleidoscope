/* Kaleidoscope-LEDEffect-Chase - A Chase LED effect for Kaleidoscope.
 * Copyright (C) 2017-2019  Keyboard.io, Inc.
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
#include "Kaleidoscope-UnderglowControl.h"

namespace kaleidoscope {
namespace plugin {

class LEDChaseEffect : public LEDMode {
 public:
  LEDChaseEffect(void) {}

  uint16_t update_delay() {
    return update_delay_;
  }
  void update_delay(uint16_t delay) {
    update_delay_ = delay;
  }
  uint8_t distance() {
    return distance_;
  }
  void distance(uint8_t value) {
    distance_ = value;
  }

 protected:
  void update(void) final;

 private:
  int8_t pos_ = 0;
  int8_t direction_ = 1;
  uint8_t distance_ = 5;
  uint16_t update_delay_ = 150;
  uint16_t last_update_ = 0;
};

class UnderglowChaseEffect : public UnderglowEffect {
 public:
  UnderglowChaseEffect(void) {}

  uint16_t update_delay() {
    return update_delay_;
  }
  void update_delay(uint16_t delay) {
    update_delay_ = delay;
  }
  uint8_t distance() {
    return distance_;
  }
  void distance(uint8_t value) {
    distance_ = value;
  }

 protected:
  void update(void) final;

 private:
  int8_t pos_ = 0;
  int8_t direction_ = 1;
  uint8_t distance_ = 2;
  uint16_t update_delay_ = 150;
  uint16_t last_update_ = 0;
};

}
}

extern kaleidoscope::plugin::LEDChaseEffect LEDChaseEffect;
extern kaleidoscope::plugin::UnderglowChaseEffect UnderglowChaseEffect;
