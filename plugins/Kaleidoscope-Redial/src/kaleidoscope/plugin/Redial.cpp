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

#include <Kaleidoscope-Redial.h>
#include <Kaleidoscope-FocusSerial.h>
#include "kaleidoscope/keyswitch_state.h"

namespace kaleidoscope {
namespace plugin {

Key Redial::last_key_;

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

}
}

kaleidoscope::plugin::Redial Redial;
