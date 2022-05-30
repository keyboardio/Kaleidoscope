/* -*- mode: c++ -*-
 * Kaleidoscope-PrefixLayer -- Sends a prefix key for every key in a layer.
 * Copyright (C) 2017, 2022  iliana etaoin <iliana@buttslol.net>
 * Copyright (C) 2017  James Cash
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

#include "kaleidoscope/plugin/PrefixLayer.h"

#include <Arduino.h>  // for PROGMEM
#include <stdint.h>   // for uint8_t

#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/Runtime.h"               // for Runtime
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/key_defs.h"              // for Key
#include "kaleidoscope/keyswitch_state.h"       // for keyToggledOn
#include "kaleidoscope/layers.h"                // for Layer

namespace kaleidoscope {
namespace plugin {

EventHandlerResult PrefixLayer::onKeyEvent(KeyEvent &event) {
  if (event.state & INJECTED)
    return EventHandlerResult::OK;
  if (!keyToggledOn(event.state))
    return EventHandlerResult::OK;
  if (event.key == Key_NoKey)
    return EventHandlerResult::OK;
  if (!event.key.isKeyboardKey())
    return EventHandlerResult::OK;
  if (event.key.isKeyboardModifier())
    return EventHandlerResult::OK;

  for (uint8_t i = 0; i < prefix_layers_length_; i++) {
    if (Layer.isActive(prefix_layers_[i].layer)) {
      current_prefix_ = prefix_layers_[i].prefix;
      Runtime.handleKeyEvent(KeyEvent{KeyAddr::none(), IS_PRESSED | INJECTED, current_prefix_});
      Runtime.handleKeyEvent(KeyEvent{KeyAddr::none(), WAS_PRESSED | INJECTED, current_prefix_});
      current_prefix_ = Key_NoKey;
    }
  }

  return EventHandlerResult::OK;
}

EventHandlerResult PrefixLayer::onAddToReport(Key key) {
  if (current_prefix_ != Key_NoKey && current_prefix_ != key && key.isKeyboardModifier()) {
    return EventHandlerResult::ABORT;
  }

  return EventHandlerResult::OK;
}

void PrefixLayer::setPrefixLayers(const PrefixLayer::Entry *prefix_layers) {
  prefix_layers_        = prefix_layers;
  prefix_layers_length_ = sizeof(prefix_layers) / sizeof(PrefixLayer::Entry);
}

const PrefixLayer::Entry *PrefixLayer::getPrefixLayers() {
  return prefix_layers_;
}

uint8_t PrefixLayer::getPrefixLayersLength() {
  return prefix_layers_length_;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::PrefixLayer PrefixLayer;
