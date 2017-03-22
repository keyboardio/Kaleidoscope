/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Keymap-Programmer -- On-the-fly reprogrammable keymap.
 * Copyright (C) 2017  Gergely Nagy
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

namespace KaleidoscopePlugins {
  class EEPROMKeymapProgrammer : public KaleidoscopePlugin {
  public:
    typedef enum {
      CODE,
      COPY,
    } mode_t;

    EEPROMKeymapProgrammer (void);

    virtual void begin (void) final;

    static void mode (mode_t programmerMode);
    static void nextState (void);
    static void cancel (void);

  private:
    static mode_t programmerMode;
    typedef enum {
      INACTIVE,
      WAIT_FOR_KEY,
      WAIT_FOR_CODE,
      WAIT_FOR_SOURCE_KEY,
    } state_t;
    static state_t state;

    static uint16_t updatePosition; // layer, row, col
    static Key newKey;

    static Key eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState);
  };
};

extern KaleidoscopePlugins::EEPROMKeymapProgrammer EEPROMKeymapProgrammer;
