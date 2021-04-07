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

#include "./common.h"

#undef min
#undef max
#include <iostream>

// *INDENT-OFF*
KEYMAPS(
    [0] = KEYMAP_STACKED
    (
        Key_A, ___, ___, ___, ___, ___, ___,
        ShiftToLayer(1), ___, ___, ___, ___, ___, ___,
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
    [1] = KEYMAP_STACKED
    (
        Key_X, ___, ___, ___, ___, ___, ___,
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

class ConvertXtoY : public kaleidoscope::Plugin {
 public:
  EventHandlerResult onKeyEvent(KeyEvent &event) {
    if (keyToggledOn(event.state)) {
      if (event.key == Key_X)
        event.key = Key_Y;
    }
    // It should be impossible to get a `Key_X` at this point, because the
    // previous block changes any `Key_X` to `Key_Y`, which results in the
    // active keys cache storing that value. On subsequent cycles (while the key
    // is still pressed), the `key` value should be `Key_Y`.
    if (keyIsPressed(event.state) && event.key == Key_X) {
      std::cerr << "t=" << Runtime.millisAtCycleStart() << ": "
                << "Error: we shouldn't see a key with value `X`" << std::endl;
    }
    // When `Key_Y` toggles off, return `EVENT_CONSUMED`. Despite this, the
    // active keys cache entry should be cleared. If it's not, then a subsequent
    // press of the same key without the layer shift in effect will still result
    // in `Key_Y` instead of `Key_A`.
    if (keyToggledOff(event.state) && (event.key == Key_Y)) {
      return EventHandlerResult::EVENT_CONSUMED;
    }
    return EventHandlerResult::OK;
  }
};

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::ConvertXtoY ConvertXtoY;

KALEIDOSCOPE_INIT_PLUGINS(ConvertXtoY);

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}
