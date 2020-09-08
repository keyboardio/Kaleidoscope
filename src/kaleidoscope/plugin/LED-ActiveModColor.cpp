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

cRGB ActiveModColorEffect::highlight_color = (cRGB) {
  160, 160, 160
};

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
    if (::OneShot.isOneShotKey(key) ||
        (highlight_normal_modifiers_ &&
         ((key >= Key_LeftControl && key <= Key_RightGui) ||
          (key.getFlags() == (SYNTHETIC | SWITCH_TO_KEYMAP))))) {
      mod_key_bits_.set(key_addr);
    }
  } else if (keyToggledOff(key_state)) {
    // When a non-OneShot key toggles off, if it was being
    // highlighted, we clear it immediately. We don't do this for
    // OneShot keys, because they remain active after toggling off.
    if (mod_key_bits_.read(key_addr) &&
        (! ::OneShot.isOneShotKey(key))) {
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
    Key key = Layer.lookup(key_addr);

    if (::OneShot.isOneShotKey(key)) {
      // First, we deal with OneShot keys:
      if (::OneShot.isSticky(key)) {
        // If the OneShot key is sticky, it gets a different color:
        ::LEDControl.setCrgbAt(key_addr, sticky_color);
      } else if (::OneShot.isActive(key) || ::OneShot.isPressed(key)) {
        // Non-sticky active OneShot keys also get highlighted:
        ::LEDControl.setCrgbAt(key_addr, highlight_color);
      } else {
        // If the OneShot key isn't active anymore at this point, we
        // unset the for its address in the bitfield, and reset the
        // color of the LED for that key:
        mod_key_bits_.clear(key_addr);
        ::LEDControl.refreshAt(key_addr);
      }
    } else {
      // Any non-OneShot keys here must be modifiers or layer-shift
      // keys that are still being held, so we don't need to do any
      // more checking, and just highlight them now:
      ::LEDControl.setCrgbAt(key_addr, highlight_color);
    }
  }

  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::ActiveModColorEffect ActiveModColorEffect;
