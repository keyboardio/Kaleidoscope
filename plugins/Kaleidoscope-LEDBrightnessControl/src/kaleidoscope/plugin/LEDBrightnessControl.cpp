/* Kaleidoscope-LEDBrightnessControl -- LED brightness up/down keys
 * Copyright 2025 Keyboard.io, inc.
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

#include "kaleidoscope/plugin/LEDBrightnessControl.h"

#include <Kaleidoscope-FocusSerial.h>  // for Focus
#include <stdint.h>                    // for uint8_t, uint16_t

#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/keyswitch_state.h"       // for keyToggledOn
#include "kaleidoscope/keyswitch_state.h"       // for keyToggledOn
#include "kaleidoscope/plugin/LEDControl.h"     // for LEDControl

namespace kaleidoscope {
namespace plugin {

using ledbrightnesscontrol::BRIGHTNESS_INCREMENT;

EventHandlerResult LEDBrightnessControl::onKeyEvent(KeyEvent &event) {
  if ((event.key == Key_LEDBrightnessUp ||
       event.key == Key_LEDBrightnessDown) &&
      keyToggledOn(event.state)) {
    uint8_t brightness = LEDControl::getBrightness();
    bool increase      = (event.key == Key_LEDBrightnessUp);

    for (Key active_key : live_keys.all()) {
      if (active_key.isKeyboardShift()) {
        increase = !increase;
        break;
      }
    }

    if (increase && brightness <= 255 - BRIGHTNESS_INCREMENT) {
      brightness += BRIGHTNESS_INCREMENT;
    } else if (!increase && brightness >= BRIGHTNESS_INCREMENT) {
      brightness -= BRIGHTNESS_INCREMENT;
    }
    LEDControl::setBrightness(brightness);
  }
}

EventHandlerResult LEDBrightnessControl::onNameQuery() {
  return ::Focus.sendName(F("LEDBrightnessControl"));
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::LEDBrightnessControl LEDBrightnessControl;
