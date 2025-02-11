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

#pragma once

#include <stdint.h>  // for uint8_t

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/device/device.h"         // for cRGB
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/key_defs.h"              // for Key, KEY_FLAGS, Key_NoKey, LockLayer
#include "kaleidoscope/layers.h"                // for Layer, Layer_
#include "kaleidoscope/plugin/LEDControl.h"     // for LEDControl
#include <Kaleidoscope-LED-Palette-Theme.h>     // for LEDPaletteTheme

namespace kaleidoscope {
namespace plugin {

// Data structure for an individual qukey
struct Overlay {
  uint8_t layer;
  KeyAddr addr;
  uint8_t palette_index;

  // Default constructor required for array allocation
  constexpr Overlay()
    : layer(0), addr(KeyAddr::none()), palette_index(0) {}

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
    // Delete old overlays if they exist
    if (overlays_ != nullptr) {
      delete[] overlays_;
    }

    // Allocate and copy new overlays
    Overlay *new_overlays = new Overlay[_overlay_count];
    for (uint8_t i = 0; i < _overlay_count; i++) {
      new_overlays[i] = overlays[i];
    }

    overlays_      = new_overlays;
    overlay_count_ = _overlay_count;
  }

  template<uint8_t _layer_count>
  void configureOverlays(uint8_t **overlays) {
    // First count how many overlays we'll need
    uint8_t count = 0;
    for (int layer_ = 0; layer_ < _layer_count; layer_++) {
      for (int key_index_ = 0; key_index_ < kaleidoscope_internal::device.matrix_rows * kaleidoscope_internal::device.matrix_columns; key_index_++) {
        int8_t color_index_ = overlays[layer_][key_index_];
        if (color_index_ >= 0 && color_index_ < ::LEDPaletteTheme.getPaletteSize() &&
            color_index_ != no_color_overlay) {
          count++;
        }
      }
    }

    // Allocate memory for the overlays
    Overlay *new_overlays = new Overlay[count];
    uint8_t overlay_index = 0;

    // Fill in the overlays
    for (int layer_ = 0; layer_ < _layer_count; layer_++) {
      for (int key_index_ = 0; key_index_ < kaleidoscope_internal::device.matrix_rows * kaleidoscope_internal::device.matrix_columns; key_index_++) {
        int8_t color_index_ = overlays[layer_][key_index_];
        if (color_index_ >= 0 && color_index_ < ::LEDPaletteTheme.getPaletteSize() &&
            color_index_ != no_color_overlay) {
          new_overlays[overlay_index++] = Overlay(layer_, KeyAddr(key_index_), color_index_);
        }
      }
    }

    // Delete old overlays if they exist
    if (overlays_ != nullptr) {
      delete[] overlays_;
    }

    // Update member variables
    overlays_      = new_overlays;
    overlay_count_ = count;
  }
  // A wildcard value for an overlay that applies on every layer.
  static constexpr int8_t layer_wildcard{-1};
  static constexpr int8_t no_color_overlay{-1};

  EventHandlerResult onSetup();
  EventHandlerResult beforeSyncingLeds();

  ~ColormapOverlay() {
    if (overlays_ != nullptr) {
      delete[] overlays_;
    }
  }

 private:
  static uint16_t map_base_;
  Overlay *overlays_;
  uint8_t overlay_count_;
  cRGB selectedColor;

  bool hasOverlay(KeyAddr k);
  void setLEDOverlayColors();
};

// clang-format off

#define COLORMAP_OVERLAYS_MAP(layers...)                                \
  namespace kaleidoscope {                                              \
  namespace plugin {                                                    \
  const int8_t overlays_[][kaleidoscope_internal::device.matrix_rows * kaleidoscope_internal::device.matrix_columns] = { \
      layers                                                            \
  };                                                                    \
  ColormapOverlay.configureOverlays(overlays_);                         \
  } /* plugin */                                                        \
  } /* kaleidoscope */

#define __IDENTITY__(X) X

#ifdef PER_KEY_DATA_STACKED
#define COLORMAP_OVERLAY_STACKED(...)                                   \
  {                                                                     \
    MAP_LIST(__IDENTITY__, PER_KEY_DATA_STACKED(0, __VA_ARGS__))        \
  }
#endif

#ifdef PER_KEY_DATA
#define COLORMAP_OVERLAY(...)                            \
  {                                                      \
    MAP_LIST(__IDENTITY__, PER_KEY_DATA(0, __VA_ARGS__)) \
  }
#endif

// clang-format on

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::ColormapOverlay ColormapOverlay;

#define COLORMAP_OVERLAYS(overlays...)                                   \
  {                                                                      \
    static kaleidoscope::plugin::Overlay const overlays_[] = {overlays}; \
    ColormapOverlay.configureOverlays(overlays_);                        \
  }
