/* -*- mode: c++ -*-
 * Kaleidoscope-HostOS -- Host OS detection and tracking for Kaleidoscope
 * Copyright (C) 2016, 2017  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <Kaleidoscope.h>

namespace kaleidoscope {
namespace hostos {

typedef enum {
  LINUX,
  OSX,
  WINDOWS,
  OTHER,

  AUTO = 0xff,
} Type;

class Base : public KaleidoscopePlugin {
 public:
  void begin(void) final;

  Type os(void);
  void os(Type new_os);

  bool focusHook(const char *command);

 protected:
  virtual void autoDetect(void) {}
  Type os_;

 private:
  uint16_t eeprom_slice_;
  bool is_configured_ = false;
};

}
}

extern kaleidoscope::hostos::Base HostOS;

#define FOCUS_HOOK_HOSTOS FOCUS_HOOK(HostOS.focusHook, "hostos.type")
