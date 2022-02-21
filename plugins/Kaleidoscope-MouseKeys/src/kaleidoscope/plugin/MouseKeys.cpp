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

uint8_t MouseKeys_::move_direction_ = 0;
uint8_t MouseKeys_::wheel_direction_ = 0;
uint8_t MouseKeys_::pending_move_direction_ = 0;
uint8_t MouseKeys_::pending_wheel_direction_ = 0;

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
  if (Runtime.hasTimeExpired(move_start_time_, speedDelay))
    sendMouseMoveReport();

  // Check timeout for scroll report interval.
  if (Runtime.hasTimeExpired(wheel_start_time_, wheelDelay))
    sendMouseWheelReport();

  return EventHandlerResult::OK;
}

// -----------------------------------------------------------------------------
EventHandlerResult MouseKeys_::onKeyEvent(KeyEvent &event) {
  if (!isMouseKey(event.key))
    return EventHandlerResult::OK;

  pending_move_direction_ = 0;
  pending_wheel_direction_ = 0;

  if (isMouseButtonKey(event.key)) {
    sendMouseButtonReport(event);

  } else if (isMouseWarpKey(event.key)) {
    if (keyToggledOn(event.state)) {
      sendMouseWarpReport(event);
    }
  }

  // Reports for mouse cursor and wheel movement keys are sent from the
  // `afterReportingState()` handler (when first toggled on) and
  // `afterEachCycle()` handler (when held).  We need to return `OK` here so
  // that processing of events for these keys will complete.
  return EventHandlerResult::OK;
}

// -----------------------------------------------------------------------------
EventHandlerResult MouseKeys_::afterReportingState(const KeyEvent &event) {
  if (!isMouseKey(event.key))
    return EventHandlerResult::OK;

  // A mouse key event has been successfully registered, and we have now
  // gathered all the information on held mouse movement and wheel keys, so it's
  // safe to update the direction information.
  move_direction_ = pending_move_direction_;
  wheel_direction_ = pending_wheel_direction_;
  pending_move_direction_ = 0;
  pending_wheel_direction_ = 0;

  if (isMouseMoveKey(event.key)) {
    // When a cursor movement key toggles on, set the acceleration start time in
    // order to get consistent behavior.
    accel_start_time_ = Runtime.millisAtCycleStart();
    sendMouseMoveReport();

  } else if (isMouseWheelKey(event.key)) {
    sendMouseWheelReport();
  }

  return EventHandlerResult::OK;
}

// -----------------------------------------------------------------------------
// This handler is responsible for gathering information on which mouse cursor
// and wheel movement keys are currently pressed, so the direction(s) of motion
// will be up to date at the end of processing a mouse key event.  We add bits
// to the `pending` directions only; these get copied later if the event isn't
// aborted.
EventHandlerResult MouseKeys_::onAddToReport(Key key) {
  if (!isMouseKey(key))
    return EventHandlerResult::OK;

  if (isMouseMoveKey(key))
    pending_move_direction_ |= key.getKeyCode();

  if (isMouseWheelKey(key))
    pending_wheel_direction_ |= key.getKeyCode();

  return EventHandlerResult::OK;
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

// -----------------------------------------------------------------------------
void MouseKeys_::sendMouseMoveReport() {
  move_start_time_ = Runtime.millisAtCycleStart();

  int8_t vx = 0;
  int8_t vy = 0;

  if (move_direction_ == 0) {
    // If there are no mouse movement keys held, reset speed to zero.
    MouseWrapper.accelStep = 0;
  } else {
    // For each active direction, add the mouse movement speed.
    if (move_direction_ & KEY_MOUSE_LEFT)
      vx -= speed;
    if (move_direction_ & KEY_MOUSE_RIGHT)
      vx += speed;
    if (move_direction_ & KEY_MOUSE_UP)
      vy -= speed;
    if (move_direction_ & KEY_MOUSE_DOWN)
      vy += speed;

    // Prepare the mouse report.
    MouseWrapper.move(vx, vy);
    // Send the report.
    Runtime.hid().mouse().sendReport();
  }
}

// -----------------------------------------------------------------------------
void MouseKeys_::sendMouseWheelReport() {
  wheel_start_time_ = Runtime.millisAtCycleStart();

  int8_t vx = 0;
  int8_t vy = 0;

  if (wheel_direction_ != 0) {
    // Horizontal scroll wheel:
    if (wheel_direction_ & KEY_MOUSE_LEFT)
      vx -= wheelSpeed;
    if (wheel_direction_ & KEY_MOUSE_RIGHT)
      vx += wheelSpeed;
    // Vertical scroll wheel (note coordinates are opposite movement):
    if (wheel_direction_ & KEY_MOUSE_UP)
      vy += wheelSpeed;
    if (wheel_direction_ & KEY_MOUSE_DOWN)
      vy -= wheelSpeed;

    // Add scroll wheel changes to HID report.
    Runtime.hid().mouse().move(0, 0, vy, vx);
    // Send the report.
    Runtime.hid().mouse().sendReport();
  }
}

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::MouseKeys_ MouseKeys;
