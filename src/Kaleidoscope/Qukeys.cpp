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

#include <Kaleidoscope-Qukeys.h>
#include <kaleidoscope/hid.h>

#ifdef ARDUINO_VIRTUAL
#define debug_print(...) printf(__VA_ARGS__)
#else
#define debug_print(...)
#endif


namespace kaleidoscope {

Qukey::Qukey(int8_t layer, byte row, byte col, Key alt_keycode) {
  this->layer = layer;
  this->addr = addr::addr(row, col);
  this->alt_keycode = alt_keycode;
  this->state = QUKEY_STATE_UNDETERMINED;
}

Qukeys::Qukeys() {}

Qukeys::init(Qukey *qukeys, uint8_t qukeys_count) {
  qukeys_ = qukeys;
  qukeys_count_ = qukeys_count;
}

int8_t Qukeys::lookupQukey(uint8_t key_addr) {
  if (key_addr == QUKEY_UNKNOWN_ADDR)
    return QUKEY_NOT_FOUND;
  for (int8_t i; i < qukeys_count_; i++) {
    Qukey qukey = qukeys_[i];
    if (qukey.addr == key_addr) {
      byte row = addr::row(key_addr);
      byte col = addr::col(key_addr);
      if ((qukey.layer == QUKEY_ALL_LAYERS) ||
          (qukey.layer == Layer.lookupActiveLayer(row, col))) {
        return i;
      }
    }
  }
  return QUKEY_NOT_FOUND;
}

void Qukeys::enqueue(uint8_t key_addr) {
  if (key_queue_length_ == QUKEYS_QUEUE_MAX) {
    flushKey(QUKEY_STATE_PRIMARY);
  }
  key_queue_[key_queue_length_].addr = key_addr;
  key_queue_[key_queue_length_].flush_time = millis() + time_limit_;
  key_queue_length_++;
}

int8_t Qukeys::searchQueue(uint8_t key_addr) {
  for (int8_t i = 0; i < key_queue_length_; i++) {
    if (key_queue_[i].addr == key_addr)
      return i;
  }
  return -1;
}

// flush a single entry from the head of the queue
void Qukeys::flushKey(int8_t state) {
  int8_t qukey_index = Qukeys.lookupQukey(key_addr);
  if (qukey_index != QUKEY_NOT_FOUND) {
    qukeys_[qukey_index].state = state;
  }
  byte row = addr::row(key_queue[0].addr);
  byte col = addr::col(key_queue[0].addr);
  Key keycode = Key_NoKey;
  if (state == QUKEY_STATE_ALTERNATE && qukey_index != QUKEY_NOT_FOUND) {
    keycode = qukeys_[qukey_index].alt_keycode;
  } else {
    keycode = Layer.lookup(row, col);
  }
  // Since we're in the middle of the key scan, we don't necessarily
  // have a full HID report, and we don't want to accidentally turn
  // off keys that the scan hasn't reached yet, so we force the
  // current report to be the same as the previous one, then proceed
  hid::copyPrevKeyboardReport();
  // Instead of just calling pressKey here, we start processing the
  // key again, as if it was just pressed, and mark it as injected, so
  // we can ignore it and don't start an infinite loop. It would be
  // nice if we could use key_state to also indicate which plugin
  // injected the key.
  handleKeySwitchEvent(keycode, row, col, IS_PRESSED | INJECTED);
  // Now we send the report (if there were any changes)
  hid::sendKeyboardReport();

  // Shift the queue, so key_queue[0] is always the first key that gets processed
  for (byte i = 0; i < key_queue_length_; i++) {
    key_queue_[i] = key_queue_[i + 1];
  }
  key_queue_length_--;
}

void Qukeys::flushQueue(int8_t state, int8_t index) {
  for (int8_t i = 0; i <= index; i++) {
    if (key_queue_length_ == 0)
      break;
    flushKey(state);
  }
}

Key Qukeys::keyScanHook(Key mapped_key, byte row, byte col, uint8_t key_state) {

  // If Qukeys is turned off, continue to next plugin
  if (!active)
    return mapped_key;

  // If the key was injected (from the queue being flushed), continue to next plugin
  if (key_state & INJECTED)
    return mapped_key;

  // If the key isn't active, and didn't just toggle off, continue to next plugin
  if (!keyIsPressed(key_state) && !keyWasPressed(key_state))
    return mapped_key;

  // get key addr & qukey (if any)
  uint8_t key_addr = addr::addr(row, col);
  int8_t qukey_index = Qukeys.lookupQukey(key_addr);

  // If the key was just pressed:
  if (keyToggledOn(key_state)) {
    // I think I may need to call maskKey() somewhere here, but I'm not sure
    if (key_queue_length) {
      enqueue(key_addr);
    } else {
      if (qukey_index == QUKEY_NOT_FOUND)
        return mapped_key;
      enqueue(key_addr);
    }
  }

  // In all other cases, we need to know if the key is queued already
  int8_t queue_index = searchQueue(key_addr);

  // If the key was just released:
  if (keyToggledOff(key_state)) {
    if (queue_index == QUKEY_NOT_FOUND)
      return mapped_key;
    flushQueue(QUKEY_STATE_ALTERNATE, queue_index);
    return Key_NoKey;
  }

  // Otherwise, the key is still pressed

  // If the key is not a qukey:
  if (qukey_index == QUKEY_NOT_FOUND) {
    // If the key was pressed before the keys in the queue:
    if (queue_index == QUKEY_NOT_FOUND) {
      return mapped_key;
    } else {
      // suppress this keypress; it's still in the queue
      return Key_NoKey;
    }
  }

  // qukeys that have already decided their keycode
  if (qukeys_[qukey_index].state == QUKEY_STATE_PRIMARY)
    return mapped_key;
  if (qukeys_[qukey_index].state == QUKEY_STATE_ALTERNATE)
    return qukeys_[qukey_index].alt_keycode;
  // else state is undetermined; block. I could check timeouts here,
  // but I'd rather do that in the pre-report hook
  return Key_NoKey;
}

void preReportHook(void) {
  // If the qukey has been held longer than the time limit, set its
  // state to the alternate keycode and add it to the report
  uint32_t current_time = millis();
  for (int8_t i = 0; i < key_queue_length_; i++) {
    if (current_time > key_queue_[i].flush_time) {
      flushKey(QUKEY_STATE_ALTERNATE);
    } else {
      break;
    }
  }
}

void loopHook(bool post_clear) {
  if (!post_clear)
    return preReportHook();
}

void Qukeys::begin() {
  // initializing the key_queue seems unnecessary, actually
  for (int8_t i = 0; i < QUKEYS_QUEUE_MAX; i++) {
    key_queue_[i].addr = QUKEY_UNKNOWN_ADDR;
    key_queue_[i].flush_time = 0;
  }
  key_queue_length_ = 0;

  Kaleidoscope.useEventHandlerHook(keyScanHook);
  Kaleidoscope.useLoopHook(loopHook);
}

} // namespace kaleidoscope {

kaleidoscope::Qukeys Qukeys;
