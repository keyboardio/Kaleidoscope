/* Kaleidoscope-LEDBrightnessControl -- LED brightness up/down keys
 * Copyright 2022-2025 Keyboard.io, inc.
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

#include <Kaleidoscope-Ranges.h>  // for LED_BRIGHTNESS_UP, LED_BRIGHTNESS_DOWN
#include <stdint.h>               // for uint8_t, uint16_t

#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/plugin.h"                // for Plugin
#include "kaleidoscope/key_defs.h"              // for Key

constexpr Key Key_LEDBrightnessUp   = Key(kaleidoscope::ranges::LED_BRIGHTNESS_UP);
constexpr Key Key_LEDBrightnessDown = Key(kaleidoscope::ranges::LED_BRIGHTNESS_DOWN);

namespace kaleidoscope {
namespace plugin {

namespace ledbrightnesscontrol {
constexpr uint8_t BRIGHTNESS_INCREMENT = 16;
}

class LEDBrightnessControl : public kaleidoscope::Plugin {
 public:
  EventHandlerResult onNameQuery();
  EventHandlerResult onKeyEvent(KeyEvent &event);
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::LEDBrightnessControl LEDBrightnessControl;
