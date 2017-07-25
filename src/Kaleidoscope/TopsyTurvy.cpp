/* -*- mode: c++ -*-
 * Kaleidoscope-TopsyTurvy -- Turn the effect of Shift upside down for certain keys
 * Copyright (C) 2017  Gergely Nagy
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

#include <Kaleidoscope-TopsyTurvy.h>
#include "kaleidoscope/hid.h"

#define TOPSYTURVY 0b01000000

namespace kaleidoscope {

uint8_t TopsyTurvy::mod_state_;
uint8_t TopsyTurvy::last_pressed_position_;

TopsyTurvy::TopsyTurvy(void) {
}

void TopsyTurvy::begin(void) {
  event_handler_hook_use(eventHandlerHook);
}

Key TopsyTurvy::eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state) {
  if (key_state & TOPSYTURVY)
    return mapped_key;

  if (mapped_key.raw == Key_LeftShift.raw)
    bitWrite(mod_state_, 0, keyIsPressed(key_state));
  if (mapped_key.raw == Key_RightShift.raw)
    bitWrite(mod_state_, 1, keyIsPressed(key_state));

  if (!keyIsPressed(key_state) && !keyWasPressed(key_state))
    return mapped_key;

  if (mapped_key < ranges::TT_FIRST || mapped_key > ranges::TT_LAST)
    return mapped_key;

  if (keyToggledOn(key_state)) {
    last_pressed_position_ = row * COLS + col;
  } else {
    if (last_pressed_position_ != row * COLS + col)
      return Key_NoKey;
  }

  Key new_key = {.raw = mapped_key.raw - ranges::TT_FIRST};
  if (new_key.raw == Key_NoKey.raw)
    return mapped_key;

  // invert the shift state

  if (!mod_state_) {
    if (keyIsPressed(key_state))
      hid::pressRawKey(Key_LeftShift);
    handleKeyswitchEvent(new_key, row, col, key_state | TOPSYTURVY | INJECTED);
    hid::sendKeyboardReport();
    if (keyToggledOff(key_state))
      hid::releaseRawKey(Key_LeftShift);
  } else {
    hid::releaseRawKey(Key_LeftShift);
    hid::releaseRawKey(Key_RightShift);
    hid::sendKeyboardReport();
    handleKeyswitchEvent(new_key, row, col, key_state | TOPSYTURVY | INJECTED);
    hid::sendKeyboardReport();

    if (bitRead(mod_state_, 0))
      hid::pressRawKey(Key_LeftShift);
    if (bitRead(mod_state_, 1))
      hid::pressRawKey(Key_RightShift);
  }

  return Key_NoKey;
}

}

kaleidoscope::TopsyTurvy TopsyTurvy;
