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

#include "kaleidoscope/plugin/MouseKeys.h"

#include <Arduino.h>                   // for F, __FlashStringHelper
#include <Kaleidoscope-FocusSerial.h>  // for Focus, FocusSerial
#include <stdint.h>                    // for uint8_t, uint16_t, int8_t

#include "kaleidoscope/KeyEvent.h"                             // for KeyEvent
#include "kaleidoscope/Runtime.h"                              // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"                        // for Base<>::HID, VirtualProps:...
#include "kaleidoscope/driver/hid/keyboardio/AbsoluteMouse.h"  // for AbsoluteMouse
#include "kaleidoscope/driver/hid/keyboardio/Mouse.h"          // for Mouse
#include "kaleidoscope/event_handler_result.h"                 // for EventHandlerResult, EventH...
#include "kaleidoscope/key_defs.h"                             // for Key, SYNTHETIC
#include "kaleidoscope/keyswitch_state.h"                      // for keyToggledOn
#include "kaleidoscope/plugin/mousekeys/MouseKeyDefs.h"        // for KEY_MOUSE_BUTTON, KEY_MOUS...
#include "kaleidoscope/plugin/mousekeys/MouseWrapper.h"        // for MouseWrapper, wrapper, WAR...

namespace kaleidoscope {
namespace plugin {

uint8_t MouseKeys::speed       = 1;
uint16_t MouseKeys::speedDelay = 1;

uint8_t MouseKeys::accelSpeed  = 1;
uint16_t MouseKeys::accelDelay = 64;

uint8_t MouseKeys::wheelSpeed  = 1;
uint16_t MouseKeys::wheelDelay = 50;

// =============================================================================
// Configuration functions

void MouseKeys::setWarpGridSize(uint8_t grid_size) {
  mousekeys::wrapper.warp_grid_size = grid_size;
}

void MouseKeys::setSpeedLimit(uint8_t speed_limit) {
  mousekeys::wrapper.speed_limit = speed_limit;
}

// =============================================================================
// Key variant tests

bool MouseKeys::isMouseKey(const Key &key) const {
  return (key.getFlags() == (SYNTHETIC | IS_MOUSE_KEY));
}

bool MouseKeys::isMouseButtonKey(const Key &key) const {
  uint8_t variant = key.getKeyCode() & (KEY_MOUSE_BUTTON | KEY_MOUSE_WARP);
  return variant == KEY_MOUSE_BUTTON;
}

bool MouseKeys::isMouseMoveKey(const Key &key) const {
  uint8_t mask    = (KEY_MOUSE_BUTTON | KEY_MOUSE_WARP | KEY_MOUSE_WHEEL);
  uint8_t variant = key.getKeyCode() & mask;
  return variant == 0;
}

bool MouseKeys::isMouseWarpKey(const Key &key) const {
  return (key.getKeyCode() & KEY_MOUSE_WARP) != 0;
}

bool MouseKeys::isMouseWheelKey(const Key &key) const {
  uint8_t mask    = (KEY_MOUSE_BUTTON | KEY_MOUSE_WARP | KEY_MOUSE_WHEEL);
  uint8_t variant = key.getKeyCode() & mask;
  return variant == KEY_MOUSE_WHEEL;
}

// =============================================================================
// Event Handlers

// -----------------------------------------------------------------------------
EventHandlerResult MouseKeys::onNameQuery() {
  return ::Focus.sendName(F("MouseKeys"));
}

// -----------------------------------------------------------------------------
EventHandlerResult MouseKeys::onSetup() {
  kaleidoscope::Runtime.hid().mouse().setup();
  kaleidoscope::Runtime.hid().absoluteMouse().setup();

  return EventHandlerResult::OK;
}

// -----------------------------------------------------------------------------
EventHandlerResult MouseKeys::afterEachCycle() {
  // Check timeout for accel update interval.
  if (Runtime.hasTimeExpired(accel_start_time_, accelDelay)) {
    accel_start_time_ = Runtime.millisAtCycleStart();
    // `accel_step` determines the movement speed of the mouse pointer, and gets
    // reset to zero when no mouse movement keys is pressed (see below).
    if (mousekeys::wrapper.accel_step < 255 - accelSpeed) {
      mousekeys::wrapper.accel_step += accelSpeed;
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
EventHandlerResult MouseKeys::onKeyEvent(KeyEvent &event) {
  if (!isMouseKey(event.key))
    return EventHandlerResult::OK;

  pending_directions_ = 0;

  if (isMouseButtonKey(event.key)) {
    // Clear button state; it will be repopulated by `onAddToReport()`, and the
    // report will be sent by `afterReportingState()`.
    buttons_ = 0;

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
EventHandlerResult MouseKeys::afterReportingState(const KeyEvent &event) {
  if (!isMouseKey(event.key))
    return EventHandlerResult::OK;

  // If a mouse button key has toggled on or off, we send a mouse report with
  // the updated information.
  if (isMouseButtonKey(event.key)) {
    sendMouseButtonReport();
  }

  // A mouse key event has been successfully registered, and we have now
  // gathered all the information on held mouse movement and wheel keys, so it's
  // safe to update the direction information.
  directions_         = pending_directions_;
  pending_directions_ = 0;

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
EventHandlerResult MouseKeys::onAddToReport(Key key) {
  if (!isMouseKey(key))
    return EventHandlerResult::OK;

  if (isMouseButtonKey(key))
    buttons_ |= (key.getKeyCode() & ~KEY_MOUSE_BUTTON);

  if (isMouseMoveKey(key))
    pending_directions_ |= key.getKeyCode();

  if (isMouseWheelKey(key))
    pending_directions_ |= (key.getKeyCode() << wheel_offset_);

  return EventHandlerResult::OK;
}

// =============================================================================
// HID report helper functions

// -----------------------------------------------------------------------------
void MouseKeys::sendMouseButtonReport() const {
  Runtime.hid().mouse().releaseAllButtons();
  Runtime.hid().mouse().pressButtons(buttons_);
  Runtime.hid().mouse().sendReport();
}

// -----------------------------------------------------------------------------
void MouseKeys::sendMouseWarpReport(const KeyEvent &event) const {
  mousekeys::wrapper.warp(
    ((event.key.getKeyCode() & KEY_MOUSE_WARP_END) ? WARP_END : 0x00) |
    ((event.key.getKeyCode() & KEY_MOUSE_UP) ? WARP_UP : 0x00) |
    ((event.key.getKeyCode() & KEY_MOUSE_DOWN) ? WARP_DOWN : 0x00) |
    ((event.key.getKeyCode() & KEY_MOUSE_LEFT) ? WARP_LEFT : 0x00) |
    ((event.key.getKeyCode() & KEY_MOUSE_RIGHT) ? WARP_RIGHT : 0x00));
}

// -----------------------------------------------------------------------------
void MouseKeys::sendMouseMoveReport() {
  move_start_time_ = Runtime.millisAtCycleStart();

  int8_t vx         = 0;
  int8_t vy         = 0;
  uint8_t direction = directions_ & move_mask_;

  if (direction == 0) {
    // If there are no mouse movement keys held, reset speed to zero.
    mousekeys::wrapper.accel_step = 0;
  } else {
    // For each active direction, add the mouse movement speed.
    if (direction & KEY_MOUSE_LEFT)
      vx -= speed;
    if (direction & KEY_MOUSE_RIGHT)
      vx += speed;
    if (direction & KEY_MOUSE_UP)
      vy -= speed;
    if (direction & KEY_MOUSE_DOWN)
      vy += speed;

    // Prepare the mouse report.
    mousekeys::wrapper.move(vx, vy);
    // Send the report.
    Runtime.hid().mouse().sendReport();
  }
}

// -----------------------------------------------------------------------------
void MouseKeys::sendMouseWheelReport() {
  wheel_start_time_ = Runtime.millisAtCycleStart();

  int8_t vx         = 0;
  int8_t vy         = 0;
  uint8_t direction = directions_ >> wheel_offset_;

  if (direction != 0) {
    // Horizontal scroll wheel:
    if (direction & KEY_MOUSE_LEFT)
      vx -= wheelSpeed;
    if (direction & KEY_MOUSE_RIGHT)
      vx += wheelSpeed;
    // Vertical scroll wheel (note coordinates are opposite movement):
    if (direction & KEY_MOUSE_UP)
      vy += wheelSpeed;
    if (direction & KEY_MOUSE_DOWN)
      vy -= wheelSpeed;

    // Add scroll wheel changes to HID report.
    Runtime.hid().mouse().move(0, 0, vy, vx);
    // Send the report.
    Runtime.hid().mouse().sendReport();
  }
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::MouseKeys MouseKeys;
