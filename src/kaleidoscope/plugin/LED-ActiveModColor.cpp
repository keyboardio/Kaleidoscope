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

#include <stdint.h>                                  // for uint8_t
#include "kaleidoscope/Runtime.h"         // for Kaleidoscope
#include "kaleidoscope/KeyAddr.h"                    // for KeyAddr
#include "kaleidoscope/device/device.h"              // for cRGB, CRGB
#include "kaleidoscope/event_handler_result.h"       // for EventHandlerResult
#include "kaleidoscope/key_defs.h"                   // for Key, Key_LeftCon...
#include "kaleidoscope/key_defs_keymaps.h"           // for LAYER_SHIFT_OFFSET
#include "kaleidoscope/layers.h"                     // for Layer, Layer_
#include "kaleidoscope/plugin/LED-ActiveModColor.h"  // for ActiveModColorEf...
#include "kaleidoscope/plugin/OneShot.h"             // for OneShot
#include "kaleidoscope/plugin/LEDControl.h"
#include "kaleidoscope/hid.h"

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
  if (!Runtime.has_leds)
    return EventHandlerResult::OK;

  mod_key_count_ = 0;

  for (auto key_addr : KeyAddr::all()) {
    Key k = Layer.lookupOnActiveLayer(key_addr);

    if (::OneShot.isOneShotKey(k) ||
        (highlight_normal_modifiers_ && (
           (k >= Key_LeftControl && k <= Key_RightGui) ||
           (k.getFlags() == (SYNTHETIC | SWITCH_TO_KEYMAP))))) {
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
        ::LEDControl.setCrgbAt(key_addr, sticky_color);
      else if (::OneShot.isActive(k))
        ::LEDControl.setCrgbAt(key_addr, highlight_color);
      else
        ::LEDControl.refreshAt(key_addr);
    } else if (k >= Key_LeftControl && k <= Key_RightGui) {
      if (kaleidoscope::Runtime.hid().keyboard().isModifierKeyActive(k))
        ::LEDControl.setCrgbAt(key_addr, highlight_color);
      else
        ::LEDControl.refreshAt(key_addr);
    } else if (k.getFlags() == (SYNTHETIC | SWITCH_TO_KEYMAP)) {
      uint8_t layer = k.getKeyCode();
      if (layer >= LAYER_SHIFT_OFFSET)
        layer -= LAYER_SHIFT_OFFSET;

      if (Layer.isActive(layer))
        ::LEDControl.setCrgbAt(key_addr, highlight_color);
      else
        ::LEDControl.refreshAt(key_addr);
    }
  }

  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::ActiveModColorEffect ActiveModColorEffect;
