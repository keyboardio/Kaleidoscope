/* -*- mode: c++ -*-
 * Kaleidoscope-Leader -- VIM-style leader keys
 * Copyright (C) 2016, 2017  Gergely Nagy
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
#include <Kaleidoscope-Ranges.h>

#define LEADER_MAX_SEQUENCE_LENGTH 16

#define LEAD(n) (Key){ .raw = KaleidoscopePlugins::Ranges::LEAD_FIRST + n }

#define LEADER_SEQ(...) { __VA_ARGS__, Key_NoKey }
#define LEADER_DICT(...) { __VA_ARGS__, {{Key_NoKey}, NULL} }

namespace KaleidoscopePlugins {
  class Leader : public KaleidoscopePlugin {
  public:
    typedef void (*action_t) (uint8_t seqIndex);
    typedef struct {
      Key sequence[LEADER_MAX_SEQUENCE_LENGTH + 1];
      action_t action;
    } dictionary_t;

    Leader (void);

    virtual void begin (void) final;

    static void configure (const dictionary_t dictionary[]);
    static void reset (void);
    static uint16_t timeOut;

    void inject (Key key, uint8_t keyState);

  private:
    static Key sequence[LEADER_MAX_SEQUENCE_LENGTH + 1];
    static uint8_t sequencePos;
    static uint32_t endTime;
    static const Leader::dictionary_t *dictionary;

    static Key eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState);
    static void loopHook (bool postClear);
    static int8_t lookup (void);
  };
};

extern KaleidoscopePlugins::Leader Leader;
