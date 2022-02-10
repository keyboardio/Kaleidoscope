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

#include <Kaleidoscope-FocusSerial.h>

#ifdef __AVR__
#include <avr/pgmspace.h>
#endif

namespace kaleidoscope {
namespace plugin {

char FocusSerial::command_[32];

EventHandlerResult FocusSerial::afterEachCycle() {
  // If the serial buffer is empty, we don't have any work to do
  if (Runtime.serialPort().available() == 0)
    return EventHandlerResult::OK;

  uint8_t i = 0;
  memset(command_, 0, sizeof(command_));

  do {
    command_[i++] = Runtime.serialPort().read();

    if (Runtime.serialPort().peek() == '\n')
      break;
  } while (command_[i - 1] != ' ' && i < 32);

  // If this was a command with a space-delimited payload, strip the space delimiter off
  if (command_[i - 1] == SEPARATOR )
    command_[i - 1] = '\0';


  // If there was no command, there's nothing to do
  if (command_[0] == '\0') {
    return EventHandlerResult::OK;
  }

  Runtime.onFocusEvent(command_);

  while (Runtime.serialPort().available()) {
    char c =  Runtime.serialPort().read();
    if (c == NEWLINE) {
      // newline serves as an end-of-command marker
      // don't drain the buffer past there
      break;
    }
  }
  Runtime.serialPort().println(F("\r\n."));


  return EventHandlerResult::OK;
}

bool FocusSerial::handleHelp(const char *command,
                             const char *help_message) {
  if (strcmp_P(command, PSTR("help")) != 0)
    return false;

  Runtime.serialPort().println((const __FlashStringHelper *)help_message);
  return true;
}

EventHandlerResult FocusSerial::onFocusEvent(const char *command) {
  if (handleHelp(command, PSTR("help\nplugins")))
    return EventHandlerResult::OK;

  if (strcmp_P(command, PSTR("plugins")) == 0) {
    kaleidoscope::Hooks::onNameQuery();
    return EventHandlerResult::EVENT_CONSUMED;
  }

  return EventHandlerResult::OK;
}

void FocusSerial::printBool(bool b) {
  Runtime.serialPort().print((b) ? F("true") : F("false"));
}

}
}

kaleidoscope::plugin::FocusSerial Focus;
