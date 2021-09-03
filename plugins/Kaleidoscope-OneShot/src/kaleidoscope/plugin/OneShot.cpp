/* -*- mode: c++ -*-
 * Kaleidoscope-OneShot -- One-shot modifiers and layers
 * Copyright (C) 2016-2021  Keyboard.io, Inc.
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
#include <Kaleidoscope-FocusSerial.h>
#include "kaleidoscope/keyswitch_state.h"
#include "kaleidoscope/key_events.h"
#include "kaleidoscope/layers.h"

namespace kaleidoscope {
namespace plugin {

// ----------------------------------------------------------------------------
// Configuration variables

uint16_t OneShot::timeout_ = 2500;
uint16_t OneShot::hold_timeout_ = 250;
int16_t OneShot::double_tap_timeout_ = -1;

// Deprecated
#ifndef NDEPRECATED
uint16_t OneShot::time_out = 2500;
uint16_t OneShot::hold_time_out = 250;
int16_t OneShot::double_tap_time_out = -1;
#endif

// ----------------------------------------------------------------------------
// State variables

uint16_t OneShot::stickable_keys_ = -1;

bool OneShot::auto_modifiers_ = false;
bool OneShot::auto_layers_ = false;

KeyAddrBitfield OneShot::temp_addrs_;
KeyAddrBitfield OneShot::glue_addrs_;

uint16_t OneShot::start_time_ = 0;
KeyAddr OneShot::prev_key_addr_ = OneShot::invalid_key_addr;


// ============================================================================
// Public interface

// ----------------------------------------------------------------------------
// Configuration functions

void OneShot::enableStickability(Key key) {
  uint8_t n = getKeyIndex(key);
  stickable_keys_ |= (1 << n);
}

void OneShot::disableStickability(Key key) {
  uint8_t n = getKeyIndex(key);
  stickable_keys_ &= ~(1 << n);
}

void OneShot::enableStickabilityForModifiers() {
  stickable_keys_ |= stickable_modifiers_mask;
}

void OneShot::disableStickabilityForModifiers() {
  stickable_keys_ &= ~stickable_modifiers_mask;
}

void OneShot::enableStickabilityForLayers() {
  stickable_keys_ |= stickable_layers_mask;
}

void OneShot::disableStickabilityForLayers() {
  stickable_keys_ &= ~stickable_layers_mask;
}

// ----------------------------------------------------------------------------
// Global tests for any OneShot key

bool OneShot::isActive() {
  for (KeyAddr key_addr __attribute__((unused)) : temp_addrs_) {
    return true;
  }
  for (KeyAddr key_addr __attribute__((unused)) : glue_addrs_) {
    return true;
  }
  return false;
}

bool OneShot::isSticky() {
  for (KeyAddr key_addr : glue_addrs_) {
    if (! temp_addrs_.read(key_addr)) {
      return true;
    }
  }
  return false;
}

// ----------------------------------------------------------------------------
// Key-specific OneShot key tests

// These functions are particularly useful for ActiveModColor, which
// could potentially use three different color values for the three
// states (sticky | active && !sticky | pressed && !active).

__attribute__((weak))
bool OneShot::isStickable(Key key) {
  return isStickableDefault(key);
}

bool OneShot::isStickableDefault(Key key) {
  int8_t n;
  // If the key is either a keyboard modifier or a layer shift, we check to see
  // if it has been set to be non-stickable.
  if (key.isKeyboardModifier()) {
    n = key.getKeyCode() - Key_LeftControl.getKeyCode();
    return bitRead(stickable_keys_, n);
  } else if (key.isLayerShift()) {
    n = oneshot_mod_count + key.getKeyCode() - LAYER_SHIFT_OFFSET;
    // We only keep track of the stickability of the first 8 layers.
    if (n < oneshot_key_count) {
      return bitRead(stickable_keys_, n);
    }
  }
  // The default is for all keys to be "stickable"; if the default was false,
  // any user code or other plugin that uses `setPending()` to turn a key into a
  // OneShot would need to override `isStickable()` in order to make that key
  // stickable (the default `OSM()` behaviour).
  return true;
}

bool OneShot::isTemporary(KeyAddr key_addr) {
  return temp_addrs_.read(key_addr);
}

bool OneShot::isPending(KeyAddr key_addr) {
  return (glue_addrs_.read(key_addr) && temp_addrs_.read(key_addr));
}

bool OneShot::isSticky(KeyAddr key_addr) {
  return (glue_addrs_.read(key_addr) && !temp_addrs_.read(key_addr));
}

bool OneShot::isActive(KeyAddr key_addr) {
  return (isTemporary(key_addr) || glue_addrs_.read(key_addr));
}

// ----------------------------------------------------------------------------
// Public state-setting functions

void OneShot::setPending(KeyAddr key_addr) {
  temp_addrs_.set(key_addr);
  glue_addrs_.clear(key_addr);
  start_time_ = Runtime.millisAtCycleStart();
}

void OneShot::setOneShot(KeyAddr key_addr) {
  temp_addrs_.set(key_addr);
  glue_addrs_.set(key_addr);
  start_time_ = Runtime.millisAtCycleStart();
}

void OneShot::setSticky(KeyAddr key_addr) {
  temp_addrs_.clear(key_addr);
  glue_addrs_.set(key_addr);
}

void OneShot::clear(KeyAddr key_addr) {
  temp_addrs_.clear(key_addr);
  glue_addrs_.clear(key_addr);
}

// ----------------------------------------------------------------------------
// Other functions

// Cancel all active OneShot keys (if `cancel_stickies` is true) or
// just non-sticky active OneShot keys. This function is called by
// Escape-OneShot to release active OneShot keys.
void OneShot::cancel(bool cancel_stickies) {
  if (cancel_stickies) {
    for (KeyAddr key_addr : glue_addrs_) {
      releaseKey(key_addr);
    }
  }
  for (KeyAddr key_addr : temp_addrs_) {
    if (glue_addrs_.read(key_addr)) {
      releaseKey(key_addr);
    } else {
      temp_addrs_.clear(key_addr);
    }
  }
}

// ----------------------------------------------------------------------------
// Plugin hook functions

EventHandlerResult OneShot::onNameQuery() {
  return ::Focus.sendName(F("OneShot"));
}

EventHandlerResult OneShot::onKeyEvent(KeyEvent& event) {

  // Ignore injected key events. This prevents re-processing events that the
  // hook functions generate (by calling `injectNormalKey()` via one of the
  // `*OneShot()` functions). There are more robust ways to do this, but since
  // OneShot is intended to react to only physical keypresses, this is adequate.
  if (keyIsInjected(event.state))
    return EventHandlerResult::OK;

  bool temp = temp_addrs_.read(event.addr);
  bool glue = glue_addrs_.read(event.addr);

  if (keyToggledOn(event.state)) {

    if (!temp && !glue) {
      // The key is in the "normal" state. The first thing we need to do is
      // convert OneShot keys to their equivalent values, and record the fact
      // that the key that just toggled on should transition to the "pending"
      // state.
      bool is_oneshot = false;
      if (isOneShotKey(event.key)) {
        event.key = decodeOneShotKey(event.key);
        is_oneshot = true;
      }

      if (is_oneshot ||
          (auto_modifiers_ && event.key.isKeyboardModifier()) ||
          (auto_layers_ && event.key.isLayerShift())) {
        temp_addrs_.set(event.addr);
        start_time_ = Runtime.millisAtCycleStart();
      } else if (!event.key.isKeyboardModifier() &&
                 !event.key.isLayerShift()) {
        // Only trigger release of temporary one-shot keys if the pressed key is
        // neither a modifier nor a layer shift. We need the actual release of
        // those keys to happen after the current event is finished, however, so
        // we trigger it by back-dating the start time, so that the timeout
        // check will trigger in the afterEachCycle() hook.
        start_time_ -= timeout_;
      }

    } else if (temp && glue) {
      // temporary one-shot
      temp_addrs_.clear(event.addr);

      if (event.addr == prev_key_addr_ &&
          isStickable(event.key) &&
          !hasDoubleTapTimedOut()) {
        // The same stickable key has been double-tapped within the double-tap
        // timeout window. We cancel the second press event, emulating a single
        // press-and-hold. This doesn't interfere with `prev_key_addr_`, since
        // it's the same key again.
        return EventHandlerResult::ABORT;
      } else {
        // A second tap that's not a double-tap cancels the one-shot state
        glue_addrs_.clear(event.addr);
      }

    } else if (!temp && glue) {
      // sticky state
      temp_addrs_.clear(event.addr);
      glue_addrs_.clear(event.addr);

    } else { /* if (temp && !glue) */
      // A key has been pressed that is in the "pending" one-shot state. Since
      // this key should have entered the "temporary" one-shot state as soon as
      // it was released (from its first press), it should only be possible to
      // release a key that's in this state.
    }
    // Always record the address of a keypress. It might be useful for other
    // plugins, so this could perhaps be tracked in the Kaleidoscope core.
    prev_key_addr_ = event.addr;

  } else {

    // Key toggled off
    if (temp || glue) {
      // Any key in the "pending" one-shot state needs its `glue` state bit set
      // to make it "temporary". If it's in the "sticky" OneShot state, this is
      // redundant, but we're trading execution speed to get a smaller binary.
      glue_addrs_.set(event.addr);
      // This is an active one-shot key that has just been released. We need to
      // stop that event from sending a report, and instead send a "hold"
      // event. This is handled in the `beforeReportingState()` hook below.
      return EventHandlerResult::ABORT;
    }

  }

  return EventHandlerResult::OK;
}

