/* -*- mode: c++ -*-
 * Kaleidoscope-TapDance -- Tap-dance keys
 * Copyright (C) 2016, 2017, 2018  Keyboard.io, Inc
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

#include <Kaleidoscope-TapDance.h>
#include <kaleidoscope/hid.h>

namespace kaleidoscope {
namespace plugin {

// --- state ---
uint32_t TapDance::end_time_;
uint16_t TapDance::time_out = 200;
TapDance::TapDanceState TapDance::state_[TapDance::TAPDANCE_KEY_COUNT];
Key TapDance::last_tap_dance_key_;
byte TapDance::last_tap_dance_row_;
byte TapDance::last_tap_dance_col_;

// --- actions ---

void TapDance::interrupt(byte row, byte col) {
  uint8_t idx = last_tap_dance_key_.raw - ranges::TD_FIRST;

  tapDanceAction(idx, last_tap_dance_row_, last_tap_dance_col_, state_[idx].count, Interrupt);
  state_[idx].triggered = true;

  end_time_ = 0;

  KeyboardHardware.maskKey(row, col);
  kaleidoscope::hid::sendKeyboardReport();
  kaleidoscope::hid::releaseAllKeys();

  if (state_[idx].pressed)
    return;

  release(idx);
}

void TapDance::timeout(void) {
  uint8_t idx = last_tap_dance_key_.raw - ranges::TD_FIRST;

  tapDanceAction(idx, last_tap_dance_row_, last_tap_dance_col_, state_[idx].count, Timeout);
  state_[idx].triggered = true;

  if (state_[idx].pressed)
    return;

  last_tap_dance_key_.raw = Key_NoKey.raw;

  release(idx);
}

void TapDance::release(uint8_t tap_dance_index) {
  end_time_ = 0;
  last_tap_dance_key_.raw = Key_NoKey.raw;

  state_[tap_dance_index].pressed = false;
  state_[tap_dance_index].triggered = false;
  state_[tap_dance_index].release_next = true;
}

void TapDance::tap(void) {
  uint8_t idx = last_tap_dance_key_.raw - ranges::TD_FIRST;

  state_[idx].count++;
  end_time_ = millis() + time_out;

  tapDanceAction(idx, last_tap_dance_row_, last_tap_dance_col_, state_[idx].count, Tap);
}

// --- api ---

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
    handleKeyswitchEvent(key, last_tap_dance_row_, last_tap_dance_col_, IS_PRESSED | INJECTED);
    break;
  case Hold:
    handleKeyswitchEvent(key, last_tap_dance_row_, last_tap_dance_col_, IS_PRESSED | WAS_PRESSED | INJECTED);
    break;
  case Release:
    hid::sendKeyboardReport();
    handleKeyswitchEvent(key, last_tap_dance_row_, last_tap_dance_col_, WAS_PRESSED | INJECTED);
    break;
  }
}

// --- hooks ---

EventHandlerResult TapDance::onKeyswitchEvent(Key &mapped_key, byte row, byte col, uint8_t keyState) {
  if (keyState & INJECTED)
    return EventHandlerResult::OK;

  if (mapped_key.raw < ranges::TD_FIRST || mapped_key.raw > ranges::TD_LAST) {
    if (last_tap_dance_key_.raw == Key_NoKey.raw)
      return EventHandlerResult::OK;

    if (keyToggledOn(keyState))
      interrupt(row, col);

    if (KeyboardHardware.isKeyMasked(row, col)) {
      KeyboardHardware.unMaskKey(row, col);
      return EventHandlerResult::EVENT_CONSUMED;
    }
    return EventHandlerResult::OK;
  }

  uint8_t tap_dance_index = mapped_key.raw - ranges::TD_FIRST;

  if (keyToggledOff(keyState))
    state_[tap_dance_index].pressed = false;

  if (last_tap_dance_key_.raw != mapped_key.raw) {
    if (last_tap_dance_key_.raw == Key_NoKey.raw) {
      if (state_[tap_dance_index].triggered) {
        if (keyToggledOff(keyState)) {
          release(tap_dance_index);
        }

        return EventHandlerResult::EVENT_CONSUMED;
      }

      last_tap_dance_key_.raw = mapped_key.raw;
      last_tap_dance_row_ = row;
      last_tap_dance_col_ = col;

      tap();

      return EventHandlerResult::EVENT_CONSUMED;
    } else {
      if (keyToggledOff(keyState) && state_[tap_dance_index].count) {
        release(tap_dance_index);
        return EventHandlerResult::EVENT_CONSUMED;
      }

      if (!keyToggledOn(keyState)) {
        return EventHandlerResult::EVENT_CONSUMED;
      }

      interrupt(row, col);
    }
  }

  // in sequence

  if (keyToggledOff(keyState)) {
    return EventHandlerResult::EVENT_CONSUMED;
  }

  last_tap_dance_key_.raw = mapped_key.raw;
  last_tap_dance_row_ = row;
  last_tap_dance_col_ = col;
  state_[tap_dance_index].pressed = true;

  if (keyToggledOn(keyState)) {
    tap();
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (state_[tap_dance_index].triggered)
    tapDanceAction(tap_dance_index, row, col, state_[tap_dance_index].count, Hold);

  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult TapDance::afterEachCycle() {
  for (uint8_t i = 0; i < TAPDANCE_KEY_COUNT; i++) {
    if (!state_[i].release_next)
      continue;

    tapDanceAction(i, last_tap_dance_row_, last_tap_dance_col_, state_[i].count, Release);
    state_[i].count = 0;
    state_[i].release_next = false;
  }

  if (last_tap_dance_key_.raw == Key_NoKey.raw)
    return EventHandlerResult::OK;

  if (end_time_ && millis() > end_time_)
    timeout();

  return EventHandlerResult::OK;
}

}
}

__attribute__((weak)) void tapDanceAction(uint8_t tap_dance_index, byte row, byte col, uint8_t tap_count,
    kaleidoscope::plugin::TapDance::ActionType tap_dance_action) {
}

kaleidoscope::plugin::TapDance TapDance;
