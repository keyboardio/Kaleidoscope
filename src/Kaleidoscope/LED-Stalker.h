/* -*- mode: c++ -*-
 * Kaleidoscope-LED-Stalker -- Stalk keys pressed by lighting up and fading back the LED under them
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

#pragma once

#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>

#define STALKER(v, ...) ({static kaleidoscope::stalker::v _effect __VA_ARGS__; &_effect;})

namespace kaleidoscope {
class StalkerEffect : public LEDMode {
 public:
  class ColorComputer {
   public:
    virtual cRGB compute(uint8_t *step) = 0;
  };

  StalkerEffect(void) {}

  static ColorComputer *variant;
  static uint16_t step_length;

 protected:
  void setup(void) final;
  void onActivate(void) final;
  void update(void) final;

 private:
  static uint32_t step_end_time_;
  static uint8_t map_[ROWS][COLS];

  static Key eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state);
};

namespace stalker {

class Haunt : public StalkerEffect::ColorComputer {
 public:
  explicit Haunt(const cRGB highlight_color);
  Haunt(void) : Haunt(CRGB(0x40, 0x80, 0x80)) {}

  cRGB compute(uint8_t *step) final;
 private:
  static cRGB highlight_color_;
};

class BlazingTrail : public StalkerEffect::ColorComputer {
 public:
  BlazingTrail(void);

  cRGB compute(uint8_t *step) final;
};

}
}

extern kaleidoscope::StalkerEffect StalkerEffect;
