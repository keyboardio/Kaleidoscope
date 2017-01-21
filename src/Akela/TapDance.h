/* -*- mode: c++ -*-
 * Akela -- Animated Keyboardio Extension Library for Anything
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

#include <Akela-Core.h>

#define TD(n) (Key){.raw = Akela::Ranges::TD_FIRST + n }

#define tapDanceActionKeys(tapCount, tapDanceAction, ...) ({            \
      static const Key __k[] PROGMEM = { __VA_ARGS__ };                 \
      TapDance.actionKeys (tapCount, tapDanceAction,                    \
                           sizeof (__k) / sizeof (Key), &__k[0]);       \
    })

namespace Akela {
  class TapDance : public KeyboardioPlugin {
  public:
    typedef enum {
      Tap,
      Hold,
      Interrupt,
      Timeout,
      Release,
    } ActionType;

    TapDance (void);

    virtual void begin (void) final;

    void actionKeys (uint8_t tapCount, ActionType tapDanceAction, uint8_t maxKeys, const Key tapKeys[]);

  private:
    static uint16_t timer;
    static uint16_t timeOut;
    static uint8_t tapCount[32];
    static uint32_t pressedState;
    static uint32_t triggeredState;
    static Key lastTapDanceKey;
    static byte lastTapDanceRow;
    static byte lastTapDanceCol;

    static Key eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState);
    static void loopHook (bool postClear);

    static Key tap (void);
    static void interrupt (void);
    static void timeout (void);
    static Key release (uint8_t tapDanceIndex);
  };
};

void tapDanceAction (uint8_t tapDanceIndex, byte row, byte col, uint8_t tapCount, Akela::TapDance::ActionType tapDanceAction);

extern Akela::TapDance TapDance;
