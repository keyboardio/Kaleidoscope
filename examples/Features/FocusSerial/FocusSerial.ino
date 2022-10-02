/* -*- mode: c++ -*-
 * Kaleidoscope-FocusSerial -- Bidirectional communication plugin
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
#include <Kaleidoscope-FocusSerial.h>

// clang-format off
KEYMAPS(
  [0] = KEYMAP_STACKED
  (Key_NoKey,         Key_1, Key_2, Key_3, Key_4, Key_5, Key_NoKey,
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

namespace kaleidoscope {
class FocusTestCommand : public Plugin {
 public:
  FocusTestCommand() {}

  EventHandlerResult onFocusEvent(const char *input) {
    const char *cmd = PSTR("test");

    if (::Focus.inputMatchesHelp(input))
      return ::Focus.printHelp(cmd);

    if (::Focus.inputMatchesCommand(input, cmd)) {
      ::Focus.send(F("ok!"));
      return EventHandlerResult::EVENT_CONSUMED;
    }

    return EventHandlerResult::OK;
  }
};

class FocusHelpCommand : public Plugin {
 public:
  FocusHelpCommand() {}

  EventHandlerResult onFocusEvent(const char *input) {
    if (::Focus.inputMatchesHelp(input))
      return ::Focus.printHelp(PSTR("help"));

    return EventHandlerResult::OK;
  }
};

}  // namespace kaleidoscope

kaleidoscope::FocusTestCommand FocusTestCommand;
kaleidoscope::FocusHelpCommand FocusHelpCommand;

KALEIDOSCOPE_INIT_PLUGINS(Focus, FocusTestCommand, FocusHelpCommand);

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}
