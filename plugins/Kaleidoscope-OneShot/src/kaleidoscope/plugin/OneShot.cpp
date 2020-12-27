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
Key OneShot::prev_key_;
bool OneShot::should_cancel_ = false;
bool OneShot::should_cancel_stickies_ = false;

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
void OneShot::injectNormalKey(uint8_t idx, uint8_t key_state) {
  Key key;

  if (idx < 8) {
    key = Key(Key_LeftControl.getKeyCode() + idx,
              Key_LeftControl.getFlags());
  } else {
    key = Key(LAYER_SHIFT_OFFSET + idx - 8,
              KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP);
  }

  handleKeyswitchEvent(key, UnknownKeyswitchLocation, key_state | INJECTED);
}

void OneShot::activateOneShot(uint8_t idx) {
  injectNormalKey(idx, IS_PRESSED);
}

void OneShot::cancelOneShot(uint8_t idx) {
  state_[idx].active = false;
  injectNormalKey(idx, WAS_PRESSED);
}

EventHandlerResult OneShot::onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t keyState) {
  uint8_t idx = mapped_key.getRaw() - ranges::OS_FIRST;

  if (keyState & INJECTED)
    return EventHandlerResult::OK;

  if (!isActive()) {
    if (!isOneShotKey_(mapped_key)) {
      return EventHandlerResult::OK;
    }

    if (keyToggledOff(keyState)) {
      state_[idx].pressed = false;
    } else if (keyToggledOn(keyState)) {
      start_time_ = Runtime.millisAtCycleStart();
      state_[idx].position = key_addr.toInt();
      state_[idx].pressed = true;
      state_[idx].active = true;
      prev_key_ = mapped_key;

      activateOneShot(idx);
    }

    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (isOneShotKey_(mapped_key)) {
    if (state_[idx].sticky) {
      if (keyToggledOn(keyState)) {  // maybe on _off instead?
        prev_key_ = mapped_key;
        state_[idx].sticky = false;
        cancelOneShot(idx);
        should_cancel_ = false;
      }
    } else {
      if (keyToggledOff(keyState)) {
        state_[idx].pressed = false;
        if (Runtime.hasTimeExpired(start_time_, hold_time_out)) {
          cancelOneShot(idx);
          should_cancel_ = false;
        }
      }

      if (keyToggledOn(keyState)) {
        state_[idx].pressed = true;

        if (prev_key_ == mapped_key && isStickable(mapped_key)) {
          uint16_t dtto = (double_tap_time_out == -1) ? time_out : double_tap_time_out;
          if (!Runtime.hasTimeExpired(start_time_, dtto)) {
            state_[idx].sticky = true;
            prev_key_ = mapped_key;
          }
        } else {
          start_time_ = Runtime.millisAtCycleStart();

          state_[idx].position = key_addr.toInt();
          state_[idx].active = true;
          prev_key_ = mapped_key;

          activateOneShot(idx);
        }
      }
    }

    return EventHandlerResult::EVENT_CONSUMED;
  }

  // ordinary key here, with some event

  if (keyIsPressed(keyState)) {
    prev_key_ = mapped_key;
    if (!(mapped_key >= Key_LeftControl && mapped_key <= Key_RightGui) &&
        !(mapped_key.getFlags() == (KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP))) {
      should_cancel_ = true;
    }
  }

  return EventHandlerResult::OK;
}

EventHandlerResult OneShot::beforeReportingState() {
  for (uint8_t i = 0; i < ONESHOT_KEY_COUNT / 2; i++) {
    if (state_[i].active) {
      activateOneShot(i);
    }
  }

  return EventHandlerResult::OK;
}

EventHandlerResult OneShot::afterEachCycle() {
  bool oneshot_active = false;
  for (uint8_t i = 0; i < ONESHOT_KEY_COUNT; i++) {
    if (state_[i].active) {
      oneshot_active = true;
      break;
    }
  }
  if (oneshot_active && hasTimedOut())
    cancel();

  bool is_cancelled = false;

  for (uint8_t i = 0; i < ONESHOT_KEY_COUNT; i++) {
    if (should_cancel_) {
      if (state_[i].sticky) {
        if (should_cancel_stickies_) {
          is_cancelled = true;
          state_[i].sticky = false;
          cancelOneShot(i);
          state_[i].pressed = false;
        }
      } else if (state_[i].active && !state_[i].pressed) {
        is_cancelled = true;
        cancelOneShot(i);
      }
    }
  }

  if (is_cancelled) {
    should_cancel_ = false;
    should_cancel_stickies_ = false;
  }

  return EventHandlerResult::OK;
}

void OneShot::inject(Key mapped_key, uint8_t key_state) {
  onKeyswitchEvent(mapped_key, UnknownKeyswitchLocation, key_state);
}

// --- glue code ---

bool OneShot::isActive(void) {
  for (uint8_t i = 0; i < ONESHOT_KEY_COUNT; i++) {
    if ((state_[i].active && !hasTimedOut()) ||
        state_[i].pressed ||
        state_[i].sticky)
      return true;
  }
  return false;
}

bool OneShot::isActive(Key key) {
  uint8_t idx = key.getRaw() - ranges::OS_FIRST;

  return (state_[idx].active && !hasTimedOut()) ||
         state_[idx].pressed ||
         state_[idx].sticky;
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

void OneShot::cancel(bool with_stickies) {
  should_cancel_ = true;
  should_cancel_stickies_ = with_stickies;
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
