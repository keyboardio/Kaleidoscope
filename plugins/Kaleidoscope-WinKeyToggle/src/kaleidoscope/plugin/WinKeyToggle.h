/* -*- mode: c++ -*-
 * Kaleidoscope-WinKeyToggle -- Toggle the Windows (GUI) key on/off
 * Copyright (C) 2018  Keyboard.io, Inc.
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

#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/plugin.h"                // for Plugin

namespace kaleidoscope {
namespace plugin {
class WinKeyToggle : public kaleidoscope::Plugin {
 public:
  EventHandlerResult onKeyEvent(KeyEvent &event);
  void toggle() {
    enabled_ = !enabled_;
  }

 private:
  static bool enabled_;
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::WinKeyToggle WinKeyToggle;
