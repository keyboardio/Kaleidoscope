/* -*- mode: c++ -*-
 * Kaleidoscope-LED-AlphaSquare -- 4x4 pixel LED alphabet
 * Copyright (C) 2017, 2018  Keyboard.io, Inc
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
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LED-AlphaSquare.h>
#include <Kaleidoscope-Macros.h>

// clang-format off
KEYMAPS(
  [0] = KEYMAP_STACKED
  (
    Key_LEDEffectNext, Key_1, Key_2, Key_3, Key_4, Key_5, M(0),
    Key_Backtick,      Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
    Key_PageUp,        Key_A, Key_S, Key_D, Key_F, Key_G,
    Key_PageDown,      Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,

    Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
    Key_skip,

    Key_skip,  Key_6, Key_7, Key_8,     Key_9,      Key_0,         Key_skip,
    Key_Enter, Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals,
               Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,
    Key_skip,  Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus,

    Key_RightShift, Key_RightAlt, Key_Spacebar, Key_RightControl,
    Key_skip),
)
// clang-format on

const macro_t *macroAction(uint8_t macro_id, KeyEvent &event) {
  if (!keyToggledOn(event.state))
    return MACRO_NONE;

  if (macro_id == 0) {
    for (uint8_t i = Key_A.getKeyCode(); i <= Key_0.getKeyCode(); i++) {
      LEDControl.set_all_leds_to(0, 0, 0);
      LEDControl.syncLeds();
      delay(100);

      uint8_t col = 2;
      if (i % 2)
        col = 10;

      for (uint8_t step = 0; step <= 0xf0; step += 8) {
        AlphaSquare.color = {step, step, step};
        AlphaSquare.display({i, 0}, col);
        delay(10);
      }
      for (uint8_t step = 0xff; step >= 8; step -= 8) {
        AlphaSquare.color = {step, step, step};
        AlphaSquare.display({i, 0}, col);
        delay(10);
      }
      delay(100);
    }

    LEDControl.set_all_leds_to(0, 0, 0);
    LEDControl.syncLeds();
    delay(100);

    for (uint8_t step = 0; step <= 0xf0; step += 8) {
      AlphaSquare.color = {step, step, step};
      AlphaSquare.display(kaleidoscope::plugin::alpha_square::symbols::Lambda, 2);
      AlphaSquare.display(kaleidoscope::plugin::alpha_square::symbols::Lambda, 10);
      delay(10);
    }

    for (uint8_t step = 0xff; step >= 8; step -= 8) {
      AlphaSquare.color = {step, step, step};
      AlphaSquare.display(kaleidoscope::plugin::alpha_square::symbols::Lambda, 2);
      AlphaSquare.display(kaleidoscope::plugin::alpha_square::symbols::Lambda, 10);
      delay(10);
    }
    delay(100);
  }

  LEDControl.set_all_leds_to(0, 0, 0);

  return MACRO_NONE;
}

KALEIDOSCOPE_INIT_PLUGINS(LEDControl,
                          AlphaSquare,
                          AlphaSquareEffect,
                          Macros);

void setup() {
  Kaleidoscope.setup();

  AlphaSquare.color = {0xcb, 0xc0, 0xff};
}

void loop() {
  Kaleidoscope.loop();
}
