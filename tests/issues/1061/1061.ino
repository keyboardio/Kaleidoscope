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
#include <Kaleidoscope-OneShot.h>

// *INDENT-OFF*
KEYMAPS(
    [0] = KEYMAP_STACKED
    (
        Key_Insert, OSM(LeftAlt), OSM(RightAlt), ___, ___, ___, ___,
        Key_A, Key_B, ___, ___, ___, ___, ___,
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

namespace kaleidoscope {
namespace plugin {

class OneShotInsert : public Plugin {
 public:
  EventHandlerResult onKeyEvent(KeyEvent &event) {
    if (keyToggledOn(event.state) && !keyIsInjected(event.state) &&
        event.key == Key_Insert && live_keys[event.addr] != event.key)
      ::OneShot.setPending(event.addr);
    return EventHandlerResult::OK;
  }
};

bool OneShot::isStickable(Key key) const {
  if (key == Key_LeftAlt)
    return false;
  return isStickableDefault(key);
}

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::OneShotInsert OneShotInsert;

KALEIDOSCOPE_INIT_PLUGINS(OneShot, OneShotInsert);

void setup() {
  Kaleidoscope.setup();
  OneShot.setTimeout(50);
  OneShot.setHoldTimeout(20);
  OneShot.setDoubleTapTimeout(20);
}

void loop() {
  Kaleidoscope.loop();
}
