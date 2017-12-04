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

#pragma once

#include <Kaleidoscope.h>
#include <addr.h>

// Maximum number of qukeys allowed ­ not actually used
#define QUKEYS_MAX 64
// Maximum length of the pending queue
#define QUKEYS_QUEUE_MAX 8
// Total number of keys on the keyboard (assuming full grid)
#define TOTAL_KEYS ROWS * COLS

// Boolean values for storing qukey state
#define QUKEY_STATE_PRIMARY false
#define QUKEY_STATE_ALTERNATE true

// Initialization addr value for empty key_queue. This seems to be
// unnecessary, because we rely on keeping track of the lenght of the
// queue, anyway.
#define QUKEY_UNKNOWN_ADDR 0xFF
// Value to return when no match is found in Qukeys.dict. A successful
// match returns an index in the array, so this must be negative. Also
// used for failed search of the key_queue.
#define QUKEY_NOT_FOUND -1
// Wildcard value; this matches any layer
#define QUKEY_ALL_LAYERS -1

namespace kaleidoscope {

// Data structure for an individual qukey
struct Qukey {
 public:
  Qukey(void) {}
  Qukey(int8_t layer, byte row, byte col, Key alt_keycode);

  int8_t layer;
  uint8_t addr;
  Key alt_keycode;
};

// Data structure for an entry in the key_queue
struct QueueItem {
  uint8_t addr;        // keyswitch coordinates
  uint32_t flush_time; // time past which a qukey gets flushed
};

// The plugin itself
class Qukeys : public KaleidoscopePlugin {
  // I could use a bitfield to get the state values, but then we'd
  // have to check the key_queue (there are three states). Or use a
  // second bitfield for the indeterminite state. Using a bitfield
  // would enable storing the qukey list in PROGMEM, but I don't know
  // if the added complexity is worth it.
 public:
  Qukeys(void);

  //static void init(Qukey *qukeys, uint8_t qukeys_count);
  void begin(void) final;
  static void activate(void) {
    active_ = true;
  }
  static void deactivate(void) {
    active_ = false;
  }
  static void toggle(void) {
    active_ = !active_;
  }
  static void setTimeout(uint16_t time_limit) {
    time_limit_ = time_limit;
  }

  static int8_t lookupQukey(uint8_t key_addr);
  static void enqueue(uint8_t key_addr);
  static int8_t searchQueue(uint8_t key_addr);
  static void flushKey(bool qukey_state, uint8_t keyswitch_state);
  static void flushQueue(int8_t index);
  static void flushQueue(void);

  static Qukey * qukeys_;
  static uint8_t qukeys_count_;

 private:
  static bool active_;
  static uint16_t time_limit_;
  static QueueItem key_queue_[QUKEYS_QUEUE_MAX];
  static uint8_t key_queue_length_;
  //static uint8_t keyswitch_state[];

  // Qukey state bitfield
  static uint8_t qukey_state_[(TOTAL_KEYS)/8 + ((TOTAL_KEYS)%8 ? 1 : 0)];
  static bool getQukeyState(uint8_t addr) {
    return bitRead(qukey_state_[addr / 8], addr % 8);
  }
  static void setQukeyState(uint8_t addr, boolean qukey_state) {
    bitWrite(qukey_state_[addr / 8], addr % 8, qukey_state);
  }

  static Key keyScanHook(Key mapped_key, byte row, byte col, uint8_t key_state);
  static void preReportHook(void);
  static void postReportHook(void) {}
  static void loopHook(bool post_clear);
};

} // namespace kaleidoscope {

extern kaleidoscope::Qukeys Qukeys;

// macro for use in sketch file to simplify definition of qukeys
#define QUKEYS(qukey_defs...) {						\
  static kaleidoscope::Qukey qukeys[] = { qukey_defs };			\
  Qukeys.qukeys_ = qukeys;						\
  Qukeys.qukeys_count_ = sizeof(qukeys) / sizeof(kaleidoscope::Qukey);	\
}
