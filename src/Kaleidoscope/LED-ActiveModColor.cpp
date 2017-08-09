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
#include <kaleidoscope/hid.h>

namespace kaleidoscope {

cRGB ActiveModColorEffect::highlight_color = (cRGB) {
  0xff, 0xff, 0xff
};

ActiveModColorEffect::ActiveModColorEffect(void) {
}

void ActiveModColorEffect::begin(void) {
  loop_hook_use(loopHook);
}

bool ActiveModColorEffect::isModifierActive(Key key) {
  if (key.raw >= ranges::OSM_FIRST && key.raw <= ranges::OSM_LAST) {
    uint8_t idx = key.raw - ranges::OSM_FIRST;
    key.flags = 0;
    key.keyCode = Key_LeftControl.keyCode + idx;
  }

  if (key.raw < Key_LeftControl.raw || key.raw > Key_RightGui.raw)
    return false;

  return hid::isModifierKeyActive(key);
}

bool ActiveModColorEffect::isLayerKeyActive(Key key) {
  uint8_t layer = 255;

  if (key.raw >= ranges::OSL_FIRST && key.raw <= ranges::OSL_LAST) {
    layer = key.raw - ranges::OSL_FIRST;
  } else if (key.flags & (SYNTHETIC | SWITCH_TO_KEYMAP)) {
    layer = key.keyCode;
    if (layer >= MOMENTARY_OFFSET)
      layer -= MOMENTARY_OFFSET;
  }

  if (layer == 255)
    return false;

  return Layer.isOn(layer);
}

void ActiveModColorEffect::loopHook(bool is_post_clear) {
  if (is_post_clear)
    return;

  for (byte r = 0; r < ROWS; r++) {
    for (byte c = 0; c < COLS; c++) {
      Key k = Layer.lookup(r, c);

      if (isModifierActive(k) || isLayerKeyActive(k))
        LEDControl.setCrgbAt(r, c, highlight_color);
    }
  }
}

}

kaleidoscope::ActiveModColorEffect ActiveModColorEffect;
