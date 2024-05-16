/* -*- mode: c++ -*-
 * Kaleidoscope-LongPress -- Provide different key strokes on long press
 * Copyright (C) 2024  Keyboard.io, Inc
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

#include "kaleidoscope/plugin/LongPress.h"

#include "kaleidoscope/KeyAddr.h"          // for KeyAddr, MatrixAddr
#include "kaleidoscope/KeyEvent.h"         // for KeyEvent
#include "kaleidoscope/KeyEventTracker.h"  // for KeyEventTracker
#include "kaleidoscope/Runtime.h"          // for Runtime, Runtime_
#include "kaleidoscope/key_defs.h"         // for Key, Key_0, Key_1, Key_A, Key_F1, Key_F12, Key...
#include "kaleidoscope/keyswitch_state.h"  // for keyToggledOn, keyIsInjected
#include "kaleidoscope/progmem_helpers.h"  // for cloneFromProgmem

// IWYU pragma: no_include "HIDAliases.h"

namespace kaleidoscope {
namespace plugin {


// =============================================================================
// LongPress functions

void LongPress::disable() {
  settings_.enabled = false;
  if (pending_event_.addr.isValid()) {
    Runtime.handleKeyswitchEvent(pending_event_);
  }
}

// -----------------------------------------------------------------------------
// Test for whether or not to apply auto-shift to a given `Key`.  This function
// can be overridden from the user sketch.
__attribute__((weak)) bool LongPress::isAutoShiftable(Key key) {
  return autoShiftEnabledForKey(key);
}

// The default method that determines whether a particular key is an auto-shift
// candidate.  Used by `isAutoShiftable()`, separate to allow re-use when the
// caller is overridden.
bool LongPress::autoShiftEnabledForKey(Key key) {
  // We only support auto-shifting keyboard keys.  We could also explicitly
  // ignore modifier keys, but there's no need to do so, as none of the code
  // below matches modifiers.
  if (!key.isKeyboardKey())
    return false;

  // We compare only the keycode, and disregard any modifier flags applied to
  // the key.  This simplifies the comparison, and also allows AutoShift to
  // apply to keys like `RALT(Key_E)`.
  uint8_t keycode = key.getKeyCode();

  if (isAutoshiftEnabled(longpress::AutoShiftCategories::allKeys())) {
    if (keycode < HID_KEYBOARD_FIRST_MODIFIER)
      return true;
  }
  if (isAutoshiftEnabled(longpress::AutoShiftCategories::letterKeys())) {
    if (keycode >= Key_A.getKeyCode() && keycode <= Key_Z.getKeyCode())
      return true;
  }
  if (isAutoshiftEnabled(longpress::AutoShiftCategories::numberKeys())) {
    if (keycode >= Key_1.getKeyCode() && keycode <= Key_0.getKeyCode())
      return true;
  }
  if (isAutoshiftEnabled(longpress::AutoShiftCategories::symbolKeys())) {
    if ((keycode >= Key_Minus.getKeyCode() && keycode <= Key_Slash.getKeyCode()) ||
        (keycode == Key_NonUsBackslashAndPipe.getKeyCode()))
      return true;
  }
  if (isAutoshiftEnabled(longpress::AutoShiftCategories::arrowKeys())) {
    if (keycode >= Key_RightArrow.getKeyCode() &&
        keycode <= Key_LeftArrow.getKeyCode())
      return true;
  }
  if (isAutoshiftEnabled(longpress::AutoShiftCategories::functionKeys())) {
    if ((keycode >= Key_F1.getKeyCode() && keycode <= Key_F12.getKeyCode()) ||
        (keycode >= Key_F13.getKeyCode() && keycode <= Key_F24.getKeyCode()))
      return true;
  }

  return false;
}

// =============================================================================
// Event handler hook functions

// -----------------------------------------------------------------------------
EventHandlerResult LongPress::onKeyswitchEvent(KeyEvent &event) {
  // If LongPress has already processed and released this event, ignore it.
  // There's no need to update the event tracker in this one case.
  if (event_tracker_.shouldIgnore(event)) {
    // We should never get an event that's in our queue here, but just in case
    // some other plugin sends one, abort.
    if (queue_.shouldAbort(event))
      return EventHandlerResult::ABORT;
    return EventHandlerResult::OK;
  }

  // If event.addr is not a physical key, ignore it; some other plugin injected
  // it.  This check should be unnecessary.
  if (!event.addr.isValid() || keyIsInjected(event.state)) {
    return EventHandlerResult::OK;
  }

  // Do nothing if disabled.
  if (!settings_.enabled)
    return EventHandlerResult::OK;

  if (!queue_.isEmpty()) {
    // There's an unresolved LongPress key press.
    if (keyToggledOn(event.state) ||
        event.addr == queue_.addr(0) ||
        queue_.isFull()) {
      // If a new key toggled on, the unresolved key toggled off (it was a
      // "tap"), or if the queue is full, we clear the queue, and the key event
      // does not get modified.
      flushEvent(false);
      flushQueue();
    } else {
      // Otherwise, add the release event to the queue.  We do this so that
      // rollover from a modifier to an auto-shifted key will result in the
      // modifier being applied to the key.
      queue_.append(event);
      return EventHandlerResult::ABORT;
    }
  }

  if (keyToggledOn(event.state) &&
      (isExplicitlyMapped(event.addr, event.key) || isAutoShiftable(event.key))) {
    // The key is explicitly configured for long presses or is eligible to
    // be auto-shifted, so we add it to the queue and defer processing of
    // the event.
    queue_.append(event);
    return EventHandlerResult::ABORT;
  }

  return EventHandlerResult::OK;
}


bool LongPress::isExplicitlyMapped(KeyAddr addr, Key key) {
  // check the active layer as mappings may be active only for certain layers
  uint8_t active_layer = Layer.lookupActiveLayer(addr);

  // Check whether the given physical KeyAddr or logical Key has an
  // explicit mapping to a logical one for the current layer
  for (uint8_t i{0}; i < explicitmappings_count_; ++i) {
    LongPressKey mappedKey = cloneFromProgmem(explicitmappings_[i]);

    // don’t consider the mapping if it does not apply to the current or all layers
    if (mappedKey.layer != kaleidoscope::plugin::longpress::ALL_LAYERS && mappedKey.layer != active_layer) {
      continue;
    }

    // cache the mapped key to not have to search it again

    if (mappedKey.addr == addr) {
      // check for a mapping on a KeyAddr
      mapped_key_.addr             = mappedKey.addr;
      mapped_key_.key              = Key_Transparent;
      mapped_key_.longpress_result = mappedKey.longpress_result;
      return true;
    } else if (mappedKey.key == key) {
      // then check for a mapping a o Key
      mapped_key_.addr             = KeyAddr::none();
      mapped_key_.key              = mappedKey.key;
      mapped_key_.longpress_result = mappedKey.longpress_result;
      return true;
    }
  }

  // If no matches were found, clear mapped_key_ and return false
  mapped_key_.addr             = KeyAddr::none();
  mapped_key_.key              = Key_Transparent;
  mapped_key_.longpress_result = Key_Transparent;
  return false;
}


// -----------------------------------------------------------------------------
EventHandlerResult LongPress::afterEachCycle() {
  // If there's a pending LongPress event, and it has timed out, we need to
  // release the event with the `shift` flag applied.
  if (!queue_.isEmpty() &&
      Runtime.hasTimeExpired(queue_.timestamp(0), settings_.timeout)) {
    // Toggle the state of the `SHIFT_HELD` bit in the modifier flags for the
    // key for the pending event.
    flushEvent(true);
    flushQueue();
  }
  return EventHandlerResult::OK;
}

void LongPress::flushQueue() {
  while (!queue_.isEmpty()) {
    if (queue_.isRelease(0) || checkForRelease()) {
      flushEvent(false);
    } else {
      return;
    }
  }
}

bool LongPress::checkForRelease() const {
  KeyAddr queue_head_addr = queue_.addr(0);
  for (uint8_t i = 1; i < queue_.length(); ++i) {
    if (queue_.addr(i) == queue_head_addr) {
      // This key's release is also in the queue
      return true;
    }
  }
  return false;
}

void LongPress::flushEvent(bool is_long_press) {
  if (queue_.isEmpty()) {
    return;
  }

  KeyEvent event = queue_.event(0);
  if (is_long_press) {
    if (mapped_key_.addr != KeyAddr::none()) {
      // If we have an explicit mapping for that physical key, apply that.
      event.key = mapped_key_.longpress_result;
    } else if (mapped_key_.key != Key_Transparent) {
      // If we have an explicit mapping for that logical key, apply that.
      event.key = mapped_key_.longpress_result;
    } else {
      // If there was no explicit mapping, just add the shift modifier
      event.key     = Runtime.lookupKey(event.addr);
      uint8_t flags = event.key.getFlags();
      flags ^= SHIFT_HELD;
      event.key.setFlags(flags);
    }
  }
  queue_.shift();
  Runtime.handleKeyswitchEvent(event);
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::LongPress LongPress;
