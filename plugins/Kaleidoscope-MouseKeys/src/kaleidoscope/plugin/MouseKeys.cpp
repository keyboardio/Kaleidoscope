/* Kaleidoscope-MouseKeys - Mouse keys for Kaleidoscope.
 * Copyright (C) 2017-2022  Keyboard.io, Inc.
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

#include "kaleidoscope/KeyEvent.h"                       // for KeyEvent
#include "kaleidoscope/Runtime.h"                        // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"                  // for Base<>::HID, VirtualProps:...
#include "kaleidoscope/driver/hid/base/AbsoluteMouse.h"  // for AbsoluteMouse
#include "kaleidoscope/driver/hid/base/Mouse.h"          // for Mouse
#include "kaleidoscope/event_handler_result.h"           // for EventHandlerResult, EventH...
#include "kaleidoscope/key_defs.h"                       // for Key, SYNTHETIC
#include "kaleidoscope/keyswitch_state.h"                // for keyToggledOn
#include "kaleidoscope/plugin/mousekeys/MouseKeyDefs.h"  // for KEY_MOUSE_BUTTON, KEY_MOUS...
#include "kaleidoscope/plugin/mousekeys/MouseWrapper.h"  // for MouseWrapper, WARP_DOWN

namespace kaleidoscope {
namespace plugin {

#ifndef NDEPRECATED
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
uint8_t MouseKeys::speed       = 1;
uint16_t MouseKeys::speedDelay = 1;

uint8_t MouseKeys::accelSpeed  = 1;
uint16_t MouseKeys::accelDelay = 64;

uint8_t MouseKeys::wheelSpeed  = 1;
uint16_t MouseKeys::wheelDelay = 50;
#pragma GCC diagnostic pop
#endif

// =============================================================================
// Configuration functions

void MouseKeys::setWarpGridSize(uint8_t grid_size) {
  // Sanity check the grid size here, so we do not need to do that at the call
  // sites. When given an invalid grid size, just don't touch the setting.
  if (grid_size != MOUSE_WARP_GRID_2X2 &&
      grid_size != MOUSE_WARP_GRID_3X3) return;

  settings_.warp_grid_size = grid_size;
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
  kaleidoscope::Runtime.hid().absoluteMouse().setup();

  return EventHandlerResult::OK;
}

// -----------------------------------------------------------------------------
EventHandlerResult MouseKeys::afterEachCycle() {
  if (directions_ == 0)
    return EventHandlerResult::OK;

  // Check timeout for position update interval.
  if (Runtime.hasTimeExpired(last_cursor_update_time_, cursor_update_interval_)) {
    sendMouseMoveReport();
    last_cursor_update_time_ += cursor_update_interval_;
  }

  // Check timeout for scroll report interval.
  if (Runtime.hasTimeExpired(last_wheel_update_time_, settings_.wheel_update_interval)) {
    sendMouseWheelReport();
    last_wheel_update_time_ += settings_.wheel_update_interval;
  }

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
  }

  if (keyToggledOn(event.state)) {
    if (isMouseWarpKey(event.key)) {
      // If a mouse warp key toggles on, we immediately send the warp report.
      sendMouseWarpReport(event);
    } else {
      // If any non-warp mouse key toggles on, we cancel warping.
      MouseWrapper.endWarping();
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

  // If no mouse move keys were active before this event, and a mouse movement
  // key toggled on, we need to set the move start time so that acceleration can
  // begin correctly.
  if ((directions_ & cursor_mask_) == 0) {
    cursor_start_time_ = Runtime.millisAtCycleStart();
  }

  // A mouse key event has been successfully registered, and we have now
  // gathered all the information on held mouse movement and wheel keys, so it's
  // safe to update the direction information.
  directions_         = pending_directions_;
  pending_directions_ = 0;

  if (keyToggledOn(event.state)) {
    if (isMouseMoveKey(event.key)) {
      sendMouseMoveReport();
      last_cursor_update_time_ = Runtime.millisAtCycleStart();
    } else if (isMouseWheelKey(event.key)) {
      sendMouseWheelReport();
      last_wheel_update_time_ = Runtime.millisAtCycleStart();
    }
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
  MouseWrapper.warp(
    ((event.key.getKeyCode() & KEY_MOUSE_WARP_END) ? WARP_END : 0x00) |
    ((event.key.getKeyCode() & KEY_MOUSE_UP) ? WARP_UP : 0x00) |
    ((event.key.getKeyCode() & KEY_MOUSE_DOWN) ? WARP_DOWN : 0x00) |
    ((event.key.getKeyCode() & KEY_MOUSE_LEFT) ? WARP_LEFT : 0x00) |
    ((event.key.getKeyCode() & KEY_MOUSE_RIGHT) ? WARP_RIGHT : 0x00));
}

// -----------------------------------------------------------------------------
void MouseKeys::sendMouseMoveReport() const {
  int8_t dx = 0;
  int8_t dy = 0;

  uint8_t direction = directions_ & cursor_mask_;

  if (direction != 0) {
    // Calculate
    uint8_t delta = cursorDelta();
    // For each active direction, add the move update interval value to
    // normalize speed of motion regardless of the frequency of updates.
    if (direction & KEY_MOUSE_LEFT)
      dx -= delta;
    if (direction & KEY_MOUSE_RIGHT)
      dx += delta;
    if (direction & KEY_MOUSE_UP)
      dy -= delta;
    if (direction & KEY_MOUSE_DOWN)
      dy += delta;

    // Send the report.
    Runtime.hid().mouse().move(dx, dy, 0, 0);
    Runtime.hid().mouse().sendReport();
  }
}

// -----------------------------------------------------------------------------
// Get the current point on the acceleration curve's x axis, translating time
// elapsed since mouse movement started to a value between 0 and 255.
uint8_t MouseKeys::accelStep() const {
  uint16_t elapsed_time   = Runtime.millisAtCycleStart() - cursor_start_time_;
  uint16_t accel_duration = settings_.cursor_accel_duration;
  if (elapsed_time > accel_duration)
    return 255;
  uint16_t accel_step = (uint32_t(elapsed_time) * 256) / accel_duration;
  return uint8_t(accel_step);
}

// -----------------------------------------------------------------------------
// Compute the acceleration factor for mouse movement.  When a movement key is
// first pressed, the cursor starts out slow then accelerates to full speed.
// The speed during acceleration follows an approximation of a sigmoid function,
// using two parabolas for simplicity.
uint8_t accelFactor(uint8_t accel_step) {
  if (accel_step < 128) {
    uint16_t y = accel_step * accel_step;
    return 1 + (y >> 7);
  } else {
    uint16_t remaining_steps = 256 - accel_step;

    uint16_t y = remaining_steps * remaining_steps;
    return 255 - (y >> 7);
  }
}

// -----------------------------------------------------------------------------
// Compute the distance the mouse cursor should move in subpixels, return the
// number of pixels the mouse should move (in active directions), and store the
// remaining subpixels for the next move.
uint8_t MouseKeys::cursorDelta() const {
  // When the cursor speed is slow, it can be moving less than one pixel per
  // update, so we need to calculate movement in "subpixels" and store the
  // remaining subpixels to add to the next update's movement.
  static uint8_t subpixel_remainder{0};

  // First, we calculate where we are on the "time" axis of the acceleration
  // curve, based on the time passed since the first cursor movement key was
  // pressed.
  uint8_t accel_step = accelStep();

  // Next, we translate that into a speed scaling factor (from 1-255).  If we
  // had an FPU, we would do this in floating point, with a scale between 0 and
  // 1, so this is how we emulate that using only integer (i.e. fixed-point)
  // arithmetic.
  uint8_t accel_factor = accelFactor(accel_step);

  // We want the cursor to start out with some minimum speed, otherwise the user
  // presses a movement key and then waits for a while before the cursor moves
  // even one pixel.  We need to multiply our speed-scaling factor by the
  // difference between the starting speed and the full speed, then add the
  // starting speed (multiplied by the full value of the scaling factor) to get
  // the current speed.
  uint8_t max_speed       = settings_.cursor_base_speed;
  uint8_t min_speed       = settings_.cursor_init_speed;
  uint8_t speed_range     = max_speed - min_speed;
  uint16_t subpixel_speed = (speed_range * accel_factor);
  subpixel_speed += (min_speed * 256);

  // We want to end up with small numbers of pixels, otherwise the speed will be
  // too fast to be useful.  But we also want to be able to make fine
  // adjustments to the speed, so `settings_.cursor_base_speed` should be
  // allowed to have a reasonbly high value, using all eight bits.  This means
  // that "decimal point" needs to be somewhere in the high byte of this 16-bit
  // value.  In order to store only eight bits of subpixel remainder, we need to
  // do a shift first.  This amount is arbitrary, but seems like a reasonable
  // compromise.
  subpixel_speed >>= 4;

  // `max_speed` and `accel_factor` can both be up to 255.  So we can't
  // just multiply by `cursor_update_interval_ without risk of overflow.  The
  // update interval should be some low number, anyway (8 or less, I think), and
  // should probably be fixed as a constexpr, so we could just leave it out.
  subpixel_speed *= cursor_update_interval_;

  // There's no risk of overflow here: (255 * 255) + 255 = 65535
  subpixel_speed += subpixel_remainder;

  // Set minimum speed
  subpixel_speed += 64;

  // This shift should be more than eight pixels; a single update of 100 pixels
  // is a huge jump.  See above.
  uint8_t pixel_speed = subpixel_speed >> 8;
  // Truncate to get only lower 8 bits.
  subpixel_remainder = subpixel_speed;
  //subpixel_remainder = subpixel_speed - (uint16_t(pixel_speed) << 8);
  return pixel_speed;
}

// -----------------------------------------------------------------------------
// Wheel speed should be controlled by changing the update interval, not by
// setting `wheel_speed_`.
void MouseKeys::sendMouseWheelReport() const {
  int8_t dh = 0;
  int8_t dv = 0;

  uint8_t direction = directions_ >> wheel_offset_;

  if (direction != 0) {
    // Horizontal scroll wheel:
    if (direction & KEY_MOUSE_LEFT)
      dh -= 1;
    if (direction & KEY_MOUSE_RIGHT)
      dh += 1;
    // Vertical scroll wheel (note coordinates are opposite movement):
    if (direction & KEY_MOUSE_UP)
      dv += 1;
    if (direction & KEY_MOUSE_DOWN)
      dv -= 1;

    // Send the report.
    Runtime.hid().mouse().move(0, 0, dv, dh);
    Runtime.hid().mouse().sendReport();
  }
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::MouseKeys MouseKeys;
