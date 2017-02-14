/* -*- mode: c++ -*-
 * Kaleidoscope-LEDEffects -- An assorted collection of LED effects for Kaleidoscope
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
#include <Kaleidoscope-LEDControl.h>

namespace KaleidoscopePlugins {
  namespace LEDEffects {
    class TriColor : public LEDMode {
    public:
      TriColor (cRGB baseColor, cRGB modColor, cRGB escColor);
      TriColor (cRGB baseColor, cRGB modColor) : TriColor (baseColor, modColor, modColor) {};

      virtual void update (void) final;

    private:
      cRGB baseColor;
      cRGB modColor;
      cRGB escColor;
    };
  };
};
