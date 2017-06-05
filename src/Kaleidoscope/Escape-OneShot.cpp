/* -*- mode: c++ -*-
 * Kaleidoscope-Escape-OneShot -- Turn ESC into a key that cancels OneShots, if active.
 * Copyright (C) 2016, 2017  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Kaleidoscope.h>
#include <Kaleidoscope-OneShot.h>
#include <Kaleidoscope-Escape-OneShot.h>

namespace kaleidoscope {

EscapeOneShot::EscapeOneShot(void) {
}

void EscapeOneShot::begin(void) {
  event_handler_hook_use(eventHandlerHook);
}

Key EscapeOneShot::eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state) {
  if (mapped_key.raw != Key_Escape.raw ||
      (key_state & INJECTED) ||
      !key_toggled_on(key_state))
    return mapped_key;

  if (!::OneShot.isActive())
    return mapped_key;

  ::OneShot.cancel();

  return Key_NoKey;
}

}

kaleidoscope::EscapeOneShot EscapeOneShot;
