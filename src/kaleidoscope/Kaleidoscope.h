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

#pragma once

#include <Arduino.h>

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project KeyboardIO here

#define TS(X) //Serial.print(micros() );Serial.print("\t");Serial.println(X);

#include <stdio.h>
#include <math.h>
#include <stdint.h>

#include KALEIDOSCOPE_HARDWARE_H

extern HARDWARE_IMPLEMENTATION KeyboardHardware;

#ifdef PER_KEY_DATA_STACKED
#define KEYMAP_STACKED(...) { PER_KEY_DATA_STACKED(XXX, __VA_ARGS__) }
#endif

#ifdef PER_KEY_DATA
#define KEYMAP(...) { PER_KEY_DATA(XXX, __VA_ARGS__) }
#endif

#define ROWS (KeyboardHardware.matrix_rows)
#define COLS (KeyboardHardware.matrix_columns)
#define LED_COUNT (KeyboardHardware.led_count)

#include "kaleidoscope/key_events.h"
#include "kaleidoscope/hid.h"
#include "kaleidoscope/layers.h"
#include "kaleidoscope/macro_map.h"
#include "kaleidoscope_internal/event_dispatch.h"
#include "kaleidoscope_internal/LEDModeManager.h"
#include "kaleidoscope/macro_helpers.h"
#include "kaleidoscope/plugin.h"

#define HOOK_MAX 64

#ifndef VERSION
#define VERSION "locally-built"
#endif

/** Kaleidoscope API (major) version.
 *
 * The API is guaranteed to be backwards compatible for the entire duration of a
 * major version. However, breaking changes may come, and result in a major
 * version bump. To help migration, the `KALEIDOSCOPE_API_VERSION` macro can be
 * used to check the major version provided by the Kaleidoscope we are compiling
 * against. This can be used to error out with a helpful message, or change how
 * the API is used - it is entirely up to the plugin or sketch author. The point
 * of this macro is to let them easily check the version.
 */
#define KALEIDOSCOPE_API_VERSION 2

/** Required Kaleidoscope major version.
 *
 * For the sake of convenience, defining `KALEIDOSCOPE_REQUIRED_API_VERSION`
 * before including `Kaleidoscope.h` itself will result in comparing its value
 * to `KALEIDOSCOPE_API_VERSION`. If they differ, a helpful error message is
 * printed.
 *
 * Done so that a new API version would result in a helpful error message,
 * instead of cryptic compile errors.
 */
#if defined(KALEIDOSCOPE_REQUIRED_API_VERSION) && (KALEIDOSCOPE_REQUIRED_API_VERSION != KALEIDOSCOPE_API_VERSION)
#define xstr(a) str(a)
#define str(a) #a
static_assert(KALEIDOSCOPE_REQUIRED_API_VERSION == KALEIDOSCOPE_API_VERSION,
              "Kaleidoscope API version mismatch! We have version " xstr(KALEIDOSCOPE_API_VERSION)
              " available, but version " xstr(KALEIDOSCOPE_REQUIRED_API_VERSION) " is required.");
#endif

namespace kaleidoscope {

class Kaleidoscope_ {
 public:
  Kaleidoscope_(void);

  void setup(void);
  void loop(void);

  static constexpr bool has_leds = (KeyboardHardware.led_count > 0);

  /** Detaching from / attaching to the host.
   *
   * These two functions wrap the hardware plugin's similarly named functions.
   * We wrap them, because we'd like plugins and user-code not having to use
   * `KeyboardHardware` directly.
   *
   * The methods themselves implement detaching from / attaching to the host,
   * without rebooting the device, and remaining powered in between.
   *
   * Intended to be used in cases where we want to change some settings between
   * detach and attach.
   */
  void detachFromHost() {
    KeyboardHardware.detachFromHost();
  }
  void attachToHost() {
    KeyboardHardware.attachToHost();
  }

  /** Returns the timer as it was at the start of the cycle.
   * The goal of this method is two-fold:
   *  - To reduce the amount of calls to millis(), providing something cheaper.
   *  - To have a consistent timer value for the whole duration of a cycle.
   *
   * This cached value is updated at the start of each cycle as the name
   * implies. It is recommended to use this in plugins over millis() unless
   * there is good reason not to.
   */
  static uint32_t millisAtCycleStart() {
    return millis_at_cycle_start_;
  }

  /** Determines if a timer has expired.
   *
   * This method should be used whenever checking to see if a timeout has been
   * reached. It correctly computes timeout values even when integer overflow
   * occurs, given a start time and a timeout. It takes two parameters:
   *
   * - start_time: A timestamp when the timer started, which should be set by
   *      calling `Kaleidoscope.millisAtCycleStart()`. It can be any integer
   *      type.
   *
   * - ttl: The timeout value or interval to check (ttl = "time to live"). The
   *      timer expires (and `hasTimeExpired()` returns true) when the time that
   *      has elapsed since `start_time` exceeds this value. It must be an
   *      integer type that is no bigger than the type of `start_time`.
   */
  template <typename _Timestamp, typename _Timeout>
  bool hasTimeExpired(_Timestamp start_time, _Timeout ttl) {
    _Timestamp current_time = millis_at_cycle_start_;
    _Timestamp elapsed_time = current_time - start_time;
    return (elapsed_time > ttl);
  }

  EventHandlerResult onFocusEvent(const char *command) {
    return kaleidoscope::Hooks::onFocusEvent(command);
  }

 private:
  static uint32_t millis_at_cycle_start_;
};

extern kaleidoscope::Kaleidoscope_ Kaleidoscope;

} // namespace kaleidoscope

// For compatibility reasons we enable class Kaleidoscope_ also to be available
// in global namespace.
//
typedef kaleidoscope::Kaleidoscope_  Kaleidoscope_;

// For compatibility reasons we enable the global variable Kaleidoscope
// in global namespace.
//
using kaleidoscope::Kaleidoscope;

// Use this function macro to register plugins with Kaleidoscope's
// hooking system. The macro accepts a list of plugin instances that
// must have been instantiated at global scope.
//
#define KALEIDOSCOPE_INIT_PLUGINS(...) _KALEIDOSCOPE_INIT_PLUGINS(__VA_ARGS__)
