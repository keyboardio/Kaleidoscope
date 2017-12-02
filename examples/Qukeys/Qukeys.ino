// -*- mode: c++ -*-

#include <Kaleidoscope.h>
#include <Kaleidoscope-Qukeys.h>
#include <Kaleidoscope-Macros.h>

KEYMAPS(
  [0] = KEYMAP_STACKED
  (
    Key_NoKey,    Key_1, Key_2, Key_3, Key_4, Key_5, Key_NoKey,
    Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
    Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
    Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,

    Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
    ___,

    M(MACRO_TOGGLE_QUKEYS),  Key_6, Key_7, Key_8,     Key_9,      Key_0,         Key_skip,
    Key_Enter, Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals,
    Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,
    Key_skip,  Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus,

    Key_RightShift, Key_RightAlt, Key_Spacebar, Key_RightControl,
    ___),
)

// Defining a macro (on the "any" key: see above) to toggle Qukeys on and off
const macro_t *macroAction(uint8_t macro_index, uint8_t key_state) {
  switch (macro_index) {
  case MACRO_TOGGLE_QUKEYS:
    Qukeys.toggle();
    break;
  }
  return MACRO_NONE;
}

void setup() {
  // Use Qukeys
  Kaleidoscope.use(&Qukeys);

  QUKEYS(
    kaleidoscope::Qukey(0, 2, 1, Key_LeftGui),      // A/cmd
    kaleidoscope::Qukey(0, 2, 2, Key_LeftAlt),      // S/alt
    kaleidoscope::Qukey(0, 2, 3, Key_LeftControl),  // D/ctrl
    kaleidoscope::Qukey(0, 2, 4, Key_LeftShift)     // F/shift
  )
  Qukeys.setTimeout(200);

  // To toggle Qukeys off and on, we use a macro
  Kaleidoscope.use(&Macros);

  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}
