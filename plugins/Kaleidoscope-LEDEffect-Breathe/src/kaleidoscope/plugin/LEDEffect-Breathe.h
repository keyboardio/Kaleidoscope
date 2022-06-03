/* Kaleidoscope-LEDEffect-Breathe - A breathing effect on the LEDs, for Kaleidoscope.
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

#include <stdint.h>  // for uint8_t

#include "kaleidoscope/plugin.h"                   // for Plugin
#include "kaleidoscope/plugin/LEDMode.h"           // for LEDMode
#include "kaleidoscope/plugin/LEDModeInterface.h"  // for LEDModeInterface

namespace kaleidoscope {
namespace plugin {
class LEDBreatheEffect : public Plugin,
                         public LEDModeInterface {
 public:
  uint8_t hue        = 170;
  uint8_t saturation = 255;

  // This class' instance has dynamic lifetime
  //
  class TransientLEDMode : public LEDMode {
   public:
    // Please note that storing the parent ptr is only required
    // for those LED modes that require access to
    // members of their parent class. Most LED modes can do without.
    //
    explicit TransientLEDMode(const LEDBreatheEffect *parent)
      : parent_(parent) {}

   protected:
    void update() final;

   private:
    const LEDBreatheEffect *parent_;

    static constexpr uint8_t update_interval_ = 50;
    uint8_t last_update_                      = 0;
  };
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::LEDBreatheEffect LEDBreatheEffect;
