/* -*- mode: c++ -*-
 * Kaleidoscope-Heatmap -- Heatmap LED effect for Kaleidoscope.
 * Copyright (C) 2016, 2017  Gergely Nagy
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

#include <Kaleidoscope.h>
#include <Kaleidoscope-Heatmap.h>

namespace kaleidoscope {

uint8_t Heatmap::heatmap_[ROWS][COLS];
uint16_t Heatmap::total_keys_;
uint8_t Heatmap::highest_count_;
uint16_t Heatmap::update_delay = 500;
uint32_t Heatmap::end_time_;

const float Heatmap::heat_colors_[][3] = {{0.0, 0.0, 0.0}, {0.1, 1, 0.1}, {1, 1, 0.1}, {1, 0.1, 0.1}};

void Heatmap::shiftStats(void) {
  highest_count_ = total_keys_ = 0;
  for (uint8_t r = 0; r < ROWS; r++) {
    for (uint8_t c = 0; c < COLS; c++) {
      heatmap_[r][c] = heatmap_[r][c] >> 1;
      total_keys_ += heatmap_[r][c];
      if (heatmap_[r][c] > highest_count_)
        highest_count_ = heatmap_[r][c];
    }
  }
}

cRGB Heatmap::computeColor(float v) {
  float fb = 0;
  uint8_t idx1, idx2;

  if (v <= 0) {
    idx1 = idx2 = 0;
  } else if (v >= 1) {
    idx1 = idx2 = 3;
  } else {
    float val = v * 3;
    idx1 = static_cast<int>(floor(val));
    idx2 = idx1 + 1;
    fb = val - static_cast<float>(idx1);
  }

  uint8_t r = static_cast<uint8_t>(((heat_colors_[idx2][0] - heat_colors_[idx1][0]) * fb + heat_colors_[idx1][0]) * 255);
  uint8_t g = static_cast<uint8_t>(((heat_colors_[idx2][1] - heat_colors_[idx1][1]) * fb + heat_colors_[idx1][1]) * 255);
  uint8_t b = static_cast<uint8_t>(((heat_colors_[idx2][2] - heat_colors_[idx1][2]) * fb + heat_colors_[idx1][2]) * 255);

  return {b, g, r};
}

Heatmap::Heatmap(void) {
}

void Heatmap::setup(void) {
  Kaleidoscope.useEventHandlerHook(eventHook);
  Kaleidoscope.useLoopHook(loopHook);
}

Key Heatmap::eventHook(Key mapped_key, byte row, byte col, uint8_t key_state) {
  // if it is a synthetic key, skip it.
  if (key_state & INJECTED)
    return mapped_key;

  // if the key is not toggled on, return.
  if (!keyToggledOn(key_state))
    return mapped_key;

  total_keys_++;
  heatmap_[row][col]++;
  if (heatmap_[row][col] > highest_count_)
    highest_count_ = heatmap_[row][col];

  return mapped_key;
}

void
Heatmap::loopHook(bool is_post_clear) {
  if (highest_count_ > 191 || total_keys_ > 16000)
    shiftStats();
}

void
Heatmap::update(void) {
  if (end_time_ && (millis() < end_time_))
    return;

  end_time_ = millis() + update_delay;

  for (uint8_t r = 0; r < ROWS; r++) {
    for (uint8_t c = 0; c < COLS; c++) {
      uint8_t cap = max(total_keys_, 1);
      float v = static_cast<float>(heatmap_[r][c]) / cap;
      ::LEDControl.setCrgbAt(r, c, computeColor(v));
    }
  }
}

}

kaleidoscope::Heatmap HeatmapEffect;
