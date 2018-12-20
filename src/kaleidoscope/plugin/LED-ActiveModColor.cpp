/* -*- mode: c++ -*-
 * Kaleidoscope-LED-ActiveModColor -- Light up the LEDs under the active modifiers
 * Copyright (C) 2016, 2017, 2018  Keyboard.io, Inc
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

#include <Kaleidoscope-LED-ActiveModColor.h>
#include <Kaleidoscope-OneShot.h>
#include <kaleidoscope/hid.h>

namespace kaleidoscope {
namespace plugin {

uint8_t ActiveModColorEffect::mod_keys_[MAX_MODS_PER_LAYER];
uint8_t ActiveModColorEffect::mod_key_count_;

cRGB ActiveModColorEffect::highlight_color = (cRGB) {
  0xff, 0xff, 0xff
};

cRGB ActiveModColorEffect::sticky_color = CRGB(0xff, 0x00, 0x00);

EventHandlerResult ActiveModColorEffect::onLayerChange() {
  if (!Kaleidoscope.has_leds)
    return EventHandlerResult::OK;

  mod_key_count_ = 0;

  for (byte r = 0; r < ROWS; r++) {
    for (byte c = 0; c < COLS; c++) {
      Key k = Layer.lookupOnActiveLayer(r, c);

      if (::OneShot.isOneShotKey(k) ||
          (k.raw >= Key_LeftControl.raw && k.raw <= Key_RightGui.raw) ||
          (k.flags == (SYNTHETIC | SWITCH_TO_KEYMAP))) {
        uint8_t coords = r * COLS + c;
        mod_keys_[mod_key_count_++] = coords;
      }
    }
  }

  return EventHandlerResult::OK;
}

EventHandlerResult ActiveModColorEffect::beforeReportingState() {
  for (uint8_t i = 0; i < mod_key_count_; i++) {
    uint8_t coords = mod_keys_[i];
    byte c = coords % COLS;
    byte r = (coords - c) / COLS;

    Key k = Layer.lookupOnActiveLayer(r, c);

    if (::OneShot.isOneShotKey(k)) {
      if (::OneShot.isSticky(k))
        ::LEDControl.setCrgbAt(r, c, sticky_color);
      else if (::OneShot.isActive(k))
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
      if (layer >= LAYER_SHIFT_OFFSET)
        layer -= LAYER_SHIFT_OFFSET;

      if (Layer.isActive(layer))
        ::LEDControl.setCrgbAt(r, c, highlight_color);
      else
        ::LEDControl.refreshAt(r, c);
    }
  }

  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::ActiveModColorEffect ActiveModColorEffect;
