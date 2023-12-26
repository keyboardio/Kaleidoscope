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

#pragma once

#include <stdint.h>  // for uint8_t

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/device/device.h"         // for cRGB
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/key_defs.h"              // for Key, KEY_FLAGS, Key_NoKey, LockLayer
#include "kaleidoscope/layers.h"                // for Layer, Layer_
#include "kaleidoscope/plugin/LEDControl.h"     // for LEDControl

namespace kaleidoscope {
namespace plugin {

// Data structure for an individual qukey
struct Overlay {
  int8_t layer;
  KeyAddr addr;
  uint8_t palette_index;

  // This is the constructor that should be used when creating a Overlay that
  // will be used by ColormapOverlay
  constexpr Overlay(int8_t layer, KeyAddr k, uint8_t palette_index)
    : layer(layer), addr(k), palette_index(palette_index) {}
};

class ColormapOverlay : public kaleidoscope::Plugin {
 public:
  static void setup();
  // Function for defining the array of overlays. It's a template function that
  // takes as its sole argument an array reference of size `_overlay_count`, so
  // there's no need to use `sizeof` to calculate the correct size, and pass it
  // as a separate parameter.
  template<uint8_t _overlay_count>
  void configureOverlays(Overlay const (&overlays)[_overlay_count]) {
    overlays_      = overlays;
    overlay_count_ = _overlay_count;
  }

  // A wildcard value for a qukey that exists on every layer.
  static constexpr int8_t layer_wildcard{-1};

  EventHandlerResult onSetup();
  EventHandlerResult beforeSyncingLeds();

 private:
  static uint16_t map_base_;
  Overlay const *overlays_;
  uint8_t overlay_count_;
  cRGB selectedColor;

  bool hasOverlay(KeyAddr k);
  void setLEDOverlayColors();
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::ColormapOverlay ColormapOverlay;

#define COLORMAP_OVERLAYS(overlays...)                                   \
  {                                                                      \
    static kaleidoscope::plugin::Overlay const overlays_[] = {overlays}; \
    ColormapOverlay.configureOverlays(overlays_);                        \
  }
