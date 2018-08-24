/* Kaleidoscope-NumPad - A NumPad plugin for Kaleidoscope.
 * Copyright (C) 2017-2018  Keyboard.io, Inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
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

#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-Macros.h"
#include "LEDUtils.h"

class NumPad_ : public kaleidoscope::Plugin {
 public:
  NumPad_(void) {}

  static uint8_t numPadLayer;
  static cRGB color;
  static uint8_t lock_hue;

  kaleidoscope::EventHandlerResult onSetup(void);
  kaleidoscope::EventHandlerResult afterEachCycle();

 private:

  void cleanupNumlockState(void);
  void setKeyboardLEDColors(void);
  bool getNumlockState(void);
  void syncNumlockState(bool);

  static uint8_t numpad_lock_key_row;
  static uint8_t numpad_lock_key_col;
  static bool cleanupDone;
  static bool originalNumLockState;
};

extern NumPad_ NumPad;
