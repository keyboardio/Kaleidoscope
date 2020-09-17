/* -*- mode: c++ -*-
 * Kaleidoscope-OneShot -- One-shot modifiers and layers
 * Copyright (C) 2016-2021  Keyboard.io, Inc.
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
// Deprecation warning messages
#define _DEPRECATED_MESSAGE_ONESHOT_TIMEOUT                                     \
  "The `OneShot.time_out` variable is deprecated. Please use the\n"             \
  "`OneShot.setTimeout()` function instead."

#define _DEPRECATED_MESSAGE_ONESHOT_HOLD_TIMEOUT                                \
  "The `OneShot.hold_time_out` variable is deprecated. Please use the\n"        \
  "`OneShot.setHoldTimeout()` function instead."

#define _DEPRECATED_MESSAGE_ONESHOT_DOUBLE_TAP_TIMEOUT                          \
  "The `OneShot.double_tap_time_out` variable is deprecated. Please use the\n"  \
  "`OneShot.setDoubleTapTimeout()` function instead."

#define _DEPRECATED_MESSAGE_ONESHOT_INJECT                                      \
  "The `OneShot.inject(key, key_state)` function has been deprecated."

#define _DEPRECATED_MESSAGE_ONESHOT_ISACTIVE_KEY                                \
  "The `OneShot.isActive(key)` function is deprecated. Please use\n"            \
  "`OneShot.isActive(key_addr)` instead, if possible."

#define _DEPRECATED_MESSAGE_ONESHOT_ISSTICKY_KEY                                \
  "The `OneShot.isSticky(key)` function is deprecated. Please use\n"            \
  "`OneShot.isSticky(key_addr)` instead, if possible."

#define _DEPRECATED_MESSAGE_ONESHOT_ISPRESSED                                   \
  "The `OneShot.isPressed()` function is deprecated. This function now\n"       \
  "always returns false."

#define _DEPRECATED_MESSAGE_ONESHOT_ISMODIFIERACTIVE                            \
  "The `OneShot.isModifierActive()` function is deprecated."

// ----------------------------------------------------------------------------
// Keymap macros

#define OSM(kc) Key(kaleidoscope::ranges::OSM_FIRST + (Key_ ## kc).getKeyCode() - Key_LeftControl.getKeyCode())
#define OSL(n) Key(kaleidoscope::ranges::OSL_FIRST + n)

// ----------------------------------------------------------------------------
// Key constants
constexpr Key OneShot_MetaStickyKey {kaleidoscope::ranges::OS_META_STICKY};

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

  static void enableAutoModifiers() {
    auto_modifiers_ = true;
  }
  static void enableAutoLayers() {
    auto_layers_ = true;
  }
  static void enableAutoOneShot() {
    enableAutoModifiers();
    enableAutoLayers();
  }

  static void disableAutoModifiers() {
    auto_modifiers_ = false;
  }
  static void disableAutoLayers() {
    auto_layers_ = false;
  }
  static void disableAutoOneShot() {
    disableAutoModifiers();
    disableAutoLayers();
  }

  static void toggleAutoModifiers() {
    auto_modifiers_ = ! auto_modifiers_;
  }
  static void toggleAutoLayers() {
    auto_layers_ = ! auto_layers_;
  }
  static void toggleAutoOneShot() {
    if (auto_modifiers_ || auto_layers_) {
      disableAutoOneShot();
    } else {
      enableAutoOneShot();
    }
  }

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
  static bool isModifier(Key key);
  static bool isLayerShift(Key key);

  static bool isStickable(Key key); // inline?

  static bool isTemporary(KeyAddr key_addr); // inline?
  static bool isSticky(KeyAddr key_addr); // inline?
  static bool isActive(KeyAddr key_addr); // inline?

  // --------------------------------------------------------------------------
  // Utility function for other plugins to cancel OneShot keys
  static void cancel(bool with_stickies = false);

  // --------------------------------------------------------------------------
  // Deprecated functions
  DEPRECATED(ONESHOT_INJECT)
  void inject(Key key, uint8_t key_state);

  DEPRECATED(ONESHOT_ISMODIFIERACTIVE)
  static bool isModifierActive(Key key);

  DEPRECATED(ONESHOT_ISACTIVE_KEY)
  static bool isActive(Key oneshot_key);

  DEPRECATED(ONESHOT_ISSTICKY_KEY)
  static bool isSticky(Key oneshot_key);

  DEPRECATED(ONESHOT_ISPRESSED)
  static bool isPressed() {
    return false;
  }

  // --------------------------------------------------------------------------
  // Timeout onfiguration functions
  static void setTimeout(uint16_t ttl) {
    timeout_ = ttl;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    time_out = ttl;
#pragma GCC diagnostic pop
  }
  static void setHoldTimeout(uint16_t ttl) {
    hold_timeout_ = ttl;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    hold_time_out = ttl;
#pragma GCC diagnostic pop
  }
  static void setDoubleTapTimeout(int16_t ttl) {
    double_tap_timeout_ = ttl;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    double_tap_time_out = ttl;
#pragma GCC diagnostic pop
  }

  // --------------------------------------------------------------------------
  // Configuration variables (should probably be private)
  DEPRECATED(ONESHOT_TIMEOUT)
  static uint16_t time_out;
  DEPRECATED(ONESHOT_HOLD_TIMEOUT)
  static uint16_t hold_time_out;
  DEPRECATED(ONESHOT_DOUBLE_TAP_TIMEOUT)
  static int16_t double_tap_time_out;

  // --------------------------------------------------------------------------
  // Plugin hook functions

  EventHandlerResult onNameQuery();
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
  static constexpr KeyAddr invalid_key_addr = KeyAddr(KeyAddr::invalid_state);

  // --------------------------------------------------------------------------
  // Configuration variables
  static uint16_t timeout_;
  static uint16_t hold_timeout_;
  static int16_t double_tap_timeout_;

  // --------------------------------------------------------------------------
  // State variables
  static uint16_t stickable_keys_;
  static bool auto_modifiers_;
  static bool auto_layers_;
  static KeyAddr meta_sticky_key_addr_;

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
