/* Kaleidoscope-Colormap-Overlay -- Per key colors overlaying active LED effect
 * Copyright 2017-2025 Keyboard.io, inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * Additional Permissions:
 * As an additional permission under Section 7 of the GNU General Public
 * License Version 3, you may link this software against a Vendor-provided
 * Hardware Specific Software Module under the terms of the MCU Vendor
 * Firmware Library Additional Permission Version 1.0.
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
#include <Kaleidoscope.h>                       // for Kaleidoscope
#include <Kaleidoscope-FocusSerial.h>           // for Focus
#include <Kaleidoscope-LED-Palette-Theme.h>     // for LEDPaletteTheme
#include <Kaleidoscope-EEPROM-Settings.h>       // for EEPROMSettings

namespace kaleidoscope {
namespace plugin {
uint16_t ColormapOverlay::overlays_base_;
uint8_t ColormapOverlay::max_overlays_ = 64;  // TODO(EvyBongers): figure this out. How determine a good maximum?
Overlay *ColormapOverlay::overlays_;
uint8_t ColormapOverlay::overlay_count_;

void ColormapOverlay::setup() {
  ::LEDPaletteTheme.reservePalette();

  overlays_base_ = ::EEPROMSettings.requestSlice(max_overlays_ * sizeof(Overlay));

  ::EEPROMSettings.seal();

  if (!::EEPROMSettings.isValid()) {
    // When settings are invalid, better to clean it all up to prevent unwanted things from happening
    ::EEPROMSettings.invalidate();
    return;
  }

  Kaleidoscope.storage().get(overlays_base_, overlays_);
}

bool ColormapOverlay::hasOverlay(KeyAddr k) {
  uint8_t top_layer   = Layer.mostRecent();
  uint8_t layer_index = Layer.lookupActiveLayer(k);

  bool found_match_on_lower_layer = false;
  for (uint8_t i{0}; i < overlay_count_; i++) {
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

EventHandlerResult ColormapOverlay::onFocusEvent(const char *input) {
  if (!Runtime.has_leds)
    return EventHandlerResult::OK;

  const char *cmd = PSTR("colormap.overlay");

  if (::Focus.inputMatchesHelp(input))
    return ::Focus.printHelp(cmd);

  if (!::Focus.inputMatchesCommand(input, cmd))
    return EventHandlerResult::OK;

  if (::Focus.isEOL()) {
    for (uint8_t layer = 0; layer < layer_count; layer++) {
      for (uint8_t key_index_ = 0; key_index_ < Runtime.device().numKeys(); key_index_++) {
        KeyAddr k = KeyAddr(key_index_);
        for (uint8_t overlay_index{0}; overlay_index < overlay_count_; overlay_index++) {
          Overlay overlay = overlays_[overlay_index];
          if ((overlay.addr == k) && (overlay.layer == layer)) {
            ::Focus.send(overlay.palette_index);
          }
        }
        ::Focus.send(-1);
      }
    }
    return EventHandlerResult::EVENT_CONSUMED;
  }

  overlays_      = nullptr;
  overlay_count_ = 0;
  uint16_t i     = 0;
  while (!::Focus.isEOL() && (i < (uint16_t)Runtime.device().numKeys() * layer_count)) {
    int8_t color_index_;

    // Ref: plugins/Kaleidoscope-FocusSerial/src/kaleidoscope/plugin/FocusSerial.h
    // -> No overload for signed integers
    // Ref: src/kaleidoscope/device/Base.h
    // -> parseInt() seems to support signed values?
    ::Focus.read(color_index_);
    if (color_index_ >= 0) {
      uint8_t key_index_ = i % Runtime.device().numKeys();
      uint8_t layer_     = (i - key_index_) / Runtime.device().numKeys();

      overlays_[overlay_count_] = Overlay(layer_, KeyAddr(key_index_), color_index_);
      overlay_count_++;

      if (overlay_count_ >= max_overlays_) {
        // TODO(anyone): For now we just break the loop. Once focus implements
        // support for reporting back errors, this should report why we break
        // the loop
        break;
      }
    }

    i++;
  }
  Runtime.storage().commit();

  ::LEDControl.refreshAll();

  return EventHandlerResult::EVENT_CONSUMED;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::ColormapOverlay ColormapOverlay;
