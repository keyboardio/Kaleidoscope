/* -*- mode: c++ -*-
 * Kaleidoscope-LED-AlphaSquare -- 4x4 pixel LED alphabet
 * Copyright (C) 2017, 2018  Keyboard.io, Inc
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

#include <Kaleidoscope-LED-AlphaSquare.h>

namespace kaleidoscope {
namespace plugin {

uint16_t AlphaSquareEffect::length = 1000;

AlphaSquareEffect::TransientLEDMode::TransientLEDMode(AlphaSquareEffect */*parent*/)
  : last_key_left_(Key_NoKey),
    last_key_right_(Key_NoKey)
{}

void AlphaSquareEffect::TransientLEDMode::update(void) {
  if (!Kaleidoscope.has_leds)
    return;

  if (last_key_left_ != Key_NoKey &&
      Kaleidoscope.hasTimeExpired(start_time_left_, length)) {
    ::AlphaSquare.clear(last_key_left_);
    last_key_left_ = Key_NoKey;
  }
  if (last_key_right_ != Key_NoKey &&
      Kaleidoscope.hasTimeExpired(start_time_right_, length)) {
    ::AlphaSquare.clear(last_key_right_, 10);
    last_key_right_ = Key_NoKey;
  }
}

void AlphaSquareEffect::TransientLEDMode::refreshAt(byte row, byte col) {
  bool timed_out;
  uint8_t display_col = 2;
  Key key = last_key_left_;

  if (col < COLS / 2) {
    timed_out = Kaleidoscope.hasTimeExpired(start_time_left_, length);
  } else {
    key = last_key_right_;
    display_col = 10;
    timed_out = Kaleidoscope.hasTimeExpired(start_time_right_, length);
  }

  if (!::AlphaSquare.isSymbolPart(key, 0, display_col, row, col) || timed_out)
    ::LEDControl.setCrgbAt(row, col, CRGB(0, 0, 0));
}

EventHandlerResult AlphaSquareEffect::onKeyswitchEvent(Key &mappedKey, byte row, byte col, uint8_t keyState) {
  if (!Kaleidoscope.has_leds)
    return EventHandlerResult::OK;

  if (::LEDControl.get_mode_index() != led_mode_id_)
    return EventHandlerResult::OK;

  if (keyState & INJECTED)
    return EventHandlerResult::OK;

  if (mappedKey < Key_A || mappedKey > Key_0)
    return EventHandlerResult::OK;

  if (!keyIsPressed(keyState))
    return EventHandlerResult::OK;

  uint8_t display_col = 2;
  auto this_led_mode = ::LEDControl.get_mode<TransientLEDMode>();

  Key prev_key = this_led_mode->last_key_left_;

  if (col < COLS / 2) {
    this_led_mode->last_key_left_ = mappedKey;
    this_led_mode->start_time_left_ = Kaleidoscope.millisAtCycleStart();
  } else {
    prev_key = this_led_mode->last_key_right_;
    this_led_mode->last_key_right_ = mappedKey;
    this_led_mode->start_time_right_ = Kaleidoscope.millisAtCycleStart();
    display_col = 10;
  }

  if (prev_key != mappedKey)
    ::AlphaSquare.clear(prev_key, display_col);
  ::AlphaSquare.display(mappedKey, display_col);

  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::AlphaSquareEffect AlphaSquareEffect;
