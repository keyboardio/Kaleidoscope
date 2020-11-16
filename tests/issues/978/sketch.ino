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

#undef min
#undef max
#include <iostream>

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
  EventHandlerResult onKeyswitchEvent(Key &key, KeyAddr key_addr, uint8_t key_state) {
    if (key_addr == KeyAddr{0, 0} && key_state == 0) {
      handleKeyswitchEvent(Key_X, key_addr, IS_PRESSED | WAS_PRESSED);
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
