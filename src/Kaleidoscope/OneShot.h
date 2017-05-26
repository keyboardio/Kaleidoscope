/* -*- mode: c++ -*-
 * Kaleidoscope-OneShot -- One-shot modifiers and layers
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

#define OSM(kc) (Key) {.raw = KaleidoscopePlugins::Ranges::OSM_FIRST + (Key_ ## kc).keyCode - Key_LeftControl.keyCode}
#define OSL(n) (Key) {.raw = KaleidoscopePlugins::Ranges::OSL_FIRST + n}

namespace KaleidoscopePlugins {
class OneShot : public KaleidoscopePlugin {
  public:
    OneShot (void);

    virtual void begin (void) final;

    static bool isActive (void);
    static void cancel (bool withStickies);
    static void cancel (void) {
        cancel (false);
    };
    static uint16_t timeOut;
    static uint16_t holdTimeOut;

    static bool isModifierActive (Key key);

    void inject (Key key, uint8_t keyState);

  private:
    typedef union {
        struct {
            uint8_t mods;
            uint8_t layers;
        };
        uint16_t all;
    } state_t;
    static uint32_t startTime;
    static state_t State;
    static state_t stickyState;
    static state_t pressedState;
    static uint32_t leftMask;
    static uint32_t rightMask;
    static Key prevKey;
    static bool shouldCancel;
    static bool shouldCancelStickies;

    static void injectNormalKey (uint8_t idx, uint8_t keyState);
    static void activateOneShot (uint8_t idx);
    static void cancelOneShot (uint8_t idx);

    static void mask (byte row, byte col);
    static void unmask (byte row, byte col);
    static bool isMasked (byte row, byte col);

    static Key eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState);
    static void loopHook (bool postClear);
};
};

extern KaleidoscopePlugins::OneShot OneShot;
