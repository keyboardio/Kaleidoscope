/* -*- mode: c++ -*-
 * Copyright (C) 2022  iliana etaoin <iliana@buttslol.net>
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
#include <Kaleidoscope-PrefixLayer.h>

// clang-format off
KEYMAPS(
  [0] = KEYMAP_STACKED
  (ShiftToLayer(1), ___, ___, ___, ___, ___, ___,
   Key_LeftControl, Key_LeftShift, ___, ___, ___, ___, ___,
   Key_H, Key_J, Key_K, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___,
   ___,

   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
        ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___,
   ___),

  [1] = KEYMAP_STACKED
  (___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, Key_DownArrow, XXX, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___,
   ___,

   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
        ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___,
   ___),
)
// clang-format on

static const kaleidoscope::plugin::PrefixLayer::Entry prefix_layers[] PROGMEM = {
  kaleidoscope::plugin::PrefixLayer::Entry(1, LCTRL(Key_B)),
};

KALEIDOSCOPE_INIT_PLUGINS(PrefixLayer);

void setup() {
  Kaleidoscope.setup();
  PrefixLayer.setPrefixLayers(prefix_layers);
}

void loop() {
  Kaleidoscope.loop();
}
