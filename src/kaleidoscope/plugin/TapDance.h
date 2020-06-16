/* -*- mode: c++ -*-
 * Kaleidoscope-TapDance -- Tap-dance keys
 * Copyright (C) 2016, 2017, 2018, 2019  Keyboard.io, Inc
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

#define TD(n) Key(kaleidoscope::ranges::TD_FIRST + n)

#define tapDanceActionKeys(tap_count, tap_dance_action, ...) ({         \
      static const Key __k[] PROGMEM = { __VA_ARGS__ };                 \
      TapDance.actionKeys(tap_count, tap_dance_action,                  \
                          sizeof (__k) / sizeof (Key), &__k[0]);        \
    })

namespace kaleidoscope {
namespace plugin {
class TapDance : public kaleidoscope::Plugin {
 public:
  typedef enum {
    Tap,
    Hold,
    Interrupt,
    Timeout,
    Release,
  } ActionType;

  TapDance(void) {}

  static uint16_t time_out;

  void actionKeys(uint8_t tap_count, ActionType tap_dance_action, uint8_t max_keys, const Key tap_keys[]);

  EventHandlerResult onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t keyState);
  EventHandlerResult afterEachCycle();

 private:
  static constexpr uint8_t TAPDANCE_KEY_COUNT = 16;
  struct TapDanceState {
    bool pressed: 1;
    bool triggered: 1;
    bool release_next: 1;
    uint8_t count;
  };
  static TapDanceState state_[TAPDANCE_KEY_COUNT];

  static uint16_t start_time_;
  static Key last_tap_dance_key_;
  static KeyAddr last_tap_dance_addr_;

  static void tap(void);
  static void interrupt(KeyAddr key_addr);
  static void timeout(void);
  static void release(uint8_t tap_dance_index);
};
}

}

void tapDanceAction(uint8_t tap_dance_index, KeyAddr key_addr, uint8_t tap_count,
                    kaleidoscope::plugin::TapDance::ActionType tap_dance_action);

extern kaleidoscope::plugin::TapDance TapDance;
