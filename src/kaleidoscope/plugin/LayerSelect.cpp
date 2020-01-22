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

#include <Kaleidoscope-LayerSelect.h>
#include "kaleidoscope/keyswitch_state.h"
#include "kaleidoscope/layers.h"

namespace kaleidoscope {
namespace plugin {

bool LayerSelect::selector_held_;

EventHandlerResult LayerSelect::onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t key_state) {
  if (mapped_key == Key_LayerSelect) {
    selector_held_ = keyIsPressed(key_state);
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (!selector_held_)
    return EventHandlerResult::OK;

  uint8_t layer;
  if ((layer = layerSelect(mapped_key, key_addr)) == NOT_A_SELECTION)
    return EventHandlerResult::OK;

  if (keyToggledOn(key_state)) {
    Layer.move(layer);
    Runtime.device().maskKey(key_addr);
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

__attribute__((weak))
uint8_t LayerSelect::layerSelect(Key mapped_key, KeyAddr key_addr) {
  if (mapped_key < Key_1 || mapped_key > Key_0)
    return NOT_A_SELECTION;

  uint8_t layer;
  if (mapped_key == Key_0)
    layer = 0;
  else
    layer = mapped_key.getRaw() - Key_1.getRaw() + 1;

  return layer;
}

}
}

kaleidoscope::plugin::LayerSelect LayerSelect;
