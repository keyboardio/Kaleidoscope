/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Keymap-Programmer -- On-the-fly reprogrammable keymap.
 * Copyright (C) 2017, 2018  Gergely Nagy
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
#include <Kaleidoscope-EEPROM-Keymap.h>

namespace kaleidoscope {
class EEPROMKeymapProgrammer : public kaleidoscope::Plugin {
 public:
  typedef enum {
    CODE,
    COPY,
  } mode_t;
  static mode_t mode;

  EEPROMKeymapProgrammer(void) {}

  static void activate(void) {
    nextState();
  }
  static void nextState(void);
  static void cancel(void);

  static bool focusHook(const char *command);

  EventHandlerResult onKeyswitchEvent(Key &mapped_key, byte row, byte col, uint8_t key_state);

#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
  kaleidoscope::EventHandlerResult onSetup() {
    return kaleidoscope::EventHandlerResult::OK;
  }

 protected:
  void begin();
  static Key legacyEventHandler(Key mapped_key, byte row, byte col, uint8_t key_state);
#endif

 private:
  typedef enum {
    INACTIVE,
    WAIT_FOR_KEY,
    WAIT_FOR_CODE,
    WAIT_FOR_SOURCE_KEY,
  } state_t;
  static state_t state_;

  static uint16_t update_position_;  // layer, row, col
  static Key new_key_;
};
}

extern kaleidoscope::EEPROMKeymapProgrammer EEPROMKeymapProgrammer;

#define FOCUS_HOOK_KEYMAP_PROGRAMMER FOCUS_HOOK(EEPROMKeymapProgrammer.focusHook, \
                                                "keymap.toggleProgrammer")
