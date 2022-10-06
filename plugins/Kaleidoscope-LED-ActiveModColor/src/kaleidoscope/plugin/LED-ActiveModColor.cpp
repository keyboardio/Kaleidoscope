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

#include "kaleidoscope/plugin/LED-ActiveModColor.h"

#include <Kaleidoscope-OneShot.h>          // for OneShot
#include <Kaleidoscope-OneShotMetaKeys.h>  // for OneShot_ActiveStickyKey

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr, MatrixAddr, MatrixAddr<>::Range
#include "kaleidoscope/KeyAddrBitfield.h"       // for KeyAddrBitfield, KeyAddrBitfield::Iterator
#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/LiveKeys.h"              // for LiveKeys, live_keys
#include "kaleidoscope/device/device.h"         // for CRGB, cRGB
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/key_defs.h"              // for Key, Key_Inactive, Key_Masked
#include "kaleidoscope/keyswitch_state.h"       // for keyToggledOn
#include "kaleidoscope/plugin/LEDControl.h"     // for LEDControl

namespace kaleidoscope {
namespace plugin {

KeyAddrBitfield ActiveModColorEffect::mod_key_bits_;
bool ActiveModColorEffect::highlight_normal_modifiers_ = true;

cRGB ActiveModColorEffect::highlight_color_ = CRGB(160, 160, 160);
cRGB ActiveModColorEffect::oneshot_color_   = CRGB(160, 160, 0);
cRGB ActiveModColorEffect::sticky_color_    = CRGB(160, 0, 0);

// -----------------------------------------------------------------------------
EventHandlerResult ActiveModColorEffect::onKeyEvent(KeyEvent &event) {

  // If `event.addr` is not a physical key address, ignore it:
  if (!event.addr.isValid()) {
    return EventHandlerResult::OK;
  }

  if (keyToggledOn(event.state)) {
    // If a key toggles on, we check its value. If it's a OneShot key, it will
    // get highlighted. Conditionally (if `highlight_normal_modifiers_` is set),
    // we also highlight modifier and layer-shift keys.
    if (event.key.isMomentary() ||
        ::OneShot.isOneShotKey(event.key) ||
        ::OneShot.isActive(event.addr)) {
      mod_key_bits_.set(event.addr);
    }
    if (event.key == OneShot_ActiveStickyKey) {
      for (KeyAddr entry_addr : KeyAddr::all()) {
        // Get the entry from the live keys array
        Key entry_key = live_keys[entry_addr];
        // Skip empty entries
        if (entry_key == Key_Inactive || entry_key == Key_Masked) {
          continue;
        }
        // Highlight everything else
        mod_key_bits_.set(entry_addr);
      }
    }
  } else {  // if (keyToggledOff(event.state))
    // Things get a bit ugly here because this plugin might come before OneShot
    // in the order, so we can't just count on OneShot stopping the suppressed
    // release event before we see it here.
    if (mod_key_bits_.read(event.addr) && !::OneShot.isActive(event.addr)) {
      mod_key_bits_.clear(event.addr);
      ::LEDControl.refreshAt(event.addr);
    }
  }

  return EventHandlerResult::OK;
}

// -----------------------------------------------------------------------------
EventHandlerResult ActiveModColorEffect::beforeSyncingLeds() {

  // This loop iterates through only the `key_addr`s that have their bits in the
  // `mod_key_bits_` bitfield set.
  for (KeyAddr key_addr : mod_key_bits_) {
    if (::OneShot.isTemporary(key_addr)) {
      // Temporary OneShot keys get one color:
      ::LEDControl.setCrgbAt(key_addr, oneshot_color_);
    } else if (::OneShot.isSticky(key_addr)) {
      // Sticky OneShot keys get another color:
      ::LEDControl.setCrgbAt(key_addr, sticky_color_);
    } else if (highlight_normal_modifiers_) {
      // Normal modifiers get a third color:
      ::LEDControl.setCrgbAt(key_addr, highlight_color_);
    }
  }

  return EventHandlerResult::OK;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::ActiveModColorEffect ActiveModColorEffect;
