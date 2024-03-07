/* -*- mode: c++ -*-
 * Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2022  Keyboard.io, Inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "kaleidoscope/plugin/DefaultLEDModeConfig.h"

#include <Arduino.h>                       // for PSTR, F, __FlashStringHelper
#include <Kaleidoscope-EEPROM-Settings.h>  // for EEPROMSettings
#include <Kaleidoscope-FocusSerial.h>      // for Focus, FocusSerial
#include <stdint.h>                        // for uint8_t, uint16_t

#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"         // for VirtualProps::Storage, Base<>::Storage
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/plugin/LEDControl.h"     // for LEDControl

namespace kaleidoscope {
namespace plugin {

uint16_t DefaultLEDModeConfig::settings_base_;
struct DefaultLEDModeConfig::settings DefaultLEDModeConfig::settings_;

EventHandlerResult DefaultLEDModeConfig::onSetup() {
  bool success = ::EEPROMSettings.requestSliceAndLoadData(&settings_base_, &settings_);
  if (success) {
    ::LEDControl.set_mode(settings_.default_mode_index);
  }
  return EventHandlerResult::OK;
}

EventHandlerResult DefaultLEDModeConfig::onFocusEvent(const char *input) {
  const char *cmd = PSTR("led_mode.default");

  if (::Focus.inputMatchesHelp(input))
    return ::Focus.printHelp(cmd);

  if (!::Focus.inputMatchesCommand(input, cmd))
    return EventHandlerResult::OK;

  if (::Focus.isEOL()) {
    ::Focus.send(settings_.default_mode_index);
  } else {
    uint8_t idx;
    ::Focus.read(idx);
    settings_.default_mode_index = idx;

    ::LEDControl.set_mode(idx);
    Runtime.storage().put(settings_base_, settings_);
    Runtime.storage().commit();
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult DefaultLEDModeConfig::onNameQuery() {
  return ::Focus.sendName(F("DefaultLEDModeConfig"));
}

void DefaultLEDModeConfig::activateLEDModeIfUnconfigured(LEDModeInterface *plugin) {
  if (!Runtime.storage().isSliceUninitialized(settings_base_, sizeof(settings_)))
    return;

  plugin->activate();
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::DefaultLEDModeConfig DefaultLEDModeConfig;
