/* Kaleidoscope-WinKeyToggle -- Toggle the Windows (GUI) key on/off
 * Copyright 2018-2025 Keyboard.io, inc.
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

#include "kaleidoscope/plugin/WinKeyToggle.h"

#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/key_defs.h"              // for Key, Key_LeftGui, Key_RightGui

namespace kaleidoscope {
namespace plugin {

bool WinKeyToggle::enabled_;

EventHandlerResult WinKeyToggle::onKeyEvent(KeyEvent &event) {
  if (!enabled_)
    return EventHandlerResult::OK;

  if (event.key == Key_LeftGui || event.key == Key_RightGui) {
    return EventHandlerResult::ABORT;
  }

  return EventHandlerResult::OK;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::WinKeyToggle WinKeyToggle;
