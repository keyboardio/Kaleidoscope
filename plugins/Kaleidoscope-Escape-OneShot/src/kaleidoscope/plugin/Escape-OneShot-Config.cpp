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

#include "kaleidoscope/plugin/Escape-OneShot.h"

#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-FocusSerial.h>

#include "kaleidoscope/Runtime.h"

namespace kaleidoscope {
namespace plugin {

uint16_t EscapeOneShotConfig::settings_base_;

EventHandlerResult EscapeOneShotConfig::onSetup() {
  settings_base_ = ::EEPROMSettings.requestSlice(sizeof(EscapeOneShot::settings_));
  uint16_t checker;

  Runtime.storage().get(settings_base_, checker);

  // Check if we have an empty eeprom...
  if (checker == 0xffff) {
    // ...if the eeprom was empty, store the default settings.
    Runtime.storage().put(settings_base_, EscapeOneShot::settings_);
    Runtime.storage().commit();
  }

  Runtime.storage().get(settings_base_, EscapeOneShot::settings_);
  return EventHandlerResult::OK;
}

EventHandlerResult EscapeOneShotConfig::onNameQuery() {
  return ::Focus.sendName(F("EscapeOneShot"));
}

EventHandlerResult EscapeOneShotConfig::onFocusEvent(const char *command) {
  if (::Focus.handleHelp(command, PSTR("escape_oneshot.cancel_key")))
    return EventHandlerResult::OK;

  if (strcmp_P(command, PSTR("escape_oneshot.cancel_key")) != 0)
    return EventHandlerResult::OK;

  if (::Focus.isEOL()) {
    ::Focus.send(::EscapeOneShot.getCancelKey());
  } else {
    Key k;
    ::Focus.read(k);
    ::EscapeOneShot.setCancelKey(k);
  }

  Runtime.storage().put(settings_base_, EscapeOneShot::settings_);
  Runtime.storage().commit();
  return EventHandlerResult::EVENT_CONSUMED;
}

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::EscapeOneShotConfig EscapeOneShotConfig;
