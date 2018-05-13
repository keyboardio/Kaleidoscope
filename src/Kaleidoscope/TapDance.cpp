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

#include <Kaleidoscope-TapDance.h>
#include <kaleidoscope/hid.h>

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

void TapDance::interrupt(byte row, byte col) {
  uint8_t idx = last_tap_dance_key_.raw - ranges::TD_FIRST;

  tapDanceAction(idx, last_tap_dance_row_, last_tap_dance_col_, tap_count_[idx], Interrupt);
  bitWrite(triggered_state_, idx, 1);

  end_time_ = 0;

  KeyboardHardware.maskKey(row, col);
  kaleidoscope::hid::sendKeyboardReport();
  kaleidoscope::hid::releaseAllKeys();

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

void TapDance::release(uint8_t tap_dance_index) {
  end_time_ = 0;
  last_tap_dance_key_.raw = Key_NoKey.raw;

  bitClear(pressed_state_, tap_dance_index);
  bitClear(triggered_state_, tap_dance_index);
  bitWrite(release_next_state_, tap_dance_index, 1);
}

void TapDance::tap(void) {
  uint8_t idx = last_tap_dance_key_.raw - ranges::TD_FIRST;

  tap_count_[idx]++;
  end_time_ = millis() + time_out;

  tapDanceAction(idx, last_tap_dance_row_, last_tap_dance_col_, tap_count_[idx], Tap);
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

  if (!keyIsPressed(keyState) && !keyWasPressed(keyState)) {
    if (isTapDance(mapped_key)) {
      return EventHandlerResult::EVENT_CONSUMED;
    }

    return EventHandlerResult::OK;
  }

  if (!isTapDance(mapped_key)) {
    if (!isActive())
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
    bitClear(pressed_state_, tap_dance_index);

  if (!isInSeq(mapped_key)) {
    if (!isActive()) {
      if (bitRead(triggered_state_, tap_dance_index)) {
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
      if (keyToggledOff(keyState) && stillHeld(tap_dance_index)) {
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
  bitSet(pressed_state_, tap_dance_index);

  if (keyToggledOn(keyState)) {
    tap();
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (bitRead(triggered_state_, tap_dance_index))
    tapDanceAction(tap_dance_index, row, col, tap_count_[tap_dance_index], Hold);

  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult TapDance::afterEachCycle() {
  for (uint8_t i = 0; i < 16; i++) {
    if (!bitRead(release_next_state_, i))
      continue;

    tapDanceAction(i, last_tap_dance_row_, last_tap_dance_col_, tap_count_[i], Release);
    tap_count_[i] = 0;
    bitClear(release_next_state_, i);
  }

  if (!isActive())
    return EventHandlerResult::OK;

  if (end_time_ && millis() > end_time_)
    timeout();

  return EventHandlerResult::OK;
}

// Legacy V1 API

#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
void TapDance::begin() {
  Kaleidoscope.useEventHandlerHook(legacyEventHandler);
  Kaleidoscope.useLoopHook(legacyLoopHook);
}

Key TapDance::legacyEventHandler(Key mapped_key, byte row, byte col, uint8_t key_state) {
  EventHandlerResult r = ::TapDance.onKeyswitchEvent(mapped_key, row, col, key_state);
  if (r == EventHandlerResult::OK)
    return mapped_key;
  return Key_NoKey;
}

void TapDance::legacyLoopHook(bool is_post_clear) {
  if (!is_post_clear)
    return;

  ::TapDance.afterEachCycle();
}
#endif

}

__attribute__((weak)) void tapDanceAction(uint8_t tap_dance_index, byte row, byte col, uint8_t tap_count,
    kaleidoscope::TapDance::ActionType tap_dance_action) {
}

kaleidoscope::TapDance TapDance;
