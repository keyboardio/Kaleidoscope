/* -*- mode: c++ -*-
 * Kaleidoscope-GhostInTheFirmware -- Let the keyboard write for you!
 * Copyright (C) 2017  Gergely Nagy
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

namespace KaleidoscopePlugins {
class GhostInTheFirmware : public KaleidoscopePlugin {
 public:
  typedef struct {
    byte row;
    byte col;
    uint16_t pressTime;
    uint16_t delay;
  } GhostKey;

  GhostInTheFirmware(void);

  void begin(void) final;
  static void configure(const GhostKey ghostKeys[]);
  static void activate(void);

 private:
  static GhostKey *ghostKeys;
  static bool isActive;
  static bool isPressed;
  static uint16_t currentPos;
  static uint32_t startTime;
  static uint16_t pressTimeOut;
  static uint16_t delayTimeOut;

  static void loopHook(bool postClear);
};

};

extern KaleidoscopePlugins::GhostInTheFirmware GhostInTheFirmware;
