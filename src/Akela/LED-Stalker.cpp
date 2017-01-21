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
    cRGB StalkerEffect::highlightColor = (cRGB) {64, 128, 128};
    uint8_t StalkerEffect::map[ROWS][COLS];

    StalkerEffect::StalkerEffect (void) {
    }

    void
    StalkerEffect::begin (void) {
      event_handler_hook_add (eventHandlerHook);
      loop_hook_add (loopHook);
    }

    void
    StalkerEffect::configure (const cRGB highlightColor_) {
      highlightColor = highlightColor_;
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

      float mb = highlightColor.b / 255.0;
      float mg = highlightColor.g / 255.0;
      float mr = highlightColor.r / 255.0;

      for (byte r = 0; r < ROWS; r++) {
        for (byte c = 0; c < COLS; c++) {
          cRGB color = {(uint8_t)min(map[r][c] * mb, 255),
                        (uint8_t)min(map[r][c] * mg, 255),
                        (uint8_t)min(map[r][c] * mr, 255)};

          if (map[r][c])
            led_set_crgb_at (r, c, color);

          if (map[r][c] >= 0xf0)
            map[r][c]--;
          else if (map[r][c] >= 0x40)
            map[r][c] -= 16;
          else if (map[r][c] >= 32)
            map[r][c] -= 32;
          else
            map[r][c] = 0;
        }
      }
    }

  };
};

Akela::LEDEffects::StalkerEffect StalkerEffect;
