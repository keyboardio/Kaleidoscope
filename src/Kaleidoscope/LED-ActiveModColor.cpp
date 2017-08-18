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
#include <Kaleidoscope-OneShot.h>
#include <kaleidoscope/hid.h>

namespace kaleidoscope {

cRGB ActiveModColorEffect::highlight_color = (cRGB) {
  0xff, 0xff, 0xff
};

void ActiveModColorEffect::begin(void) {
  Kaleidoscope.useLoopHook(loopHook);
}

void ActiveModColorEffect::loopHook(bool is_post_clear) {
  if (is_post_clear)
    return;

  for (byte r = 0; r < ROWS; r++) {
    for (byte c = 0; c < COLS; c++) {
      Key k = Layer.lookupOnActiveLayer(r, c);

      if (::OneShot.isOneShotKey(k)) {
        if (::OneShot.isActive(k))
          ::LEDControl.setCrgbAt(r, c, highlight_color);
        else
          ::LEDControl.refreshAt(r, c);
      } else if (k.raw >= Key_LeftControl.raw && k.raw <= Key_RightGui.raw) {
        if (hid::isModifierKeyActive(k))
          ::LEDControl.setCrgbAt(r, c, highlight_color);
        else
          ::LEDControl.refreshAt(r, c);
      } else if (k.flags == (SYNTHETIC | SWITCH_TO_KEYMAP)) {
        uint8_t layer = k.keyCode;
        if (layer >= MOMENTARY_OFFSET)
          layer -= MOMENTARY_OFFSET;

        if (Layer.isOn(layer))
          ::LEDControl.setCrgbAt(r, c, highlight_color);
        else
          ::LEDControl.refreshAt(r, c);
      }
    }
  }
}

}

kaleidoscope::ActiveModColorEffect ActiveModColorEffect;
