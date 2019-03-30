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

KeyAddr ActiveModColorEffect::mod_keys_[MAX_MODS_PER_LAYER];
uint8_t ActiveModColorEffect::mod_key_count_;
bool ActiveModColorEffect::highlight_normal_modifiers_ = true;

cRGB ActiveModColorEffect::highlight_color = (cRGB) {
  160, 160, 160
};

cRGB ActiveModColorEffect::sticky_color = CRGB(160, 0, 0);

EventHandlerResult ActiveModColorEffect::onLayerChange() {
  if (!Kaleidoscope.has_leds)
    return EventHandlerResult::OK;

  mod_key_count_ = 0;

  for (auto key_addr : KeyAddr{}) {
    Key k = Layer.lookupOnActiveLayer(key_addr);

    if (::OneShot.isOneShotKey(k) ||
        (highlight_normal_modifiers_ && (
           (k.raw >= Key_LeftControl.raw && k.raw <= Key_RightGui.raw) ||
           (k.flags == (SYNTHETIC | SWITCH_TO_KEYMAP))))) {
      mod_keys_[mod_key_count_++] = key_addr;
    }
  }

  return EventHandlerResult::OK;
}

EventHandlerResult ActiveModColorEffect::beforeReportingState() {
  if (mod_key_count_ == 0) {
    onLayerChange();
  }

  for (uint8_t i = 0; i < mod_key_count_; i++) {
    const KeyAddr &key_addr = mod_keys_[i];

    Key k = Layer.lookupOnActiveLayer(key_addr);

    if (::OneShot.isOneShotKey(k)) {
      if (::OneShot.isSticky(k))
        ::LEDControl.setCrgbAt(KeyLEDAddr(key_addr), sticky_color);
      else if (::OneShot.isActive(k))
        ::LEDControl.setCrgbAt(KeyLEDAddr(key_addr), highlight_color);
      else
        ::LEDControl.refreshAt(KeyLEDAddr(key_addr));
    } else if (k.raw >= Key_LeftControl.raw && k.raw <= Key_RightGui.raw) {
      if (hid::isModifierKeyActive(k))
        ::LEDControl.setCrgbAt(KeyLEDAddr(key_addr), highlight_color);
      else
        ::LEDControl.refreshAt(KeyLEDAddr(key_addr));
    } else if (k.flags == (SYNTHETIC | SWITCH_TO_KEYMAP)) {
      uint8_t layer = k.keyCode;
      if (layer >= LAYER_SHIFT_OFFSET)
        layer -= LAYER_SHIFT_OFFSET;

      if (Layer.isActive(layer))
        ::LEDControl.setCrgbAt(KeyLEDAddr(key_addr), highlight_color);
      else
        ::LEDControl.refreshAt(KeyLEDAddr(key_addr));
    }
  }

  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::ActiveModColorEffect ActiveModColorEffect;
