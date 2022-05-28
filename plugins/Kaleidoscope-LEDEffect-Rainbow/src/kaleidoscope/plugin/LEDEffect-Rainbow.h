/* Kaleidoscope-LEDEffect-Rainbow - Rainbow LED effects for Kaleidoscope.
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

#include <stdint.h>  // for uint8_t, uint16_t

#include "kaleidoscope/plugin.h"                   // for Plugin
#include "kaleidoscope/plugin/LEDMode.h"           // for LEDMode
#include "kaleidoscope/plugin/LEDModeInterface.h"  // for LEDModeInterface

namespace kaleidoscope {
namespace plugin {
class LEDRainbowEffect : public Plugin,
                         public LEDModeInterface {
 public:
  void brightness(uint8_t);
  uint8_t brightness() {
    return rainbow_value;
  }
  void update_delay(uint8_t);
  uint8_t update_delay() {
    return rainbow_update_delay;
  }

  // This class' instance has dynamic lifetime
  //
  class TransientLEDMode : public LEDMode {
   public:
    // Please note that storing the parent ptr is only required
    // for those LED modes that require access to
    // members of their parent class. Most LED modes can do without.
    //
    explicit TransientLEDMode(const LEDRainbowEffect *parent)
      : parent_(parent) {}

    void update() final;

   private:
    const LEDRainbowEffect *parent_;

    uint16_t rainbow_hue = 0;  //  stores 0 to 614

    uint8_t rainbow_steps       = 1;  //  number of hues we skip in a 360 range per update
    uint8_t rainbow_last_update = 0;

    uint8_t rainbow_saturation = 255;
  };

 private:
  uint8_t rainbow_update_delay = 40;  // delay between updates (ms)
  uint8_t rainbow_value        = 50;
};


class LEDRainbowWaveEffect : public Plugin, public LEDModeInterface {
 public:
  void brightness(uint8_t);
  uint8_t brightness() {
    return rainbow_value;
  }
  void update_delay(uint8_t);
  uint8_t update_delay() {
    return rainbow_update_delay;
  }

  // This class' instance has dynamic lifetime
  //
  class TransientLEDMode : public LEDMode {
   public:
    // Please note that storing the parent ptr is only required
    // for those LED modes that require access to
    // members of their parent class. Most LED modes can do without.
    //
    explicit TransientLEDMode(const LEDRainbowWaveEffect *parent)
      : parent_(parent) {}

    void update() final;

   private:
    const LEDRainbowWaveEffect *parent_;

    uint16_t rainbow_hue = 0;  //  stores 0 to 614

    uint8_t rainbow_wave_steps  = 1;  //  number of hues we skip in a 360 range per update
    uint8_t rainbow_last_update = 0;

    uint8_t rainbow_saturation = 255;
  };

  uint8_t rainbow_update_delay = 40;  // delay between updates (ms)
  uint8_t rainbow_value        = 50;
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::LEDRainbowEffect LEDRainbowEffect;
extern kaleidoscope::plugin::LEDRainbowWaveEffect LEDRainbowWaveEffect;
