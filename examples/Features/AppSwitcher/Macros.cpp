/* -*- mode: c++ -*-
 * AppSwitcher -- A Kaleidoscope Example
 * Copyright (C) 2016-2018  Keyboardio, Inc.
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

#define KALEIDOSCOPE_HOSTOS_GUESSER 1

#include <Kaleidoscope-HostOS.h>

#include "Macros.h"

namespace H = kaleidoscope::hostos;

static bool appSwitchActive = false;

const macro_t *macroAppSwitch(uint8_t keyState) {
  appSwitchActive = true;

  // Key was just pressed, or is being held
  if (keyIsPressed(keyState)) {
    if (HostOS.os() == H::OSX)
      return MACRO(Dr(Key_LeftGui), D(Tab));
    else
      return MACRO(Dr(Key_LeftAlt), D(Tab));
  }
  // Key was just released
  if (keyToggledOff(keyState)) {
    if (HostOS.os() == H::OSX)
      return MACRO(U(Tab), Dr(Key_LeftGui));
    else
      return MACRO(U(Tab), Dr(Key_LeftAlt));
  }
  // otherwise we do nothing
  return MACRO_NONE;
}

const macro_t *macroAppCancel(uint8_t keyState) {
  if (keyToggledOn(keyState))
    appSwitchActive = false;
  return MACRO_NONE;
}

void macroAppSwitchLoop() {
  Key mod = Key_LeftAlt;

  if (HostOS.os() == H::OSX)
    mod = Key_LeftGui;

  // if appSwitchActive is true, we continue holding Alt.
  if (appSwitchActive) {
    handleKeyswitchEvent(mod, UnknownKeyswitchLocation, IS_PRESSED);
  }
}
