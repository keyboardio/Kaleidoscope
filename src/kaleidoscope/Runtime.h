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

#include <stdint.h>  // for uint32_t

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/LiveKeys.h"              // for LiveKeys, live_keys
#include "kaleidoscope/device/device.h"         // for Device
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/hooks.h"                 // for Hooks
#include "kaleidoscope/key_defs.h"              // for Key, Key_Transparent
#include "kaleidoscope/layers.h"                // for Layer, Layer_
#include "kaleidoscope_internal/device.h"       // for device

namespace kaleidoscope {

class Runtime_ {
 public:
  Runtime_(void);

  void setup(void);
  void loop(void);

  static constexpr kaleidoscope::Device &device() {
    return kaleidoscope_internal::device;
  }

  static constexpr bool has_leds = (kaleidoscope_internal::device.led_count > 0);

  /** Detaching from / attaching to the host.
   *
   * These two functions wrap the hardware plugin's similarly named functions.
   * We wrap them, because we'd like plugins and user-code not having to use
   * `Runtime.device()` directly.
   *
   * The methods themselves implement detaching from / attaching to the host,
   * without rebooting the device, and remaining powered in between.
   *
   * Intended to be used in cases where we want to change some settings between
   * detach and attach.
   */
  void detachFromHost() {
    device().detachFromHost();
  }
  void attachToHost() {
    device().attachToHost();
  }

  /** Wrapper functions for some device features.
   *
   * The next three functions wrap methods of the device plugin, to make using
   * them in user sketches easier, and require less typing.
   *
   * For what they do, please see the documentation in the
   * `kaleidoscope::device::Base` class.
   */
  auto serialPort() -> decltype(device().serialPort()) & {
    return device().serialPort();
  }

  auto storage() -> decltype(device().storage()) & {
    return device().storage();
  }

  auto hid() -> decltype(device().hid()) & {
    return device().hid();
  }

  void rebootBootloader() {
    device().rebootBootloader();
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
   *      calling `Runtime.millisAtCycleStart()`. It can be any integer
   *      type.
   *
   * - ttl: The timeout value or interval to check (ttl = "time to live"). The
   *      timer expires (and `hasTimeExpired()` returns true) when the time that
   *      has elapsed since `start_time` exceeds this value. It must be an
   *      integer type that is no bigger than the type of `start_time`.
   *
   * Warning: When using hasTimeExpired, make sure that the value of ttl
   *          is at least by the size of the call interval smaller than
   *          the maximum value that the datatype of timeout can store.
   *          Otherwise false negatives are likely to occur
   *          when checking (elapsed_time > ttl).
   *
   * Example: When both arguments of hasTimeExpired are of type uint16_t and
   *          the function is called in every cycle with a cycle duration
   *          of x ms, the value of ttl must not be larger than
   *          std::numeric_limits<uint16_t>::max() - x.
   */
  template<typename _Timestamp, typename _Timeout>
  static bool hasTimeExpired(_Timestamp start_time, _Timeout ttl) {
    _Timestamp current_time = millis_at_cycle_start_;
    _Timestamp elapsed_time = current_time - start_time;
    return (elapsed_time >= ttl);
  }

  EventHandlerResult onFocusEvent(const char *input) {
    return kaleidoscope::Hooks::onFocusEvent(input);
  }

  /** Handle a physical keyswitch event
   *
   * This method is called in response to physical keyswitch state changes. Its
   * job is to call the `onKeyswitchEvent()` plugin handler functions, used by
   * plugins that are particularly concerned about the timing of those
   * events. It takes only one parameter, of type `KeyEvent`, which encapsulates
   * the information about that event:
   *
   * - `event.key_addr`: The address of the key that was pressed or released.
   * - `event.state`: The state of the keyswitch event (toggled on or off).
   * - `event.key`: The `Key` value for this event.
   * - `event.id`: A semi-unique ID value for the event.
   *
   * The ID value is used to help plugins that delay events to coordinate with
   * each other so that they can avoid re-processing the same event, possibly
   * causing endless loops.
   */
  void handleKeyswitchEvent(KeyEvent event);

  /** Handle a logical key event
   *
   * This method triggers the handling of a logical "key event". Ususally that
   * event is the result of a call to `handleKeyswitchEvent()`, but it can also
   * be called by plugins that need to generate extra events without a 1:1
   * mapping to physical keyswitch state transitions.
   */
  void handleKeyEvent(KeyEvent event);

  /** Prepare a new set of USB HID reports
   *
   * This method gets called when a key event results in at least one new HID
   * report being sent to the host, usually as a result of a call to
   * `handleKeyEvent()`. It clears the keyboard report (after plugins have
   * already responded to the new event that triggered the forthcoming report),
   * then populates the new report based on the values stored in the `live_keys`
   * state array.
   */
  void prepareKeyboardReport(const KeyEvent &event);

  /** Add keycode(s) to a USB HID report
   *
   * This method gets called from `prepareKeyboardReport()` to add keycodes
   * corresponding to active keys in the `live_keys` state array to the Keyboard
   * & Consumer Control HID reports. It calls the `onAddToReport()` plugin
   * handlers first to give them a chance to abort.
   */
  void addToReport(Key key);

  /** Send the new USB HID report(s)
   *
   * This method is called by `handleKeyEvent()` after `prepareKeyboardReport()`
   * is done. It uses the information about the new event to guard against
   * modifier and mod-flags rollover issues, and calls the
   * `beforeReportingState()` plugin handler functions before sending the
   * complete Keyboard and Consumer Control HID reports.
   */
  void sendKeyboardReport(const KeyEvent &event);

  /** Get the current value of a keymap entry
   *
   * Returns the `Key` value for a given `KeyAddr` entry in the current keymap,
   * overridden by any active entry in the `live_keys` array.
   */
  Key lookupKey(KeyAddr key_addr) {
    // First, check for an active key value in the `live_keys` array.
    Key key = live_keys[key_addr];
    // If that entry is clear, look up the entry from the active keymap layers.
    if (key == Key_Transparent) {
      key = Layer.lookupOnActiveLayer(key_addr);
    }
    return key;
  }

 private:
  static uint32_t millis_at_cycle_start_;
  static KeyAddr last_addr_toggled_on_;
};

extern kaleidoscope::Runtime_ Runtime;

}  // namespace kaleidoscope