// ----------------------------------------------------------------------------
EventHandlerResult OneShot::afterReportingState(const KeyEvent& event) {
  return afterEachCycle();
}

// ----------------------------------------------------------------------------
EventHandlerResult OneShot::afterEachCycle() {

  bool oneshot_expired = hasTimedOut(timeout_);
  bool hold_expired = hasTimedOut(hold_timeout_);
  bool any_temp_keys = false;

  for (KeyAddr key_addr : temp_addrs_) {
    any_temp_keys = true;

    if (glue_addrs_.read(key_addr)) {
      // Release keys in "one-shot" state that have timed out or been cancelled
      // by another key press.
      if (oneshot_expired)
        releaseKey(key_addr);
    } else {
      // Cancel "pending" state of keys held longer than the hold timeout.
      if (hold_expired)
        temp_addrs_.clear(key_addr);
    }
  }

  // Keep the start time from getting stale; if there are no keys waiting for a
  // timeout, it's safe to advance the timer to the current time.
  if (!any_temp_keys) {
    start_time_ = Runtime.millisAtCycleStart();
  }

  // Temporary fix for deprecated variables
#ifndef NDEPRECATED
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
  timeout_ = time_out;
  hold_timeout_ = hold_time_out;
  double_tap_timeout_ = double_tap_time_out;
#pragma GCC diagnostic pop
#endif

  return EventHandlerResult::OK;
}

