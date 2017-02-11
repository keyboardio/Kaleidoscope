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

namespace KaleidoscopePlugins {
  uint8_t Heatmap::heatmap[ROWS][COLS];
  uint16_t Heatmap::totalKeys;
  uint8_t Heatmap::highestCount;
  uint16_t Heatmap::updateFrequency = 500;
  uint32_t Heatmap::startTime;

  const float Heatmap::heatColors[][3] = {{0.0, 0.0, 0.0}, {0.1, 1, 0.1}, {1, 1, 0.1}, {1, 0.1, 0.1}};

  void
  Heatmap::shiftStats (void) {
    highestCount = totalKeys = 0;
    for (uint8_t r = 0; r < ROWS; r++) {
      for (uint8_t c = 0; c < COLS; c++) {
        heatmap[r][c] = heatmap[r][c] >> 1;
        totalKeys += heatmap[r][c];
        if (heatmap[r][c] > highestCount)
          highestCount = heatmap[r][c];
      }
    }
  }

  cRGB
  Heatmap::computeColor (float v) {
    float fb = 0;
    uint8_t idx1, idx2;

    if (v <= 0) {
      idx1 = idx2 = 0;
    } else if (v >= 1) {
      idx1 = idx2 = 3;
    } else {
      float val = v * 3;
      idx1 = int(floor(val));
      idx2 = idx1 + 1;
      fb = val - float(idx1);
    }

    uint8_t r = (int)(((heatColors[idx2][0] - heatColors[idx1][0]) * fb + heatColors[idx1][0]) * 255);
    uint8_t g = (int)(((heatColors[idx2][1] - heatColors[idx1][1]) * fb + heatColors[idx1][1]) * 255);
    uint8_t b = (int)(((heatColors[idx2][2] - heatColors[idx1][2]) * fb + heatColors[idx1][2]) * 255);

    return {b, g, r};
  }

  Heatmap::Heatmap (void) {
  }

  void
  Heatmap::begin (void) {
    LEDControl.mode_add (this);
    event_handler_hook_use (this->eventHook);
    loop_hook_use (this->loopHook);
  }

  void
  Heatmap::configure (uint16_t updateFreq) {
    updateFrequency = updateFreq;
  }

  Key
  Heatmap::eventHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    // if it is a synthetic key, skip it.
    if (keyState & INJECTED)
      return mappedKey;

    // if the key is not toggled on, return.
    if (!key_toggled_on (keyState))
      return mappedKey;

    totalKeys++;
    heatmap[row][col]++;
    if (heatmap[row][col] > highestCount)
      highestCount = heatmap[row][col];

    return mappedKey;
  }

  void
  Heatmap::loopHook (bool postClear) {
    if (highestCount > 191 || totalKeys > 16000)
      shiftStats();
  }

  void
  Heatmap::update (void) {
    if (startTime && (millis () - startTime) < updateFrequency)
      return;

    startTime = millis ();

    for (uint8_t r = 0; r < ROWS; r++) {
      for (uint8_t c = 0; c < COLS; c++) {
        uint8_t cap = max(totalKeys, 1);
        float v = float(heatmap[r][c]) / cap;
        LEDControl.led_set_crgb_at (r, c, computeColor (v));
      }
    }
  }
};

KaleidoscopePlugins::Heatmap HeatmapEffect;
