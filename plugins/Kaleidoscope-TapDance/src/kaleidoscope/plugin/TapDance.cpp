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
#include "kaleidoscope/keyswitch_state.h"

namespace kaleidoscope {
namespace plugin {

// --- state ---
uint16_t TapDance::start_time_;
uint16_t TapDance::time_out = 200;
TapDance::TapDanceState TapDance::state_[TapDance::TAPDANCE_KEY_COUNT];
Key TapDance::last_tap_dance_key_ = Key_NoKey;
KeyAddr TapDance::last_tap_dance_addr_;

// --- actions ---

void TapDance::interrupt(KeyAddr key_addr) {
  uint8_t idx = last_tap_dance_key_.getRaw() - ranges::TD_FIRST;

  tapDanceAction(idx, last_tap_dance_addr_, state_[idx].count, Interrupt);
  state_[idx].triggered = true;

  last_tap_dance_key_ = Key_NoKey;

  Runtime.hid().keyboard().sendReport();
  Runtime.hid().keyboard().releaseAllKeys();

  if (state_[idx].pressed)
    return;

  release(idx);
}

void TapDance::timeout(void) {
  uint8_t idx = last_tap_dance_key_.getRaw() - ranges::TD_FIRST;

  tapDanceAction(idx, last_tap_dance_addr_, state_[idx].count, Timeout);
  state_[idx].triggered = true;

  if (state_[idx].pressed)
    return;

  last_tap_dance_key_ = Key_NoKey;

  release(idx);
}

void TapDance::release(uint8_t tap_dance_index) {
  last_tap_dance_key_ = Key_NoKey;

  state_[tap_dance_index].pressed = false;
  state_[tap_dance_index].triggered = false;
  state_[tap_dance_index].release_next = true;
}

void TapDance::tap(void) {
  uint8_t idx = last_tap_dance_key_.getRaw() - ranges::TD_FIRST;

  state_[idx].count++;
  start_time_ = Runtime.millisAtCycleStart();

  tapDanceAction(idx, last_tap_dance_addr_, state_[idx].count, Tap);
}

// --- api ---

void TapDance::actionKeys(uint8_t tap_count, ActionType tap_dance_action, uint8_t max_keys, const Key tap_keys[]) {
  if (tap_count > max_keys)
    tap_count = max_keys;

  Key key = tap_keys[tap_count - 1].readFromProgmem();

  switch (tap_dance_action) {
  case Tap:
    break;
  case Interrupt:
  case Timeout:
    handleKeyswitchEvent(key, last_tap_dance_addr_, IS_PRESSED | INJECTED);
    break;
  case Hold:
    handleKeyswitchEvent(key, last_tap_dance_addr_, IS_PRESSED | WAS_PRESSED | INJECTED);
    break;
  case Release:
    kaleidoscope::Runtime.hid().keyboard().sendReport();
    handleKeyswitchEvent(key, last_tap_dance_addr_, WAS_PRESSED | INJECTED);
    break;
  }
}

// --- hooks ---

EventHandlerResult TapDance::onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t keyState) {
  if (keyState & INJECTED)
    return EventHandlerResult::OK;

  if (mapped_key.getRaw() < ranges::TD_FIRST || mapped_key.getRaw() > ranges::TD_LAST) {
    if (last_tap_dance_key_ == Key_NoKey)
      return EventHandlerResult::OK;

    if (keyToggledOn(keyState)) {
      interrupt(key_addr);
      mapped_key = Key_NoKey;
    }

    return EventHandlerResult::OK;
  }

  uint8_t tap_dance_index = mapped_key.getRaw() - ranges::TD_FIRST;

  if (keyToggledOff(keyState))
    state_[tap_dance_index].pressed = false;

  if (last_tap_dance_key_ != mapped_key) {
    if (last_tap_dance_key_ == Key_NoKey) {
      if (state_[tap_dance_index].triggered) {
        if (keyToggledOff(keyState)) {
          release(tap_dance_index);
        }

        return EventHandlerResult::EVENT_CONSUMED;
      }

      last_tap_dance_key_ = mapped_key;
      last_tap_dance_addr_ = key_addr;

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

      interrupt(key_addr);
    }
  }

  // in sequence

  if (keyToggledOff(keyState)) {
    return EventHandlerResult::EVENT_CONSUMED;
  }

  last_tap_dance_key_ = mapped_key;
  last_tap_dance_addr_ = key_addr;
  state_[tap_dance_index].pressed = true;

  if (keyToggledOn(keyState)) {
    tap();
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (state_[tap_dance_index].triggered)
    tapDanceAction(tap_dance_index, key_addr, state_[tap_dance_index].count, Hold);

  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult TapDance::afterEachCycle() {
  for (uint8_t i = 0; i < TAPDANCE_KEY_COUNT; i++) {
    if (!state_[i].release_next)
      continue;

    tapDanceAction(i, last_tap_dance_addr_, state_[i].count, Release);
    state_[i].count = 0;
    state_[i].release_next = false;
  }

  if (last_tap_dance_key_ == Key_NoKey)
    return EventHandlerResult::OK;

  if (Runtime.hasTimeExpired(start_time_, time_out))
    timeout();

  return EventHandlerResult::OK;
}

}
}

__attribute__((weak)) void tapDanceAction(uint8_t tap_dance_index, KeyAddr key_addr, uint8_t tap_count,
                                          kaleidoscope::plugin::TapDance::ActionType tap_dance_action) {
}

kaleidoscope::plugin::TapDance TapDance;
