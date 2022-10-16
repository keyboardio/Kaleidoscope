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

#pragma once

#include <stdint.h>  // for uint8_t, uint16_t

#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/key_defs.h"              // for Key
#include "kaleidoscope/plugin.h"                // for Plugin

#include "kaleidoscope/plugin/mousekeys/MouseWarpModes.h"  // for warp modes
// =============================================================================
// Deprecated MousKeys code
#include "kaleidoscope_internal/deprecations.h"  // for DEPRECATED

#define _DEPRECATED_MESSAGE_MOUSEKEYS_SET_SPEED_LIMIT                           \
  "The `MouseKeys.setSpeedLimit()` function is deprecated.  It no longer has\n" \
  "any function, and can be safely removed."

#define _DEPRECATED_MESSAGE_MOUSEKEYS_SPEED                                  \
  "Direct access to the `MouseKeys.speed` variable has been deprecated.\n"   \
  "Please refer to the MouseKeys documentation for instructions on how to\n" \
  "configure the plugin.\n"

#define _DEPRECATED_MESSAGE_MOUSEKEYS_SPEED_DELAY                               \
  "Direct access to the `MouseKeys.speedDelay` variable has been deprecated.\n" \
  "Please refer to the MouseKeys documentation for instructions on how to\n"    \
  "configure the plugin.\n"

#define _DEPRECATED_MESSAGE_MOUSEKEYS_ACCEL_SPEED                               \
  "Direct access to the `MouseKeys.accelSpeed` variable has been deprecated.\n" \
  "Please refer to the MouseKeys documentation for instructions on how to\n"    \
  "configure the plugin.\n"

#define _DEPRECATED_MESSAGE_MOUSEKEYS_ACCEL_DELAY                               \
  "Direct access to the `MouseKeys.accelDelay` variable has been deprecated.\n" \
  "Please refer to the MouseKeys documentation for instructions on how to\n"    \
  "configure the plugin.\n"

#define _DEPRECATED_MESSAGE_MOUSEKEYS_WHEEL_SPEED                               \
  "Direct access to the `MouseKeys.wheelSpeed` variable has been deprecated.\n" \
  "Please refer to the MouseKeys documentation for instructions on how to\n"    \
  "configure the plugin.\n"

#define _DEPRECATED_MESSAGE_MOUSEKEYS_WHEEL_DELAY                               \
  "Direct access to the `MouseKeys.wheelDelay` variable has been deprecated.\n" \
  "Please refer to the MouseKeys documentation for instructions on how to\n"    \
  "configure the plugin.\n"


namespace kaleidoscope {
namespace plugin {
class MouseKeys : public kaleidoscope::Plugin {
 public:
#ifndef NDEPRECATED
  DEPRECATED(MOUSEKEYS_SPEED)
  static uint8_t speed;
  DEPRECATED(MOUSEKEYS_SPEED_DELAY)
  static uint16_t speedDelay;
  DEPRECATED(MOUSEKEYS_ACCEL_SPEED)
  static uint8_t accelSpeed;
  DEPRECATED(MOUSEKEYS_ACCEL_DELAY)
  static uint16_t accelDelay;
  DEPRECATED(MOUSEKEYS_WHEEL_SPEED)
  static uint8_t wheelSpeed;
  DEPRECATED(MOUSEKEYS_WHEEL_DELAY)
  static uint16_t wheelDelay;

  DEPRECATED(MOUSEKEYS_SET_SPEED_LIMIT)
  static void setSpeedLimit(uint8_t speed_limit) {}
#endif

  void setWarpGridSize(uint8_t grid_size);
  uint8_t getWarpGridSize() {
    return settings_.warp_grid_size;
  }

  /// Get the current mouse (full) speed setting
  ///
  /// This returns the value for full-speed mouse movement (after the initial
  /// acceleration period), not the current speed of the mouse cursor on screen.
  /// The value does not have straightforward units, but it is linear.
  uint8_t getCursorBaseSpeed() const {
    return settings_.cursor_base_speed;
  }
  /// Set the full-speed mouse movement value
  void setCursorBaseSpeed(uint8_t speed) {
    settings_.cursor_base_speed = speed;
  }

