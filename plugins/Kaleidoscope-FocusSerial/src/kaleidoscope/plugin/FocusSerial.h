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

#pragma once

#include <Arduino.h>                            // for __FlashStringHelper
#include <HardwareSerial.h>                     // for HardwareSerial
#include <stdint.h>                             // for uint8_t, uint16_t

#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"         // for cRGB
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/key_defs.h"              // for Key
#include "kaleidoscope/plugin.h"                // for Plugin

namespace kaleidoscope {
namespace plugin {
class FocusSerial : public kaleidoscope::Plugin {
 public:
  FocusSerial(void) {}

  static constexpr char COMMENT = '#';
  static constexpr char SEPARATOR = ' ';
  static constexpr char NEWLINE = '\n';

  bool handleHelp(const char *command,
                  const char *help_message);

  EventHandlerResult sendName(const __FlashStringHelper *name) {
    Runtime.serialPort().print(name);
    Runtime.serialPort().print(NEWLINE);
    return EventHandlerResult::OK;
  }

  void send() {}
  void send(const cRGB color) {
    send(color.r, color.g, color.b);
  }
  void send(const Key key) {
    send(key.getRaw());
  }
  void send(const bool b) {
    printBool(b);
    Runtime.serialPort().print(SEPARATOR);
  }
  template <typename V>
  void send(V v) {
    Runtime.serialPort().print(v);
    Runtime.serialPort().print(SEPARATOR);
  }
  template <typename Var, typename... Vars>
  void send(Var v, Vars... vars) {
    send(v);
    send(vars...);
  }

  void sendRaw() {}
  template <typename Var, typename... Vars>
  void sendRaw(Var v, Vars... vars) {
    Runtime.serialPort().print(v);
    sendRaw(vars...);
  }

  const char peek() {
    return Runtime.serialPort().peek();
  }

  void read(Key &key) {
    key.setRaw(Runtime.serialPort().parseInt());
  }
  void read(cRGB &color) {
    color.r = Runtime.serialPort().parseInt();
    color.g = Runtime.serialPort().parseInt();
    color.b = Runtime.serialPort().parseInt();
  }
  void read(uint8_t &u8) {
    u8 = Runtime.serialPort().parseInt();
  }
  void read(uint16_t &u16) {
    u16 = Runtime.serialPort().parseInt();
  }

  bool isEOL() {
    return Runtime.serialPort().peek() == NEWLINE;
  }


  /* Hooks */
  EventHandlerResult afterEachCycle();
  EventHandlerResult onFocusEvent(const char *command);

 private:
  static char command_[32];
  static uint8_t buf_cursor_;
  static void printBool(bool b);
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::FocusSerial Focus;
