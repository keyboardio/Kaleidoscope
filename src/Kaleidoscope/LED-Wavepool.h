/* -*- mode: c++ -*-
 * Kaleidoscope-LED-Wavepool
 * Copyright (C) 2017 Selene Scriven
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

#define WAVEPOOL(v, ...) ({static kaleidoscope::wavepool::v _effect __VA_ARGS__; &_effect;})

#define WP_WID 14
#define WP_HGT 5

namespace kaleidoscope {
class WavepoolEffect : public LEDMode {
 public:
  class ColorComputer {
   public:
    virtual cRGB compute(uint8_t *step) = 0;
  };

  WavepoolEffect(void);

  void begin(void) final;
  void init(void) final;
  void update(void) final;

 private:
  static uint8_t frames_since_event;
  static int8_t map_[2][WP_WID*WP_HGT];
  static uint8_t page;
  //static uint8_t positions[WP_HGT*WP_WID];  // unused
  static uint8_t rc2pos[ROWS*COLS];

  static Key eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state);
};

}

extern kaleidoscope::WavepoolEffect WavepoolEffect;
