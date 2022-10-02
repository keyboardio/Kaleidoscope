/* -*- mode: c++ -*-
 * Kaleidoscope-Escape-OneShot -- Turn ESC into a key that cancels OneShots, if active.
 * Copyright (C) 2016-2021  Keyboard.io, Inc
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

#include "kaleidoscope/plugin/Escape-OneShot.h"  // IWYU pragma: associated

#include <Arduino.h>                       // for PSTR, F, __FlashStringHelper
#include <Kaleidoscope-EEPROM-Settings.h>  // for EEPROMSettings
#include <Kaleidoscope-FocusSerial.h>      // for Focus, FocusSerial

#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"         // for VirtualProps::Storage, Base<>::Storage
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/key_defs.h"              // for Key

namespace kaleidoscope {
namespace plugin {

EventHandlerResult EscapeOneShotConfig::onSetup() {
  settings_base_ = ::EEPROMSettings.requestSlice(sizeof(EscapeOneShot::settings_));

  if (Runtime.storage().isSliceUninitialized(
        settings_base_,
        sizeof(EscapeOneShot::Settings))) {
    // If our slice is uninitialized, set sensible defaults.
    Runtime.storage().put(settings_base_, ::EscapeOneShot.settings_);
    Runtime.storage().commit();
  }

  Runtime.storage().get(settings_base_, ::EscapeOneShot.settings_);
  return EventHandlerResult::OK;
}

EventHandlerResult EscapeOneShotConfig::onNameQuery() {
  return ::Focus.sendName(F("EscapeOneShot"));
}

EventHandlerResult EscapeOneShotConfig::onFocusEvent(const char *input) {
  const char *cmd_cancel_key = PSTR("escape_oneshot.cancel_key");
  if (::Focus.inputMatchesHelp(input))
    return ::Focus.printHelp(cmd_cancel_key);

  if (!::Focus.inputMatchesCommand(input, cmd_cancel_key))
    return EventHandlerResult::OK;

  if (::Focus.isEOL()) {
    ::Focus.send(::EscapeOneShot.getCancelKey());
  } else {
    Key k;
    ::Focus.read(k);
    ::EscapeOneShot.setCancelKey(k);
  }

  Runtime.storage().put(settings_base_, ::EscapeOneShot.settings_);
  Runtime.storage().commit();
  return EventHandlerResult::EVENT_CONSUMED;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::EscapeOneShotConfig EscapeOneShotConfig;
