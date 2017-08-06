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

#include <Kaleidoscope-LED-Wavepool.h>
#include <LEDUtils.h>

namespace kaleidoscope {

int8_t WavepoolEffect::surface[2][WP_WID*WP_HGT];
uint8_t WavepoolEffect::page = 0;
uint8_t WavepoolEffect::frames_since_event = 0;
uint16_t WavepoolEffect::idle_timeout = 5000;  // 5 seconds
/* unused
// map geometric space (14x5) into native keyboard coordinates (16x4)
PROGMEM const uint8_t WavepoolEffect::positions[WP_HGT*WP_WID] = {
     0,  1,  2,  3,  4,  5,  6,    9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 64,   64, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 22,   25, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 37,   40, 58, 59, 60, 61, 62, 63,
    64, 64,  7, 23, 39, 55, 54,   57, 56, 40, 24,  8, 64, 64,
};
*/
// map native keyboard coordinates (16x4) into geometric space (14x5)
PROGMEM const uint8_t WavepoolEffect::rc2pos[ROWS*COLS] = {
     0,  1,  2,  3,  4,  5,  6,    59, 66,    7,  8,  9, 10, 11, 12, 13,
    14, 15, 16, 17, 18, 19, 34,    60, 65,   35, 22, 23, 24, 25, 26, 27,
    28, 29, 30, 31, 32, 33, 48,    61, 64,   49, 36, 37, 38, 39, 40, 41,
    42, 43, 44, 45, 46, 47,     58,62, 63,67,    50, 51, 52, 53, 54, 55,
};

WavepoolEffect::WavepoolEffect(void) {
}

void WavepoolEffect::begin(void) {
  event_handler_hook_use(eventHandlerHook);
  LEDMode::begin();
}

Key WavepoolEffect::eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state) {
  if (row >= ROWS || col >= COLS)
    return mapped_key;

  if (keyIsPressed(key_state)) {
    uint8_t offset = (row*COLS)+col;
    surface[page][pgm_read_byte(rc2pos + offset)] = 0x7f;
    frames_since_event = 0;
  }

  return mapped_key;
}

void WavepoolEffect::update(void) {

  // limit the frame rate; one frame every 64 ms
  static uint8_t prev_time = 0;
  uint8_t now = (millis()>>6) % 0xff;
  if (now != prev_time) {
      prev_time = now;
  } else {
      return;
  }

  // rotate the colors over time
  // (side note: it's weird that this is a 16-bit int instead of 8-bit,
  //  but that's what the library function wants)
  //static uint16_t current_hue = 0;
  //current_hue ++;
  //current_hue &= 0xff;
  static uint8_t current_hue = 0;
  current_hue ++;

  frames_since_event ++;

  // needs two pages of height map to do the calculations
  int8_t *newpg = &surface[page^1][0];
  int8_t *oldpg = &surface[page][0];

  // rain a bit while idle
  static uint8_t frames_till_next_drop = 0;
  if (idle_timeout > 0) {
    if (frames_since_event >= (frames_till_next_drop + (idle_timeout >> 6))) {
        frames_till_next_drop = rand() & 0x3f;
        frames_since_event = idle_timeout >> 6;

        uint8_t x = rand() % WP_WID;
        uint8_t y = rand() % WP_HGT;
        uint8_t rainspot = (y*WP_WID) + x;

        oldpg[rainspot] = 0x7f;
        if (y > 0) oldpg[rainspot-WP_WID] = 0x50;
        if (y < (WP_HGT-1)) oldpg[rainspot+WP_WID] = 0x50;
        if (x > 0) oldpg[rainspot-1] = 0x50;
        if (x < (WP_WID-1)) oldpg[rainspot+1] = 0x50;
    }
  }

  // calculate water movement
  // (originally skipped edges, but this keyboard is too small for that)
  //for (uint8_t y = 1; y < WP_HGT-1; y++) {
  //  for (uint8_t x = 1; x < WP_WID-1; x++) {
  for (uint8_t y = 0; y < WP_HGT; y++) {
    for (uint8_t x = 0; x < WP_WID; x++) {
      uint8_t offset = (y*WP_WID) + x;

      int16_t value;
      int8_t offsets[] = { -WP_WID,    WP_WID,
                                  -1,         1,
                           -WP_WID-1, -WP_WID+1,
                            WP_WID-1,  WP_WID+1
                         };
      // don't wrap around edges or go out of bounds
      if (y==0) {
          offsets[0] = 0;
          offsets[4] += WP_WID;
          offsets[5] += WP_WID;
          }
      else if (y==WP_HGT-1) {
          offsets[1] = 0;
          offsets[6] -= WP_WID;
          offsets[7] -= WP_WID;
          }
      if (x==0) {
          offsets[2] = 0;
          offsets[4] += 1;
          offsets[6] += 1;
          }
      else if (x==WP_WID-1) {
          offsets[3] = 0;
          offsets[5] -= 1;
          offsets[7] -= 1;
          }

      value = ((oldpg[offset + offsets[0]]
               +oldpg[offset + offsets[1]]
               +oldpg[offset + offsets[2]]
               +oldpg[offset + offsets[3]]
               +oldpg[offset + offsets[4]]
               +oldpg[offset + offsets[5]]
               +oldpg[offset + offsets[6]]
               +oldpg[offset + offsets[7]]
               ) >> 2)
               - newpg[offset];

      newpg[offset] = value - (value >> 3);
    }
  }

  // draw the water on the keys
  for (byte r = 0; r < ROWS; r++) {
    for (byte c = 0; c < COLS; c++) {
      uint8_t offset = (r*COLS) + c;
      int8_t value = oldpg[pgm_read_byte(rc2pos+offset)];

      cRGB color;
      uint16_t intensity;

      // intensity = abs(value) * 2
      if (value >= 0) intensity = value * 2;
      else intensity = (-value) * 2;
      // color starts white but gets dimmer and more saturated as it fades,
      // with hue wobbling according to height map
      color = hsvToRgb(current_hue + (uint16_t)value + (uint16_t)value,
                       0xff - intensity,
                       intensity*2);

      LEDControl.setCrgbAt(r, c, color);
    }
  }

  // swap pages every frame
  page ^= 1;

}

}

kaleidoscope::WavepoolEffect WavepoolEffect;
