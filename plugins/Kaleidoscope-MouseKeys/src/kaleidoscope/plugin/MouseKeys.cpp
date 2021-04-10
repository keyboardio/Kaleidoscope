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

#include <Arduino.h>

#include "kaleidoscope/Runtime.h"
#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-FocusSerial.h"
#include "kaleidoscope/keyswitch_state.h"

namespace kaleidoscope {
namespace plugin {

uint8_t MouseKeys_::speed = 1;
uint16_t MouseKeys_::speedDelay = 1;

uint8_t MouseKeys_::accelSpeed = 1;
uint16_t MouseKeys_::accelDelay = 64;

uint8_t MouseKeys_::wheelSpeed = 1;
uint16_t MouseKeys_::wheelDelay = 50;

uint16_t MouseKeys_::move_start_time_;
uint16_t MouseKeys_::accel_start_time_;
uint16_t MouseKeys_::wheel_start_time_;

// =============================================================================
// Configuration functions

void MouseKeys_::setWarpGridSize(uint8_t grid_size) {
  MouseWrapper.warp_grid_size = grid_size;
}

void MouseKeys_::setSpeedLimit(uint8_t speed_limit) {
  MouseWrapper.speedLimit = speed_limit;
}

// =============================================================================
// Key variant tests

bool MouseKeys_::isMouseKey(const Key& key) const {
  return (key.getFlags() == (SYNTHETIC | IS_MOUSE_KEY));
}

bool MouseKeys_::isMouseButtonKey(const Key& key) const {
  uint8_t variant = key.getKeyCode() & (KEY_MOUSE_BUTTON | KEY_MOUSE_WARP);
  return variant == KEY_MOUSE_BUTTON;
}

bool MouseKeys_::isMouseMoveKey(const Key& key) const {
  uint8_t mask = (KEY_MOUSE_BUTTON | KEY_MOUSE_WARP | KEY_MOUSE_WHEEL);
  uint8_t variant = key.getKeyCode() & mask;
  return variant == 0;
}

bool MouseKeys_::isMouseWarpKey(const Key& key) const {
  return (key.getKeyCode() & KEY_MOUSE_WARP) != 0;
}

bool MouseKeys_::isMouseWheelKey(const Key& key) const {
  uint8_t mask = (KEY_MOUSE_BUTTON | KEY_MOUSE_WARP | KEY_MOUSE_WHEEL);
  uint8_t variant = key.getKeyCode() & mask;
  return variant == KEY_MOUSE_WHEEL;
}

// =============================================================================
// Event Handlers

// -----------------------------------------------------------------------------
EventHandlerResult MouseKeys_::onNameQuery() {
  return ::Focus.sendName(F("MouseKeys"));
}

// -----------------------------------------------------------------------------
EventHandlerResult MouseKeys_::onSetup(void) {
  kaleidoscope::Runtime.hid().mouse().setup();
  kaleidoscope::Runtime.hid().absoluteMouse().setup();

  return EventHandlerResult::OK;
}

// -----------------------------------------------------------------------------
EventHandlerResult MouseKeys_::afterEachCycle() {
  // Check timeout for accel update interval.
  if (Runtime.hasTimeExpired(accel_start_time_, accelDelay)) {
    accel_start_time_ = Runtime.millisAtCycleStart();
    // `accelStep` determines the movement speed of the mouse pointer, and gets
    // reset to zero when no mouse movement keys is pressed (see below).
    if (MouseWrapper.accelStep < 255 - accelSpeed) {
      MouseWrapper.accelStep += accelSpeed;
    }
  }

  // Check timeout for position update interval.
  bool update_position = Runtime.hasTimeExpired(move_start_time_, speedDelay);
  if (update_position) {
    move_start_time_ = Runtime.millisAtCycleStart();
    // Determine which mouse movement directions are active by searching through
    // all the currently active keys for mouse movement keys, and adding them to
    // a bitfield (`directions`).
    uint8_t directions = 0;
    int8_t vx = 0;
    int8_t vy = 0;
    for (Key key : live_keys.all()) {
      if (isMouseKey(key) && isMouseMoveKey(key)) {
        directions |= key.getKeyCode();
      }
    }

    if (directions == 0) {
      // If there are no mouse movement keys held, reset speed to zero.
      MouseWrapper.accelStep = 0;
    } else {
      // For each active direction, add the mouse movement speed.
      if (directions & KEY_MOUSE_LEFT)
        vx -= speed;
      if (directions & KEY_MOUSE_RIGHT)
        vx += speed;
      if (directions & KEY_MOUSE_UP)
        vy -= speed;
      if (directions & KEY_MOUSE_DOWN)
        vy += speed;

      // Prepare the mouse report.
      MouseWrapper.move(vx, vy);
      // Send the report.
      Runtime.hid().mouse().sendReport();
    }
  }

  // Check timeout for scroll report interval.
  bool update_wheel = Runtime.hasTimeExpired(wheel_start_time_, wheelDelay);
  if (update_wheel) {
    wheel_start_time_ = Runtime.millisAtCycleStart();
    // Determine which scroll wheel keys are active, and add their directions to
    // a bitfield (`directions`).
    uint8_t directions = 0;
    int8_t vx = 0;
    int8_t vy = 0;
    for (Key key : live_keys.all()) {
      if (isMouseKey(key) && isMouseWheelKey(key)) {
        directions |= key.getKeyCode();
      }
    }

    if (directions != 0) {
      // Horizontal scroll wheel:
      if (directions & KEY_MOUSE_LEFT)
        vx -= wheelSpeed;
      if (directions & KEY_MOUSE_RIGHT)
        vx += wheelSpeed;
      // Vertical scroll wheel (note coordinates are opposite movement):
      if (directions & KEY_MOUSE_UP)
        vy += wheelSpeed;
      if (directions & KEY_MOUSE_DOWN)
        vy -= wheelSpeed;

      // Add scroll wheel changes to HID report.
      Runtime.hid().mouse().move(0, 0, vy, vx);
      // Send the report.
      Runtime.hid().mouse().sendReport();
    }
  }

  return EventHandlerResult::OK;
}

// -----------------------------------------------------------------------------
EventHandlerResult MouseKeys_::onKeyEvent(KeyEvent &event) {
  if (!isMouseKey(event.key))
    return EventHandlerResult::OK;

  if (isMouseButtonKey(event.key)) {
    sendMouseButtonReport(event);

  } else if (isMouseWarpKey(event.key)) {
    if (keyToggledOn(event.state)) {
      sendMouseWarpReport(event);
    }

  } else if (isMouseMoveKey(event.key)) {
    // No report is sent here; that's handled in `afterEachCycle()`.
    move_start_time_ = Runtime.millisAtCycleStart() - speedDelay;
    accel_start_time_ = Runtime.millisAtCycleStart();

  } else if (isMouseWheelKey(event.key)) {
    // No report is sent here; that's handled in `afterEachCycle()`.
    wheel_start_time_ = Runtime.millisAtCycleStart() - wheelDelay;
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

// =============================================================================
// HID report helper functions

// -----------------------------------------------------------------------------
void MouseKeys_::sendMouseButtonReport(const KeyEvent &event) const {
  // Get ready to send a new mouse report by building it from live_keys. Note
  // that this also clears the movement and scroll values, but since those are
  // relative, that's what we want.
  Runtime.hid().mouse().releaseAllButtons();

  uint8_t buttons = 0;
  for (KeyAddr key_addr : KeyAddr::all()) {
    if (key_addr == event.addr)
      continue;
    Key key = live_keys[key_addr];
    if (isMouseKey(key) && isMouseButtonKey(key)) {
      buttons |= key.getKeyCode();
    }
  }
  if (keyToggledOn(event.state))
    buttons |= event.key.getKeyCode();
  buttons &= ~KEY_MOUSE_BUTTON;
  Runtime.hid().mouse().pressButtons(buttons);
  Runtime.hid().mouse().sendReport();
}

// -----------------------------------------------------------------------------
void MouseKeys_::sendMouseWarpReport(const KeyEvent &event) const {
  MouseWrapper.warp(
    ((event.key.getKeyCode() & KEY_MOUSE_WARP_END) ? WARP_END : 0x00) |
    ((event.key.getKeyCode() & KEY_MOUSE_UP) ? WARP_UP : 0x00)        |
    ((event.key.getKeyCode() & KEY_MOUSE_DOWN) ? WARP_DOWN : 0x00)    |
    ((event.key.getKeyCode() & KEY_MOUSE_LEFT) ? WARP_LEFT : 0x00)    |
    ((event.key.getKeyCode() & KEY_MOUSE_RIGHT) ? WARP_RIGHT : 0x00));
}

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::MouseKeys_ MouseKeys;
