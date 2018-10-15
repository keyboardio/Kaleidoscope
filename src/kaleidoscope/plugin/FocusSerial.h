/* -*- mode: c++ -*-
 * Kaleidoscope-FocusSerial -- Bidirectional communication plugin
 * Copyright (C) 2017, 2018  Keyboard.io, Inc
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

#pragma once

#include <Kaleidoscope.h>

namespace kaleidoscope {
namespace plugin {
class FocusSerial : public kaleidoscope::Plugin {
 public:
  FocusSerial(void) {}

  bool handleHelp(const char *command,
                  const char *help_message);

  /* Helpers */
  static void printNumber(uint16_t number);
  static void printSpace(void);
  static void printColor(uint8_t r, uint8_t g, uint8_t b);
  static void printSeparator(void);
  static void printBool(bool b);

  static void readColor(cRGB &color);

  /* Hooks */
  EventHandlerResult onSetup() {
    Serial.begin(9600);
    return EventHandlerResult::OK;
  }
  EventHandlerResult beforeReportingState();
  EventHandlerResult onFocusEvent(const char *command);

 private:
  static char command_[32];

  static void drain(void);
};
}
}

extern kaleidoscope::plugin::FocusSerial Focus;