// ============================================================================
// Private functions, not exposed to other plugins

// ----------------------------------------------------------------------------
// Helper functions for acting on OneShot key events

uint8_t OneShot::getOneShotKeyIndex(Key oneshot_key) {
  // The calling function is responsible for verifying that
  // `oneshot_key` is an actual OneShot key (i.e. call
  // `isOneShotKey(oneshot_key)` first).
  uint8_t index = oneshot_key.getRaw() - ranges::OS_FIRST;
  return index;
}

uint8_t OneShot::getKeyIndex(Key key) {
  // Default to returning a value that's out of range. This should be
  // harmless because we only use the returned index to reference a
  // bit in a bitfield, not as a memory address.
  uint8_t n{oneshot_key_count};

  if (isOneShotKey(key)) {
    n = getOneShotKeyIndex(key);
  } else if (key.isKeyboardModifier()) {
    n = key.getKeyCode() - Key_LeftControl.getKeyCode();
  } else if (key.isLayerShift()) {
    n = oneshot_mod_count + key.getKeyCode() - LAYER_SHIFT_OFFSET;
  }
  return n;
}

Key OneShot::decodeOneShotKey(Key oneshot_key) {
  // The calling function is responsible for verifying that
  // `oneshot_key` is an actual OneShot key (i.e. call
  // `isOneShotKey(oneshot_key)` first).
  uint8_t n = getOneShotKeyIndex(oneshot_key);
  if (n < oneshot_mod_count) {
    return Key(Key_LeftControl.getKeyCode() + n,
               Key_LeftControl.getFlags());
  } else {
    return Key(LAYER_SHIFT_OFFSET + n - oneshot_mod_count,
               KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP);
  }
}

