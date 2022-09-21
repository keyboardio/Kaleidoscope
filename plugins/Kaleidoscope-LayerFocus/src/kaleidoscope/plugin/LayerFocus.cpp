/* -*- mode: c++ -*-
 * Kaleidoscope-LayerFocus -- Focus commands to work with layers
 * Copyright (C) 2020, 2021  Keyboard.io, Inc
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

#include "kaleidoscope/plugin/LayerFocus.h"

#include <Arduino.h>                   // for PSTR, F, __FlashStringHelper
#include <Kaleidoscope-FocusSerial.h>  // for Focus, FocusSerial
#include <stdint.h>                    // for uint8_t

#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/layers.h"                // for Layer, Layer_

namespace kaleidoscope {
namespace plugin {

EventHandlerResult LayerFocus::onNameQuery() {
  return ::Focus.sendName(F("LayerFocus"));
}

EventHandlerResult LayerFocus::onFocusEvent(const char *input) {
  const char *cmd_activate   = PSTR("layer.activate");
  const char *cmd_deactivate = PSTR("layer.deactivate");
  const char *cmd_isActive   = PSTR("layer.isActive");
  const char *cmd_moveTo     = PSTR("layer.moveTo");
  const char *cmd_state      = PSTR("layer.state");

  if (::Focus.inputMatchesHelp(input))
    return ::Focus.printHelp(cmd_activate,
                             cmd_deactivate,
                             cmd_isActive,
                             cmd_moveTo,
                             cmd_state);

  if (::Focus.inputMatchesCommand(input, cmd_activate)) {
    if (!::Focus.isEOL()) {
      uint8_t layer;
      ::Focus.read(layer);
      ::Layer.activate(layer);
    }
  } else if (::Focus.inputMatchesCommand(input, cmd_deactivate)) {
    if (!::Focus.isEOL()) {
      uint8_t layer;
      ::Focus.read(layer);
      ::Layer.deactivate(layer);
    }
  } else if (::Focus.inputMatchesCommand(input, cmd_isActive)) {
    if (!::Focus.isEOL()) {
      uint8_t layer;
      ::Focus.read(layer);
      ::Focus.send(::Layer.isActive(layer));
    }
  } else if (::Focus.inputMatchesCommand(input, cmd_moveTo)) {
    if (!::Focus.isEOL()) {
      uint8_t layer;
      ::Focus.read(layer);
      ::Layer.move(layer);
    }
  } else if (::Focus.inputMatchesCommand(input, cmd_state)) {
    if (::Focus.isEOL()) {
      for (uint8_t i = 0; i < 32; i++) {
        ::Focus.send(::Layer.isActive(i) ? 1 : 0);
      }
    } else {
      ::Layer.move(0);
      ::Layer.deactivate(0);

      for (uint8_t i = 0; i < 32 && !::Focus.isEOL(); i++) {
        uint8_t b;
        ::Focus.read(b);
        if (b)
          ::Layer.activate(i);
      }
    }
  } else {
    return EventHandlerResult::OK;
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::LayerFocus LayerFocus;
