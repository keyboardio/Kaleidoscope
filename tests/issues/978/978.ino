/* -*- mode: c++ -*-
 * Copyright (C) 2020  Keyboard.io, Inc.
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

#include "./common.h"

// *INDENT-OFF*
KEYMAPS(
    [0] = KEYMAP_STACKED
    (
        Key_A, ___, ___, ___, ___, ___, ___,
        ___, ___, ___, ___, ___, ___, ___,
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

class IdleKeyDetector : public kaleidoscope::Plugin {
 public:
  // handleKeyswitchEvent() is going to mask the underlying issue if it recurs,
  // but leaving this here is better than nothing.
  EventHandlerResult onKeyswitchEvent(KeyEvent &event) {
    if (event.addr == KeyAddr{0, 0} && event.state == 0) {
      event.key = Key_X;
      event.state = IS_PRESSED;
    }
    return EventHandlerResult::OK;
  }
};

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::IdleKeyDetector IdleKeyDetector;

KALEIDOSCOPE_INIT_PLUGINS(IdleKeyDetector);

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}
