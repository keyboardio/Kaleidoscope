/* -*- mode: c++ -*-
 * Kaleidoscope-GhostInTheFirmware -- Let the keyboard write for you!
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
#include <Kaleidoscope-GhostInTheFirmware.h>
#include <Kaleidoscope-LED-Stalker.h>
#include <Kaleidoscope-Macros.h>

// This sketch is set up to demonstrate the GhostInTheFirmware plugin. The left
// palm key will activate the plugin, virtually pressing each key on the bottom
// row in sequence, and lighting up the keys using the Stalker LED effect. It
// will type out the letters from A to N, but the right palm key can be used to
// toggle the custom EventDropper plugin to suppress USB output.

// clang-format off
KEYMAPS(
  [0] = KEYMAP_STACKED
  (___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___,
   Key_A, Key_B, Key_C, Key_D, Key_E, Key_F, Key_G,

   ___, ___, ___, ___,
   M(0),

   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
        ___, ___, ___, ___, ___, ___,
   Key_H, Key_I, Key_J, Key_K, Key_L, Key_M, Key_N,

   ___, ___, ___, ___,
   M(1)),
        )
// clang-format on

namespace kaleidoscope {
namespace plugin {

class EventDropper : public Plugin {
 public:
  EventHandlerResult onKeyEvent(KeyEvent &event) {
    if (active_)
      return EventHandlerResult::EVENT_CONSUMED;
    return EventHandlerResult::OK;
  }
  void toggle() {
    active_ = !active_;
  }

 private:
  bool active_ = false;
};

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::EventDropper EventDropper;

const macro_t *macroAction(uint8_t macro_id, KeyEvent &event) {
  if (macro_id == 0 && keyToggledOn(event.state))
    GhostInTheFirmware.activate();
  if (macro_id == 1 && keyToggledOn(event.state))
    EventDropper.toggle();

  return MACRO_NONE;
}

static const kaleidoscope::plugin::GhostInTheFirmware::GhostKey ghost_keys[] PROGMEM = {
  {KeyAddr(3, 0), 200, 50},
  {KeyAddr(3, 1), 200, 50},
  {KeyAddr(3, 2), 200, 50},
  {KeyAddr(3, 3), 200, 50},
  {KeyAddr(3, 4), 200, 50},
  {KeyAddr(3, 5), 200, 50},
  {KeyAddr(2, 6), 200, 50},
  {KeyAddr(2, 9), 200, 50},
  {KeyAddr(3, 10), 200, 50},
  {KeyAddr(3, 11), 200, 50},
  {KeyAddr(3, 12), 200, 50},
  {KeyAddr(3, 13), 200, 50},
  {KeyAddr(3, 14), 200, 50},
  {KeyAddr(3, 15), 200, 50},

  {KeyAddr::none(), 0, 0}};

KALEIDOSCOPE_INIT_PLUGINS(GhostInTheFirmware,
                          LEDControl,
                          StalkerEffect,
                          Macros,
                          EventDropper);

void setup() {
  Kaleidoscope.setup();

  StalkerEffect.variant         = STALKER(BlazingTrail);
  GhostInTheFirmware.ghost_keys = ghost_keys;
}

void loop() {
  Kaleidoscope.loop();
}
