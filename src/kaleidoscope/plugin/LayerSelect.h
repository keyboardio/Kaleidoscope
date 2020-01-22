/* -*- mode: c++ -*-
 * Kaleidoscope-LayerSelect -- Modifier-like layer selection
 * Copyright (C) 2020  Keyboard.io, Inc
 * Copyright (C) 2020  DygmaLab, SE.
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

#include "kaleidoscope/Runtime.h"
#include <Kaleidoscope-Ranges.h>

namespace kaleidoscope {
namespace plugin {

#define Key_LayerSelect Key(kaleidoscope::ranges::LAYER_SELECT)

class LayerSelect: public Plugin {
 public:
  static constexpr uint8_t NOT_A_SELECTION = 255;

  EventHandlerResult onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t key_state);

  uint8_t layerSelect(Key mapped_key, KeyAddr key_addr);

 private:
  static bool selector_held_;
};

}
}

extern kaleidoscope::plugin::LayerSelect LayerSelect;
