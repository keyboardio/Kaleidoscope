/* -*- mode: c++ -*-
 * Kaleidoscope-LED-ActiveModColor -- Light up the LEDs under the active modifiers
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

#include <Kaleidoscope-LED-ActiveModColor.h>
#include <Kaleidoscope-Ranges.h>

namespace KaleidoscopePlugins {
namespace LEDEffects {
cRGB ActiveModColorEffect::highlightColor = (cRGB) {
  0xff, 0xff, 0xff
};

ActiveModColorEffect::ActiveModColorEffect(void) {
}

void
ActiveModColorEffect::begin(void) {
  loop_hook_use(loopHook);
}

void
ActiveModColorEffect::configure(const cRGB highlightColor_) {
  highlightColor = highlightColor_;
}

void
ActiveModColorEffect::loopHook(bool postClear) {
  if (postClear)
    return;

  for (byte r = 0; r < ROWS; r++) {
    for (byte c = 0; c < COLS; c++) {
      Key k = Layer.lookup(r, c);

      if (k.raw >= KaleidoscopePlugins::Ranges::OSM_FIRST && k.raw <= KaleidoscopePlugins::Ranges::OSM_LAST) {
        uint8_t idx = k.raw - KaleidoscopePlugins::Ranges::OSM_FIRST;
        k.flags = 0;
        k.keyCode = Key_LeftControl.keyCode + idx;
      }

      if (k.raw < Key_LeftControl.raw || k.raw > Key_RightGui.raw)
        continue;

      if (Keyboard.isModifierActive(k.keyCode))
        LEDControl.led_set_crgb_at(r, c, highlightColor);
    }
  }
}

};
};

KaleidoscopePlugins::LEDEffects::ActiveModColorEffect ActiveModColorEffect;
