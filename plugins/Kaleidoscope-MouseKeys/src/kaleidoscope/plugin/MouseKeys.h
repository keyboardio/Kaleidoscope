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

#include "kaleidoscope/Runtime.h"
#include "kaleidoscope/plugin/MouseKeys/MouseKeyDefs.h"
#include "kaleidoscope/plugin/MouseKeys/MouseWarpModes.h"
#include "kaleidoscope/plugin/MouseKeys/MouseWrapper.h"

namespace kaleidoscope {
namespace plugin {
class MouseKeys_ : public kaleidoscope::Plugin {
 public:
  MouseKeys_(void) {}

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

 private:
  static uint16_t move_start_time_;
  static uint16_t accel_start_time_;
  static uint16_t wheel_start_time_;

  bool isMouseKey(const Key &key) const;
  bool isMouseButtonKey(const Key &key) const;
  bool isMouseMoveKey(const Key &key) const;
  bool isMouseWarpKey(const Key &key) const;
  bool isMouseWheelKey(const Key &key) const;

  void sendMouseButtonReport(const KeyEvent &event) const;
  void sendMouseWarpReport(const KeyEvent &event) const;

};
}
}

extern kaleidoscope::plugin::MouseKeys_ MouseKeys;
