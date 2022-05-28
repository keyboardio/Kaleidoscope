/* Kaleidoscope-NumPad - A NumPad plugin for Kaleidoscope.
 * Copyright (C) 2017-2018  Keyboard.io, Inc.
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

#include "kaleidoscope/plugin/NumPad.h"

#include <stdint.h>  // for uint8_t

#include "kaleidoscope/KeyAddr.h"                     // for KeyAddr, MatrixAddr, MatrixAddr<>::...
#include "kaleidoscope/device/device.h"               // for cRGB, CRGB
#include "kaleidoscope/event_handler_result.h"        // for EventHandlerResult, EventHandlerRes...
#include "kaleidoscope/key_defs.h"                    // for Key, KEY_FLAGS, Key_NoKey, LockLayer
#include "kaleidoscope/layers.h"                      // for Layer, Layer_
#include "kaleidoscope/plugin/LEDControl.h"           // for LEDControl
#include "kaleidoscope/plugin/LEDControl/LEDUtils.h"  // for breath_compute

namespace kaleidoscope {
namespace plugin {

// public:
uint8_t NumPad::numPadLayer;
cRGB NumPad::color       = CRGB(160, 0, 0);
uint8_t NumPad::lock_hue = 170;

// private:
KeyAddr NumPad::numpadLayerToggleKeyAddr;
bool NumPad::numpadActive = false;

EventHandlerResult NumPad::onSetup() {
  return EventHandlerResult::OK;
}

void NumPad::setKeyboardLEDColors() {
  ::LEDControl.set_mode(::LEDControl.get_mode_index());

  for (auto key_addr : KeyAddr::all()) {
    Key k         = Layer.lookupOnActiveLayer(key_addr);
    Key layer_key = Layer.getKey(numPadLayer, key_addr);

    if (k == LockLayer(numPadLayer)) {
      numpadLayerToggleKeyAddr = key_addr;
    }

    if ((k != layer_key) || (k == Key_NoKey) || (k.getFlags() != KEY_FLAGS)) {
      ::LEDControl.refreshAt(KeyAddr(key_addr));
    } else {
      ::LEDControl.setCrgbAt(KeyAddr(key_addr), color);
    }
  }

  if (numpadLayerToggleKeyAddr.isValid()) {
    cRGB lock_color = breath_compute(lock_hue);
    ::LEDControl.setCrgbAt(KeyAddr(numpadLayerToggleKeyAddr), lock_color);
  }
}

EventHandlerResult NumPad::afterEachCycle() {
  if (!Layer.isActive(numPadLayer)) {
    if (numpadActive) {
      ::LEDControl.set_mode(::LEDControl.get_mode_index());
      numpadActive = false;
    }
  } else {
    if (!numpadActive) {
      numpadActive = true;
    }
    setKeyboardLEDColors();
  }
  return EventHandlerResult::OK;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::NumPad NumPad;
