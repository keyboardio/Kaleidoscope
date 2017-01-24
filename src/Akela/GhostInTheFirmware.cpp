/* -*- mode: c++ -*-
 * Akela -- Animated Keyboardio Extension Library for Anything
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

#include <Akela-GhostInTheFirmware.h>

namespace Akela {
  GhostInTheFirmware::GhostKey *GhostInTheFirmware::ghostKeys;
  bool GhostInTheFirmware::isActive;
  uint16_t GhostInTheFirmware::currentPos;
  uint8_t GhostInTheFirmware::timer;
  uint8_t GhostInTheFirmware::timeOut;

  GhostInTheFirmware::GhostInTheFirmware (void) {
  }

  void
  GhostInTheFirmware::begin (void) {
    loop_hook_add (this->loopHook);
  }

  void
  GhostInTheFirmware::activate (void) {
    isActive = true;
  }

  void
  GhostInTheFirmware::configure (const GhostKey ghostKeys_[]) {
    ghostKeys = (GhostKey *)ghostKeys_;
  }

  void
  GhostInTheFirmware::loopHook (bool postClear) {
    if (postClear || !isActive)
      return;

    if (timeOut == 0) {
      timeOut = pgm_read_byte (&(ghostKeys[currentPos].delay));

      if (timeOut == 0) {
        currentPos = 0;
        isActive = false;
        return;
      }
    } else if (timer == timeOut / 3 && timer != 0) {
      byte row = pgm_read_byte (&(ghostKeys[currentPos].row));
      byte col = pgm_read_byte (&(ghostKeys[currentPos].col));

      handle_key_event (Key_NoKey, row, col, WAS_PRESSED);
    } else if (timer < timeOut / 3) {
      byte row = pgm_read_byte (&(ghostKeys[currentPos].row));
      byte col = pgm_read_byte (&(ghostKeys[currentPos].col));

      handle_key_event (Key_NoKey, row, col, IS_PRESSED);
    }

    timer++;

    if (timer > timeOut) {
      currentPos++;
      timer = timeOut = 0;
    }
  }

};

Akela::GhostInTheFirmware GhostInTheFirmware;