  /// Get the initial mouse cursor movement speed setting
  uint8_t getCursorInitSpeed() const {
    return settings_.cursor_init_speed;
  }
  /// Set the initial mouse cursor movement speed
  void setCursorInitSpeed(uint8_t speed) {
    settings_.cursor_init_speed = speed;
  }

  /// Get the current acceleration window duration
  uint16_t getCursorAccelDuration() const {
    return settings_.cursor_accel_duration;
  }
  /// Set the acceleration window duration
  void setCursorAccelDuration(uint16_t duration) {
    settings_.cursor_accel_duration = duration;
  }

  /// Get the current mouse wheel update interval
  ///
  /// Returns the interval (in milliseconds) between mouse wheel updates while a
  /// mouse wheel key is active (held).
  uint8_t getScrollInterval() const {
    return settings_.wheel_update_interval;
  }
  /// Set the current mouse wheel update interval
  ///
  /// Sets the wheel update interval to the specified number of milliseconds.
  void setScrollInterval(uint8_t interval) {
    settings_.wheel_update_interval = interval;
  }

  EventHandlerResult onSetup();
  EventHandlerResult onNameQuery();
  EventHandlerResult afterEachCycle();
  EventHandlerResult onKeyEvent(KeyEvent &event);
  EventHandlerResult onAddToReport(Key key);
  EventHandlerResult afterReportingState(const KeyEvent &event);

  // ---------------------------------------------------------------------------
  // Structure for storing all user-configurable settings.
  struct Settings {
    uint8_t wheel_update_interval  = 50;
    uint8_t cursor_init_speed      = 1;
    uint8_t cursor_base_speed      = 50;
    uint16_t cursor_accel_duration = 1000;
    uint8_t warp_grid_size         = MOUSE_WARP_GRID_2X2;
  };

  // ---------------------------------------------------------------------------
  // This lets the MouseKeysConfig plugin access the internal config variables
  // directly. Mainly useful for calls to `Runtime.storage.get()`/`.put()`.
  friend class MouseKeysConfig;

 private:
  static constexpr uint8_t cursor_update_interval_ = 4;

  Settings settings_;

  uint16_t cursor_start_time_      = 0;
  uint8_t last_cursor_update_time_ = 0;
  uint8_t last_wheel_update_time_  = 0;

  // Mouse cursor and wheel movement directions are stored in a single bitfield
  // to save space.  The low four bits are for cursor movement, and the high
  // four are for wheel movement.
  static constexpr uint8_t wheel_offset_ = 4;
  static constexpr uint8_t wheel_mask_   = 0b11110000;
  static constexpr uint8_t cursor_mask_  = 0b00001111;

  uint8_t directions_         = 0;
  uint8_t pending_directions_ = 0;
  uint8_t buttons_            = 0;

  bool isMouseKey(const Key &key) const;
  bool isMouseButtonKey(const Key &key) const;
  bool isMouseMoveKey(const Key &key) const;
  bool isMouseWarpKey(const Key &key) const;
  bool isMouseWheelKey(const Key &key) const;

  void sendMouseButtonReport() const;
  void sendMouseWarpReport(const KeyEvent &event) const;
  void sendMouseMoveReport() const;
  void sendMouseWheelReport() const;

  uint8_t accelStep() const;
  uint8_t cursorDelta() const;
};

// =============================================================================
// Plugin for configuration of MouseKeys via Focus and persistent storage of
// settins in EEPROM (i.e. Chrysalis).
class MouseKeysConfig : public Plugin {
 public:
  EventHandlerResult onSetup();
  EventHandlerResult onFocusEvent(const char *command);

 private:
  // The base address in persistent storage for configuration data:
  uint16_t settings_addr_;
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::MouseKeys MouseKeys;
extern kaleidoscope::plugin::MouseKeysConfig MouseKeysConfig;
