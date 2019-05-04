/* -*- mode: c++ -*-
 * Kaleidoscope-Qukeys -- Assign two keycodes to a single key
 * Copyright (C) 2017-2019  Michael Richters
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <Kaleidoscope.h>
#include <Kaleidoscope-Ranges.h>
#include "kaleidoscope/plugin/Qukeys/KeyAddrEventQueue.h"

// DualUse Key definitions for Qukeys in the keymap
#define MT(mod, key) Key(                                               \
    kaleidoscope::ranges::DUM_FIRST +                                   \
    (((Key_ ## mod).keyCode - Key_LeftControl.keyCode) << 8) +          \
    (Key_ ## key).keyCode                                               \
)
#define SFT_T(key) MT(LeftShift, key)
#define CTL_T(key) MT(LeftControl, key)
#define ALT_T(key) MT(LeftAlt, key)
#define GUI_T(key) MT(LeftGui, key)

#define LT(layer, key) Key(kaleidoscope::ranges::DUL_FIRST + (layer << 8) + (Key_ ## key).keyCode)

#define _DEPRECATED_MESSAGE_QUKEY_ROW_COL_CONSTRUCTOR                        \
  "The `Qukey(layer, row, col, alternate_key)` constructor using separate\n" \
  "`row` & `col` parameters has been deprecated. Please replace this\n"      \
  "constructor with the new `KeyAddr` version:\n"                            \
  "    `Qukey(layer, KeyAddr(row, col), alternate_key)`"

#define _DEPRECATED_MESSAGE_QUKEYS_TIMEOUT                                  \
  "The Qukeys.setTimeout() function has been renamed to setHoldTimeout()\n" \
  "in order to distinguish it from the other timeouts more clearly."

#define _DEPRECATED_MESSAGE_QUKEYS_RELEASEDELAY                               \
  "The Qukeys.setReleaseDelay() is now obsolete. The rollover grace period\n" \
  "for qukey release has been replaced with an improved version based on\n"   \
  "the percentage of overlap between the qukey and the subsequent\n"          \
  "key. Please use the setOverlapThreshold() function instead."

namespace kaleidoscope {
namespace plugin {

// Data structure for an individual qukey
struct Qukey {
  // The layer this qukey is mapped on.
  int8_t layer;
  // The keyswitch address of the qukey.
  KeyAddr addr;
  // The alternake Key value this qukey should use (when held).
  Key alternate_key;

  // This is the constructor that should be used when creating a Qukey object in
  // the PROGMEM array that will be used by Qukeys (i.e. in the `QUKEYS()`
  // macro).
  constexpr
  Qukey(int8_t layer, KeyAddr k, Key alternate_key)
    : layer(layer), addr(k), alternate_key(alternate_key) {}
  // This constructor is here so that we can create an empty Qukey object in RAM
  // into which we can copy the values from a PROGMEM Qukey object.
  Qukey() = default;

  // Old-style Qukey constructor for backwards compatibility.
  DEPRECATED(QUKEY_ROW_COL_CONSTRUCTOR)
  constexpr
  Qukey(int8_t layer, uint8_t row, uint8_t col, Key alternate_key)
    : layer(layer), addr(KeyAddr(row, col)), alternate_key(alternate_key) {}
};


class Qukeys : public kaleidoscope::Plugin {

 public:
  // Methods for turning the plugin on and off.
  void activate() {
    active_ = true;
  }
  void deactivate() {
    active_ = false;
  }
  void toggle() {
    active_ = !active_;
  }

  // Set the timeout (in milliseconds) for a held qukey. If a qukey is held at
  // least this long, it will take on its alternate value (unless its primary
  // value is a modifier -- i.e. a SpaceCadet type key).
  void setHoldTimeout(uint16_t hold_timeout) {
    hold_timeout_ = hold_timeout;
  }

  // Set the percentage of the duration of a subsequent key's press that must
  // overlap with the qukey preceding it above which the qukey will take on its
  // alternate key value. In other words, if the user presses qukey `Q`, then
  // key `K`, and the overlap of the two keys (`O`) is less than this
  // percentage, the qukey will take on its primary value, but if it's greater
  // (i.e. `K` spends a very small fraction of its time pressed after the `Q` is
  // released), the qukey will take on its alternate value.
  void setOverlapThreshold(uint8_t percentage) {
    // Only percentages less than 100% are meaningful. 0% means to turn off the
    // rollover grace period and rely solely on release order to determine the
    // qukey's state.
    if (percentage < 100) {
      overlap_threshold_ = percentage;
    } else {
      overlap_threshold_ = 0;
    }
  }

  // Function for defining the array of qukeys data (in PROGMEM). It's a
  // template function that takes as its sole argument an array reference of
  // size `_qukeys_count`, so there's no need to use `sizeof` to calculate the
  // correct size, and pass it as a separate parameter.
  template <uint8_t _qukeys_count>
  void configureQukeys(Qukey const(&qukeys)[_qukeys_count]) {
    qukeys_ = qukeys;
    qukeys_count_ = _qukeys_count;
  }

  // Obsolete configuration functions.
  DEPRECATED(QUKEYS_TIMEOUT)
  void setTimeout(uint16_t time_limit) {
    setHoldTimeout(time_limit);
  }
  DEPRECATED(QUKEYS_RELEASEDELAY)
  void setReleaseDelay(uint8_t release_delay) {
    if (release_delay == 0) {
      overlap_threshold_ = 0;
    } else {
      overlap_threshold_ = 60;
    }
  }

  // A wildcard value for a qukey that exists on every layer.
  static constexpr int8_t layer_wildcard{-1};

  // Kaleidoscope hook functions.
  EventHandlerResult onKeyswitchEvent(Key &mapped_key,
                                      KeyAddr key_addr,
                                      uint8_t key_state);
  EventHandlerResult beforeReportingState();

 private:
  // An array of Qukey objects in PROGMEM.
  Qukey const * qukeys_{nullptr};
  uint8_t qukeys_count_{0};

  // The maximum number of events in the queue at a time.
  static constexpr uint8_t queue_capacity_{8};

  // The event queue stores a series of press and release events.
  KeyAddrEventQueue<queue_capacity_> event_queue_;

  // This determines whether the plugin is on or off.
  bool active_{true};

  // This variable stores the percentage number between 0 and 99 that determines
  // how forgiving the plugin is of rollover from a qukey to a modified key.
  uint8_t overlap_threshold_{80};

  // The number of milliseconds until a qukey held on its own will take on its
  // alternate state (or primary state, in the case of a SpaceCadet-type qukey).
  uint16_t hold_timeout_{250};

  // This is a guard against re-processing events when qukeys flushes them from
  // its event queue. We can't just use an "injected" key state flag, because
  // that would cause other plugins to also ignore the event.
  bool flushing_queue_{false};

  // A cache of the current qukey's primary and alternate key values, so we
  // don't have to keep looking them up from PROGMEM.
  struct {
    Key primary_key{Key_Transparent};
    Key alternate_key{Key_Transparent};
  } queue_head_;

  // Internal helper methods.
  bool processQueue();
  void flushEvent(Key event_key);
  bool isQukey(KeyAddr k);
  bool isDualUseKey(Key key);
  bool releaseDelayed(uint16_t overlap_start, uint16_t overlap_end) const;
  bool isKeyAddrInQueueBeforeIndex(KeyAddr k, uint8_t index) const;
};

// This function returns true for any key that we expect to be used chorded with
// a "modified" key. This includes actual keyboard modifiers, but also layer
// shift keys. Used for determining if a qukey is a SpaceCadet-type key.
bool isModifierKey(Key key);

} // namespace plugin {
} // namespace kaleidoscope {

extern kaleidoscope::plugin::Qukeys Qukeys;

// Macro for use in sketch file to simplify definition of the qukeys array and
// guarantee that the count is set correctly. This is considerably less
// important than it used to be, with the `configureQukeys()` function taking
// care of guaranteeing the correct count setting.
#define QUKEYS(qukey_defs...) {                                         \
    static kaleidoscope::plugin::Qukey const qk_table[] PROGMEM = {     \
      qukey_defs                                                        \
    };                                                                  \
    Qukeys.configureQukeys(qk_table);                                   \
}
