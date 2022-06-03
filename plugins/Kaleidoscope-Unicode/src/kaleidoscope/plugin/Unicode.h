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

#include <stdint.h>  // for uint8_t, uint32_t

#include "kaleidoscope/key_defs.h"  // for Key
#include "kaleidoscope/plugin.h"    // for Plugin

namespace kaleidoscope {
namespace plugin {
class Unicode : public kaleidoscope::Plugin {
 public:
  static void start();
  static void input();
  static void end();

  static void type(uint32_t unicode);
  static void typeCode(uint32_t unicode);

  static void input_delay(uint8_t delay) {
    input_delay_ = delay;
  }
  static uint8_t input_delay() {
    return input_delay_;
  }

  static void setLinuxKey(const Key key) {
    linux_key_ = key;
  }
  static Key getLinuxKey() {
    return linux_key_;
  }

 private:
  static Key linux_key_;
  static uint8_t input_delay_;
};

}  // namespace plugin
}  // namespace kaleidoscope

Key hexToKey(uint8_t hex);
Key hexToKeysWithNumpad(uint8_t hex);

void unicodeCustomStart();
void unicodeCustomEnd();
void unicodeCustomInput();

extern kaleidoscope::plugin::Unicode Unicode;
