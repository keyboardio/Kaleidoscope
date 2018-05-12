/* -*- mode: c++ -*-
 * Kaleidoscope-Escape-OneShot -- Turn ESC into a key that cancels OneShots, if active.
 * Copyright (C) 2016, 2017, 2018  Gergely Nagy
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

EventHandlerResult EscapeOneShot::onKeyswitchEvent(Key &mapped_key, byte row, byte col, uint8_t keyState) {
  if (mapped_key.raw != Key_Escape.raw ||
      (keyState & INJECTED) ||
      !keyToggledOn(keyState))
    return EventHandlerResult::OK;

  if (!::OneShot.isActive())
    return EventHandlerResult::OK;

  KeyboardHardware.maskKey(row, col);

  ::OneShot.cancel(true);
  return EventHandlerResult::EVENT_CONSUMED;
}

// Legacy V1 API
#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
void EscapeOneShot::begin() {
  Kaleidoscope.useEventHandlerHook(legacyEventHandler);
}

Key EscapeOneShot::legacyEventHandler(Key mapped_key, byte row, byte col, uint8_t keyState) {
  EventHandlerResult r = ::EscapeOneShot.onKeyswitchEvent(mapped_key, row, col, keyState);
  if (r == EventHandlerResult::OK)
    return mapped_key;
  return Key_NoKey;
}
#endif


}

kaleidoscope::EscapeOneShot EscapeOneShot;
