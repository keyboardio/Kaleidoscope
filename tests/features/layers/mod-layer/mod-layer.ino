// -*- mode: c++ -*-

#include <Kaleidoscope.h>
#include <Kaleidoscope-Qukeys.h>

// *INDENT-OFF*
KEYMAPS(
  [0] = KEYMAP_STACKED
  (
      Key_NoKey,    Key_1, Key_2, Key_3, Key_4, Key_5, Key_NoKey,
      Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
      Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
      Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,

      Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
      ML(LeftShift, 1),

      XXX,       Key_6, Key_7, Key_8,     Key_9,      Key_0,         Key_skip,
      Key_Enter, Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals,
                 Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,
      Key_skip,  Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus,

      Key_RightShift, Key_RightAlt, Key_Spacebar, Key_RightControl,
      ShiftToLayer(1)
   ),
  [1] = KEYMAP_STACKED
  (
      ___,   Key_B, Key_C, Key_D, Key_E, Key_F, Key_G,
      Key_A, Key_B, Key_C, Key_D, Key_E, Key_F, Key_G,
      Key_A, Key_B, Key_C, Key_D, Key_E, Key_F,
      Key_A, Key_B, Key_C, Key_D, Key_E, Key_F, Key_G,

      Key_1, Key_2, Key_3, Key_4,
      ___,


      ___,   Key_B, Key_C, Key_D, Key_E, Key_F, Key_G,
      Key_A, Key_B, Key_C, Key_D, Key_E, Key_F, Key_G,
      Key_A, Key_B, Key_C, Key_D, Key_E, Key_F,
      Key_A, Key_B, Key_C, Key_D, Key_E, Key_F, Key_G,

      Key_1, Key_2, Key_3, Key_4,
      ___
   ),
)
// *INDENT-ON*

// Use Qukeys
KALEIDOSCOPE_INIT_PLUGINS(Qukeys);

void setup() {
  QUKEYS(
    kaleidoscope::plugin::Qukey(0, KeyAddr(2, 1), Key_LeftGui),      // A/cmd
    kaleidoscope::plugin::Qukey(0, KeyAddr(2, 2), Key_LeftAlt),      // S/alt
    kaleidoscope::plugin::Qukey(0, KeyAddr(2, 3), Key_LeftControl),  // D/ctrl
    kaleidoscope::plugin::Qukey(0, KeyAddr(2, 4), Key_LeftShift),    // F/shift

    kaleidoscope::plugin::Qukey(0, KeyAddr(1, 1), ML(LeftGui, 1)),      // Q/cmd+1
    kaleidoscope::plugin::Qukey(0, KeyAddr(1, 2), ML(LeftAlt, 1)),      // W/alt+1
    kaleidoscope::plugin::Qukey(0, KeyAddr(1, 3), ML(LeftControl, 1)),  // E/ctrl+1
    kaleidoscope::plugin::Qukey(0, KeyAddr(1, 4), ML(LeftShift, 1))     // R/shift+1
  );
  Qukeys.setHoldTimeout(20);
  Qukeys.setOverlapThreshold(0);
  Qukeys.setMinimumHoldTime(0);
  Qukeys.setMinimumPriorInterval(0);
  Qukeys.setMaxIntervalForTapRepeat(10);

  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}
