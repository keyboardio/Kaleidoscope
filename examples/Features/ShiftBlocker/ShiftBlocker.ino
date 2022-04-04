/* -*- mode: c++ -*-
 * ShiftBlocker -- A Kaleidoscope Example
 * Copyright (C) 2016-2022  Keyboard.io, Inc.
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

#include "Kaleidoscope.h"
#include "Kaleidoscope-Macros.h"

// clang-format off
KEYMAPS(
  [0] = KEYMAP_STACKED
  (
   Key_NoKey,    Key_1, Key_2, Key_3, Key_4, Key_5, Key_NoKey,
   Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
   Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_PageDown,   Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,

   Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
   M(0),

   Key_skip,  Key_6, Key_7, Key_8,     Key_9,      Key_0,         Key_skip,
   Key_Enter, Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals,
              Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,
   Key_skip,  Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus,

   Key_RightShift, Key_RightAlt, Key_Spacebar, Key_RightControl,
   M(0)
   ),
)
// clang-format on

namespace kaleidoscope {
namespace plugin {

// When activated, this plugin will suppress any `Shift` key (including modifier
// combos with `Shift`) before it's added to the HID report.
class ShiftBlocker : public Plugin {

 public:
  EventHandlerResult onAddToReport(Key key) {
    if (active_ && key.isKeyboardShift())
      return EventHandlerResult::ABORT;
    return EventHandlerResult::OK;
  }

  void enable() {
    active_ = true;
  }
  void disable() {
    active_ = false;
  }

 private:
  bool active_{false};
};

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::ShiftBlocker ShiftBlocker;

const macro_t *macroAction(uint8_t macro_id, KeyEvent &event) {
  if (keyToggledOn(event.state)) {
    switch (macro_id) {
    case 0:
      // First, enable ShiftBlocker to suppress any held `Shift` key(s).
      ShiftBlocker.enable();
      // Tap `AltGr` + `7` to activate the grave accent dead key.
      Macros.tap(RALT(Key_7));
      // Disable ShiftBlocker so it won't affect the `E` event.
      ShiftBlocker.disable();
      // Change the Macros key into a plain `E` key before its press event is
      // processed.
      event.key = Key_E;
      break;
    }
  }
  return MACRO_NONE;
}

KALEIDOSCOPE_INIT_PLUGINS(Macros,
                          ShiftBlocker);

void setup() {
  Kaleidoscope.setup();
  // Uncomment to manually set the OS, as Kaleidoscope will not autodetect it.
  // (Possible values are in HostOS.h.)
  // HostOS.os(kaleidoscope::hostos::LINUX);
}

void loop() {
  Kaleidoscope.loop();
}
