/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2025 Keyboard.io, inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * Additional Permissions:
 * As an additional permission under Section 7 of the GNU General Public
 * License Version 3, you may link this software against a Vendor-provided
 * Hardware Specific Software Module under the terms of the MCU Vendor
 * Firmware Library Additional Permission Version 1.0.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include <Arduino.h>
#include <initializer_list>

namespace kaleidoscope {
namespace driver {
namespace speaker {

struct BaseProps {
  static constexpr uint8_t kMaxTuneLength = 16;
};

template<typename _SpeakerProps>
class Base {
 public:
  Base() {}

  virtual void setup() {}
  virtual void playTone(unsigned int frequency, uint32_t duration) {}
  virtual void playTone(unsigned int frequency) {
    playTone(frequency, 0);  // 0 duration means play until stopped
  }
  virtual void stopTone() {}
  virtual bool isPlaying() { return false; }

  void playTune(const uint16_t *notes, const uint16_t *durations, uint8_t length);
  void playTune(std::initializer_list<uint16_t> notes, std::initializer_list<uint16_t> durations) {
    playTune(notes.begin(), durations.begin(), min(notes.size(), durations.size()));
  }
  virtual void stopTune();
  virtual void update();

  // Prepare speaker for sleep - noop in base implementation
  virtual void prepareForSleep() {}

 protected:
  virtual void playNextTone();

  uint16_t current_notes_[_SpeakerProps::kMaxTuneLength];
  uint16_t current_durations_[_SpeakerProps::kMaxTuneLength];
  uint8_t current_length_ {0};
  uint8_t current_index_ {0};
  uint32_t next_note_time_ {0};
};

}  // namespace speaker
}  // namespace driver
}  // namespace kaleidoscope
