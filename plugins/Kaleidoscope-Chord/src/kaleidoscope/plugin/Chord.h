/* -*- mode: c++ -*-
 * Kaleidoscope-Chord -- Respond to chords of keys as a single keystroke
 * Copyright (C) 2023 Lisa Ugray
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

#include <Arduino.h>  // for PROGMEM
#include <stdint.h>   // for uint8_t, uint16_t, int8_t

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/KeyEventTracker.h"       // for KeyEventTracker
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/plugin.h"                // for Plugin
#include "kaleidoscope/key_defs.h"              // for Key, Key_Transparent
#include "kaleidoscope/progmem_helpers.h"       // for cloneFromProgmem


namespace kaleidoscope {
namespace plugin {
class Chord : public kaleidoscope::Plugin {
 public:
  EventHandlerResult onKeyswitchEvent(KeyEvent &event);
  EventHandlerResult afterEachCycle();
  void setTimeout(uint8_t timeout);

  template<uint8_t _chord_defs_size>
  void configure(Key const (&chords)[_chord_defs_size]) {
    chord_defs_      = chords;
    chord_defs_size_ = _chord_defs_size;
  }

 private:
  void resolveOrArpeggiate();
  void resolve(Key target_key);
  bool inChord(uint8_t index, Key key);
  bool isChordStrictSubset();
  bool isChordStrictSubsetOf(uint8_t c, uint8_t cs);
  uint8_t chordSize(uint8_t index);
  bool isChord(uint8_t c, uint8_t cs);
  Key getChord();
  void appendEvent(KeyEvent event);
  void arpeggiate();

  KeyEventTracker event_tracker_;
  uint16_t start_time_;

  static constexpr uint8_t kMaxChordSize{10};
  KeyEvent potential_chord_[kMaxChordSize];
  uint8_t potential_chord_size_{0};

  Key const *chord_defs_{nullptr};
  uint8_t chord_defs_size_{0};

  uint8_t timeout_ = 50;
};
}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::Chord Chord;

#define CHORDS(chord_defs...)                            \
  {                                                      \
    static Key const chord_def[] PROGMEM = {chord_defs}; \
    Chord.configure(chord_def);                          \
  }
#define CHORD(chord_keys...) chord_keys, Key_NoKey
