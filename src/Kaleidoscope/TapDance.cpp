/* -*- mode: c++ -*-
 * Kaleidoscope-TapDance -- Tap-dance keys
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

#include <Kaleidoscope-TapDance.h>

namespace kaleidoscope {

// --- state ---
uint32_t TapDance::end_time_;
uint16_t TapDance::time_out = 200;
uint8_t TapDance::tap_count_[16];
uint16_t TapDance::pressed_state_;
uint16_t TapDance::triggered_state_;
uint16_t TapDance::release_next_state_;
Key TapDance::last_tap_dance_key_;
byte TapDance::last_tap_dance_row_;
byte TapDance::last_tap_dance_col_;

// --- helpers ---

#define isTapDance(k) (k.raw >= ranges::TD_FIRST && k.raw <= ranges::TD_LAST)
#define isInSeq(k) (last_tap_dance_key_.raw == k.raw)
#define stillHeld(idx) (tap_count_[idx])
#define isActive() (last_tap_dance_key_.raw != Key_NoKey.raw)

// --- actions ---

void TapDance::interrupt(void) {
  uint8_t idx = last_tap_dance_key_.raw - ranges::TD_FIRST;

  tapDanceAction(idx, last_tap_dance_row_, last_tap_dance_col_, tap_count_[idx], Interrupt);
  bitWrite(triggered_state_, idx, 1);

  end_time_ = 0;

  if (bitRead(pressed_state_, idx))
    return;

  release(idx);
}

void TapDance::timeout(void) {
  uint8_t idx = last_tap_dance_key_.raw - ranges::TD_FIRST;

  tapDanceAction(idx, last_tap_dance_row_, last_tap_dance_col_, tap_count_[idx], Timeout);
  bitWrite(triggered_state_, idx, 1);

  if (bitRead(pressed_state_, idx))
    return;

  last_tap_dance_key_.raw = Key_NoKey.raw;

  release(idx);
}

Key TapDance::release(uint8_t tap_dance_index) {
  end_time_ = 0;
  last_tap_dance_key_.raw = Key_NoKey.raw;

  bitClear(pressed_state_, tap_dance_index);
  bitClear(triggered_state_, tap_dance_index);
  bitWrite(release_next_state_, tap_dance_index, 1);
  return Key_NoKey;
}

Key TapDance::tap(void) {
  uint8_t idx = last_tap_dance_key_.raw - ranges::TD_FIRST;

  tap_count_[idx]++;
  end_time_ = millis() + time_out;

  tapDanceAction(idx, last_tap_dance_row_, last_tap_dance_col_, tap_count_[idx], Tap);

  return Key_NoKey;
}

// --- api ---

TapDance::TapDance(void) {
}

void TapDance::begin(void) {
  event_handler_hook_use(eventHandlerHook);
  loop_hook_use(loopHook);
}

void TapDance::actionKeys(uint8_t tap_count, ActionType tap_dance_action, uint8_t max_keys, const Key tap_keys[]) {
  if (tap_count > max_keys)
    tap_count = max_keys;

  Key key;
  key.raw = pgm_read_word(&(tap_keys[tap_count - 1].raw));

  switch (tap_dance_action) {
  case Tap:
    break;
  case Interrupt:
  case Timeout:
    handle_keyswitch_event(key, last_tap_dance_row_, last_tap_dance_col_, IS_PRESSED | INJECTED);
    break;
  case Hold:
    handle_keyswitch_event(key, last_tap_dance_row_, last_tap_dance_col_, IS_PRESSED | WAS_PRESSED | INJECTED);
    break;
  case Release:
    Keyboard.sendReport();
    handle_keyswitch_event(key, last_tap_dance_row_, last_tap_dance_col_, WAS_PRESSED | INJECTED);
    break;
  }
}

// --- hooks ---

Key TapDance::eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state) {
  if (key_state & INJECTED)
    return mapped_key;

  if (!key_is_pressed(key_state) && !key_was_pressed(key_state)) {
    if (isTapDance(mapped_key))
      return Key_NoKey;
    return mapped_key;
  }

  if (!isTapDance(mapped_key)) {
    if (!isActive())
      return mapped_key;

    if (key_toggled_on(key_state))
      interrupt();

    return mapped_key;
  }

  uint8_t tap_dance_index = mapped_key.raw - ranges::TD_FIRST;

  if (key_toggled_off(key_state))
    bitClear(pressed_state_, tap_dance_index);

  if (!isInSeq(mapped_key)) {
    if (!isActive()) {
      if (bitRead(triggered_state_, tap_dance_index)) {
        if (key_toggled_off(key_state))
          return release(tap_dance_index);
        return Key_NoKey;
      }

      last_tap_dance_key_.raw = mapped_key.raw;
      last_tap_dance_row_ = row;
      last_tap_dance_col_ = col;
      return tap();
    } else {
      if (key_toggled_off(key_state) && stillHeld(tap_dance_index)) {
        return release(tap_dance_index);
      }

      if (!key_toggled_on(key_state))
        return Key_NoKey;

      interrupt();
    }
  }

  // in sequence

  if (key_toggled_off(key_state))
    return Key_NoKey;

  last_tap_dance_key_.raw = mapped_key.raw;
  last_tap_dance_row_ = row;
  last_tap_dance_col_ = col;
  bitSet(pressed_state_, tap_dance_index);

  if (key_toggled_on(key_state))
    return tap();

  if (bitRead(triggered_state_, tap_dance_index))
    tapDanceAction(tap_dance_index, row, col, tap_count_[tap_dance_index], Hold);

  return Key_NoKey;
}

void
TapDance::loopHook(bool is_post_clear) {
  if (!is_post_clear)
    return;

  for (uint8_t i = 0; i < 16; i++) {
    if (!bitRead(release_next_state_, i))
      continue;

    tapDanceAction(i, last_tap_dance_row_, last_tap_dance_col_, tap_count_[i], Release);
    tap_count_[i] = 0;
    bitClear(release_next_state_, i);
  }

  if (!isActive())
    return;

  if (end_time_ && millis() > end_time_)
    timeout();
}

}

__attribute__((weak)) void tapDanceAction(uint8_t tap_dance_index, byte row, byte col, uint8_t tap_count,
    kaleidoscope::TapDance::ActionType tap_dance_action) {
}

kaleidoscope::TapDance TapDance;
