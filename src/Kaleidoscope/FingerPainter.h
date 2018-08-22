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
class FingerPainter : public LEDMode {
 public:
  FingerPainter(void) {}

  static void toggle(void);
  static bool focusHook(const char *command);

  EventHandlerResult onKeyswitchEvent(Key &mapped_key, byte row, byte col, uint8_t key_state);
  EventHandlerResult onSetup();

 protected:
  void update(void) final;
  void refreshAt(byte row, byte col) final;

 private:
  static uint16_t color_base_;
  static bool edit_mode_;
};
};

extern kaleidoscope::FingerPainter FingerPainter;

#define FOCUS_HOOK_FINGERPAINTER FOCUS_HOOK(FingerPainter.focusHook,  \
                                            "fingerpainter.toggle\n"  \
                                            "fingerpainter.clear")
