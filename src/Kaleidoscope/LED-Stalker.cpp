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

#include <Kaleidoscope-LED-Stalker.h>
#include <LEDUtils.h>

namespace KaleidoscopePlugins {
namespace LEDEffects {
uint8_t StalkerEffect::map[ROWS][COLS];
StalkerEffect::ColorComputer *StalkerEffect::colorComputer;
uint16_t StalkerEffect::stepLength = 50;
uint32_t StalkerEffect::stepEndTime;

StalkerEffect::StalkerEffect (void) {
}

void
StalkerEffect::configure (ColorComputer *colorComputer_) {
    colorComputer = colorComputer_;
}

void
StalkerEffect::begin (void) {
    event_handler_hook_use (eventHandlerHook);
    LEDMode::begin ();
}

void
StalkerEffect::init (void) {
    memset (map, 0, sizeof (map));
}

Key
StalkerEffect::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (row >= ROWS || col >= COLS)
        return mappedKey;

    if (key_is_pressed (keyState)) {
        map[row][col] = 0xff;
    }

    return mappedKey;
}

void
StalkerEffect::update (void) {
    if (!colorComputer)
        return;

    bool timeOut = millis () >= stepEndTime;

    for (byte r = 0; r < ROWS; r++) {
        for (byte c = 0; c < COLS; c++) {
            uint8_t step = map[r][c];
            if (step) {
                LEDControl.led_set_crgb_at (r, c, colorComputer->compute (&step));
            }

            bool wasZero = (map[r][c] == 0);

            if (timeOut) {
                map[r][c] = step;
            }

            if (!wasZero && !map[r][c])
                LEDControl.led_set_crgb_at (r, c, (cRGB) {
                0, 0, 0
            });
        }
    }

    if (timeOut)
        stepEndTime = millis () + stepLength;
}

namespace Stalker {

cRGB Haunt::highlightColor;

// Haunt
Haunt::Haunt (const cRGB highlightColor_) {
    highlightColor = highlightColor_;
}

cRGB
Haunt::compute (uint8_t *step) {
    cRGB color = CRGB((uint8_t)min(*step * highlightColor.r / 255, 255),
                      (uint8_t)min(*step * highlightColor.g / 255, 255),
                      (uint8_t)min(*step * highlightColor.b / 255, 255));

    if (*step >= 0xf0)
        *step -= 1;
    else if (*step >= 0x40)
        *step -= 16;
    else if (*step >= 32)
        *step -= 32;
    else
        *step = 0;

    return color;
}

// BlazingTrail
BlazingTrail::BlazingTrail (...) {
}

cRGB
BlazingTrail::compute (uint8_t *step) {
    cRGB color;

    if (*step >= 0xff - 30) {
        color = hsv_to_rgb (0xff - *step, 255, 255);
    } else {
        color = hsv_to_rgb (30, 255, 255);

        color.r = min(*step * color.r / 255, 255);
        color.g = min(*step * color.g / 255, 255);
    }

    if (*step >= 0xf0 - 30)
        *step -= 1;
    else if (*step >= 0x40)
        *step -= 16;
    else if (*step >= 32)
        *step -= 32;
    else
        *step = 0;

    return color;
}

};

};
};

KaleidoscopePlugins::LEDEffects::StalkerEffect StalkerEffect;
