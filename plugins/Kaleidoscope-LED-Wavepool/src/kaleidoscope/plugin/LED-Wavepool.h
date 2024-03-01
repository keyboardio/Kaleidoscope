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

#if defined(ARDUINO_AVR_MODEL01) || defined(ARDUINO_keyboardio_model_100)

#include <Arduino.h>  // for PROGMEM
#include <stdint.h>   // for uint8_t, int16_t, int8_t, INT16_MAX

#include "kaleidoscope/KeyEvent.h"                       // for KeyEvent
#include "kaleidoscope/Runtime.h"                        // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"                  // for Device
#include "kaleidoscope/event_handler_result.h"           // for EventHandlerResult
#include "kaleidoscope/plugin.h"                         // for Plugin
#include "kaleidoscope/plugin/AccessTransientLEDMode.h"  // for AccessTransientLEDMode
#include "kaleidoscope/plugin/LEDMode.h"                 // for LEDMode
#include "kaleidoscope/plugin/LEDModeInterface.h"        // for LEDModeInterface

#define WP_WID 14
#define WP_HGT 5

namespace kaleidoscope {
namespace plugin {
class WavepoolEffect : public Plugin,
                       public LEDModeInterface,
                       public AccessTransientLEDMode {
 public:
  WavepoolEffect() { led_mode_name_ = "Wavepool"; }
  explicit WavepoolEffect(const char *led_mode_name) { led_mode_name_ = led_mode_name; }

  EventHandlerResult onKeyEvent(KeyEvent &event);

  // ms before idle animation starts after last keypress
  static uint16_t idle_timeout;
  static int16_t ripple_hue;

  static constexpr int16_t rainbow_hue = INT16_MAX;

  // This class' instance has dynamic lifetime
  //
  class TransientLEDMode : public LEDMode {
   public:
    // Please note that storing the parent ptr is only required
    // for those LED modes that require access to
    // members of their parent class. Most LED modes can do without.
    //
    explicit TransientLEDMode(const WavepoolEffect *parent);

    EventHandlerResult onKeyEvent(KeyEvent &event);

   protected:
    void update() final;

   private:
    uint8_t frames_since_event_;
    int8_t surface_[2][WP_WID * WP_HGT];
    uint8_t page_;
    static PROGMEM const uint8_t rc2pos[Runtime.device().numKeys()];

    void raindrop(uint8_t x, uint8_t y, int8_t *page);
    uint8_t wp_rand();

    friend class WavepoolEffect;
  };
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::WavepoolEffect WavepoolEffect;

#endif
