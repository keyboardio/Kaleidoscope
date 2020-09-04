/* -*- mode: c++ -*-
 * Kaleidoscope-OneShot -- One-shot modifiers and layers
 * Copyright (C) 2016-2020  Keyboard.io, Inc.
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
#include "kaleidoscope/KeyAddrBitfield.h"

// ----------------------------------------------------------------------------
// Keymap macros

#define OSM(kc) Key(kaleidoscope::ranges::OSM_FIRST + (Key_ ## kc).getKeyCode() - Key_LeftControl.getKeyCode())
#define OSL(n) Key(kaleidoscope::ranges::OSL_FIRST + n)

namespace kaleidoscope {
namespace plugin {

class OneShot : public kaleidoscope::Plugin {
 public:
  // Constructor
  OneShot() {}

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

  // --------------------------------------------------------------------------
  // Single-key test functions
  static bool isOneShotKey(Key key) {
    return (key.getRaw() >= kaleidoscope::ranges::OS_FIRST &&
            key.getRaw() <= kaleidoscope::ranges::OS_LAST);
  }
  static bool isStickable(Key key); // inline?

  static bool isTemporary(KeyAddr key_addr); // inline?
  static bool isSticky(KeyAddr key_addr); // inline?
  static bool isActive(KeyAddr key_addr); // inline?

  // --------------------------------------------------------------------------
  // Utility function for other plugins to cancel OneShot keys
  static void cancel(bool with_stickies = false);

  // --------------------------------------------------------------------------
  // Vestigial functions?
  void inject(Key key, uint8_t key_state) {}
  static bool isModifierActive(Key key) {
    return false;
  }

  // --------------------------------------------------------------------------
  // Configuration variables (should probably be private)
  static uint16_t time_out;
  static uint16_t hold_time_out;
  static int16_t double_tap_time_out;

  // --------------------------------------------------------------------------
  // Plugin hook functions

  EventHandlerResult onKeyswitchEvent(Key &key, KeyAddr key_addr, uint8_t key_state);
  EventHandlerResult beforeReportingState();
  EventHandlerResult afterEachCycle();

 private:

  // --------------------------------------------------------------------------
  // Constants
  static constexpr uint8_t oneshot_key_count = 16;
  static constexpr uint8_t oneshot_mod_count = 8;
  static constexpr uint8_t oneshot_layer_count = oneshot_key_count - oneshot_mod_count;
  static constexpr uint16_t stickable_modifiers_mask = uint16_t(uint16_t(-1) >> oneshot_layer_count);
  static constexpr uint16_t stickable_layers_mask = uint16_t(uint16_t(-1) << oneshot_mod_count);

  // --------------------------------------------------------------------------
  // State variables
  static uint16_t stickable_keys_;

  static KeyAddrBitfield temp_addrs_;
  static KeyAddrBitfield glue_addrs_;

  static uint16_t start_time_;
  static KeyAddr prev_key_addr_;
  static uint8_t release_countdown_;

  // --------------------------------------------------------------------------
  // Internal utility functions
  static bool hasTimedOut(uint16_t ttl) {
    return Runtime.hasTimeExpired(start_time_, ttl);
  }
  static uint8_t getOneShotKeyIndex(Key oneshot_key);
  static uint8_t getKeyIndex(Key key);
  static Key decodeOneShotKey(Key oneshot_key);

  static void pressKey(KeyAddr key_addr, Key oneshot_key);
  static void holdKey(KeyAddr key_addr);
  static void releaseKey(KeyAddr key_addr);
};

} // namespace plugin
} // namespace kaleidoscope

extern kaleidoscope::plugin::OneShot OneShot;
