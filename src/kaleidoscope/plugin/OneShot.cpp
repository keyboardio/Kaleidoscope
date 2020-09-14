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
#include "kaleidoscope/layers.h"

namespace kaleidoscope {
namespace plugin {

// ----------------------------------------------------------------------------
// Configuration variables

uint16_t OneShot::timeout_ = 2500;
uint16_t OneShot::hold_timeout_ = 250;
int16_t OneShot::double_tap_timeout_ = -1;

// Deprecated
uint16_t OneShot::time_out = 2500;
uint16_t OneShot::hold_time_out = 250;
int16_t OneShot::double_tap_time_out = -1;

// ----------------------------------------------------------------------------
// State variables

uint16_t OneShot::stickable_keys_ = -1;

KeyAddrBitfield OneShot::temp_addrs_;
KeyAddrBitfield OneShot::glue_addrs_;

uint16_t OneShot::start_time_ = 0;
KeyAddr OneShot::prev_key_addr_ = KeyAddr::none();
uint8_t OneShot::release_countdown_ = 0;


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

bool OneShot::isStickable(Key key) {
  int8_t n;
  if (key.isKeyboardModifier()) {
    n = key.getKeyCode() - Key_LeftControl.getKeyCode();
    return bitRead(stickable_keys_, n);
  } else if (key.isLayerShift()) {
    n = oneshot_mod_count + key.getKeyCode() - LAYER_SHIFT_OFFSET;
    if (n < oneshot_key_count) {
      return bitRead(stickable_keys_, n);
    }
  }
  return false;
}

// These functions are particularly useful for ActiveModColor, which could
// potentially use three different color values for the three states (sticky |
// active && !sticky | pressed && !active).

bool OneShot::isTemporary(KeyAddr key_addr) {
  return temp_addrs_.read(key_addr);
}

bool OneShot::isSticky(KeyAddr key_addr) {
  return (glue_addrs_.read(key_addr) && !temp_addrs_.read(key_addr));
}

bool OneShot::isActive(KeyAddr key_addr) {
  return (isTemporary(key_addr) || glue_addrs_.read(key_addr));
}

// ----------------------------------------------------------------------------
// Other functions

// Cancel all active OneShot keys (if `cancel_stickies` is true) or just
// non-sticky active OneShot keys. This function is called by Escape-OneShot to
// release active OneShot keys.
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

EventHandlerResult OneShot::onKeyswitchEvent(
  Key &key, KeyAddr key_addr, uint8_t key_state) {

  // Ignore injected key events. This prevents re-processing events that the
  // hook functions generate (by calling `injectNormalKey()` via one of the
  // `*OneShot()` functions). There are more robust ways to do this, but since
  // OneShot is intended to react to only physical keypresses, this is adequate.
  if (key_state & INJECTED)
    return EventHandlerResult::OK;

  bool temp = temp_addrs_.read(key_addr);
  bool glue = glue_addrs_.read(key_addr);

  if (keyToggledOn(key_state)) {

    if (!temp && !glue) {
      // This key_addr is not in a OneShot state.
      if (isOneShotKey(key)) {
        // Replace the OneShot key with its corresponding normal key.
        pressKey(key_addr, key);
        return EventHandlerResult::ABORT;
      } else if (!key.isKeyboardModifier() && !key.isLayerShift()) {
        // Only trigger release of temporary OneShot keys if the pressed key is
        // neither a modifier nor a layer shift.
        release_countdown_ = (1 << 1);
      }
      // return EventHandlerResult::OK;

    } else if (temp && glue) {
      // This key_addr is in the temporary OneShot state.
      if (key_addr == prev_key_addr_) {
        // The same OneShot key has been pressed twice in a row. It will either
        // become sticky (if it has been double-tapped), or it will be become a
        // normal key. Either way, its `temp` state will be cleared.
        temp_addrs_.clear(key_addr);

        // Derive the true double-tap timeout value if we're using the default.
        uint16_t dtto = (double_tap_timeout_ < 0) ? timeout_ : double_tap_timeout_;

        // If the key is not stickable, or the double-tap timeout has expired,
        // clear the `glue` state, as well; this OneShot key has been cancelled,
        // and will become a normal key.
        if (!isStickable(key) || hasTimedOut(dtto)) {
          glue_addrs_.clear(key_addr);
        } else {
          return EventHandlerResult::ABORT;
        }
      } else {
        // This is a temporary OneShot key, but has not been pressed twice in a
        // row, so we need to clear its state.
        temp_addrs_.clear(key_addr);
        glue_addrs_.clear(key_addr);
      }

    } else if (!temp && glue) {
      // This is a sticky OneShot key that has been pressed. Clear state now, so
      // it will become a normal key.
      glue_addrs_.clear(key_addr);

    } else { // (temp && !glue)
      // A key has been pressed that is in the "pending" OneShot state. Since
      // this key should have entered the "temporary" OneShot state as soon as
      // it was released (from its first press), it should only be possible to
      // release a key that's in this state.
    }
    // Always record the address of a keypress. It might be useful for other
    // plugins, so this could perhaps be tracked in the Kaleidoscope core.
    prev_key_addr_ = key_addr;

  } else if (keyToggledOff(key_state)) {

    if (temp || glue) {
      // Any key in the "pending" OneShot state needs its `glue` state bit set
      // to make it "temporary". If it's in the "sticky" OneShot state, this is
      // redundant, but we're trading time efficiency to get smaller binary
      // size.
      glue_addrs_.set(key_addr);
      // This is an active OneShot key that has just been released. We need to
      // stop that event from sending a report, and instead send a "hold"
      // event. This is handled in the `beforeReportingState()` hook below.
      return EventHandlerResult::ABORT;
    }

  } else {
    // This key is being held.
    if (temp && !glue) {
      // This key is in the "pending" OneShot state. We need to check its hold
      // timeout, and turn it back into a normal key if it has timed out.
      if (hasTimedOut(hold_timeout_)) {
        temp_addrs_.clear(key_addr);
      }
    }

    if (isOneShotKey(key)) {
      // whoops! someone cancelled a oneshot key while it was being held;
      // reactivate it, but set it as a normal modifier instead. Or better yet,
      // mask it, in case of a layer change.
    }
  }

  return EventHandlerResult::OK;
}


// For any active OneShot modifier keys, keep those modifiers active in the
// keyboard HID report.
EventHandlerResult OneShot::beforeReportingState() {
  for (KeyAddr key_addr : glue_addrs_) {
    holdKey(key_addr);
  }
  return EventHandlerResult::OK;
}


EventHandlerResult OneShot::afterEachCycle() {
  // If a normal, non-modifier key has been pressed, or if active, non-sticky
  // OneShot keys have timed out, this is where they get released. Release is
  // triggered when `release_countdown_` gets to 1, not 0, because most of the
  // time it will be 0 (see below). It gets set to 2 on the press of a normal
  // key when there are any active OneShot keys; that way, the OneShot keys will
  // stay active long enough to apply to the newly-pressed key.
  if ((release_countdown_ == 1) || hasTimedOut(timeout_)) {
    for (KeyAddr key_addr : temp_addrs_) {
      if (glue_addrs_.read(key_addr)) {
        releaseKey(key_addr);
      }
      temp_addrs_.clear(key_addr);
    }
  }
  // Also, advance the counter for OneShot keys that have been cancelled by the
  // press of a non-OneShot, non-modifier key. An unconditional bit shift should
  // be more efficient than checking for zero to avoid underflow.
  release_countdown_ >>= 1;

  // Temporary fix for deprecated variables
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
  timeout_ = time_out;
  hold_timeout_ = hold_time_out;
  double_tap_timeout_ = double_tap_time_out;
#pragma GCC diagnostic pop

  return EventHandlerResult::OK;
}

// ============================================================================
// Private functions, not exposed to other plugins

// ----------------------------------------------------------------------------
// Helper functions for acting on OneShot key events

uint8_t OneShot::getOneShotKeyIndex(Key oneshot_key) {
  // The calling function is responsible for verifying that `oneshot_key` is an
  // actual OneShot key (i.e. call `isOneShotKey(oneshot_key)` first).
  uint8_t index = oneshot_key.getRaw() - ranges::OS_FIRST;
  return index;
}

uint8_t OneShot::getKeyIndex(Key key) {
  // Default to returning a value that's out of range. This should be harmless
  // because we only use the returned index to reference a bit in a bitfield,
  // not as a memory address.
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
  // The calling function is responsible for verifying that `oneshot_key` is an
  // actual OneShot key (i.e. call `isOneShotKey(oneshot_key)` first).
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

void OneShot::pressKey(KeyAddr key_addr, Key oneshot_key) {
  Key key = decodeOneShotKey(oneshot_key);
  prev_key_addr_ = key_addr;
  start_time_ = Runtime.millisAtCycleStart();
  temp_addrs_.set(key_addr);
  handleKeyswitchEvent(key, key_addr, IS_PRESSED | INJECTED);
}

void OneShot::holdKey(KeyAddr key_addr) {
  handleKeyswitchEvent(Key_NoKey, key_addr, WAS_PRESSED | IS_PRESSED | INJECTED);
}

void OneShot::releaseKey(KeyAddr key_addr) {
  glue_addrs_.clear(key_addr);
  temp_addrs_.clear(key_addr);
  handleKeyswitchEvent(Key_NoKey, key_addr, WAS_PRESSED | INJECTED);
}

// ------------------------------------------------------------------------------
// Deprecated functions

void OneShot::inject(Key key, uint8_t key_state) {
  if (! isOneShotKey(key)) {
    return;
  }
  // Find an idle keyswitch to use for the injected OneShot key and activate
  // it. This is an ugly hack, but it will work. It does mean that whatever key
  // is used will be unavailable for its normal function until the injected
  // OneShot key is deactivated, so use of `inject()` is strongly discouraged.
  for (KeyAddr key_addr : KeyAddr::all()) {
    if (Runtime.activeKey(key_addr) == Key_Transparent) {
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
    if (Runtime.activeKey(key_addr) == key) {
      return true;
    }
  }
  return false;
}

bool OneShot::isActive(Key oneshot_key) {
  if (! isOneShotKey(oneshot_key)) {
    return false;
  }
  Key key = decodeOneShotKey(oneshot_key);
  for (KeyAddr key_addr : glue_addrs_) {
    if (Runtime.activeKey(key_addr) == key) {
      return true;
    }
  }
  return false;
}

bool OneShot::isSticky(Key oneshot_key) {
  if (! isOneShotKey(oneshot_key)) {
    return false;
  }
  Key key = decodeOneShotKey(oneshot_key);
  for (KeyAddr key_addr : glue_addrs_) {
    if (Runtime.activeKey(key_addr) == key &&
        !temp_addrs_.read(key_addr)) {
      return true;
    }
  }
  return false;
}


} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::OneShot OneShot;
