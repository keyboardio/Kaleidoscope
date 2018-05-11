// -*- mode: c++ -*-

#include "Kaleidoscope.h"
#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-LEDEffect-DigitalRain.h"

#include "LED-Off.h"

// *INDENT-OFF*

KEYMAPS(
  [0] = KEYMAP_STACKED
  (XXX,          Key_1, Key_2, Key_3, Key_4, Key_5, Key_LEDEffectNext,
   Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
   Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,

   Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
   XXX,

   XXX,          Key_6, Key_7, Key_8,     Key_9,         Key_0,         XXX,
   Key_Enter,    Key_Y, Key_U, Key_I,     Key_O,         Key_P,         Key_Equals,
                 Key_H, Key_J, Key_K,     Key_L,         Key_Semicolon, Key_Quote,
   Key_RightAlt, Key_N, Key_M, Key_Comma, Key_Period,    Key_Slash,     Key_Minus,

   Key_RightShift, Key_LeftAlt, Key_Spacebar, Key_RightControl,
   XXX)
) // KEYMAPS(

// *INDENT-ON*

void setup() {
  Kaleidoscope.setup();

  Kaleidoscope.use(&LEDControl, &LEDDigitalRainEffect, &LEDOff);
  LEDDigitalRainEffect.activate();
}

void loop() {
  Kaleidoscope.loop();
}
