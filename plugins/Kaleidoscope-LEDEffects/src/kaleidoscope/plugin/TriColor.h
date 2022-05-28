/* -*- mode: c++ -*-
 * Kaleidoscope-LEDEffects -- An assorted collection of LED effects for Kaleidoscope
 * Copyright (C) 2016, 2017  Keyboard.io, Inc
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

#include "kaleidoscope/device/device.h"            // for cRGB
#include "kaleidoscope/plugin.h"                   // for Plugin
#include "kaleidoscope/plugin/LEDMode.h"           // for LEDMode
#include "kaleidoscope/plugin/LEDModeInterface.h"  // for LEDModeInterface

namespace kaleidoscope {
namespace plugin {
class TriColor : public Plugin,
                 public LEDModeInterface {
 public:
  TriColor(cRGB base_color, cRGB mod_color, cRGB esc_color);
  TriColor(cRGB base_color, cRGB mod_color)
    : TriColor(base_color, mod_color, mod_color) {}

  // This class' instance has dynamic lifetime
  //
  class TransientLEDMode : public LEDMode {
   public:
    // Please note that storing the parent ptr is only required
    // for those LED modes that require access to
    // members of their parent class. Most LED modes can do without.
    //
    explicit TransientLEDMode(const TriColor *parent)
      : parent_(parent) {}

   protected:
    void update() final;

   private:
    const TriColor *parent_;
  };

 private:
  cRGB base_color_;
  cRGB mod_color_;
  cRGB esc_color_;
};

}  // namespace plugin
}  // namespace kaleidoscope
