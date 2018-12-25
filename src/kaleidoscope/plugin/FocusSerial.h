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

  void send() {}
  void send(const cRGB color) {
    send(color.r, color.g, color.b);
  }
  void send(const Key key) {
    send(key.raw);
  }
  void send(const bool b) {
    printBool(b);
    Serial.print(SEPARATOR);
  }
  template <typename V>
  void send(V v) {
    Serial.print(v);
    Serial.print(SEPARATOR);
  }
  template <typename Var, typename... Vars>
  void send(Var v, const Vars&... vars) {
    send(v);
    send(vars...);
  }

  void sendRaw() {}
  template <typename Var, typename... Vars>
  void sendRaw(Var v, const Vars&... vars) {
    Serial.print(v);
    sendRaw(vars...);
  }

  const char peek() {
    return Serial.peek();
  }

  void read(Key &key) {
    key.raw = Serial.parseInt();
  }
  void read(cRGB &color) {
    color.r = Serial.parseInt();
    color.g = Serial.parseInt();
    color.b = Serial.parseInt();
  }
  void read(uint8_t &u8) {
    u8 = Serial.parseInt();
  }
  void read(uint16_t &u16) {
    u16 = Serial.parseInt();
  }

  bool isEOL() {
    return Serial.peek() == '\n';
  }

  static constexpr char COMMENT = '#';
  static constexpr char SEPARATOR = ' ';

  /* Hooks */
  EventHandlerResult beforeReportingState();
  EventHandlerResult onFocusEvent(const char *command);

 private:
  static char command_[32];

  static void drain(void);
  static void printBool(bool b);
};
}
}

extern kaleidoscope::plugin::FocusSerial Focus;
