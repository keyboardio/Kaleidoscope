/* -*- mode: c++ -*-
 * Kaleidoscope-Colormap -- Per-layer colormap effect
 * Copyright (C) 2016, 2017, 2018  Keyboard.io, Inc
 *
 * This program is free software: you can redistribute it and/or modify it under it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LED-Palette-Theme.h>

namespace kaleidoscope {
class ColormapEffect : public LEDMode {
 public:
  ColormapEffect(void) {}

  void max_layers(uint8_t max_);

  EventHandlerResult onFocusEvent(const char *command);

 protected:
  void onActivate(void) final;
  void update(void) final;
  void refreshAt(byte row, byte col) final;

 private:
  static uint8_t last_highest_layer_;
  static uint8_t max_layers_;
  static uint16_t map_base_;
};
}

extern kaleidoscope::ColormapEffect ColormapEffect;
