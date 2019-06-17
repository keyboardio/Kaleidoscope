/* -*- mode: c++ -*-
 * Kaleidoscope-Qukeys -- Assign two keycodes to a single key
 * Copyright (C) 2017  Michael Richters
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

#include <Kaleidoscope.h>
#include <Kaleidoscope-Qukeys.h>
#include <kaleidoscope/hid.h>
#include <MultiReport/Keyboard.h>
#include <Kaleidoscope-Ranges.h>

#ifdef KALEIDOSCOPE_VIRTUAL_BUILD
#define debug_print(...) printf(__VA_ARGS__)
#else
#define debug_print(...)
#endif


namespace kaleidoscope {
namespace plugin {

inline
bool isDualUse(Key k) {
  if (k.raw < ranges::DU_FIRST || k.raw > ranges::DU_LAST)
    return false;
  return true;
}

inline
bool isDualUse(byte key_addr) {
  byte row = addr::row(key_addr);
  byte col = addr::col(key_addr);
  Key k = Layer.lookup(row, col);
  return isDualUse(k);
}

Key getDualUsePrimaryKey(Key k) {
  if (k.raw >= ranges::DUM_FIRST && k.raw <= ranges::DUM_LAST) {
    k.raw -= ranges::DUM_FIRST;
    k.flags = 0;
  } else if (k.raw >= ranges::DUL_FIRST && k.raw <= ranges::DUL_LAST) {
    k.raw -= ranges::DUL_FIRST;
    k.flags = 0;
  }
  return k;
}

Key getDualUseAlternateKey(Key k) {
  if (k.raw >= ranges::DUM_FIRST && k.raw <= ranges::DUM_LAST) {
    k.raw -= ranges::DUM_FIRST;
    k.raw = (k.raw >> 8) + Key_LeftControl.keyCode;
  } else if (k.raw >= ranges::DUL_FIRST && k.raw <= ranges::DUL_LAST) {
    k.raw -= ranges::DUL_FIRST;
    byte layer = k.flags;
    // Should be `ShiftToLayer(layer)`, but that gives "narrowing conversion"
    // warnings that I can't figure out how to resolve
    k.keyCode = layer + LAYER_SHIFT_OFFSET;
    k.flags = KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP;
  }
  return k;
}


Qukey::Qukey(int8_t layer, byte row, byte col, Key alt_keycode) {
  this->layer = layer;
  this->addr = addr::addr(row, col);
  this->alt_keycode = alt_keycode;
}

Qukey * Qukeys::qukeys;
uint8_t Qukeys::qukeys_count = 0;

bool Qukeys::active_ = true;
uint16_t Qukeys::time_limit_ = 250;
uint8_t Qukeys::release_delay_ = 0;
QueueItem Qukeys::key_queue_[] = {};
uint8_t Qukeys::key_queue_length_ = 0;
bool Qukeys::flushing_queue_ = false;
uint8_t Qukeys::delayed_qukey_addr_ = QUKEY_UNKNOWN_ADDR;
int16_t Qukeys::delayed_qukey_start_time_ = 0;

constexpr uint16_t QUKEYS_RELEASE_DELAY_OFFSET = 4096;

// Empty constructor; nothing is stored at the instance level
Qukeys::Qukeys(void) {}

int8_t Qukeys::lookupQukey(uint8_t key_addr) {
  if (key_addr == QUKEY_UNKNOWN_ADDR) {
    return QUKEY_NOT_FOUND;
  }
  for (int8_t i = 0; i < qukeys_count; i++) {
    if (qukeys[i].addr == key_addr) {
      byte row = addr::row(key_addr);
      byte col = addr::col(key_addr);
      if ((qukeys[i].layer == QUKEY_ALL_LAYERS) ||
          (qukeys[i].layer == Layer.lookupActiveLayer(row, col))) {
        return i;
      }
    }
  }
  return QUKEY_NOT_FOUND;
}

void Qukeys::enqueue(uint8_t key_addr) {
  if (key_queue_length_ == QUKEYS_QUEUE_MAX) {
    flushKey(QUKEY_STATE_PRIMARY, IS_PRESSED | WAS_PRESSED);
    flushQueue();
  }
  // default to alternate state to stop keys being flushed from the queue before the grace
  // period timeout
  key_queue_[key_queue_length_].addr = key_addr;
  key_queue_[key_queue_length_].start_time = millis();
  key_queue_length_++;
}

int8_t Qukeys::searchQueue(uint8_t key_addr) {
  for (int8_t i = 0; i < key_queue_length_; i++) {
    if (key_queue_[i].addr == key_addr)
      return i;
  }
  return QUKEY_NOT_FOUND;
}

// flush a single entry from the head of the queue
bool Qukeys::flushKey(bool qukey_state, uint8_t keyswitch_state) {
  int8_t qukey_index = lookupQukey(key_queue_[0].addr);
  bool is_qukey = (qukey_index != QUKEY_NOT_FOUND);
  byte row = addr::row(key_queue_[0].addr);
  byte col = addr::col(key_queue_[0].addr);
  Key keycode = Layer.lookupOnActiveLayer(row, col);
  bool is_dual_use = isDualUse(keycode);
  if (is_qukey || is_dual_use) {
    if (qukey_state == QUKEY_STATE_PRIMARY) {
      // If there's a release delay in effect, and there's at least one key after it in
      // the queue, delay this key's release event:
      if (release_delay_ > 0 && key_queue_length_ > 1
          && delayed_qukey_addr_ == QUKEY_UNKNOWN_ADDR) {
        delayed_qukey_start_time_ = millis();
        // Store the delayed key's address to send the toggle-off event later, if
        // appropriate:
        delayed_qukey_addr_ = key_queue_[0].addr;
        return false;
      }
      keycode = getDualUsePrimaryKey(keycode);
    }
    if (qukey_state == QUKEY_STATE_ALTERNATE) {
      if (is_dual_use) {
        keycode = getDualUseAlternateKey(keycode);
      } else { // is_qukey
        keycode = qukeys[qukey_index].alt_keycode;
      }
    }
  }

  // Before calling handleKeyswitchEvent() below, make sure Qukeys knows not to handle
  // these events:
  flushing_queue_ = true;

  // Since we're in the middle of the key scan, we don't necessarily
  // have a full HID report, and we don't want to accidentally turn
  // off keys that the scan hasn't reached yet, so we force the
  // current report to be the same as the previous one, then proceed
  HID_KeyboardReport_Data_t curr_hid_report;
  // First, save the current report & previous report's modifiers
  memcpy(&curr_hid_report, &Keyboard.keyReport, sizeof(curr_hid_report));
  byte prev_hid_report_modifiers = Keyboard.lastKeyReport.modifiers;
  // Next, copy the old report
  memcpy(&Keyboard.keyReport, &Keyboard.lastKeyReport, sizeof(Keyboard.keyReport));
  // Instead of just calling pressKey here, we start processing the
  // key again, as if it was just pressed, and mark it as injected, so
  // we can ignore it and don't start an infinite loop. It would be
  // nice if we could use key_state to also indicate which plugin
  // injected the key.
  handleKeyswitchEvent(keycode, row, col, IS_PRESSED);
  // Now we send the report (if there were any changes)
  hid::sendKeyboardReport();

  // Next, we restore the current state of the report
  memcpy(&Keyboard.keyReport, &curr_hid_report, sizeof(curr_hid_report));

  // Last, if the key is still down, add its code back in
  if (keyswitch_state & IS_PRESSED) {
    handleKeyswitchEvent(keycode, row, col, IS_PRESSED | WAS_PRESSED);
  } else {
    // If this is the key that was released, send that release event now
    handleKeyswitchEvent(Key_NoKey, row, col, WAS_PRESSED);
    // ...and if there's another key in the queue that's about to also be
    // flushed, we need to do something to clear this one's modifier flags (if
    // any) from the previous report
    if (key_queue_length_ > 1) {
      // Restore the previous report; whatever was added by this key flush
      // should not appear in the next one, because this key has now been
      // released.  This is necessary to handle the case where a qukey's primary
      // key value has a modifier flag.  Because we copy the last report
      // directly, we're bypassing the mod-flag rollover protection offered by
      // the HIDAdapter.  Unfortunately, this does not help if we're rolling
      // over multiple keys, and one of the unreleased ones has a mod flag.
      // That's probably rare enough that it won't be noticed, however.  THIS IS
      // AN UGLY HACK, AND IT SHOULD BE FIXED WITH SOMETHING BETTER EVENTUALLY.
      // Doing it right will most likely involve either major changes in
      // KeyboardioHID or Kaleidoscope itself.
      Keyboard.lastKeyReport.modifiers = prev_hid_report_modifiers;
    }
  }

  // Now that we're done sending the report(s), Qukeys can process events again:
  flushing_queue_ = false;

  // Shift the queue, so key_queue[0] is always the first key that gets processed
  for (byte i = 0; i < key_queue_length_; i++) {
    key_queue_[i] = key_queue_[i + 1];
  }
  key_queue_length_--;
  return true;
}

// flushQueue() is called when a key that's in the key_queue is
// released. This means that all the keys ahead of it in the queue are
// still being held, so first we flush them, then we flush the
// released key (with different parameters).
void Qukeys::flushQueue(int8_t index) {
  if (index == QUKEY_NOT_FOUND)
    return;
  for (int8_t i = 0; i < index; i++) {
    if (key_queue_length_ == 0)
      return;
    flushKey(QUKEY_STATE_ALTERNATE, IS_PRESSED | WAS_PRESSED);
  }
  flushKey(QUKEY_STATE_PRIMARY, WAS_PRESSED);
}

// Flush all the non-qukey keys from the front of the queue
void Qukeys::flushQueue() {
  // flush keys until we find a qukey:
  while (key_queue_length_ > 0 && !isQukey(key_queue_[0].addr)) {
    if (flushKey(QUKEY_STATE_PRIMARY, IS_PRESSED | WAS_PRESSED) == false)
      break;
  }
}

inline
bool Qukeys::isQukey(uint8_t addr) {
  return (isDualUse(addr) || lookupQukey(addr) != QUKEY_NOT_FOUND);
}

EventHandlerResult Qukeys::onKeyswitchEvent(Key &mapped_key, byte row, byte col, uint8_t key_state) {

  // If key_addr is not a physical key, ignore it; some other plugin injected it
  if (row >= ROWS || col >= COLS || (key_state & INJECTED) != 0)
    return EventHandlerResult::OK;

  // If Qukeys is turned off, continue to next plugin
  if (!active_) {
    mapped_key = getDualUsePrimaryKey(mapped_key);
    return EventHandlerResult::OK;
  }

  // get key addr & qukey (if any)
  uint8_t key_addr = addr::addr(row, col);
  int8_t qukey_index = lookupQukey(key_addr);

  // If the key was injected (from the queue being flushed)
  if (flushing_queue_) {
    return EventHandlerResult::OK;
  }

  // If the key was just pressed:
  if (keyToggledOn(key_state)) {
    // If the queue is empty and the key isn't a qukey, proceed:
    if (key_queue_length_ == 0 &&
        ! isDualUse(mapped_key) &&
        qukey_index == QUKEY_NOT_FOUND) {
      return EventHandlerResult::OK;
    }

    // Otherwise, queue the key and stop processing:
    enqueue(key_addr);
    // flushQueue() has already handled this key release
    return EventHandlerResult::EVENT_CONSUMED;
  }

  // In all other cases, we need to know if the key is queued already
  int8_t queue_index = searchQueue(key_addr);

  // If the key was just released:
  if (keyToggledOff(key_state)) {
    // If the key isn't in the key_queue, proceed
    if (queue_index == QUKEY_NOT_FOUND) {
      return EventHandlerResult::OK;
    }
    // Finally, send the release event of the delayed qukey, if any. This is necessary in
    // order to send a toggle off of a `ShiftToLayer()` key; otherwise, that layer gets
    // stuck on if there's a release delay and a rollover.
    if (delayed_qukey_addr_ != QUKEY_UNKNOWN_ADDR) {
      int8_t r = addr::row(delayed_qukey_addr_);
      int8_t c = addr::col(delayed_qukey_addr_);
      flushQueue(queue_index);
      flushQueue();
      flushing_queue_ = true;
      handleKeyswitchEvent(Key_NoKey, r, c, WAS_PRESSED);
      flushing_queue_ = false;
      delayed_qukey_addr_ = QUKEY_UNKNOWN_ADDR;
    } else {
      flushQueue(queue_index);
      flushQueue();
    }
    //if (delayed_qukey_addr_ != QUKEY_UNKNOWN_ADDR)
    //  return EventHandlerResult::EVENT_CONSUMED;
    mapped_key = getDualUsePrimaryKey(mapped_key);
    return EventHandlerResult::OK;
  }

  // Otherwise, the key is still pressed

  // Only keys in the queue can still evaluate as qukeys, so all we need to do here is
  // block events for held keys that are still in the queue.
  if (queue_index == QUKEY_NOT_FOUND) {
    // The key is not in the queue; proceed:
    return EventHandlerResult::OK;
  } else {
    // The key is still in the queue; abort:
    return EventHandlerResult::EVENT_CONSUMED;
  }
}

EventHandlerResult Qukeys::beforeReportingState() {

  uint16_t current_time = millis();

  if (delayed_qukey_addr_ != QUKEY_UNKNOWN_ADDR) {
    int16_t diff_time = current_time - delayed_qukey_start_time_;
    if (diff_time > release_delay_) {
      flushKey(QUKEY_STATE_PRIMARY, WAS_PRESSED);
      flushQueue();
      // If the release delay has timed out, we need to prevent the wrong toggle-off
      // event from being sent:
      delayed_qukey_addr_ = QUKEY_UNKNOWN_ADDR;
    }
    return EventHandlerResult::OK;
  }

  // If the qukey has been held longer than the time limit, set its
  // state to the alternate keycode and add it to the report
  while (key_queue_length_ > 0) {
    if ((current_time - key_queue_[0].start_time) > time_limit_) {
      flushKey(QUKEY_STATE_ALTERNATE, IS_PRESSED | WAS_PRESSED);
      flushQueue();
    } else {
      break;
    }
  }

  return EventHandlerResult::OK;
}

EventHandlerResult Qukeys::onSetup() {
  // initializing the key_queue seems unnecessary, actually
  for (int8_t i = 0; i < QUKEYS_QUEUE_MAX; i++) {
    key_queue_[i].addr = QUKEY_UNKNOWN_ADDR;
    key_queue_[i].start_time = 0;
  }
  key_queue_length_ = 0;

  return EventHandlerResult::OK;
}

} // namespace plugin {
} // namespace kaleidoscope {

kaleidoscope::plugin::Qukeys Qukeys;
