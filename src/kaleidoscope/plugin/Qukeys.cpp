/* -*- mode: c++ -*-
 * Kaleidoscope-Qukeys -- Assign two keycodes to a single key
 * Copyright (C) 2017-2019  Michael Richters
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

#include "kaleidoscope/plugin/Qukeys.h"

#include "kaleidoscope/Runtime.h"
#include <Kaleidoscope-Ranges.h>
#include "kaleidoscope/progmem_helpers.h"
#include "kaleidoscope/layers.h"


namespace kaleidoscope {
namespace plugin {

// This is the event handler. It ignores certain events, but mostly just adds
// them to the Qukeys event queue.
EventHandlerResult Qukeys::onKeyswitchEvent(Key& key, KeyAddr k, uint8_t key_state) {
  // If k is not a physical key, ignore it; some other plugin injected it.
  if (! k.isValid() || (key_state & INJECTED) != 0) {
    return EventHandlerResult::OK;
  }

  // If the key was injected (from the queue being flushed), we need to ignore
  // it.
  if (flushing_queue_) {
    return EventHandlerResult::OK;
  }

  // If Qukeys is turned off, continue to next plugin.
  if (! active_) {
    if (isDualUseKey(key)) {
      key = queue_head_.primary_key;
    }
    return EventHandlerResult::OK;
  }

  // Deal with keyswitch state changes.
  if (keyToggledOn(key_state) || keyToggledOff(key_state)) {
    // If we can't trivially ignore the event, just add it to the queue.
    event_queue_.append(k, key_state);
    // In order to prevent overflowing the queue, process it now.
    if (event_queue_.isFull()) {
      processQueue();
    }
    // Any event that gets added to the queue gets re-processed later, so we
    // need to abort processing now.
    return EventHandlerResult::EVENT_CONSUMED;
  }

  // The key is being held. We need to determine if we should block it because
  // its key press event is still in the queue, waiting to be
  // flushed. Therefore, we search the event queue for the same key. If the
  // first event we find there is a key press, that means we need to suppress
  // this hold, because it's still waiting on an earlier event.
  for (uint8_t i{0}; i < event_queue_.length(); ++i) {
    if (event_queue_.addr(i) == k) {
      // If the first matching event is a release, we do not suppress it,
      // because its press event has already been flushed.
      if (event_queue_.isRelease(i)) {
        break;
      }
      // Otherwise, the first matching event was a key press, so we need to
      // suppress it for now.
      return EventHandlerResult::EVENT_CONSUMED;
    }
  }

  // Either this key doesn't have an event in the queue at all, or its first
  // event in the queue is a release. We treat the key as a normal held key.
  return EventHandlerResult::OK;
}


// This hook runs once each cycle, and checks to see if the first event in the
// queue is ready to be flushed. It only allows one event to be flushed per
// cycle, because the keyboard HID report can't store all of the information
// necessary to correctly handle all of the rollover corner cases.
EventHandlerResult Qukeys::beforeReportingState() {
  // For keys that have been physically released, but whose release events are
  // still waiting to be flushed from the queue, we need to restore them,
  // because `handleKeyswitchEvent()` didn't get called for those KeyAddrs.
  for (uint8_t i{0}; i < event_queue_.length(); ++i) {
    if (event_queue_.isRelease(i)) {
      KeyAddr k = event_queue_.addr(i);
      // Now for the tricky bit. Before "restoring" this key hold, we need to
      // make sure that its key press event has already been flushed from the
      // queue, so we need to search for a matching key press event preceding
      // this release event. If we find one, we need to ignore it.
      if (isKeyAddrInQueueBeforeIndex(k, i)) {
        continue;
      }
      flushing_queue_ = true;
      handleKeyswitchEvent(Key_NoKey, k, IS_PRESSED | WAS_PRESSED);
      flushing_queue_ = false;
    }
  }

  // If any events get flushed from the queue, stop there; we can only safely
  // send the one report per cycle.
  if (processQueue()) {
    return EventHandlerResult::OK;
  }

  // If we get here, that means that the first event in the queue is a qukey
  // press. All that's left to do is to check if it's been held long enough that
  // it has timed out.
  if (Runtime.hasTimeExpired(event_queue_.timestamp(0), hold_timeout_)) {
    // If it's a SpaceCadet-type key, it takes on its primary value, otherwise
    // it takes on its secondary value.
    Key event_key = isModifierKey(queue_head_.primary_key) ?
                    queue_head_.primary_key : queue_head_.alternate_key;
    flushEvent(event_key);
  }
  return EventHandlerResult::OK;
}


// -----------------------------------------------------------------------------


// This function contains most of the logic behind Qukeys. It gets called after
// an event gets added to the queue, and again once per cycle. It returns `true`
// if nothing more should be done, either because the queue is empty, or because
// an event has already been flushed. It's not perfect because we might be
// getting more than one event in a given cycle, and because the queue might
// overflow, but those are both rare cases, and should not cause any serious
// problems even when they do come up.
bool Qukeys::processQueue() {
  // If the queue is empty, signal that the beforeReportingState() process
  // should abort before checking for a hold timeout (since there's nothing to
  // do).
  if (event_queue_.isEmpty()) {
    return true;
  }

  // In other cases, we will want the KeyAddr of the first event in the queue.
  KeyAddr queue_head_addr = event_queue_.addr(0);

  // If that first event is a key release, it can be flushed right away.
  if (event_queue_.isRelease(0)) {
    flushEvent(Key_NoKey);
    return true;
  }

  // We now know that the first event is a key press. If it's not a qukey, or if
  // it's only there because the plugin was just turned off, we can flush it
  // immediately.
  if (! isQukey(queue_head_addr) || ! active_) {
    flushEvent(queue_head_.primary_key);
    return true;
  }

  // Now we know that the first event is a key press, and that it's a qukey. In
  // addition, `queue_head_` now contains the primary and secondary Key values
  // for that qukey.

  // This variable will be used to record the index in the event queue of the
  // first subsequent key press (after the qukey), if any.
  uint8_t next_keypress_index{0};

  // Next we record if the qukey (at the head of the queue) is a SpaceCadet-type
  // key, so we don't need to do it repeatedly later.
  bool qukey_is_spacecadet = isModifierKey(queue_head_.primary_key);

  // Now we search the queue for events that will let us decide if the qukey
  // should be flushed (and if so, in which of its two states). We start with
  // the second event in the queue (index 1).
  for (uint8_t i{1}; i < event_queue_.length(); ++i) {
    if (event_queue_.isPress(i)) {
      // If some other key was pressed after a SpaceCadet key, that means the
      // SpaceCadet qukey press should be flushed immediately, in its primary
      // (modifier) state. SpaceCadet keys only fall into their alternate state
      // if they are pressed and released with no rollover.
      if (qukey_is_spacecadet) {
        flushEvent(queue_head_.primary_key);
        return true;
      }
      // Otherwise, we've found a subsequent key press, so we record it for the
      // overlap comparison later, unless we've already done so.
      if (next_keypress_index == 0) {
        next_keypress_index = i;
      }
      continue;
    }

    // Now we know the event `i` is a key release. Next, we check to see if it
    // is a release of the qukey.
    if (event_queue_.addr(i) == queue_head_addr) {
      // The qukey (at the head of the queue) was released. If it is a
      // SpaceCadet key, or if no rollover compensation is being used, we can
      // flush it now. Its state depends on whether or not it's a
      // SpaceCadet-type key.
      if (next_keypress_index == 0 || overlap_threshold_ == 0) {
        Key event_key = qukey_is_spacecadet ?
                        queue_head_.alternate_key : queue_head_.primary_key;
        flushEvent(event_key);
        return true;
      }
      // Now we know the qukey has been released, but we need to check to see if
      // it's release should continue to be delayed during rollover -- if the
      // subsequent key is released soon enough after the qukey is released, it
      // will meet the maximum overlap requirement to make the qukey take on its
      // alternate state.
      uint16_t overlap_start = event_queue_.timestamp(next_keypress_index);
      uint16_t overlap_end = event_queue_.timestamp(i);
      if (releaseDelayed(overlap_start, overlap_end)) {
        continue;
      }
      // The subsequent key was held long enough that the qukey can now be
      // flushed in its primary state. We're treating the rollover as normal
      // typing rollover, not deliberate chording.
      flushEvent(queue_head_.primary_key);
      return true;
    }

    // Event `i` is a key release of some other key than the qukey. Now we check
    // to see if it's also a key that was pressed subsequent to the press of the
    // qukey. We search from the next event after the qukey was pressed, and
    // stop when we get to the release event we're currently looking at.
    for (uint8_t j{1}; j < i; ++j) {
      // If we find an event with a matching KeyAddr, that means there are two
      // events for the same key in the queue after the qukey was pressed. Since
      // the second (or maybe third) event `i` is a key release, even if `j` is
      // not a key press, there must be one in the queue, so it shouldn't be
      // necessary to confirm that `j` is a actually a key press.
      if (event_queue_.addr(j) == event_queue_.addr(i)) {
        // Next, verify that enough time has passed after the qukey was pressed
        // to make it eligible for its alternate value. This helps faster
        // typists avoid unintended modifiers in the output.
        if (Runtime.hasTimeExpired(event_queue_.timestamp(0),
                                   minimum_hold_time_)) {
          flushEvent(queue_head_.alternate_key);
          return true;
        }
      }
    }
  }

  // Last, since our event queue is finite, we must make sure that there's
  // always room to add another event to the queue by flushing one whenever the
  // queue fills up. We could get multiple events in the same cycle, so this is
  // necessary to avoid reading and writing past the end of the array.
  if (event_queue_.isFull()) {
    flushEvent(queue_head_.primary_key);
    return true;
  }

  // If we got here, that means we're still waiting for an event (or a timeout)
  // that will determine the state of the qukey. We do know that the event at
  // the head of the queue is a qukey press, and that the `queue_head_.*_key`
  // values are valid. We return false to let the `beforeReportingState()` hook
  // check for hold timeout.
  return false;
}


// Flush one event from the head of the queue, with the specified Key value.
void Qukeys::flushEvent(Key event_key) {
  // First we record the address and state of the event:
  KeyAddr queue_head_addr = event_queue_.addr(0);
  uint8_t keyswitch_state = event_queue_.isRelease(0) ? WAS_PRESSED : IS_PRESSED;

  // Remove the head event from the queue:
  event_queue_.shift();
  // This ensures that the flushed event will be ignored by the event handler hook:
  flushing_queue_ = true;
  handleKeyswitchEvent(event_key, queue_head_addr, keyswitch_state);
  flushing_queue_ = false;
}


// Test if the key at address `k` is a qukey. As a side effect, if it is, cache
// that qukey's primary and alternate `Key` values for use later. We do this
// because it's much more efficient than doing that as a separate step.
bool Qukeys::isQukey(KeyAddr k) {
  // First, look up the value from the keymap. We need to do a full lookup, not
  // just looking up the cached value (i.e. `Layer.lookup(k)`), because the
  // cached value will be out of date if a layer change happened since the
  // keyswitch toggled on.
  Key key = Layer.lookupOnActiveLayer(k);

  // Next, we check to see if this is a DualUse-type qukey (defined in the keymap)
  if (isDualUseKey(key)) {
    return true;
  }

  // Last, we check the qukeys array for a match
  uint8_t layer_index = Layer.lookupActiveLayer(k);
  for (uint8_t i{0}; i < qukeys_count_; ++i) {
    Qukey qukey = cloneFromProgmem(qukeys_[i]);
    if (qukey.addr == k) {
      if ((qukey.layer == layer_index) ||
          (qukey.layer == layer_wildcard)) {
        queue_head_.primary_key = key;
        queue_head_.alternate_key = qukey.alternate_key;
        return true;
      }
    }
  }

  // If no matches were found, clear queue_head_ and return false
  queue_head_.primary_key = key;
  queue_head_.alternate_key = Key_Transparent;
  return false;
}

// Specific test for DualUse keys (in-keymap qukeys); this is a separate
// function because it gets called on its own when Qukeys is turned off. Like
// isQukey(), it sets `queue_head_.*` as a side effect.
bool Qukeys::isDualUseKey(Key key) {
  // Test for DualUse modifiers:
  if (key >= ranges::DUM_FIRST && key <= ranges::DUM_LAST) {
    key.setRaw(key.getRaw() - ranges::DUM_FIRST);

    queue_head_.primary_key = key;
    queue_head_.primary_key.setFlags(0);

    queue_head_.alternate_key.setRaw(key.getFlags() + Key_LeftControl.getKeyCode());
    return true;
  }
  // Test for DualUse layer shifts:
  if (key >= ranges::DUL_FIRST && key <= ranges::DUL_LAST) {
    key.setRaw(key.getRaw() - ranges::DUL_FIRST);

    queue_head_.primary_key = key;
    queue_head_.primary_key.setFlags(0);

    int8_t layer = key.getFlags();
    queue_head_.alternate_key = ShiftToLayer(layer);
    return true;
  }
  // It's not a DualUse Key:
  return false;
}


// Return true if the release of the qukey still needs to be delayed due to
// rollover. This is called when a qukey is released before a subsequent key,
// and that key is still being held. It checks to see if the subsequent key has
// been held long enough that the qukey should be flushed in its primary state
// (in which case we return `false`).
bool Qukeys::releaseDelayed(uint16_t overlap_start,
                            uint16_t overlap_end) const {
  // We want to calculate the timeout by dividing the overlap duration by the
  // percentage required to make the qukey take on its alternate state. Since
  // we're doing integer arithmetic, we need to first multiply by 100, then
  // divide by the percentage value (as an integer). We use 32-bit integers
  // here to make sure it doesn't overflow when we multiply by 100.
  uint32_t overlap_duration = overlap_end - overlap_start;
  uint32_t release_timeout = (overlap_duration * 100) / overlap_threshold_;
  return !Runtime.hasTimeExpired(overlap_start, uint16_t(release_timeout));
}


// Helper function for `beforeReportingState()`. In order to restore a key that
// has been physically released, but whose release event has not yet been
// flushed from the queue, we need to be able to search the queue for a given
// release event's corresponding press event, to add only those holds that
// should still be present.
bool Qukeys::isKeyAddrInQueueBeforeIndex(KeyAddr k, uint8_t index) const {
  for (uint8_t i{0}; i < index; ++i) {
    if (event_queue_.addr(i) == k) {
      return true;
    }
  }
  return false;
}


// -----------------------------------------------------------------------------

// This function could get lifted into Kaleidoscope proper, since it might be
// more generally useful. It's here to provide the test for a SpaceCadet-type
// qukey, which is any Qukey that has a modifier (including layer shifts) as its
// primary value.
bool isModifierKey(Key key) {
  // If it's a plain keyboard key, return true if its base keycode is a
  // modifier, otherwise return false:
  if ((key.getFlags() & (SYNTHETIC | RESERVED)) == 0) {
    return (key.getKeyCode() >= HID_KEYBOARD_FIRST_MODIFIER &&
            key.getKeyCode() <= HID_KEYBOARD_LAST_MODIFIER);
  }
  // If it's a layer shift key, return true:
  if (key.getFlags() == (SYNTHETIC | SWITCH_TO_KEYMAP) &&
      key.getKeyCode() >= LAYER_SHIFT_OFFSET) {
    return true;
  }
  // In all other cases, return false:
  return false;
}

} // namespace plugin {
} // namespace kaleidoscope {

kaleidoscope::plugin::Qukeys Qukeys;
