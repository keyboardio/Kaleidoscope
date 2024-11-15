// -*- mode: c++ -*-

#include <Kaleidoscope.h>

#include <Kaleidoscope-LongPress.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-EEPROM-Keymap.h>
#include <Kaleidoscope-FocusSerial.h>
#include <Kaleidoscope-Macros.h>

enum {
  TOGGLE_LONGPRESS,
};  // macros

enum {
  QWERTY,
};  // layers

// clang-format off
KEYMAPS(
  [QWERTY] = KEYMAP_STACKED
  (
      Key_NoKey,    Key_1, Key_2, Key_3, Key_4, Key_5, Key_NoKey,
      Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
      Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
      Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,

      Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
      XXX,

      M(TOGGLE_LONGPRESS), Key_6, Key_7, Key_8,     Key_9,      Key_0,         Key_skip,
      Key_Enter,           Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals,
                           Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,
      Key_skip,            Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus,

      Key_RightShift, Key_RightAlt, Key_Spacebar, Key_RightControl,
      XXX
   ),
)
// clang-format on

// Defining a macro (on the "any" key: see above) to turn LongPress on and off
const macro_t *macroAction(uint8_t macro_id, KeyEvent &event) {
  switch (macro_id) {
  case TOGGLE_LONGPRESS:
    if (keyToggledOn(event.state))
      LongPress.toggle();
    break;
  }
  return MACRO_NONE;
}

// This sketch uses the LongPressConfig plugin, which enables run-time
// configuration of LongPress configuration settings.  All of the plugins marked
// "for LongPressConfig" are optional; LongPress itself will work without them.
KALEIDOSCOPE_INIT_PLUGINS(
  EEPROMSettings,        // for LongPressConfig
  EEPROMKeymap,          // for LongPressConfig
  Focus,                 // for LongPressConfig
  FocusEEPROMCommand,    // for LongPressConfig
  FocusSettingsCommand,  // for LongPressConfig
  LongPress,
  LongPressConfig,  // for LongPressConfig
  Macros            // for toggle LongPress Macro
);

void setup() {
  // Enable AutoShift for letter keys and number keys only:
  LongPress.setAutoshiftEnabled(LongPress.letterKeys() | LongPress.numberKeys());
  // Add symbol keys to the enabled autoshift categories:
  LongPress.enableAutoshift(LongPress.symbolKeys());

  LONGPRESS(
    // Long pressing the second key in the first row on the QWERTY layer
    // produces a 0 instead of a 1 (and instead of Shift-1)
    kaleidoscope::plugin::LongPressKey(QWERTY, KeyAddr(0, 1), Key_0),

    // instead of shifting, produce a backslash on long pressing slash on
    // all layers
    kaleidoscope::plugin::LongPressKey(kaleidoscope::plugin::longpress::ALL_LAYERS, Key_Slash, Key_Backslash), )

  // Set the LongPress trigger time to 150ms:
  LongPress.setTimeout(150);
  // Start with LongPress turned off:
  LongPress.disable();

  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}
