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

#ifdef ARDUINO_VIRTUAL
#define debug_print(...) printf(__VA_ARGS__)
#else
#define debug_print(...)
#endif


namespace kaleidoscope {
namespace plugin {

constexpr uint16_t QUKEYS_RELEASE_DELAY_OFFSET = 4096;

Qukey * Qukeys::qukeys;
uint8_t Qukeys::qukeys_count{0};

bool Qukeys::active_{true};
uint16_t Qukeys::time_limit_{250};
uint8_t Qukeys::release_delay_{0};
QueueItem Qukeys::key_queue_[] = {};
uint8_t Qukeys::key_queue_length_{0};
uint8_t Qukeys::key_queue_release_bitfield_{0};
uint8_t Qukeys::delayed_qukey_addr_ = QUKEY_UNKNOWN_ADDR;
int16_t Qukeys::delayed_qukey_start_time_ = 0;
bool Qukeys::flushing_queue_{false};
bool Qukeys::delayed_qukey_{false};

inline
bool isDualUse(Key key) {
  if (key.raw < ranges::DU_FIRST || key.raw > ranges::DU_LAST)
    return false;
  return true;
}

inline
bool isDualUse(KeyAddr key_addr) {
  Key key = Layer.lookup(key_addr);
  return isDualUse(key);
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


Qukey::Qukey(int8_t layer, KeyAddr key_addr, Key alt_keycode) {
  this->layer = layer;
  this->addr = key_addr;
  this->alt_keycode = alt_keycode;
}

// Empty constructor; nothing is stored at the instance level
Qukeys::Qukeys(void) {}

int8_t Qukeys::lookupQukey(KeyAddr k) {
  if (k.toInt() == QUKEY_UNKNOWN_ADDR) {
    return QUKEY_NOT_FOUND;
  }
  for (int8_t i{0}; i < qukeys_count; ++i) {
    if (qukeys[i].addr == k) {
      if ((qukeys[i].layer == QUKEY_ALL_LAYERS) ||
          (qukeys[i].layer == Layer.lookupActiveLayer(k))) {
        return i;
      }
    }
  }
  return QUKEY_NOT_FOUND;
}

void Qukeys::enqueue(KeyAddr key_addr) {
  // If the queue is already full, flush the first key to make room. That qukey
  // ends up in its primary state, because someone is mashing the keys, and
  // that's less likely to cause them problems.
  if (key_queue_length_ == QUKEYS_QUEUE_MAX) {
    flushKey(QUKEY_STATE_PRIMARY);
  }

  // Add the new key to the queue:
  key_queue_[key_queue_length_].addr = key_addr;
  key_queue_[key_queue_length_].start_time = Kaleidoscope.millisAtCycleStart();
  ++key_queue_length_;
}

int8_t Qukeys::searchQueue(KeyAddr key_addr) {
  for (int8_t i{0}; i < key_queue_length_; ++i) {
    if (key_queue_[i].addr == key_addr)
      return i;
  }
  return QUKEY_NOT_FOUND;
}

// Flush a single entry from the head of the queue, and send a keypress event
// for that key.
void Qukeys::flushKey(bool qukey_state_alternate) {

  KeyAddr k{key_queue_[0].addr};
  int8_t qukey_index = lookupQukey(k);
  bool is_qukey = (qukey_index != QUKEY_NOT_FOUND);
  Key key = Layer.lookupOnActiveLayer(k);
  bool is_dual_use = isDualUse(key);

  if (is_qukey || is_dual_use) {
    if (! qukey_state_alternate) {
      key = getDualUsePrimaryKey(key);
    } else {
      if (is_dual_use) {
        key = getDualUseAlternateKey(key);
      } else { // is_qukey
        key = qukeys[qukey_index].alt_keycode;
      }
    }
  }

  // Before calling handleKeyswitchEvent() below, make sure Qukeys knows not to
  // handle these events:
  flushing_queue_ = true;

  // Instead of just calling pressKey here, we start processing the key again,
  // as if it was just pressed.
  handleKeyswitchEvent(key, k, IS_PRESSED);

  // Now that we're done sending the keypress, Qukeys can process events again:
  flushing_queue_ = false;

  // Shift the queue, so key_queue[0] is always the first key that gets processed
  for (uint8_t i{0}; i < key_queue_length_; ++i) {
    key_queue_[i] = key_queue_[i + 1];
  }
  --key_queue_length_;

  // Shift the released bits, too, to keep them in sync with the queue:
  key_queue_release_bitfield_ >>= 1;

  // Only the first item in the queue can be delayed, so when it's flushed,
  // there's no more delay:
  delayed_qukey_ = false;
}

#if 0
inline
void Qukeys::shiftQueue() {
  // Shift the queue, so key_queue[0] is always the first key that gets processed
  for (uint8_t i{0}; i < key_queue_length_; ++i) {
    key_queue_[i] = key_queue_[i + 1];
  }
  --key_queue_length_;

  // Shift the released bits, too, to keep them in sync with the queue:
  key_queue_release_bitfield_ >>= 1;
}
#endif

inline
bool Qukeys::isQukey(KeyAddr addr) {
  return (isDualUse(addr) || lookupQukey(addr) != QUKEY_NOT_FOUND);
}

EventHandlerResult Qukeys::onKeyswitchEvent(Key &mapped_key,
                                            KeyAddr key_addr,
                                            uint8_t key_state) {

  // If key_addr is not a physical key, ignore it; some other plugin injected it
  if (!key_addr.isValid() || (key_state & INJECTED) != 0)
    return EventHandlerResult::OK;

  // If the key was injected (from the queue being flushed)
  if (flushing_queue_) {
    return EventHandlerResult::OK;
  }

  // If Qukeys is turned off, continue to next plugin
  if (!active_) {
    mapped_key = getDualUsePrimaryKey(mapped_key);
    return EventHandlerResult::OK;
  }

  // get key addr & qukey (if any)
  int8_t qukey_index = lookupQukey(key_addr);

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

    // Set this key's bit in the released keys bitfield:
    bitSet(key_queue_release_bitfield_, queue_index);

    // Stop processing; this release event will finish in the next hook
    // function, before the report is sent:
    return EventHandlerResult::EVENT_CONSUMED;
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


void Qukeys::sendReport(bool qukey_state_alternate) {
  KeyAddr k = key_queue_[0].addr;
  // bool key_released = bitRead(key_queue_release_bitfield_, 0);
  bool key_released = key_queue_release_bitfield_ & 1;
  flushKey(qukey_state_alternate);
  hid::sendKeyboardReport();
  if (key_released) {
    flushing_queue_ = true;
    handleKeyswitchEvent(Key_NoKey, k, WAS_PRESSED);
    flushing_queue_ = false;
    hid::sendKeyboardReport();
  }
}

EventHandlerResult Qukeys::beforeReportingState() {

  uint16_t current_time = Kaleidoscope.millisAtCycleStart();

  // Redesign: assuming nothing gets changed in KeyboardioHID, I want to remove
  // direct dependence on it. That means no sending reports in the
  // onKeyswitchEvent() hook. Instead, we'll start doing them from here. Every
  // cycle, we go through the queue:
  while (key_queue_length_ > 0) {
    if (! isQukey(key_queue_[0].addr)) {
      // flush key, send press event & send report
      // if queue item is released, send release event & send report
      sendReport(QUKEY_STATE_PRIMARY);
      return EventHandlerResult::OK;
    }

    // The first queue item IS a qukey

    // `key_queue_released_state_` is a bitfield that corresponds to the queue
    // items. 1 means the key has been released; 0 means it's still being
    // held.
    if (key_queue_release_bitfield_ == 0) {

      uint16_t elapsed_time = current_time - key_queue_[0].start_time;
      if (elapsed_time > time_limit_) {
        // timed out; flush qukey alternate, send press event & report.
        sendReport(QUKEY_STATE_ALTERNATE);
        return EventHandlerResult::OK;
      }

      // stop here; the first item in the queue is a qukey that is still being held
      break;
    }

    // At least one key in the queue has been released

    if (bitRead(key_queue_release_bitfield_, 0)) {
      // first queue item is a qukey, and it has been released
      if (release_delay_ != 0) {
        // there is a release delay configured
        if (! delayed_qukey_) {
          // no qukey is currently delayed; delay this one. Maybe this should
          // be handled in onKeyswitchevent()?
          delayed_qukey_ = true;
          delayed_qukey_start_time_ = current_time;
          break;
        }

        // this qukey was already delayed; check to see if it has timed out
        int16_t elapsed_time = current_time - delayed_qukey_start_time_;
        if (elapsed_time < release_delay_) {
          // still waiting for release delay
          break;
        }
        // timed out
      }
      // either timed out release delay, or no release delay:
      // flush qukey primary, send press & release reports
      sendReport(QUKEY_STATE_PRIMARY);
      return EventHandlerResult::EVENT_CONSUMED;
    }

    // some other queue item has been released
    // flush qukey as alternate, send press report
    sendReport(QUKEY_STATE_ALTERNATE);
    return EventHandlerResult::OK;
  }

  return EventHandlerResult::OK;
}

} // namespace plugin {
} // namespace kaleidoscope {

kaleidoscope::plugin::Qukeys Qukeys;
