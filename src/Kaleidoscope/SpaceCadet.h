/* -*- mode: c++ -*-
 * Kaleidoscope-SpaceCadet -- Space Cadet Shift Extended
 * Copyright (C) 2016, 2017  Gergely Nagy, Ben Gemperline
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

#ifndef SPACECADET_MAP_END
#define SPACECADET_MAP_END (kaleidoscope::SpaceCadet::KeyBinding) { Key_NoKey, Key_NoKey, 0 }
#endif

namespace kaleidoscope {
//Declaration for the method (implementing KaleidoscopePlugin)
class SpaceCadet : public KaleidoscopePlugin {
 public:
  //Internal Class
  //Declarations for the modifier key mapping
  class KeyBinding {
   public:
    //Empty constructor; set the vars separately
    KeyBinding(void) {}
    //Constructor with input and output
    KeyBinding(Key input_, Key output_);
    //Constructor with all three set
    KeyBinding(Key input_, Key output_, uint16_t timeout_);
    //The key that is pressed
    Key input;
    //the key that is sent
    Key output;
    //The timeout (default to global timeout)
    uint16_t timeout = 0;
    //The flag (set to 0)
    bool flagged = false;
    //the start time for this key press
    uint32_t start_time = 0;
  };

  //Empty constructor
  SpaceCadet(void);

  //Methods
  void begin(void) final;

  //Publically accessible variables
  static uint16_t time_out;  //  The global timeout in milliseconds
  static SpaceCadet::KeyBinding * map;  // The map of key bindings

 private:
  static Key eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state);
};
};

extern kaleidoscope::SpaceCadet SpaceCadet;
