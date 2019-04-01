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

#ifdef ARDUINO_AVR_MODEL01

#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>

#define WP_WID 14
#define WP_HGT 5

namespace kaleidoscope {
namespace plugin {
class WavepoolEffect : public LEDMode {
 public:
  WavepoolEffect(void) {}

  EventHandlerResult onKeyswitchEvent2(Key &mapped_key, KeyAddr key_addr, uint8_t key_state);

  // ms before idle animation starts after last keypress
  static uint16_t idle_timeout;
  static int16_t ripple_hue;

  static constexpr int16_t rainbow_hue = INT16_MAX;
 protected:
  void update(void) final;

 private:
  static uint8_t frames_since_event;
  static int8_t surface[2][WP_WID * WP_HGT];
  static uint8_t page;
  static PROGMEM const uint8_t rc2pos[ROWS * COLS];

  static void raindrop(uint8_t x, uint8_t y, int8_t *page);
  static uint8_t wp_rand();
};

}
}

extern kaleidoscope::plugin::WavepoolEffect WavepoolEffect;

#endif
