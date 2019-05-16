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

namespace kaleidoscope {
namespace plugin {
class LEDChaseEffect : public Plugin,
  public LEDModeInterface {
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

  // This class' instance has dynamic lifetime
  //
  class TransientLEDMode : public LEDMode {
   public:

    // Please note that storing the parent ptr is only required
    // for those LED modes that require access to
    // members of their parent class. Most LED modes can do without.
    //
    TransientLEDMode(const LEDChaseEffect *parent)
      : parent_(parent) {}

   protected:

    virtual void update() final;

   private:

    const LEDChaseEffect *parent_;

    int8_t pos_ = uint8_t(0);
    int8_t direction_ = 1;
    uint16_t last_update_ = 0;
  };

 private:
  uint8_t distance_ = 5;
  uint16_t update_delay_ = 150;
};
}
}

extern kaleidoscope::plugin::LEDChaseEffect LEDChaseEffect;
