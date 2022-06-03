/* Kaleidoscope-LEDEffect-SolidColor - Solid color LED effects for Kaleidoscope.
 * Copyright (C) 2017  Keyboard.io, Inc.
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

#include "kaleidoscope/KeyAddr.h"                  // for KeyAddr
#include "kaleidoscope/plugin.h"                   // for Plugin
#include "kaleidoscope/plugin/LEDMode.h"           // for LEDMode
#include "kaleidoscope/plugin/LEDModeInterface.h"  // for LEDModeInterface

namespace kaleidoscope {
namespace plugin {
class LEDSolidColor : public Plugin,
                      public LEDModeInterface {
 public:
  LEDSolidColor(uint8_t r, uint8_t g, uint8_t b)
    : r_(r), g_(g), b_(b) {}

  // This class' instance has dynamic lifetime
  //
  class TransientLEDMode : public LEDMode {
   public:
    // Please note that storing the parent ptr is only required
    // for those LED modes that require access to
    // members of their parent class. Most LED modes can do without.
    //
    explicit TransientLEDMode(const LEDSolidColor *parent)
      : parent_(parent) {}

   protected:
    void onActivate() final;
    void refreshAt(KeyAddr key_addr) final;

   private:
    const LEDSolidColor *parent_;
  };

 private:
  uint8_t r_, g_, b_;
};

}  // namespace plugin
}  // namespace kaleidoscope
