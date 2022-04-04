// -*- mode: c++ -*-

#include <Kaleidoscope.h>

#include <Kaleidoscope-CharShift.h>

// clang-format off
KEYMAPS(
  [0] = KEYMAP_STACKED
  (
      XXX,          Key_1, Key_2, Key_3, Key_4, Key_5, Key_NoKey,
      Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
      Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
      Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,

      Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
      XXX,

      XXX,       Key_6, Key_7, Key_8, Key_9, Key_0,     Key_skip,
      Key_Enter, Key_Y, Key_U, Key_I, Key_O, Key_P,     Key_Equals,
                 Key_H, Key_J, Key_K, Key_L, CS(2),     Key_Quote,
      Key_skip,  Key_N, Key_M, CS(0), CS(1), Key_Slash, Key_Minus,

      Key_RightShift, Key_RightAlt, Key_Spacebar, Key_RightControl,
      XXX
   ),
)
// clang-format on

KALEIDOSCOPE_INIT_PLUGINS(CharShift);

void setup() {
  CS_KEYS(
    kaleidoscope::plugin::CharShift::KeyPair(Key_Comma, Key_Semicolon),               // CS(0)
    kaleidoscope::plugin::CharShift::KeyPair(Key_Period, LSHIFT(Key_Semicolon)),      // CS(1)
    kaleidoscope::plugin::CharShift::KeyPair(LSHIFT(Key_Comma), LSHIFT(Key_Period)),  // CS(2)
  );
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}
