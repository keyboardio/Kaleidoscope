/* -*- mode: c++ -*-
 * Kaleidoscope-Unicode -- Unicode input helpers
 * Copyright (C) 2016, 2017, 2018  Keyboard.io, Inc
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

#include "kaleidoscope/Runtime.h"
#include <Kaleidoscope-HostOS.h>

namespace kaleidoscope {
namespace plugin {
class Unicode : public kaleidoscope::Plugin {
 public:
  Unicode(void) {}

  static void start(void);
  static void input(void);
  static void end(void);

  static void type(uint32_t unicode);
  static void typeCode(uint32_t unicode);

  static void input_delay(uint8_t delay) {
    input_delay_ = delay;
  }
  static uint8_t input_delay() {
    return input_delay_;
  }
 private:
  static uint8_t input_delay_;
};
}
}

Key hexToKey(uint8_t hex);
Key hexToKeysWithNumpad(uint8_t hex);

void unicodeCustomStart(void);
void unicodeCustomEnd(void);
void unicodeCustomInput(void);

extern kaleidoscope::plugin::Unicode Unicode;
