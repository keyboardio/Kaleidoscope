/* Kaleidoscope-MouseKeys - Mouse keys for Kaleidoscope.
 * Copyright (C) 2017-2021  Keyboard.io, Inc.
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

#include <stdint.h>                             // for uint8_t, uint16_t

#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/key_defs.h"              // for Key
#include "kaleidoscope/plugin.h"                // for Plugin

namespace kaleidoscope {
namespace plugin {
class MouseKeys : public kaleidoscope::Plugin {
 public:
  static uint8_t speed;
  static uint16_t speedDelay;
  static uint8_t accelSpeed;
  static uint16_t accelDelay;
  static uint8_t wheelSpeed;
  static uint16_t wheelDelay;

  static void setWarpGridSize(uint8_t grid_size);
  static void setSpeedLimit(uint8_t speed_limit);

  EventHandlerResult onSetup();
  EventHandlerResult onNameQuery();
  EventHandlerResult afterEachCycle();
  EventHandlerResult onKeyEvent(KeyEvent &event);
  EventHandlerResult onAddToReport(Key key);
  EventHandlerResult afterReportingState(const KeyEvent &event);

 private:
  uint16_t move_start_time_ = 0;
  uint16_t accel_start_time_ = 0;
  uint16_t wheel_start_time_ = 0;

  // Mouse cursor and wheel movement directions are stored in a single bitfield
  // to save space.  The low four bits are for cursor movement, and the high
  // four are for wheel movement.
  static constexpr uint8_t wheel_offset_ = 4;
  static constexpr uint8_t wheel_mask_ = 0b11110000;
  static constexpr uint8_t move_mask_  = 0b00001111;
  uint8_t directions_ = 0;
  uint8_t pending_directions_ = 0;
  uint8_t buttons_ = 0;

  bool isMouseKey(const Key &key) const;
  bool isMouseButtonKey(const Key &key) const;
  bool isMouseMoveKey(const Key &key) const;
  bool isMouseWarpKey(const Key &key) const;
  bool isMouseWheelKey(const Key &key) const;

  void sendMouseButtonReport() const;
  void sendMouseWarpReport(const KeyEvent &event) const;
  void sendMouseMoveReport();
  void sendMouseWheelReport();

};

} // namespace plugin
} // namespace kaleidoscope

extern kaleidoscope::plugin::MouseKeys MouseKeys;
