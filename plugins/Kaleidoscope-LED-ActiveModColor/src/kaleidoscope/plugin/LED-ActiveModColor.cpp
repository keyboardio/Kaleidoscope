/* -*- mode: c++ -*-
 * Kaleidoscope-LED-ActiveModColor -- Light up the LEDs under the active modifiers
 * Copyright (C) 2016-2020  Keyboard.io, Inc
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
#include "kaleidoscope/layers.h"
#include "kaleidoscope/keyswitch_state.h"

namespace kaleidoscope {
namespace plugin {

KeyAddrBitfield ActiveModColorEffect::mod_key_bits_;
bool ActiveModColorEffect::highlight_normal_modifiers_ = true;

cRGB ActiveModColorEffect::highlight_color = CRGB(160, 160, 160);
cRGB ActiveModColorEffect::oneshot_color = CRGB(160, 160, 0);
cRGB ActiveModColorEffect::sticky_color = CRGB(160, 0, 0);

EventHandlerResult ActiveModColorEffect::onKeyswitchEvent(
  Key &key,
  KeyAddr key_addr,
  uint8_t key_state) {

  // If `key_addr` is not a physical key address, ignore it:
  if (! key_addr.isValid()) {
    return EventHandlerResult::OK;
  }

  if (keyToggledOn(key_state)) {
    // If a key toggles on, we check its value. If it's a OneShot key,
    // it will get highlighted. Conditionally (if
    // `highlight_normal_modifiers_` is set), we also highlight
    // modifier and layer-shift keys.
    if (::OneShot.isModifier() || ::OneShot.isLayerShift()) {
      mod_key_bits_.set(key_addr);
    }
  } else if (keyToggledOff(key_state)) {
    // Things get a bit ugly here because this plugin might come
    // before OneShot in the order, so we can't just count on OneShot
    // stopping the suppressed release event before we see it here.
    if (mod_key_bits_.read(key_addr) && !::OneShot.isActive(key_addr)) {
      mod_key_bits_.clear(key_addr);
      ::LEDControl.refreshAt(key_addr);
    }
  }

  return EventHandlerResult::OK;
}

EventHandlerResult ActiveModColorEffect::beforeReportingState() {

  // This loop iterates through only the `key_addr`s that have their
  // bits in the `mod_key_bits_` bitfield set.
  for (KeyAddr key_addr : mod_key_bits_) {

    if (::OneShot.isTemporary(key_addr)) {
      // Temporary OneShot keys get one color:
      ::LEDControl.setCrgbAt(key_addr, oneshot_color);
    } else if (::OneShot.isSticky(key_addr)) {
      // Sticky OneShot keys get another color:
      ::LEDControl.setCrgbAt(key_addr, sticky_color);
    } else if (highlight_normal_modifiers_) {
      // Normal modifiers get a third color:
      ::LEDControl.setCrgbAt(key_addr, highlight_color);
    }
  }

  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::ActiveModColorEffect ActiveModColorEffect;
