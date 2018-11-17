/* Kaleidoscope-Macros - Macro keys for Kaleidoscope.
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

typedef enum {
  MACRO_ACTION_END,

  MACRO_ACTION_STEP_INTERVAL,
  MACRO_ACTION_STEP_WAIT,

  MACRO_ACTION_STEP_KEYDOWN,
  MACRO_ACTION_STEP_KEYUP,
  MACRO_ACTION_STEP_TAP,

  MACRO_ACTION_STEP_KEYCODEDOWN,
  MACRO_ACTION_STEP_KEYCODEUP,
  MACRO_ACTION_STEP_TAPCODE,

  MACRO_ACTION_STEP_EXPLICIT_REPORT,
  MACRO_ACTION_STEP_IMPLICIT_REPORT,
  MACRO_ACTION_STEP_SEND_REPORT,
} MacroActionStepType;

typedef uint8_t macro_t;

#define MACRO_NONE 0
#define MACRO(...) ({static const macro_t __m[] PROGMEM = { __VA_ARGS__, MACRO_ACTION_END }; &__m[0]; })
#define MACRODOWN(...) (keyToggledOn(keyState) ? MACRO(__VA_ARGS__) : MACRO_NONE)

#define I(n)  MACRO_ACTION_STEP_INTERVAL, n
#define W(n)  MACRO_ACTION_STEP_WAIT, n

#define Dr(k) MACRO_ACTION_STEP_KEYDOWN, (k).flags, (k).keyCode
#define D(k)  Dr(Key_ ## k)
#define Ur(k) MACRO_ACTION_STEP_KEYUP, (k).flags, (k).keyCode
#define U(k)  Ur(Key_ ## k)
#define Tr(k) MACRO_ACTION_STEP_TAP, (k).flags, (k).keyCode
#define T(k)  Tr(Key_ ## k)

#define Dc(k) MACRO_ACTION_STEP_KEYCODEDOWN, (Key_ ## k).keyCode
#define Uc(k) MACRO_ACTION_STEP_KEYCODEUP, (Key_ ## k).keyCode
#define Tc(k) MACRO_ACTION_STEP_TAPCODE, (Key_ ## k).keyCode

#define WITH_EXPLICIT_REPORT MACRO_ACTION_STEP_EXPLICIT_REPORT
#define WITH_IMPLICIT_REPORT MACRO_ACTION_STEP_IMPLICIT_REPORT
#define SEND_REPORT MACRO_ACTION_STEP_SEND_REPORT

__attribute__((deprecated("END is no longer required to end macros"))) const MacroActionStepType END = MACRO_ACTION_END;
