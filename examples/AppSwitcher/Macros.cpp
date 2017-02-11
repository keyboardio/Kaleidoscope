/* -*- mode: c++ -*-
 * AppSwitcher -- A Kaleidoscope Example
 * Copyright (C) 2016, 2017  Gergely Nagy
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#define KALEIDOSCOPE_HOSTOS_GUESSER 1

#include <Kaleidoscope-HostOS.h>

#include "Macros.h"

using namespace KaleidoscopePlugins::HostOS;

static bool appSwitchActive = false;

const macro_t *macroAppSwitch (uint8_t keyState) {
    Key mod = Key_LAlt;

    if (HostOS.os () == OSX)
        mod = Key_LGUI;

    // Key was just pressed, or is being held
    if (key_is_pressed(keyState)) {
        return MACRO(Dr(mod), D(Tab), END);
    }
    // Key was just released
    if (key_toggled_off(keyState)) {
        return MACRO(U(Tab), Dr(mod), END);
    }
    // Key is not pressed, and was not just released.
    // if appSwitchActive is true, we continue holding Alt.
    if (appSwitchActive) {
        return MACRO(Dr(mod), END);
    }
    // otherwise we do nothing
    return MACRO_NONE;
}

const macro_t *macroAppCancel (uint8_t keyState) {
    if (key_toggled_on (keyState))
        appSwitchActive = false;
    return MACRO_NONE;
}
