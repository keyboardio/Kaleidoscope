/* -*- mode: c++ -*-
 * Kaleidoscope-Redial -- Redial support for Kaleidoscope
 * Copyright (C) 2018-2021  Keyboard.io, Inc.
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

#include "kaleidoscope/plugin/Redial.h"

#include <Arduino.h>                   // for F, __FlashStringHelper
#include <Kaleidoscope-FocusSerial.h>  // for Focus, FocusSerial

#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/key_defs.h"              // for Key, Key_0, Key_1, Key_A, Key_Z
#include "kaleidoscope/keyswitch_state.h"       // for keyToggledOn

namespace kaleidoscope {
namespace plugin {

EventHandlerResult Redial::onNameQuery() {
  return ::Focus.sendName(F("Redial"));
}

EventHandlerResult Redial::onKeyEvent(KeyEvent &event) {
  if (keyToggledOn(event.state)) {
    if (event.key == Key_Redial) {
      event.key = last_key_;
    } else if (shouldRemember(event.key)) {
      last_key_ = event.key;
    }
  }
  return EventHandlerResult::OK;
}

__attribute__((weak)) bool Redial::shouldRemember(Key mapped_key) {
  if (mapped_key >= Key_A && mapped_key <= Key_Z)
    return true;
  if (mapped_key >= Key_1 && mapped_key <= Key_0)
    return true;

  return false;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::Redial Redial;
