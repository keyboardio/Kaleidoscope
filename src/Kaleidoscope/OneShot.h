/* -*- mode: c++ -*-
 * Kaleidoscope-OneShot -- One-shot modifiers and layers
 * Copyright (C) 2016, 2017  Gergely Nagy
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

#define OSM(kc) (Key) {.raw = kaleidoscope::ranges::OSM_FIRST + (Key_ ## kc).keyCode - Key_LeftControl.keyCode}
#define OSL(n) (Key) {.raw = kaleidoscope::ranges::OSL_FIRST + n}

namespace kaleidoscope {

class OneShot : public KaleidoscopePlugin {
 public:
  OneShot(void);

  void begin(void) final;

  static bool isActive(void);
  static void cancel(bool with_stickies);
  static void cancel(void) {
    cancel(false);
  }
  static uint16_t time_out;
  static uint16_t hold_time_out;

  static bool isModifierActive(Key key);

  void inject(Key key, uint8_t key_state);

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

  static void injectNormalKey(uint8_t idx, uint8_t key_state);
  static void activateOneShot(uint8_t idx);
  static void cancelOneShot(uint8_t idx);

  static Key eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state);
  static void loopHook(bool is_post_clear);
};

}

extern kaleidoscope::OneShot OneShot;
