/* -*- mode: c++ -*-
 * Kaleidoscope-Macros Examples
 * Copyright (C) 2021  Keyboard.io, Inc.
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

#include <Kaleidoscope.h>
#include <Kaleidoscope-Macros.h>
#include <Kaleidoscope-OneShot.h>

// Macros
enum {
  TOGGLE_ONESHOT,
};

// clang-format off
KEYMAPS(
  [0] = KEYMAP_STACKED
  (___, M(1), M(2), M(3), M(4), M(5), ___,
   Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
   Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,

   Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
   ShiftToLayer(1),

   ___, M(6), M(7), M(8), M(9), M(0), ___,
   Key_Enter, Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals,
              Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,
   ___,       Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus,

   Key_RightShift, Key_LeftAlt, Key_Spacebar, Key_RightControl,
   ShiftToLayer(1)),

  [1] = KEYMAP_STACKED
  (
    ___, ___, ___, ___, ___, ___, ___,
    ___, ___, ___, ___, ___, ___, ___,
    ___, ___, ___, ___, ___, ___,
    ___, ___, ___, ___, ___, ___, ___,

    ___, ___, ___, ___,
    ___,

    ___, ___,         ___,         ___,        ___,                ___, ___,
    ___, ___,         ___,         ___,        ___,                ___, ___,
         Key_UpArrow, Key_DownArrow, Key_LeftArrow, Key_RightArrow,___, ___,
    ___, ___,         ___,         ___,        ___,                ___, ___,

    ___, ___, ___, ___,
    ___),
)
// clang-format on

// Example macro for typing a string of characters.
void macroTypeString(KeyEvent &event) {
  if (keyToggledOn(event.state)) {
    Macros.type(PSTR("Hello, world!"));
  }
}

// Example macro for macro step sequence.
const macro_t *macroSteps(KeyEvent &event) {
  if (keyToggledOn(event.state)) {
    // Note that the following sequence leaves two keys down (`Key_RightAlt` and
    // `Key_C`). These virtual keys will remain in effect until the Macros key
    // is released.
    return MACRO(I(200), D(LeftShift), T(A), D(RightAlt), T(B), U(LeftShift), D(C));
  }
  return MACRO_NONE;
}

// Example macro that sets `event.key`.
const macro_t *macroNewSentence1(KeyEvent &event) {
  if (keyToggledOn(event.state)) {
    event.key = OSM(LeftShift);
    return MACRO(Tc(Period), Tc(Spacebar), Tc(Spacebar));
  }
  return MACRO_NONE;
}

// Alternate example for above.
void macroNewSentence2(KeyEvent &event) {
  if (keyToggledOn(event.state)) {
    Macros.type(PSTR(".  "));
    event.key = OSM(LeftShift);
  }
}

// Macro that calls `handleKeyEvent()`. This version works even if the OneShot
// plugin is registered before Macros in `KALEIDOSCOPE_INIT_PLUGINS()`.
void macroNewSentence3(KeyEvent &event) {
  Macros.tap(Key_Period);
  Macros.tap(Key_Spacebar);
  Macros.tap(Key_Spacebar);
  // Change the event into a OneShot key event.
  event.key = OSM(LeftShift);
  kaleidoscope::Runtime.handleKeyEvent(event);
  // We can effectively erase the Macros key event, effectively aborting it.
  event.key = Key_NoKey;
  event.addr.clear();
}

// Macro that auto-repeats?

const macro_t *macroAction(uint8_t macro_id, KeyEvent &event) {
  switch (macro_id) {

  case 0:
    macroTypeString(event);
    break;

  case 1:
    return macroNewSentence1(event);

  case 2:
    macroNewSentence2(event);
    break;

  case 3:
    macroNewSentence3(event);
    break;

  case 4:
    return macroSteps(event);

  default:
    break;
  }
  return MACRO_NONE;
}

// For some of the above examples, it's important that Macros is registered
// before OneShot here.
KALEIDOSCOPE_INIT_PLUGINS(Macros, OneShot);

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}
