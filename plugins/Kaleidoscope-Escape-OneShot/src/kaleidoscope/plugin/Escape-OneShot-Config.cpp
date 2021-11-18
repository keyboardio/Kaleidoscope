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
  struct {
    uint8_t b;
    uint16_t w;
  } checker;

  Runtime.storage().get(settings_base_, checker);

  // Check if we have an empty eeprom...
  if (checker.b == 0xff && checker.w == 0xffff) {
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
  enum {
    ENABLED,
    CANCEL_KEY
  } subCommand;

  if (::Focus.handleHelp(command, PSTR("escape_oneshot.enabled\n"
                                       "escape_oneshot.cancel_key")))
    return EventHandlerResult::OK;

  if (strncmp_P(command, PSTR("escape_oneshot."), 15) != 0)
    return EventHandlerResult::OK;
  if (strcmp_P(command + 15, PSTR("enabled")) == 0)
    subCommand = ENABLED;
  else if (strcmp_P(command + 15, PSTR("cancel_key")) == 0)
    subCommand = CANCEL_KEY;
  else
    return EventHandlerResult::OK;

  switch (subCommand) {
  case ENABLED:
    if (::Focus.isEOL()) {
      ::Focus.send(::EscapeOneShot.isEnabled());
    } else {
      uint8_t v;
      ::Focus.read(v);
      if (v != 0) {
        ::EscapeOneShot.enable();
      } else {
        ::EscapeOneShot.disable();
      }
    }
    break;

  case CANCEL_KEY:
    if (::Focus.isEOL()) {
      ::Focus.send(::EscapeOneShot.getCancelKey());
    } else {
      Key k;
      ::Focus.read(k);
      ::EscapeOneShot.setCancelKey(k);
    }
    break;
  }

  Runtime.storage().put(settings_base_, EscapeOneShot::settings_);
  Runtime.storage().commit();
  return EventHandlerResult::EVENT_CONSUMED;
}

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::EscapeOneShotConfig EscapeOneShotConfig;
