/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2018  Keyboard.io, Inc.
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

#include "kaleidoscope_internal/LEDModeManager.h"

#include "kaleidoscope/plugin/LEDMode.h"  // for LEDMode

namespace kaleidoscope {
namespace internal {

namespace {

// We want to export as little symbols as possible. That's why the
// internal state of the LED mode management lives in
// an anonymous namespace.

uint8_t cur_mode_id = 255;  // We use 255 as a flag value that signals
// uninitialized. That's why we can only have
// LED mode ids in the range [0..254].

// A pointer to cache the current transient LED mode.
//
kaleidoscope::plugin::LEDMode *cur_led_mode = nullptr;

bool current_led_mode_dynamic = false;

}  // namespace

kaleidoscope::plugin::LEDMode *LEDModeManager::getLEDMode(uint8_t mode_id) {

  // If we end up here, the requested LED mode is a dynamic one.

  // Check if the requested LED mode is already active
  //
  if (cur_mode_id == mode_id) {
    return cur_led_mode;
  }

  // If there is already an active LED mode, its obviously the wrong one
  // (see test above). To generate a new transient LED mode, we need
  // to destroy the current one. To achieve this, we call its
  // (possibly - see explanation below) virtual destuctor and let
  // it take care of the cleanup.
  //
  // Please note that due to the fact that transient LED modes are
  // allocated using placement new within a pre-existing static buffer,
  // there is no need to free any memory after the destructor of the
  // current LED mode was called. We can just reuse the buffer
  // for the next LED mode instance.
  //
  // Please note: Currently, LEDMode has no virtual destructor.
  //              That's why the explicit destructor call below is a noop
  //              that is optimized out by the compiler. It is there
  //              to enable the possible future introduction of a virtual
  //              destructor for class LEDMode.
  //
  if (current_led_mode_dynamic) {
    cur_led_mode->~LEDMode();
  }

  // Store the current mode id to enable cache access (see above).
  //
  cur_mode_id = mode_id;

  // Get a factory struct for the creation of the new LED mode.
  //
  led_mode_management::LEDModeFactory fac;

  retreiveLEDModeFactoryFromPROGMEM(mode_id, fac);

  // The factories for persistent LED modes serve to pass through the LED
  // mode plugin's pointer in parent_plugin_.
  //
  if (fac.isPersistentLEDMode()) {
    current_led_mode_dynamic = false;
    cur_led_mode             = fac.getPersistentLEDMode();
  } else {
    current_led_mode_dynamic = true;

    // Generate a new led mode by calling the factory function
    // (fac.generate_led_mode_).
    //
    // We store a pointer to the newly created LED mode to enable
    // cached access (see check above).
    //
    cur_led_mode = fac.generateTransientLEDMode(led_mode_buffer_, mode_id);
  }

  return cur_led_mode;
}

}  // namespace internal
}  // namespace kaleidoscope
