/* -*- mode: c++ -*-
 * Kaleidoscope-TapDance -- Tap-dance keys
 * Copyright (C) 2016, 2017, 2018  Gergely Nagy
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

#define TD(n) (Key) {.raw = kaleidoscope::ranges::TD_FIRST + n }

#define tapDanceActionKeys(tap_count, tap_dance_action, ...) ({         \
      static const Key __k[] PROGMEM = { __VA_ARGS__ };                 \
      TapDance.actionKeys(tap_count, tap_dance_action,                  \
                          sizeof (__k) / sizeof (Key), &__k[0]);        \
    })

namespace kaleidoscope {
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

  EventHandlerResult onKeyswitchEvent(Key &mapped_key, byte row, byte col, uint8_t keyState);
  EventHandlerResult afterEachCycle();

#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
 protected:
  void begin();
  static Key legacyEventHandler(Key mapped_key, byte row, byte col, uint8_t key_state);
  static void legacyLoopHook(bool is_post_clear);
#endif

 private:
  static uint32_t end_time_;
  static uint8_t tap_count_[16];
  static uint16_t pressed_state_;
  static uint16_t triggered_state_;
  static uint16_t release_next_state_;
  static Key last_tap_dance_key_;
  static byte last_tap_dance_row_;
  static byte last_tap_dance_col_;

  static void tap(void);
  static void interrupt(byte row, byte col);
  static void timeout(void);
  static void release(uint8_t tap_dance_index);
};

}

void tapDanceAction(uint8_t tap_dance_index, byte row, byte col, uint8_t tap_count,
                    kaleidoscope::TapDance::ActionType tap_dance_action);

extern kaleidoscope::TapDance TapDance;
