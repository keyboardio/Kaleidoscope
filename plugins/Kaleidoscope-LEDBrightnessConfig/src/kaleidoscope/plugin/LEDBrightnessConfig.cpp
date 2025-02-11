/* Kaleidoscope-LEDBrightnessConfig -- LED Brightness configuration, with EEPROM persistence
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

#include "kaleidoscope/plugin/LEDBrightnessConfig.h"

#include <Arduino.h>                       // for PSTR, strcmp_P, F
#include <Kaleidoscope-EEPROM-Settings.h>  // for EEPROMSettings
#include <Kaleidoscope-FocusSerial.h>      // for Focus, FocusSerial
#include <stdint.h>                        // for uint8_t, uint16_t

#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"         // for VirtualProps::Storage, Base<>::Storage
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/plugin/LEDControl.h"     // for LEDControl

namespace kaleidoscope {
namespace plugin {

uint16_t LEDBrightnessConfig::settings_base_;
struct LEDBrightnessConfig::settings LEDBrightnessConfig::settings_;

EventHandlerResult LEDBrightnessConfig::onSetup() {
  if (!::EEPROMSettings.requestSliceAndLoadData(&settings_base_, &settings_)) {
    settings_.brightness = 255;
  }
  ::LEDControl.setBrightness(settings_.brightness);

  return EventHandlerResult::OK;
}

EventHandlerResult LEDBrightnessConfig::onFocusEvent(const char *command) {
  const char *cmd = PSTR("led.brightness");

  if (::Focus.inputMatchesHelp(command))
    return ::Focus.printHelp(cmd);

  if (strcmp_P(command, cmd) != 0) return EventHandlerResult::OK;

  if (::Focus.isEOL()) {
    ::Focus.send(settings_.brightness);
  } else {
    ::Focus.read(settings_.brightness);
    ::LEDControl.setBrightness(settings_.brightness);
    Runtime.storage().put(settings_base_, settings_);
    Runtime.storage().commit();
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult LEDBrightnessConfig::onNameQuery() {
  return ::Focus.sendName(F("LEDBrightnessConfig"));
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::LEDBrightnessConfig LEDBrightnessConfig;
