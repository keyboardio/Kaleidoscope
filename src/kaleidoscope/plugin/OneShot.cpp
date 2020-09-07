/* -*- mode: c++ -*-
 * Kaleidoscope-OneShot -- One-shot modifiers and layers
 * Copyright (C) 2016-2020  Keyboard.io, Inc.
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

// ----------------------------------------------------------------------------
// Configuration variables

uint16_t OneShot::time_out = 2500;
uint16_t OneShot::hold_time_out = 250;
int16_t OneShot::double_tap_time_out = -1;

// ----------------------------------------------------------------------------
// State variables

OneShot::OneShotKeyState OneShot::state_[OneShot::ONESHOT_KEY_COUNT];
uint16_t OneShot::start_time_ = 0;
KeyAddr OneShot::prev_key_addr_ = UnknownKeyswitchLocation;
uint8_t OneShot::release_countdown_ = 0;


// ============================================================================
// Public interface

// ----------------------------------------------------------------------------
// Configuration functions

void OneShot::enableStickability(Key key) {
  if (key >= ranges::OS_FIRST && key <= ranges::OS_LAST)
    state_[key.getRaw() - ranges::OS_FIRST].stickable = true;
}

void OneShot::disableStickability(Key key) {
  if (key >= ranges::OS_FIRST && key <= ranges::OS_LAST)
    state_[key.getRaw() - ranges::OS_FIRST].stickable = false;
}

void OneShot::enableStickabilityForModifiers() {
  for (uint8_t i{0}; i < ONESHOT_MOD_COUNT; ++i) {
    state_[i].stickable = true;
  }
}

void OneShot::disableStickabilityForModifiers() {
  for (uint8_t i{0}; i < ONESHOT_MOD_COUNT; ++i) {
    state_[i].stickable = false;
  }
}

void OneShot::enableStickabilityForLayers() {
  for (uint8_t i{ONESHOT_MOD_COUNT}; i < ONESHOT_KEY_COUNT; ++i) {
    state_[i].stickable = true;
  }
}

void OneShot::disableStickabilityForLayers() {
  for (uint8_t i{ONESHOT_MOD_COUNT}; i < ONESHOT_KEY_COUNT; ++i) {
    state_[i].stickable = false;
  }
}

// ----------------------------------------------------------------------------
// Global tests for any OneShot key

bool OneShot::isActive() {
  for (uint8_t i{0}; i < ONESHOT_KEY_COUNT; ++i) {
    if (state_[i].active)
      return true;
  }
  return false;
}

bool OneShot::isSticky() {
  for (uint8_t i{0}; i < ONESHOT_KEY_COUNT; ++i) {
    if (state_[i].sticky)
      return true;
  }
  return false;
}

bool OneShot::isPressed() {
  for (uint8_t i{0}; i < ONESHOT_KEY_COUNT; ++i) {
    if (state_[i].pressed)
      return true;
  }
  return false;
}

// ----------------------------------------------------------------------------
// Key-specific OneShot key tests

// These functions are particularly useful for ActiveModColor, which
// could potentially use three different color values for the three
// states (sticky | active && !sticky | pressed && !active).

// WARNING: There is no bounds checking done before getting the value
// from the array. The caller must check `OneShot.isOneShotKey(key)`
// before calling any of these functions.
bool OneShot::isActive(Key key) {
  uint8_t n = key.getRaw() - ranges::OS_FIRST;
  return state_[n].active;
}

bool OneShot::isSticky(Key key) {
  uint8_t n = key.getRaw() - ranges::OS_FIRST;
  return state_[n].sticky;
}

bool OneShot::isPressed(Key key) {
  uint8_t n = key.getRaw() - ranges::OS_FIRST;
  return state_[n].pressed;
}

bool OneShot::isStickable(Key key) {
  uint8_t n = key.getRaw() - ranges::OS_FIRST;
  return state_[n].stickable;
}

// ----------------------------------------------------------------------------
// Other functions

// Cancel all active OneShot keys (if `cancel_stickies` is true) or
// just non-sticky active OneShot keys. This function is called by
// Escape-OneShot to release active OneShot keys.
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

// ----------------------------------------------------------------------------
// Vestigial functions?

// Is this function used by anything? It's part of the public API, but
// none of the core plugins use it. Calling the event handler doesn't
// seem like the right thing to do here, so perhaps the function
// should be deprecated, then removed.
void OneShot::inject(Key mapped_key, uint8_t key_state) {
  onKeyswitchEvent(mapped_key, UnknownKeyswitchLocation, key_state);
}

// This function also isn't called anywhere by any core
// plugins. Perhaps it should be deprecated.
bool OneShot::isModifierActive(Key key) {
  if (key < Key_LeftControl || key > Key_RightGui)
    return false;

  uint8_t n = key.getKeyCode() - Key_LeftControl.getKeyCode();
  return state_[n].active;
}


// ----------------------------------------------------------------------------
// Plugin hook functions

EventHandlerResult OneShot::onKeyswitchEvent(
  Key &mapped_key, KeyAddr key_addr, uint8_t key_state) {

  // Ignore injected key events. This prevents re-processing events
  // that the hook functions generate (by calling `injectNormalKey()`
  // via one of the `*OneShot()` functions). There are more robust
  // ways to do this, but since OneShot is intended to react to only
  // physical keypresses, this is adequate.
  if (key_state & INJECTED)
    return EventHandlerResult::OK;

  // If it's not a OneShot key, and not a modifier, set up the active,
  // non-sticky OneShot keys to be released at the end of the next
  // cycle.
  if (!isOneShotKey(mapped_key)) {
    if (keyToggledOn(key_state)) {
      prev_key_addr_ = key_addr;
      if (!(mapped_key >= Key_LeftControl && mapped_key <= Key_RightGui) &&
          !(mapped_key.getFlags() == (KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP))) {
        // We can't immediately send the release event(s) for active
        // OneShot keys here, because that would release those keys
        // before the report in which the keycodes they are meant to
        // modify gets sent. Instead, we use this variable, which will
        // get bit-shifted each cycle, and trigger the release when it
        // gets to 1 (which is why it's written as a bit-shifted 1 here).
        release_countdown_ = 1 << 1;
      }
    }
    return EventHandlerResult::OK;
  }

  // We know that `mapped_key` is a OneShot key, and the call to
  // `isOneShotKey_()` above ensures that the index is not out of
  // bounds, so this is now valid:
  uint8_t n = mapped_key.getRaw() - ranges::OS_FIRST;

  if (keyToggledOn(key_state)) {
    state_[n].pressed = true;
    if (! state_[n].active) {
      // This is the first press of an inactive OneShot key, so we
      // simply activate it, setting `active` to true.
      // `activateOneShot()` sends the toggled on event, and in
      // subsequent cycles, the `beforeReportingState()` hook will
      // send `sustainOneShot()`, sending the hold "event".
      activateOneShot(n);
    } else if (state_[n].sticky) {
      // If the OneShot key is already sticky, and it gets pressed, we
      // remove its `active` state flag. As long as it is still held
      // (`pressed == true`), it will continue to be translated into
      // its corresponding modifier in `beforeReportingState()`.
      state_[n].active = false;
      state_[n].sticky = false;
    } else {
      // The OneShot key has been pressed while it is active, but not
      // (yet) sticky. First we determine if it's a candidate for
      // becoming sticky: it can only become sticky if the same key is
      // pressed twice in a row, and that key is configured to be
      // sticky.
      if ((key_addr == prev_key_addr_) && state_[n].stickable) {
        // In addition, it must have been "double-tapped": pressed
        // twice in less than `double_tap_time_out` milliseconds. But
        // if that variable isn't set (negative), we use the default
        // `time_out` value instead.
        uint16_t dtto = (double_tap_time_out < 0) ? time_out : double_tap_time_out;
        if (! Runtime.hasTimeExpired(start_time_, dtto)) {
          // Making a OneShot sticky is as simple as setting its
          // `sticky` flag; the other hook functions handle the rest.
          state_[n].sticky = true;
        } else {
          // If two presses weren't quick enough, this second press
          // will terminate the OneShot behaviour (see above).
          state_[n].active = false;
        }
      } else {
        // If this OneShot key is not "stickable", we deactivate
        // it. It still remains in effect until it is released.
        state_[n].active = false;
      }
    }
    prev_key_addr_ = key_addr;

  } else if (keyToggledOff(key_state)) {
    state_[n].pressed = false;
    if (! state_[n].active) {
      releaseOneShot(n);
    }

  } else {
    if (state_[n].active && !state_[n].sticky) {
      // If a OneShot key is held (on its initial press) long enough
      // (i.e. > `hold_time_out` milliseconds), deactivate it. It
      // still remains in effect until it is released.
      if (Runtime.hasTimeExpired(start_time_, hold_time_out)) {
        state_[n].active = false;
      }
    }
  }

  // Finally, we allow event processing to continue so that plugins
  // later in the processing order can act on OneShot key events (in
  // particular, ActiveModColor).
  return EventHandlerResult::OK;
}


// For any active OneShot modifier keys, keep those modifiers active
// in the keyboard HID report.
EventHandlerResult OneShot::beforeReportingState() {
  for (uint8_t i{0}; i < ONESHOT_KEY_COUNT; ++i) {
    if (state_[i].active || state_[i].pressed) {
      sustainOneShot(i);
    }
  }
  return EventHandlerResult::OK;
}


EventHandlerResult OneShot::afterEachCycle() {
  // If a normal, non-modifier key has been pressed, or if active,
  // non-sticky OneShot keys have timed out, this is where they get
  // released. Release is triggered when `release_countdown_` gets to
  // 1, not 0, because most of the time it will be 0 (see below). It
  // gets set to 2 on the press of a normal key when there are any
  // active OneShot keys; that way, the OneShot keys will stay active
  // long enough to apply to the newly-pressed key.
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

// ============================================================================
// Private functions, not exposed to other plugins

// ----------------------------------------------------------------------------
// Helper functions for acting on OneShot key events

Key OneShot::getNormalKey(uint8_t n) {
  if (n < ONESHOT_MOD_COUNT) {
    return Key(Key_LeftControl.getKeyCode() + n,
               Key_LeftControl.getFlags());
  } else {
    return Key(LAYER_SHIFT_OFFSET + n - ONESHOT_MOD_COUNT,
               KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP);
  }
}

void OneShot::injectNormalKey(uint8_t n, uint8_t key_state, KeyAddr key_addr) {
  handleKeyswitchEvent(getNormalKey(n), key_addr, key_state | INJECTED);
}

void OneShot::activateOneShot(uint8_t n) {
  start_time_ = Runtime.millisAtCycleStart();
  state_[n].active = true;
  injectNormalKey(n, IS_PRESSED);
}

void OneShot::sustainOneShot(uint8_t n) {
  injectNormalKey(n, WAS_PRESSED | IS_PRESSED);
}

void OneShot::releaseOneShot(uint8_t n) {
  state_[n].active = false;
  state_[n].sticky = false;
  injectNormalKey(n, WAS_PRESSED);
}

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::OneShot OneShot;