// ------------------------------------------------------------------------------
// Helper functions for sending key events for keys in OneShot states

void OneShot::pressKey(KeyAddr key_addr, Key key) {
  if (isOneShotKey(key)) {
    key = decodeOneShotKey(key);
  }
  prev_key_addr_ = key_addr;
  start_time_ = Runtime.millisAtCycleStart();
  temp_addrs_.set(key_addr);
  KeyEvent event{key_addr, IS_PRESSED | INJECTED, key};
  Runtime.handleKeyEvent(event);
}

void OneShot::holdKey(KeyAddr key_addr) {
  KeyEvent event{key_addr, WAS_PRESSED | IS_PRESSED | INJECTED};
  Runtime.handleKeyEvent(event);
}

void OneShot::releaseKey(KeyAddr key_addr) {
  glue_addrs_.clear(key_addr);
  temp_addrs_.clear(key_addr);

  KeyEvent event{key_addr, WAS_PRESSED | INJECTED};
  Runtime.handleKeyEvent(event);
}

// ------------------------------------------------------------------------------
// Deprecated functions
#ifndef NDEPRECATED
void OneShot::inject(Key key, uint8_t key_state) {
  if (isOneShotKey(key)) {
    key = decodeOneShotKey(key);
  }
  // Find an idle keyswitch to use for the injected OneShot key and activate
  // it. This is an ugly hack, but it will work. It does mean that whatever key
  // is used will be unavailable for its normal function until the injected
  // OneShot key is deactivated, so use of `inject()` is strongly discouraged.
  for (KeyAddr key_addr : KeyAddr::all()) {
    if (live_keys[key_addr] == Key_Transparent) {
      pressKey(key_addr, key);
      glue_addrs_.set(key_addr);
      break;
    }
  }
}

bool OneShot::isModifierActive(Key key) {
  // This actually works for any `Key` value, not just modifiers. Because we're
  // just searching the keymap cache, it's also possible to return a false
  // positive (a plugin might have altered the cache for an idle `KeyAddr`), or
  // a false negative (a plugin might be inserting a modifier without a valid
  // `KeyAddr`), but as this is a deprecated function, I think this is good
  // enough.
  for (KeyAddr key_addr : KeyAddr::all()) {
    if (live_keys[key_addr] == key) {
      return true;
    }
  }
  return false;
}

bool OneShot::isActive(Key key) {
  if (isOneShotKey(key)) {
    key = decodeOneShotKey(key);
  }
  for (KeyAddr key_addr : glue_addrs_) {
    if (live_keys[key_addr] == key) {
      return true;
    }
  }
  return false;
}

bool OneShot::isSticky(Key key) {
  if (isOneShotKey(key)) {
    key = decodeOneShotKey(key);
  }
  for (KeyAddr key_addr : glue_addrs_) {
    if (live_keys[key_addr] == key &&
        !temp_addrs_.read(key_addr)) {
      return true;
    }
  }
  return false;
}
#endif


} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::OneShot OneShot;
