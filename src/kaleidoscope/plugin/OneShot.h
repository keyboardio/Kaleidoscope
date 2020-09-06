/* -*- mode: c++ -*-
 * Kaleidoscope-OneShot -- One-shot modifiers and layers
 * Copyright (C) 2016-2019  Keyboard.io, Inc.
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
#include <Kaleidoscope-Ranges.h>
#include "kaleidoscope/key_events.h"

// ----------------------------------------------------------------------------
// Keymap macros

#define OSM(kc) Key(kaleidoscope::ranges::OSM_FIRST + (Key_ ## kc).getKeyCode() - Key_LeftControl.getKeyCode())
#define OSL(n) Key(kaleidoscope::ranges::OSL_FIRST + n)

namespace kaleidoscope {
namespace plugin {

class OneShot : public kaleidoscope::Plugin {
 public:
  // Constructor
  OneShot() {
    for (uint8_t i{0}; i < ONESHOT_KEY_COUNT; ++i) {
      state_[i].stickable = true;
    }
  }

  // --------------------------------------------------------------------------
  // Configuration functions

  static inline void enableStickablity() {}
  static void enableStickability(Key key);
  template <typename... Keys>
  static void enableStickability(Key key, Keys&&... keys) {
    enableStickability(key);
    enableStickability(keys...);
  }
  static void enableStickabilityForModifiers();
  static void enableStickabilityForLayers();

  static inline void disableStickability() {}
  static void disableStickability(Key key);
  template <typename... Keys>
  static void disableStickability(Key key, Keys&&... keys) {
    disableStickability(key);
    disableStickability(keys...);
  }
  static void disableStickabilityForModifiers();
  static void disableStickabilityForLayers();

  // --------------------------------------------------------------------------
  // Global test functions

  static bool isActive();
  static bool isSticky();
  static bool isPressed();

  // --------------------------------------------------------------------------
  // Single-key test functions

  static bool isOneShotKey(Key key) {
    return (key.getRaw() >= kaleidoscope::ranges::OS_FIRST &&
            key.getRaw() <= kaleidoscope::ranges::OS_LAST);
  }
  static bool isActive(Key key);
  static bool isSticky(Key key);
  static bool isStickable(Key key);

  // --------------------------------------------------------------------------
  // Utility function for other plugins to cancel OneShot keys
  static void cancel(bool with_stickies = false);

  // --------------------------------------------------------------------------
  // Vestigial functions?
  void inject(Key mapped_key, uint8_t key_state);
  static bool isModifierActive(Key key);

  // --------------------------------------------------------------------------
  // Configuration variables (should probably be private)
  static uint16_t time_out;
  static uint16_t hold_time_out;
  static int16_t double_tap_time_out;

  // --------------------------------------------------------------------------
  // Plugin hook functions

  EventHandlerResult onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t key_state);
  EventHandlerResult beforeReportingState();
  EventHandlerResult afterEachCycle();

 private:

  // --------------------------------------------------------------------------
  // Constants
  static constexpr uint8_t ONESHOT_KEY_COUNT = 16;
  static constexpr uint8_t ONESHOT_MOD_COUNT = 8;

  // --------------------------------------------------------------------------
  // State variables
  struct OneShotKeyState {
    bool active: 1;
    bool pressed: 1;
    bool stickable: 1;
    bool sticky: 1;
    uint8_t __reserved: 4;
  };
  static OneShotKeyState state_[ONESHOT_KEY_COUNT];

  static uint16_t start_time_;
  static KeyAddr prev_key_addr_;
  static uint8_t release_countdown_;

  // --------------------------------------------------------------------------
  // Internal utility functions
  static Key getNormalKey(uint8_t n);
  static void injectNormalKey(uint8_t n, uint8_t key_state,
                              KeyAddr key_addr = UnknownKeyswitchLocation);

  static void activateOneShot(uint8_t n);
  static void sustainOneShot(uint8_t n);
  static void releaseOneShot(uint8_t n);

  static bool hasTimedOut() {
    return Runtime.hasTimeExpired(start_time_, time_out);
  }
};

} // namespace plugin
} // namespace kaleidoscope

extern kaleidoscope::plugin::OneShot OneShot;
