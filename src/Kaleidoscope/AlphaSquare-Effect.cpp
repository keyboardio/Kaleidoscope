/* -*- mode: c++ -*-
 * Kaleidoscope-LED-AlphaSquare -- 4x4 pixel LED alphabet
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

#include <Kaleidoscope-LED-AlphaSquare.h>

namespace kaleidoscope {

uint16_t AlphaSquareEffect::length = 1000;
uint32_t AlphaSquareEffect::end_time_left_, AlphaSquareEffect::end_time_right_;
Key AlphaSquareEffect::last_key_left_, AlphaSquareEffect::last_key_right_;
uint8_t AlphaSquareEffect::us_;

AlphaSquareEffect::AlphaSquareEffect(void) {
}

void
AlphaSquareEffect::begin(void) {
  Kaleidoscope.useEventHandlerHook(eventHandlerHook);
  Kaleidoscope.use(&LEDControl);
  us_ = LEDControl.mode_add(this);
}

void
AlphaSquareEffect::update(void) {
  if (end_time_left_ && millis() > end_time_left_) {
    ::AlphaSquare.clear(last_key_left_);
    end_time_left_ = 0;
  }
  if (end_time_right_ && millis() > end_time_right_) {
    ::AlphaSquare.clear(last_key_right_, 10);
    end_time_right_ = 0;
  }
}

Key
AlphaSquareEffect::eventHandlerHook(Key key, byte row, byte col, uint8_t key_state) {
  if (LEDControl.get_mode() != us_)
    return key;

  if (key_state & INJECTED)
    return key;

  if (key < Key_A || key > Key_0)
    return key;

  if (!keyIsPressed(key_state))
    return key;

  uint8_t display_col = 2;
  Key prev_key = last_key_left_;

  if (col < COLS / 2) {
    last_key_left_ = key;
    end_time_left_ = millis() + length;
  } else {
    prev_key = last_key_right_;
    last_key_right_ = key;
    end_time_right_ = millis() + length;
    display_col = 10;
  }

  if (prev_key != key)
    ::AlphaSquare.clear(prev_key, display_col);
  ::AlphaSquare.display(key, display_col);
  return key;
}

}

kaleidoscope::AlphaSquareEffect AlphaSquareEffect;
