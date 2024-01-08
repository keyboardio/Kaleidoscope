/* -*- mode: c++ -*-
 * Kaleidoscope-Colormap -- Per-layer colormap effect
 * Copyright (C) 2022  Keyboard.io, Inc
 *
 * This program is free software: you can redistribute it and/or modify it under it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <Arduino.h>  // for PROGMEM
#include <stdint.h>   // for uint8_t

#include "kaleidoscope_internal/device.h"       // for device
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/plugin.h"                // for Plugin

namespace kaleidoscope {
namespace plugin {

// clang-format off

#define COLORMAPS(layers...)                                            \
  namespace kaleidoscope {                                              \
  namespace plugin {                                                    \
  namespace defaultcolormap {                                           \
  const uint8_t colormaps[][kaleidoscope_internal::device.matrix_rows * kaleidoscope_internal::device.matrix_columns] PROGMEM = { \
      layers                                                            \
  };                                                                    \
  uint8_t colormap_layers =                                             \
      sizeof(colormaps) / sizeof(*colormaps);                           \
  } /* defaultcolormap */                                               \
  } /* plugin */                                                        \
  } /* kaleidoscope */

#define __IDENTITY__(X) X

#ifdef PER_KEY_DATA_STACKED
#define COLORMAP_STACKED(...)                                           \
  {                                                                     \
    MAP_LIST(__IDENTITY__, PER_KEY_DATA_STACKED(0, __VA_ARGS__))        \
  }
#endif

#ifdef PER_KEY_DATA
#define COLORMAP(...)                                    \
  {                                                      \
    MAP_LIST(__IDENTITY__, PER_KEY_DATA(0, __VA_ARGS__)) \
  }
#endif

// clang-format on

namespace defaultcolormap {
extern const uint8_t colormaps[][kaleidoscope_internal::device.matrix_rows * kaleidoscope_internal::device.matrix_columns];
extern uint8_t colormap_layers;
}  // namespace defaultcolormap

class DefaultColormap : public Plugin {
 public:
  static void setup();
  static void install();

  EventHandlerResult onFocusEvent(const char *input);
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::DefaultColormap DefaultColormap;
