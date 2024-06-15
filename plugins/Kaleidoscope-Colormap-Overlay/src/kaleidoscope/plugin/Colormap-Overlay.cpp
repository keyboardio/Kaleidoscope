/* Kaleidoscope-Colormap-Overlay - A LED plugin for Kaleidoscope.
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

#include "kaleidoscope/plugin/Colormap-Overlay.h"

#include <stdint.h>  // for uint8_t

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr, MatrixAddr, MatrixAddr<>::...
#include "kaleidoscope/device/device.h"         // for cRGB, CRGB
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerRes...
#include "kaleidoscope/key_defs.h"              // for Key, KEY_FLAGS, Key_NoKey, LockLayer
#include "kaleidoscope/layers.h"                // for Layer, Layer_
#include "kaleidoscope/plugin/LEDControl.h"     // for LEDControl
#include <Kaleidoscope-LED-Palette-Theme.h>     // for LEDPaletteTheme

namespace kaleidoscope {
namespace plugin {
uint16_t ColormapOverlay::map_base_;

void ColormapOverlay::setup() {
  map_base_ = ::LEDPaletteTheme.reserveThemes(1);
}

bool ColormapOverlay::hasOverlay(KeyAddr k) {
  uint8_t top_layer   = Layer.mostRecent();
  uint8_t layer_index = Layer.lookupActiveLayer(k);

  bool found_match_on_lower_layer = false;
  for (uint8_t i{0}; i < overlay_count_; ++i) {
    Overlay overlay = overlays_[i];
    if (overlay.addr == k) {
      if ((overlay.layer == top_layer) || (overlay.layer == layer_wildcard)) {
        selectedColor = ::LEDPaletteTheme.lookupPaletteColor(overlay.palette_index);
        return true;
      } else if (overlay.layer == layer_index) {
        selectedColor              = ::LEDPaletteTheme.lookupPaletteColor(overlay.palette_index);
        found_match_on_lower_layer = true;
      }
    }
  }

  return found_match_on_lower_layer;
}

EventHandlerResult ColormapOverlay::onSetup() {
  return EventHandlerResult::OK;
}

void ColormapOverlay::setLEDOverlayColors() {
  if (!Runtime.has_leds)
    return;

  for (auto key_addr : KeyAddr::all()) {
    if (ColormapOverlay::hasOverlay(key_addr)) {
      ::LEDControl.setCrgbAt(KeyAddr(key_addr), selectedColor);
    } else {
      ::LEDControl.refreshAt(KeyAddr(key_addr));
    }
  }
}

EventHandlerResult ColormapOverlay::beforeSyncingLeds() {
  setLEDOverlayColors();

  return EventHandlerResult::OK;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::ColormapOverlay ColormapOverlay;
