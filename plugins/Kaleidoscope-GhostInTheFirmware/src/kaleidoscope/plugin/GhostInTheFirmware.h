/* Kaleidoscope-GhostInTheFirmware -- Let the keyboard write for you!
 * Copyright 2017-2025 Keyboard.io, inc.
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

#pragma once

#include <stdint.h>  // for uint16_t

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/plugin.h"                // for Plugin

namespace kaleidoscope {
namespace plugin {
class GhostInTheFirmware : public kaleidoscope::Plugin {
 public:
  struct GhostKey {
    KeyAddr addr;
    uint16_t press_time;
    uint16_t delay;
  };
  const GhostKey *ghost_keys;

  void activate();

  EventHandlerResult afterEachCycle();

 private:
  bool is_active_       = false;
  uint16_t current_pos_ = 0;
  uint16_t start_time_;
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::GhostInTheFirmware GhostInTheFirmware;
