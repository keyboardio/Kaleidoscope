/* -*- mode: c++ -*-
 * Kaleidoscope-Heatmap -- Heatmap LED effect for Kaleidoscope.
 * Copyright (C) 2016, 2017, 2018  Gergely Nagy
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

#pragma once

#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>

namespace kaleidoscope {
namespace plugin {
class Heatmap : public LEDMode {
 public:
  Heatmap(void) {}

  static uint16_t update_delay;
  static const cRGB *heat_colors;
  static uint8_t heat_colors_length;
  void resetMap(void);

  EventHandlerResult onKeyswitchEvent(Key &mapped_key, byte row, byte col, uint8_t key_state);
  EventHandlerResult beforeEachCycle();

 protected:
  void update(void) final;

 private:
  static uint16_t heatmap_[ROWS][COLS];
  static uint16_t highest_;
  static uint32_t next_heatmap_comp_time_;

  static void shiftStats(void);
  static cRGB computeColor(float v);
};
}
}

extern kaleidoscope::plugin::Heatmap HeatmapEffect;
