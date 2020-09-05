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

#include <Kaleidoscope-OneShot.h>
#include "kaleidoscope/keyswitch_state.h"
#include "kaleidoscope/key_events.h"

namespace kaleidoscope {
namespace plugin {

// ---- state ---------

uint16_t OneShot::start_time_ = 0;
uint16_t OneShot::time_out = 2500;
uint16_t OneShot::hold_time_out = 250;
int16_t OneShot::double_tap_time_out = -1;
OneShot::key_state_t OneShot::state_[OneShot::ONESHOT_KEY_COUNT];
KeyAddr OneShot::prev_key_addr_ = UnknownKeyswitchLocation;
uint8_t OneShot::release_countdown_ = 0;

bool OneShot::isPressed() {
  for (uint8_t i = 0; i < ONESHOT_KEY_COUNT; i++) {
    if (state_[i].pressed)
      return true;
  }
  return false;
}

bool OneShot::isSticky() {
  for (uint8_t i = 0; i < ONESHOT_KEY_COUNT; i++) {
    if (state_[i].sticky)
      return true;
  }
  return false;
}

bool OneShot::isStickable(Key key) {
  return state_[key.getRaw() - ranges::OS_FIRST].stickable;
}

// ---- OneShot stuff ----
void OneShot::injectNormalKey(uint8_t idx, uint8_t key_state, KeyAddr key_addr) {
  Key key;

  if (idx < 8) {
    key = Key(Key_LeftControl.getKeyCode() + idx,
              Key_LeftControl.getFlags());
  } else {
    key = Key(LAYER_SHIFT_OFFSET + idx - 8,
              KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP);
  }

  handleKeyswitchEvent(key, key_addr, key_state | INJECTED);
}

void OneShot::activateOneShot(uint8_t idx) {
  start_time_ = Runtime.millisAtCycleStart();
  state_[idx].active = true;
  injectNormalKey(idx, IS_PRESSED);
}

void OneShot::sustainOneShot(uint8_t idx) {
  injectNormalKey(idx, WAS_PRESSED | IS_PRESSED);
}

void OneShot::replaceOneShot(uint8_t idx, uint8_t key_state, KeyAddr key_addr) {
  state_[idx].active = false;
  state_[idx].sticky = false;
  state_[idx].pressed = false;
  injectNormalKey(idx, key_state, key_addr);
}

void OneShot::releaseOneShot(uint8_t idx) {
  state_[idx].active = false;
  state_[idx].sticky = false;
  injectNormalKey(idx, WAS_PRESSED);
}

EventHandlerResult OneShot::onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t keyState) {
  if (keyState & INJECTED)
    return EventHandlerResult::OK;

  // If it's not a OneShot key, and not a modifier, cancel all active
  // OneShot keys except for sticky ones.
  if (!isOneShotKey_(mapped_key)) {
    if (keyToggledOn(keyState)) {
      prev_key_addr_ = key_addr;
      if (!(mapped_key >= Key_LeftControl && mapped_key <= Key_RightGui) &&
          !(mapped_key.getFlags() == (KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP))) {
        release_countdown_ = 0b10;
      }
    }
    return EventHandlerResult::OK;
  }

  // We know that `mapped_key` is a OneShot key, and the call to
  // `isOneShotKey_()` above ensures that the index is not out of
  // bounds, so this is now valid:
  uint8_t idx = mapped_key.getRaw() - ranges::OS_FIRST;

  if (keyToggledOn(keyState)) {
    state_[idx].pressed = true;
    if (! state_[idx].active) {
      // first press
      activateOneShot(idx);
    } else if (state_[idx].sticky) {
      replaceOneShot(idx, keyState, key_addr);
    } else {
      if ((key_addr == prev_key_addr_) && state_[idx].stickable) {
        uint16_t dtto = (double_tap_time_out < 0) ? time_out : double_tap_time_out;
        if (! Runtime.hasTimeExpired(start_time_, dtto)) {
          // double tap
          state_[idx].sticky = true;
        } else {
          replaceOneShot(idx, keyState, key_addr);
        }
      } else {
        replaceOneShot(idx, keyState, key_addr);
      }
    }
    prev_key_addr_ = key_addr;

  } else if (keyToggledOff(keyState)) {
    state_[idx].pressed = false;
    if (state_[idx].active && !state_[idx].sticky) {
      // check hold timeout
      if (Runtime.hasTimeExpired(start_time_, hold_time_out)) {
        releaseOneShot(idx);
      }
    }
  } else {
    // This is a OneShot key that is being held. If a keypress has
    // triggered the release of OneShot keys, `release_countdown_`
    // will be set. When it gets to 1, it's time to replace non-sticky
    // OneShot keys that are still being held with their corresponding
    // modifiers.
    if (release_countdown_ == 1) {
      if (state_[idx].active && !state_[idx].sticky) {
        replaceOneShot(idx, keyState, key_addr);
      }
    }
  }
  return EventHandlerResult::OK;
}


// For any active OneShot modifier keys, keep those modifiers active
// in the keyboard HID report.
EventHandlerResult OneShot::beforeReportingState() {
  for (uint8_t i{0}; i < ONESHOT_MOD_COUNT; ++i) {
    if (state_[i].active) {
      sustainOneShot(i);
    }
  }
  return EventHandlerResult::OK;
}


EventHandlerResult OneShot::afterEachCycle() {
  // At the end of every cycle, we check to see if any OneShot keys
  // have timed out.
  if ((release_countdown_ == 1) || hasTimedOut()) {
    for (uint8_t i{0}; i < ONESHOT_KEY_COUNT; ++i) {
      if (state_[i].active && !state_[i].sticky && !state_[i].pressed) {
        releaseOneShot(i);
      }
    }
  }

  // Also, advance the counter for OneShot keys that have been
  // cancelled by the press of a non-OneShot, non-modifier key. An
  // unconditional bit shift should be more efficient than checking
  // for zero to avoid underflow.
  release_countdown_ >>= 1;

  return EventHandlerResult::OK;
}

void OneShot::inject(Key mapped_key, uint8_t key_state) {
  onKeyswitchEvent(mapped_key, UnknownKeyswitchLocation, key_state);
}

// --- glue code ---

bool OneShot::isActive(void) {
  for (uint8_t i = 0; i < ONESHOT_KEY_COUNT; i++) {
    if (state_[i].active)
      return true;
  }
  return false;
}

bool OneShot::isActive(Key key) {
  uint8_t idx = key.getRaw() - ranges::OS_FIRST;

  return state_[idx].active;
}

bool OneShot::isSticky(Key key) {
  uint8_t idx = key.getRaw() - ranges::OS_FIRST;

  return state_[idx].sticky;
}

bool OneShot::isModifierActive(Key key) {
  if (key < Key_LeftControl || key > Key_RightGui)
    return false;

  uint8_t idx = key.getKeyCode() - Key_LeftControl.getKeyCode();
  return state_[idx].active;
}

void OneShot::cancel(bool cancel_stickies) {
  for (uint8_t i{0}; i < ONESHOT_KEY_COUNT; ++i) {
    if (state_[i].active) {
      if (state_[i].sticky && !cancel_stickies) {
        continue;
      }
      releaseOneShot(i);
    }
  }
}

void OneShot::enableStickability(Key key) {
  if (key >= ranges::OS_FIRST && key <= ranges::OS_LAST)
    state_[key.getRaw() - ranges::OS_FIRST].stickable = true;
}

void OneShot::disableStickability(Key key) {
  if (key >= ranges::OS_FIRST && key <= ranges::OS_LAST)
    state_[key.getRaw() - ranges::OS_FIRST].stickable = false;
}

void OneShot::enableStickabilityForModifiers() {
  for (uint8_t i = 0; i < ONESHOT_KEY_COUNT / 2; i++) {
    state_[i].stickable = true;
  }
}

void OneShot::enableStickabilityForLayers() {
  for (uint8_t i = ONESHOT_KEY_COUNT / 2; i < ONESHOT_KEY_COUNT; i++) {
    state_[i].stickable = true;
  }
}

void OneShot::disableStickabilityForModifiers() {
  for (uint8_t i = 0; i < ONESHOT_KEY_COUNT / 2; i++) {
    state_[i].stickable = false;
  }
}

void OneShot::disableStickabilityForLayers() {
  for (uint8_t i = ONESHOT_KEY_COUNT / 2; i < ONESHOT_KEY_COUNT; i++) {
    state_[i].stickable = false;
  }
}

}
}

kaleidoscope::plugin::OneShot OneShot;
