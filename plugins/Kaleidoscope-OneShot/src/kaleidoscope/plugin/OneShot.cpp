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

// ----------------------------------------------------------------------------
// State variables

uint16_t OneShot::stickable_keys_ = -1;

bool OneShot::auto_modifiers_ = false;
bool OneShot::auto_layers_ = false;

KeyAddrBitfield OneShot::temp_addrs_;
KeyAddrBitfield OneShot::glue_addrs_;

uint16_t OneShot::start_time_ = 0;
KeyAddr OneShot::prev_key_addr_ = OneShot::invalid_key_addr;

#ifndef ONESHOT_WITHOUT_METASTICKY
KeyAddr OneShot::meta_sticky_key_addr_ {KeyAddr::invalid_state};
#endif


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
#ifndef ONESHOT_WITHOUT_METASTICKY
  } else if (key == OneShot_MetaStickyKey) {
    return true;
#endif
  }
  return false;
}

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
  if (event.state & INJECTED)
    return EventHandlerResult::OK;

  bool temp = temp_addrs_.read(event.addr);
  bool glue = glue_addrs_.read(event.addr);

  if (keyToggledOn(event.state)) {

    // Make all held keys sticky if `OneShot_ActiveStickyKey` toggles on.
    if (event.key == OneShot_ActiveStickyKey) {
      // Skip the stickify key itself
      for (KeyAddr entry_addr : KeyAddr::all()) {
        if (entry_addr == event.addr) {
          continue;
        }
        // Get the entry from the keyboard state array
        Key entry_key = live_keys[entry_addr];
        // Skip empty entries
        if (entry_key == Key_Transparent || entry_key == Key_NoKey) {
          continue;
        }
        // Make everything else sticky
        temp_addrs_.clear(entry_addr);
        glue_addrs_.set(entry_addr);
      }
      prev_key_addr_ = event.addr;
      return EventHandlerResult::OK;
    }

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

#ifndef ONESHOT_WITHOUT_METASTICKY
      bool is_meta_sticky_key_active = meta_sticky_key_addr_.isValid();
      if (is_meta_sticky_key_active) {
        // If the meta key isn't sticky, release it
        bool ms_temp = temp_addrs_.read(meta_sticky_key_addr_);
        bool ms_glue = glue_addrs_.read(meta_sticky_key_addr_);
        if (ms_temp) {
          if (ms_glue) {
            // The meta key is in the "one-shot" state; release it immediately.
            releaseKey(meta_sticky_key_addr_);
          } else {
            // The meta key is in the "pending" state; cancel that, and let it
            // deactivate on release.
            temp_addrs_.clear(meta_sticky_key_addr_);
          }
        }
        glue_addrs_.set(event.addr);
      } else if (event.key == OneShot_MetaStickyKey) {
        meta_sticky_key_addr_ = event.addr;
        temp_addrs_.set(event.addr);
      }
      if (is_meta_sticky_key_active || (event.key == OneShot_MetaStickyKey)) {
        prev_key_addr_ = event.addr;
        start_time_ = Runtime.millisAtCycleStart();
        return EventHandlerResult::OK;
      }
#endif

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
#ifndef ONESHOT_WITHOUT_METASTICKY
    } else if (event.key == OneShot_MetaStickyKey) {
      // Turn off the meta key if it's released in its "normal" state.
      meta_sticky_key_addr_ = KeyAddr::none();
#endif
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

#ifndef ONESHOT_WITHOUT_METASTICKY
  if (live_keys[key_addr] == OneShot_MetaStickyKey)
    meta_sticky_key_addr_ = KeyAddr::none();
#endif

  KeyEvent event{key_addr, WAS_PRESSED | INJECTED};
  Runtime.handleKeyEvent(event);
}

// ------------------------------------------------------------------------------

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::OneShot OneShot;
