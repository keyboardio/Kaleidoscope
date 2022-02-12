/* -*- mode: c++ -*-
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

// *INDENT-OFF*
KEYMAPS(
  [0] = KEYMAP_STACKED
  (
      M(0), ___, ___, ___, ___, ___, ___,
      Key_LeftShift, ___, ___, ___, ___, ___, ___,
      ___, ___, ___, ___, ___, ___,
      ___, ___, ___, ___, ___, ___, ___,

      ___, ___, ___, ___,
      ___,

      ___, ___, ___, ___, ___, ___, ___,
      ___, ___, ___, ___, ___, ___, ___,
           ___, ___, ___, ___, ___, ___,
      ___, ___, ___, ___, ___, ___, ___,

      ___, ___, ___, ___,
      ___
   ),
)
// *INDENT-ON*

// ShiftBlocker plugin
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

} // namespace kaleidoscope
} // namespace plugin

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

KALEIDOSCOPE_INIT_PLUGINS(Macros, ShiftBlocker);

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}
