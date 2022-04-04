/* -*- mode: c++ -*-
 * Kaleidoscope-Qukeys -- Assign two keycodes to a single key
 * Copyright (C) 2017-2020  Michael Richters
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

#include <Arduino.h>                   // for F, __FlashStringHelper
#include <Kaleidoscope-FocusSerial.h>  // for Focus, FocusSerial
#include <Kaleidoscope-Ranges.h>       // for DUL_FIRST, DUM_FIRST, DUL_LAST, DUM_LAST

#include "kaleidoscope/KeyAddrEventQueue.h"  // for KeyAddrEventQueue
#include "kaleidoscope/KeyEvent.h"           // for KeyEvent
#include "kaleidoscope/KeyEventTracker.h"    // for KeyEventTracker
#include "kaleidoscope/Runtime.h"            // for Runtime, Runtime_
#include "kaleidoscope/keyswitch_state.h"    // for IS_PRESSED, WAS_PRESSED, keyIsInjected
#include "kaleidoscope/layers.h"             // for Layer, Layer_
#include "kaleidoscope/progmem_helpers.h"    // for cloneFromProgmem

namespace kaleidoscope {
namespace plugin {

EventHandlerResult Qukeys::onNameQuery() {
  return ::Focus.sendName(F("Qukeys"));
}

// This is the event handler. It ignores certain events, but mostly just adds
// them to the Qukeys event queue.
EventHandlerResult Qukeys::onKeyswitchEvent(KeyEvent &event) {
  // If the plugin has already processed and released this event, ignore it.
  // There's no need to update the event tracker explicitly.
  if (event_tracker_.shouldIgnore(event)) {
    // We should never get an event that's in our queue here, but just in case
    // some other plugin sends one, abort.
    if (event_queue_.shouldAbort(event))
      return EventHandlerResult::ABORT;
    return EventHandlerResult::OK;
  }

  // If event.addr is not a physical key, ignore it; some other plugin injected it.
  if (!event.addr.isValid() || keyIsInjected(event.state)) {
    return EventHandlerResult::OK;
  }

  // If Qukeys is turned off, continue to next plugin.
  if (!active_) {
    if (isDualUseKey(event.key)) {
      event.key = queue_head_.primary_key;
    }
    return EventHandlerResult::OK;
  }

  // If we can't trivially ignore the event, just add it to the queue.
  event_queue_.append(event);
  // In order to prevent overflowing the queue, process it now.
  while (processQueue())
    ;
  // Any event that gets added to the queue gets re-processed later, so we
  // need to abort processing now.
  return EventHandlerResult::ABORT;
}


// This hook runs once each cycle, and checks to see if the first event in the
// queue is ready to be flushed. It only allows one event to be flushed per
// cycle, because the keyboard HID report can't store all of the information
// necessary to correctly handle all of the rollover corner cases.
EventHandlerResult Qukeys::afterEachCycle() {
  // If there hasn't been a keypress in a while, update the prior keypress
  // timestamp to avoid integer overflow issues:
  if (Runtime.hasTimeExpired(prior_keypress_timestamp_,
                             minimum_prior_interval_)) {
    prior_keypress_timestamp_ =
      Runtime.millisAtCycleStart() - (minimum_prior_interval_ + 1);
  }

  // If there's nothing in the queue, there's nothing more to do.
  if (event_queue_.isEmpty()) {
    return EventHandlerResult::OK;
  }

  // If we get here, that means that the first event in the queue is a qukey
  // press. All that's left to do is to check if it's been held long enough that
  // it has timed out.
  if (Runtime.hasTimeExpired(event_queue_.timestamp(0), hold_timeout_)) {
    // If it's a SpaceCadet-type key, it takes on its primary value, otherwise
    // it takes on its secondary value.
    Key event_key = isModifierKey(queue_head_.primary_key) ? queue_head_.primary_key : queue_head_.alternate_key;
    flushEvent(event_key);
  }

  // Process as many events as we can from the queue.
  while (processQueue())
    ;

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
  // If there's nothing in the queue, abort.
  if (event_queue_.isEmpty()) {
    return false;
  }

  // In other cases, we will want the KeyAddr of the first event in the queue.
  KeyAddr queue_head_addr = event_queue_.addr(0);

  // If that first event is a key release, it can be flushed right away.
  if (event_queue_.isRelease(0)) {
    // We can't unconditionally flush the release event, because it might be
    // second half of a tap-repeat event. If the queue is full, we won't bother to
    // check, but otherwise, ift `tap_repeat_.addr` is set (and matches), we call
    // `shouldWaitForTapRepeat()` to determine whether or not to flush the key
    // release event.
    if (event_queue_.isFull() ||
        queue_head_addr != tap_repeat_.addr ||
        !shouldWaitForTapRepeat()) {
      flushEvent(Key_NoKey);
      return true;
    }
    // We now know that we're waiting to determine if we're getting a tap-repeat
    // sequence, so we can't flush the release event at the head of the queue.
    // Warning: Returning false here is only okay because we already checked to
    // make sure the queue isn't full.
    return false;
  }

  // We now know that the first event is a key press. If it's not a qukey, or if
  // it's only there because the plugin was just turned off, we can flush it
  // immediately.
  // Should be able to remove the `active_` check once `deactivate()` gets updated
  if (!isQukey(queue_head_addr) || !active_) {
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

  // If the qukey press followed a printable key too closely, it's not eligible
  // to take on its alternate value unless it's a SpaceCadet-type key.
  if (!Runtime.hasTimeExpired(prior_keypress_timestamp_,
                              minimum_prior_interval_) &&
      !qukey_is_spacecadet) {
    flushEvent(queue_head_.primary_key);
  }

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
        Key event_key = qukey_is_spacecadet ? queue_head_.alternate_key : queue_head_.primary_key;
        // A qukey just got released in primary state; this might turn out to be
        // the beginning of a tap-repeat sequence, so we set the tap-repeat
        // address and start time to the time of the initial press event before
        // flushing it from the queue. This will come into play when processing
        // the corresponding release event later.
        tap_repeat_.addr       = queue_head_addr;
        tap_repeat_.start_time = event_queue_.timestamp(0);
        flushEvent(event_key);
        return true;
      }
      // Now we know the qukey has been released, but we need to check to see if
      // it's release should continue to be delayed during rollover -- if the
      // subsequent key is released soon enough after the qukey is released, it
      // will meet the maximum overlap requirement to make the qukey take on its
      // alternate state.
      uint16_t overlap_start = event_queue_.timestamp(next_keypress_index);
      uint16_t overlap_end   = event_queue_.timestamp(i);
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
  KeyEvent event{queue_head_addr, keyswitch_state, event_key, event_queue_.id(0)};

  // If the flushed event is a keypress of a printable symbol, record its
  // timestamp. This lets us suppress some unintended alternate values seen by
  // fast typists by requiring a minimum interval between this keypress and the
  // next qukey press in order for that qukey to become alternate-eligible.
  if (!event_queue_.isRelease(0) &&
      ((event_key >= Key_A && event_key <= Key_0) ||
       (event_key >= Key_Minus && event_key <= Key_Slash))) {
    prior_keypress_timestamp_ = event_queue_.timestamp(0);
  }

  // Remove the head event from the queue, then call `handleKeyswitchEvent()` to
  // resume processing of the event. It's important to remove the event from the
  // queue first; otherwise `onKeyswitchEvent()` will abort it.
  event_queue_.shift();
  Runtime.handleKeyswitchEvent(event);
}


// Test if the key at address `k` is a qukey. As a side effect, if it is, cache
// that qukey's primary and alternate `Key` values for use later. We do this
// because it's much more efficient than doing that as a separate step.
bool Qukeys::isQukey(KeyAddr k) {
  // First, look up the value from the keymap. This value should be
  // correct in the cache, even if there's been a layer change since
  // the key was pressed.
  Key key = Runtime.lookupKey(k);

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
        queue_head_.primary_key   = key;
        queue_head_.alternate_key = qukey.alternate_key;
        return true;
      }
    }
  }

  // If no matches were found, clear queue_head_ and return false
  queue_head_.primary_key   = key;
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

    int8_t layer              = key.getFlags();
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
  uint32_t release_timeout  = (overlap_duration * 100) / overlap_threshold_;
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


// This question gets called early in `processQueue()` if a key release event is
// at the head of the queue, and the `tap_repeat_.addr` is the same as that
// event's KeyAddr. It returns true if `processQueue()` should wait for either
// subsequent events or a timeout instead of proceeding to flush the key release
// event immediately, and false if it is still waiting. It assumes that
// `event_queue_[0]` is a release event, and that `event_queue_[0].addr ==
// tap_repeat_.addr`. (The latter should only be set to a valid KeyAddr if a qukey
// press event has been flushed with its primary Key value, and could still
// represent the start of a double-tap or tap-repeat sequeunce.)
bool Qukeys::shouldWaitForTapRepeat() {
  // First, we set up a variable to store the queue index of a subsequent press
  // of the same qukey addr, if any.
  uint8_t second_press_index = 0;

  // Next, we search the event queue (starting at index 1 because the first
  // event in the queue is known), trying to find a matching sequeunce for
  // either a double-tap, or a tap-repeat.
  for (uint8_t i{1}; i < event_queue_.length(); ++i) {
    if (event_queue_.isPress(i)) {
      // Found a keypress event following the release of the initial primary
      // qukey.
      if (event_queue_.addr(i) == tap_repeat_.addr) {
        // The same qukey toggled on twice in a row, and because of the timeout
        // check below, we know it was quick enough that it could represent a
        // tap-repeat sequence. Now we update the start time (which had been set
        // to the timestamp of the first press event) to the timestamp of the
        // first release event (currently at the head of the queue), because we
        // want to compare the release times of the two taps to determine if
        // it's actually a double-tap sequence instead (otherwise it could be
        // too difficult to tap it fast enough).
        tap_repeat_.start_time = event_queue_.timestamp(0);
        // We also record the index of this second press event. If it turns out
        // that we've got a tap-repeat sequence, we want to silently suppress the
        // first release and second press by removing them from the queue
        // without flushing them. We don't know yet whether we'll be doing so.
        second_press_index = i;
      } else {
        // Some other key was pressed. For it to be a tap-repeat sequence, we
        // require that the same key be pressed twice in a row, with no
        // intervening presses of other keys. Therefore, we can return false to
        // signal that the release event at the head of the queue can be
        // flushed.
        return false;
      }

    } else if (event_queue_.addr(i) == tap_repeat_.addr) {
      // We've found a key release event in the queue, and it's the same key as
      // the qukey at the head of the queue, so this is the second release that
      // has occurred before timing out (see below for the timeout
      // check). Therefore, this is a double-tap sequence (the second release
      // happened very close to the first release), not a tap-repeat, so we
      // clear the tap-repeat address, and return false to trigger the flush of
      // the first release event.
      tap_repeat_.addr = KeyAddr{KeyAddr::invalid_state};
      return false;
    }
  }

  // We've now searched the queue. Either we found only irrelevant key release
  // events (for other keys that were pressed before the sequence began), or we
  // found only a second key press event of the same qukey (but not a double
  // tap). Next, we check the timeout. If we didn't find a second press event,
  // the start time will still be that of the initial key press event (already
  // flushed from the queue), but if the second press has been detected, the
  // start time will be that of the key release event currently at the head of
  // the queue.
  if (Runtime.hasTimeExpired(tap_repeat_.start_time, tap_repeat_.timeout)) {
    // Time has expired. The sequence represents either a single tap or a
    // tap-repeat of the qukey's primary value. Either way, we can clear the
    // stored address.
    tap_repeat_.addr = KeyAddr{KeyAddr::invalid_state};

    if (second_press_index > 0) {
      // A second press was found (but it's release didn't come quick enough to
      // be a double tap), so this is a tap-repeat event. To turn it into a
      // single key press and hold, we need to remove the second press event and
      // the first release event from the queue without flushing the
      // events. Order matters here!
      event_queue_.remove(second_press_index);
      event_queue_.remove(0);
    } else {
      // The key was not pressed again, so the single tap has timed out. We
      // return false to let the release event be flushed.
      return false;
    }
  }

  // We haven't found a double-tap sequence, and the timeout hasn't expired, so
  // we return true to signal that we should just wait until we get another
  // event, or until time runs out.
  return true;
}


// -----------------------------------------------------------------------------

// This function is here to provide the test for a SpaceCadet-type qukey, which
// is any Qukey that has a modifier (including layer shifts) as its primary
// value.
bool isModifierKey(Key key) {
  return (key.isKeyboardModifier() || key.isLayerShift());
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::Qukeys Qukeys;
