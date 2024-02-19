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

#include "kaleidoscope/plugin/LED-AlphaSquare/Effect.h"

#include <stdint.h>  // for uint16_t, uint8_t

#include "kaleidoscope/KeyAddr.h"                 // for KeyAddr
#include "kaleidoscope/KeyEvent.h"                // for KeyEvent
#include "kaleidoscope/Runtime.h"                 // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"           // for Device, CRGB
#include "kaleidoscope/event_handler_result.h"    // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/key_defs.h"                // for Key, Key_NoKey, Key_0, Key_A
#include "kaleidoscope/keyswitch_state.h"         // for keyIsInjected
#include "kaleidoscope/plugin/LED-AlphaSquare.h"  // for AlphaSquare
#include "kaleidoscope/plugin/LEDControl.h"       // for LEDControl

namespace kaleidoscope {
namespace plugin {

uint16_t AlphaSquareEffect::length = 1000;

AlphaSquareEffect::TransientLEDMode::TransientLEDMode(AlphaSquareEffect * /*parent*/)  // NOLINT(readability/casting)
  : last_key_left_(Key_NoKey),
    last_key_right_(Key_NoKey) {}

void AlphaSquareEffect::TransientLEDMode::update() {
  if (!Runtime.has_leds)
    return;

  if (last_key_left_ != Key_NoKey &&
      Runtime.hasTimeExpired(start_time_left_, length)) {
    ::AlphaSquare.clear(last_key_left_);
    last_key_left_ = Key_NoKey;
  }
  if (last_key_right_ != Key_NoKey &&
      Runtime.hasTimeExpired(start_time_right_, length)) {
    ::AlphaSquare.clear(last_key_right_, 10);
    last_key_right_ = Key_NoKey;
  }
}

void AlphaSquareEffect::TransientLEDMode::refreshAt(KeyAddr key_addr) {
  bool timed_out;
  uint8_t display_col = 2;
  Key key             = last_key_left_;

  if (key_addr.col() < Runtime.device().matrix_columns / 2) {
    timed_out = Runtime.hasTimeExpired(start_time_left_, length);
  } else {
    key         = last_key_right_;
    display_col = 10;
    timed_out   = Runtime.hasTimeExpired(start_time_right_, length);
  }

  if (!::AlphaSquare.isSymbolPart(key, KeyAddr(0, display_col), key_addr) || timed_out)
    ::LEDControl.setCrgbAt(key_addr, CRGB(0, 0, 0));
}

EventHandlerResult AlphaSquareEffect::onKeyEvent(KeyEvent &event) {
  if (!Runtime.has_leds)
    return EventHandlerResult::OK;

  if (::LEDControl.get_mode_index() != led_mode_id_)
    return EventHandlerResult::OK;

  if (keyIsInjected(event.state))
    return EventHandlerResult::OK;

  if (event.key < Key_A || event.key > Key_0)
    return EventHandlerResult::OK;

  // if (!keyIsPressed(event.state))
  //   return EventHandlerResult::OK;

  uint8_t display_col = 2;
  auto this_led_mode  = ::LEDControl.get_mode<TransientLEDMode>();

  Key prev_key = this_led_mode->last_key_left_;

  if (event.addr.col() < Runtime.device().matrix_columns / 2) {
    this_led_mode->last_key_left_   = event.key;
    this_led_mode->start_time_left_ = Runtime.millisAtCycleStart();
  } else {
    prev_key                         = this_led_mode->last_key_right_;
    this_led_mode->last_key_right_   = event.key;
    this_led_mode->start_time_right_ = Runtime.millisAtCycleStart();
    display_col                      = 10;
  }

  if (prev_key != event.key)
    ::AlphaSquare.clear(prev_key, display_col);
  ::AlphaSquare.display(event.key, display_col);

  return EventHandlerResult::OK;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::AlphaSquareEffect AlphaSquareEffect;
