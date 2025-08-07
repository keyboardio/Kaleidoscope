/* Kaleidoscope-Hardware-Keyboardio-Preonic -- Keyboardio Preonic hardware support for Kaleidoscope
 * Copyright 2016-2025 Keyboard.io, inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * Additional Permissions:
 * As an additional permission under Section 7 of the GNU General Public
 * License Version 3, you may link this software against a Vendor-provided
 * Hardware Specific Software Module under the terms of the MCU Vendor
 * Firmware Library Additional Permission Version 1.0.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdint.h>  // for uint16_t, uint8_t

#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/plugin.h"                // for Plugin

namespace kaleidoscope {
namespace plugin {
class PreonicBootGreetingEffect : public kaleidoscope::Plugin {
 public:
  PreonicBootGreetingEffect(void) {}

  static uint16_t timeout;

  EventHandlerResult onSetup();
  EventHandlerResult afterEachCycle();

 private:
  static bool done_;
  static uint16_t start_time;
  static constexpr uint16_t FADE_IN_DURATION  = 1000;  // 1s fade in
  static constexpr uint16_t FLAPPING_DURATION = 8000;  // 8s flapping
  static constexpr uint16_t FADE_OUT_DURATION = 1000;  // 1s fade out
  static constexpr uint16_t FLAP_CYCLE_MS     = 2000;  // 2000ms per flap cycle (0.5 flaps/sec)

  // LED indices for wings
  static constexpr uint8_t TOP_LEFT_LED     = 2;
  static constexpr uint8_t TOP_RIGHT_LED    = 3;
  static constexpr uint8_t BOTTOM_LEFT_LED  = 1;
  static constexpr uint8_t BOTTOM_RIGHT_LED = 0;

  // Orange color components (RGB) - much more red, minimal green
  static constexpr uint8_t ORANGE_RED   = 255;
  static constexpr uint8_t ORANGE_GREEN = 40;
  static constexpr uint8_t ORANGE_BLUE  = 0;
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::PreonicBootGreetingEffect PreonicBootGreetingEffect;