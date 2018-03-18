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
#include <Kaleidoscope-Ranges.h>

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

#define MT(mod, key) (Key) { \
    .raw = kaleidoscope::ranges::DUM_FIRST + \
      (((Key_ ## mod).keyCode - Key_LeftControl.keyCode) << 8) + (Key_ ## key).keyCode }
#define SFT_T(key) MT(LeftShift, key)
#define CTL_T(key) MT(LeftControl, key)
#define ALT_T(key) MT(LeftAlt, key)
#define GUI_T(key) MT(LeftGui, key)

#define LT(layer, key) (Key) { .raw = kaleidoscope::ranges::DUL_FIRST + (layer << 8) + (Key_ ## key).keyCode }

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
  uint16_t start_time; // time a queued key was pressed
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
  static void setReleaseDelay(uint8_t release_delay) {
    release_delay_ = release_delay;
  }

  static Qukey * qukeys;
  static uint8_t qukeys_count;

 private:
  static bool active_;
  static uint16_t time_limit_;
  static uint8_t release_delay_;
  static QueueItem key_queue_[QUKEYS_QUEUE_MAX];
  static uint8_t key_queue_length_;
  static bool flushing_queue_;

  // Qukey state bitfield
  static uint8_t qukey_state_[(TOTAL_KEYS) / 8 + ((TOTAL_KEYS) % 8 ? 1 : 0)];
  static bool getQukeyState(uint8_t addr) {
    return bitRead(qukey_state_[addr / 8], addr % 8);
  }
  static void setQukeyState(uint8_t addr, boolean qukey_state) {
    bitWrite(qukey_state_[addr / 8], addr % 8, qukey_state);
  }

  static int8_t lookupQukey(uint8_t key_addr);
  static void enqueue(uint8_t key_addr);
  static int8_t searchQueue(uint8_t key_addr);
  static void flushKey(bool qukey_state, uint8_t keyswitch_state);
  static void flushQueue(int8_t index);
  static void flushQueue(void);
  static bool isQukey(uint8_t addr);

  static Key keyScanHook(Key mapped_key, byte row, byte col, uint8_t key_state);
  static void preReportHook(void);
  static void postReportHook(void) {}
  static void loopHook(bool post_clear);
};

} // namespace kaleidoscope {

extern kaleidoscope::Qukeys Qukeys;

// macro for use in sketch file to simplify definition of qukeys
#define QUKEYS(qukey_defs...) {						\
  static kaleidoscope::Qukey qk_table[] = { qukey_defs };		\
  Qukeys.qukeys = qk_table;						\
  Qukeys.qukeys_count = sizeof(qk_table) / sizeof(kaleidoscope::Qukey); \
}
