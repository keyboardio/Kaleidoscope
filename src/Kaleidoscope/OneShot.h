/* -*- mode: c++ -*-
 * Kaleidoscope-OneShot -- One-shot modifiers and layers
 * Copyright (C) 2016-2018  Keyboard.io, Inc.
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

#include <Kaleidoscope.h>
#include <Kaleidoscope-Ranges.h>

#define OSM(kc) (Key) {.raw = kaleidoscope::ranges::OSM_FIRST + (Key_ ## kc).keyCode - Key_LeftControl.keyCode}
#define OSL(n) (Key) {.raw = kaleidoscope::ranges::OSL_FIRST + n}

namespace kaleidoscope {

class OneShot : public kaleidoscope::Plugin {
 public:
  OneShot(void) {}

  static bool isOneShotKey(Key key) {
    return (key.raw >= kaleidoscope::ranges::OS_FIRST && key.raw <= kaleidoscope::ranges::OS_LAST);
  }
  static bool isActive(void);
  static bool isPressed() {
    return !!pressed_state_.all;
  }
  static bool isActive(Key key);
  static bool isSticky(Key key);
  static void cancel(bool with_stickies);
  static void cancel(void) {
    cancel(false);
  }
  static uint16_t time_out;
  static int16_t double_tap_time_out;
  static uint16_t hold_time_out;
  static bool double_tap_sticky;
  static bool double_tap_sticky_layers;

  static bool isModifierActive(Key key);

  EventHandlerResult beforeReportingState();
  EventHandlerResult afterEachCycle();
  EventHandlerResult onKeyswitchEvent(Key &mapped_key, byte row, byte col, uint8_t keyState);

  void inject(Key mapped_key, uint8_t key_state);

 private:
  typedef union {
    struct {
      uint8_t mods;
      uint8_t layers;
    };
    uint16_t all;
  } state_t;
  static uint32_t start_time_;
  static state_t state_;
  static state_t sticky_state_;
  static state_t pressed_state_;
  static Key prev_key_;
  static bool should_cancel_;
  static bool should_cancel_stickies_;
  static bool should_mask_on_interrupt_;
  static uint8_t positions_[16];

  static void positionToCoords(uint8_t pos, byte *row, byte *col);

  static void injectNormalKey(uint8_t idx, uint8_t key_state);
  static void activateOneShot(uint8_t idx);
  static void cancelOneShot(uint8_t idx);
};

}

extern kaleidoscope::OneShot OneShot;
