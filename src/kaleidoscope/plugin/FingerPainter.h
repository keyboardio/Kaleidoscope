/* -*- mode: c++ -*-
 * Kaleidoscope-FingerPainter -- On-the-fly keyboard painting.
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

#pragma once

#include <Kaleidoscope-LEDControl.h>

namespace kaleidoscope {
namespace plugin {
class FingerPainter : public LEDMode {
 public:
  FingerPainter(void) {}

  static void toggle(void);

  EventHandlerResult onKeyswitchEvent2(Key &mapped_key, KeyAddr key_addr, uint8_t key_state);
  EventHandlerResult onFocusEvent(const char *command);
  EventHandlerResult onSetup();

 protected:
  void update(void) final;
  void refreshAt(LEDAddr led_addr) final;
  KS_ROW_COL_FUNC void refreshAt(byte row, byte col) final {
    refreshAt(LEDAddr(row, col));
  }

 private:
  static uint16_t color_base_;
  static bool edit_mode_;
};
}
}

extern kaleidoscope::plugin::FingerPainter FingerPainter;
