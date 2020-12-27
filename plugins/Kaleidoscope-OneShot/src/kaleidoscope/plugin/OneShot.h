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

#define OSM(kc) Key(kaleidoscope::ranges::OSM_FIRST + (Key_ ## kc).getKeyCode() - Key_LeftControl.getKeyCode())
#define OSL(n) Key(kaleidoscope::ranges::OSL_FIRST + n)

namespace kaleidoscope {
namespace plugin {

class OneShot : public kaleidoscope::Plugin {
 public:
  OneShot(void) {
    for (uint8_t i = 0; i < ONESHOT_KEY_COUNT; i++) {
      state_[i].stickable = true;
    }
  }

  static bool isOneShotKey(Key key) {
    return (key.getRaw() >= kaleidoscope::ranges::OS_FIRST && key.getRaw() <= kaleidoscope::ranges::OS_LAST);
  }
  static bool isActive(void);
  static bool isActive(Key key);
  static bool isPressed();
  static bool isSticky();
  static bool isSticky(Key key);
  static void cancel(bool with_stickies = false);

  static uint16_t time_out;
  static int16_t double_tap_time_out;
  static uint16_t hold_time_out;

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

  static bool isStickable(Key key);

  static bool isModifierActive(Key key);

  EventHandlerResult beforeReportingState();
  EventHandlerResult afterEachCycle();
  EventHandlerResult onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t keyState);

  void inject(Key mapped_key, uint8_t key_state);

 private:
  static constexpr uint8_t ONESHOT_KEY_COUNT = 16;
  typedef struct {
    bool active: 1;
    bool pressed: 1;
    bool stickable: 1;
    bool sticky: 1;
    uint8_t __reserved: 4;
    uint8_t position;
  } key_state_t;
  static key_state_t state_[ONESHOT_KEY_COUNT];

  static uint16_t start_time_;
  static Key prev_key_;
  static bool should_cancel_;
  static bool should_cancel_stickies_;

  static void injectNormalKey(uint8_t idx, uint8_t key_state);
  static void activateOneShot(uint8_t idx);
  static void cancelOneShot(uint8_t idx);

  static bool isOneShotKey_(Key key) {
    return key.getRaw() >= ranges::OS_FIRST && key.getRaw() <= ranges::OS_LAST;
  }
  static bool hasTimedOut() {
    return Runtime.hasTimeExpired(start_time_, time_out);
  }
};
}
}

extern kaleidoscope::plugin::OneShot OneShot;
