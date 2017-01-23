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

#include <Akela-LED-Stalker.h>

namespace Akela {
  namespace LEDEffects {
    uint8_t StalkerEffect::map[ROWS][COLS];
    StalkerEffect::ColorComputer *StalkerEffect::colorComputer;

    StalkerEffect::StalkerEffect (void) {
    }

    void
    StalkerEffect::configure (ColorComputer *colorComputer_) {
      colorComputer = colorComputer_;
    }

    void
    StalkerEffect::begin (void) {
      event_handler_hook_add (eventHandlerHook);
      loop_hook_add (loopHook);
    }

     Key
    StalkerEffect::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
      if (row >= ROWS || col >= COLS)
        return mappedKey;

      if (key_is_pressed (keyState))
        map[row][col] = 0xff;

      return mappedKey;
    }

    void
    StalkerEffect::loopHook (bool postClear) {
      if (postClear)
        return;

      if (!colorComputer)
        return;

      for (byte r = 0; r < ROWS; r++) {
        for (byte c = 0; c < COLS; c++) {
          if (map[r][c])
            led_set_crgb_at (r, c, colorComputer->compute (map[r][c]));

          if (map[r][c] >= 0xf0)
            map[r][c]--;
          else if (map[r][c] >= 0x40)
            map[r][c] -= 16;
          else if (map[r][c] >= 32)
            map[r][c] -= 32;
          else {
            map[r][c] = 0;
            led_set_crgb_at (r, c, (cRGB){0, 0, 0});
          }
        }
      }
    }

    namespace Stalker {

      // Haunt
      float Haunt::mb;
      float Haunt::mg;
      float Haunt::mr;

      Haunt::Haunt (const cRGB highlightColor) {
        mb = highlightColor.b / 255.0;
        mg = highlightColor.g / 255.0;
        mr = highlightColor.r / 255.0;
      }

      cRGB
      Haunt::compute (uint8_t step) {
        cRGB color = {(uint8_t)min(step * mb, 255),
                      (uint8_t)min(step * mg, 255),
                      (uint8_t)min(step * mr, 255)};

        return color;
      }

      // BlazingTrail
      BlazingTrail::BlazingTrail (...) {
      }

      cRGB
      BlazingTrail::compute (uint8_t step) {
        cRGB color;

        color.b = 0;
        color.r = step;

        if (step >= 0xf0) {
        } else if (step >= 0x80) {
          color.g = 0xa0 - step / 2;
        } else
          color.g = step;

        return color;
      }

    };

  };
};

Akela::LEDEffects::StalkerEffect StalkerEffect;
