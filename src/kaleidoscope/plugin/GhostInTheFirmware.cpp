/* -*- mode: c++ -*-
 * Kaleidoscope-GhostInTheFirmware -- Let the keyboard write for you!
 * Copyright (C) 2017  Keyboard.io, Inc
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

#include "kaleidoscope/Runtime.h"
#include <Kaleidoscope-GhostInTheFirmware.h>
#include "kaleidoscope/keyswitch_state.h"

namespace kaleidoscope {
namespace plugin {
const GhostInTheFirmware::GhostKey *GhostInTheFirmware::ghost_keys;
bool GhostInTheFirmware::is_active_;
bool GhostInTheFirmware::is_pressed_;
uint16_t GhostInTheFirmware::current_pos_;
uint32_t GhostInTheFirmware::start_time_;
uint16_t GhostInTheFirmware::press_timeout_;
uint16_t GhostInTheFirmware::delay_timeout_;

void GhostInTheFirmware::activate(void) {
  is_active_ = true;
}

EventHandlerResult GhostInTheFirmware::beforeReportingState() {
  if (!is_active_)
    return EventHandlerResult::OK;

  if (press_timeout_ == 0) {
    press_timeout_ = pgm_read_word(&(ghost_keys[current_pos_].pressTime));
    delay_timeout_ = pgm_read_word(&(ghost_keys[current_pos_].delay));

    if (press_timeout_ == 0) {
      current_pos_ = 0;
      is_active_ = false;
      return EventHandlerResult::OK;
    }
    is_pressed_ = true;
    start_time_ = Runtime.millisAtCycleStart();
  } else {
    if (is_pressed_ && Runtime.hasTimeExpired(start_time_, press_timeout_)) {
      is_pressed_ = false;
      start_time_ = Runtime.millisAtCycleStart();

      byte row = pgm_read_byte(&(ghost_keys[current_pos_].row));
      byte col = pgm_read_byte(&(ghost_keys[current_pos_].col));

      handleKeyswitchEvent(Key_NoKey, KeyAddr(row, col), WAS_PRESSED);
    } else if (is_pressed_) {
      byte row = pgm_read_byte(&(ghost_keys[current_pos_].row));
      byte col = pgm_read_byte(&(ghost_keys[current_pos_].col));

      handleKeyswitchEvent(Key_NoKey, KeyAddr(row, col), IS_PRESSED);
    } else if (Runtime.hasTimeExpired(start_time_, delay_timeout_)) {
      current_pos_++;
      press_timeout_ = 0;
    }
  }

  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::GhostInTheFirmware GhostInTheFirmware;
