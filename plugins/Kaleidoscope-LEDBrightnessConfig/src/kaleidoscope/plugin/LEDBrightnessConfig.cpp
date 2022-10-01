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

#include "kaleidoscope/plugin/LEDBrightnessConfig.h"

#include <Arduino.h>                       // for PSTR, strcmp_P, F, __FlashStringHelper
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
  settings_base_ = ::EEPROMSettings.requestSlice(sizeof(settings_));

  Runtime.storage().get(settings_base_, settings_);

  // We do not need to treat uninitialized slices in any special way, because
  // uninitialized defaults to `255`, which happens to be our desired default
  // brightness, too.
  ::LEDControl.setBrightness(settings_.brightness);

  return EventHandlerResult::OK;
}

EventHandlerResult LEDBrightnessConfig::onFocusEvent(const char *command) {
  const char *cmd = PSTR("led.brightness");

  if (::Focus.handleHelp(command, cmd))
    return EventHandlerResult::OK;

  if (strcmp_P(command, cmd) != 0)
    return EventHandlerResult::OK;

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
