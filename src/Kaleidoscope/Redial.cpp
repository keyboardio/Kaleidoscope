/* -*- mode: c++ -*-
 * Kaleidoscope-Redial -- Redial support for Kaleidoscope
 * Copyright (C) 2018  Gergely Nagy
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

#include <Kaleidoscope-Redial.h>

namespace kaleidoscope {

Key Redial::key;
Key Redial::key_to_redial_;

Key Redial::eventHandlerHook_(Key mapped_key, byte row, byte col, uint8_t key_state) {
  if (key == Key_NoKey)
    return mapped_key;

  if (mapped_key == key) {
    return key_to_redial_;
  }

  if (keyToggledOn(key_state) && shouldRemember(mapped_key)) {
    key_to_redial_ = mapped_key;
  }

  return mapped_key;
}

void Redial::begin(void) {
  Kaleidoscope.useEventHandlerHook(eventHandlerHook_);
}

__attribute__((weak)) bool Redial::shouldRemember(Key mapped_key) {
  if (mapped_key >= Key_A && mapped_key <= Key_Z)
    return true;
  if (mapped_key >= Key_1 && mapped_key <= Key_0)
    return true;

  return false;
}

}

kaleidoscope::Redial Redial;
