/* -*- mode: c++ -*-
 * Kaleidoscope-FocusSerial -- Bidirectional communication plugin
 * Copyright (C) 2017, 2018, 2021  Keyboard.io, Inc
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

#include "kaleidoscope/plugin/FocusSerial.h"

#include <Arduino.h>         // for PSTR, __FlashStringHelper, F, strcmp_P
#include <HardwareSerial.h>  // for HardwareSerial
#include <string.h>          // for memset

#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/hooks.h"                 // for Hooks

#ifdef __AVR__
#include <avr/pgmspace.h>
#endif

namespace kaleidoscope {
namespace plugin {

EventHandlerResult FocusSerial::afterEachCycle() {
  int c;
  // GD32 doesn't currently autoflush the very last packet. So manually flush here
  Runtime.serialPort().flush();
  // If the serial buffer is empty, we don't have any work to do
  if (Runtime.serialPort().available() == 0) {
    return EventHandlerResult::OK;
  }

  do {
    // If there's a newline pending, don't read it
    if (Runtime.serialPort().peek() == NEWLINE) {
      break;
    }
    c = Runtime.serialPort().read();
    // Don't store the separator; just stash it
    if (c == SEPARATOR) {
      break;
    }
    input_[buf_cursor_++] = c;
  } while (buf_cursor_ < (sizeof(input_) - 1) && Runtime.serialPort().available());

  if ((c != SEPARATOR) && (Runtime.serialPort().peek() != NEWLINE) && buf_cursor_ < (sizeof(input_) - 1)) {
    // We don't have enough command to work with yet.
    // Let's leave the buffer around for another cycle
    return EventHandlerResult::OK;
  }

  // Then process the command
  Runtime.onFocusEvent(input_);
  while (Runtime.serialPort().available()) {
    c = Runtime.serialPort().read();
    if (c == NEWLINE) {
      // newline serves as an end-of-command marker
      // don't drain the buffer past there
      break;
    }
  }
  // End of command processing is signalled with a CRLF followed by a single period
  Runtime.serialPort().println(F("\r\n."));
  buf_cursor_ = 0;
  memset(input_, 0, sizeof(input_));
  return EventHandlerResult::OK;
}

EventHandlerResult FocusSerial::onFocusEvent(const char *input) {
  const char *cmd_help    = PSTR("help");
  const char *cmd_reset   = PSTR("device.reset");
  const char *cmd_plugins = PSTR("plugins");

  if (inputMatchesHelp(input))
    return printHelp(cmd_help, cmd_reset, cmd_plugins);

  if (inputMatchesCommand(input, cmd_reset)) {
    Runtime.device().rebootBootloader();
    return EventHandlerResult::EVENT_CONSUMED;
  }
  if (inputMatchesCommand(input, cmd_plugins)) {
    kaleidoscope::Hooks::onNameQuery();
    return EventHandlerResult::EVENT_CONSUMED;
  }

  return EventHandlerResult::OK;
}

#ifndef NDEPRECATED
bool FocusSerial::handleHelp(const char *input, const char *help_message) {
  if (!inputMatchesHelp(input)) return false;

  printHelp(help_message);
  return true;
}
#endif

void FocusSerial::printBool(bool b) {
  Runtime.serialPort().print((b) ? F("true") : F("false"));
}

bool FocusSerial::inputMatchesHelp(const char *input) {
  return inputMatchesCommand(input, PSTR("help"));
}

bool FocusSerial::inputMatchesCommand(const char *input, const char *expected) {
  return strcmp_P(input, expected) == 0;
}

bool FocusSerial::isEOL() {
  int c        = -1;
  auto timeout = Runtime.serialPort().getTimeout();
  auto start   = millis();

  // Duplicate some of Stream::timedPeek because it's protected
  do {
    c = Runtime.serialPort().peek();
    if (c == NEWLINE) {
      return true;
    } else if (c >= 0) {
      return false;
    }
  } while ((millis() - start) < timeout);
  return true;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::FocusSerial Focus;
