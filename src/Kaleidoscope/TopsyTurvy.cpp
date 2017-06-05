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

#define TOPSYTURVY 0b01000000

namespace kaleidoscope {

const Key *TopsyTurvy::key_list = NULL;
uint8_t TopsyTurvy::mod_state_;

TopsyTurvy::TopsyTurvy(void) {
}

void TopsyTurvy::begin(void) {
  event_handler_hook_use(eventHandlerHook);
}

Key TopsyTurvy::eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state) {
  if (key_state & TOPSYTURVY)
    return mapped_key;

  if (!key_list)
    return mapped_key;

  if (mapped_key.raw == Key_LeftShift.raw)
    bitWrite(mod_state_, 0, key_is_pressed(key_state));
  if (mapped_key.raw == Key_RightShift.raw)
    bitWrite(mod_state_, 1, key_is_pressed(key_state));

  if (!key_is_pressed(key_state) && !key_was_pressed(key_state))
    return mapped_key;

  uint8_t idx = 0;
  Key newKey;

  do {
    newKey.raw = pgm_read_word(&(key_list[idx].raw));
    idx++;
  } while (newKey.raw != mapped_key.raw && newKey.raw != Key_NoKey.raw);

  if (newKey.raw == Key_NoKey.raw)
    return mapped_key;

  // invert the shift state

  if (!mod_state_) {
    if (key_is_pressed(key_state))
      Keyboard.press(Key_LeftShift.keyCode);
    handle_keyswitch_event(mapped_key, row, col, key_state | TOPSYTURVY);
    Keyboard.sendReport();
    if (key_toggled_off(key_state))
      Keyboard.release(Key_LeftShift.keyCode);
  } else {
    Keyboard.release(Key_LeftShift.keyCode);
    Keyboard.release(Key_RightShift.keyCode);
    Keyboard.sendReport();
    handle_keyswitch_event(mapped_key, row, col, key_state | TOPSYTURVY);
    Keyboard.sendReport();

    if (bitRead(mod_state_, 0))
      Keyboard.press(Key_LeftShift.keyCode);
    if (bitRead(mod_state_, 1))
      Keyboard.press(Key_RightShift.keyCode);
  }

  return Key_NoKey;
}

}

kaleidoscope::TopsyTurvy TopsyTurvy;
