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

#pragma once

#include <Kaleidoscope-Ranges.h>  // for REDIAL

#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/key_defs.h"              // for Key
#include "kaleidoscope/plugin.h"                // for Plugin

constexpr Key Key_Redial = Key(kaleidoscope::ranges::REDIAL);

namespace kaleidoscope {
namespace plugin {

class Redial : public kaleidoscope::Plugin {
 public:
  static bool shouldRemember(Key mappedKey);

  EventHandlerResult onNameQuery();
  EventHandlerResult onKeyEvent(KeyEvent &event);

 private:
  Key last_key_;
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::Redial Redial;
