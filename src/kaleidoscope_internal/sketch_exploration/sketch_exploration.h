/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2019  Keyboard.io, Inc.
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

// clang-format off

#pragma once

#include "kaleidoscope_internal/sketch_exploration/keymap_exploration.h"  // for _INIT_KEYMAP_EX...
#include "kaleidoscope_internal/sketch_exploration/plugin_exploration.h"  // IWYU pragma: keep

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Read carefully
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// About compile-time-only functions (COMPILE_TIME_USE_ONLY)
//
// On Harvard architecture an array like the Kaleidoscope keymap
// that is declared 'constexpr PROGMEM' must either be accessed
// by a runtime function through functions like pgm_read_byte
// or at compile time with ordinary array access. Using normal array
// access at run-time will cause a memory access failures as the MCU
// will try to read from SRAM instead of PROGMEM.
//
// There are some functions that are therefore restricted to compile-time
// use and will fail if instantiated by the compiler. There is no language
// inherent means to prevent the latter. Therefore, we flag all such functions
// as
//
// COMPILE_TIME_USE_ONLY
//
// Call such functions consistently as
//
// constexpr auto val = aCompileTimeOnlyFunction(...);
//
// This ensures that they can not be instantiated and called at runtime.
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// This macro defines a Sketch interface class that is passed to the
// exploreSketch<_Sketch>(...)-hook.
//
#define _INIT_SKETCH_EXPLORATION                                               \
                                                                               \
  _INIT_KEYMAP_EXPLORATION                                                     \
                                                                               \
  namespace kaleidoscope {                                                     \
  namespace sketch_exploration {                                               \
                                                                               \
    class Plugins;                                                             \
                                                                               \
    struct Sketch {                                                            \
       typedef sketch_exploration::StaticKeymap StaticKeymap;                  \
       typedef sketch_exploration::Plugins Plugins;                            \
    };                                                                         \
                                                                               \
    void pluginsExploreSketch() {                                              \
      Hooks::exploreSketch<Sketch>();                                          \
    }                                                                          \
  } /* namespace sketch_exploration */                                         \
  } /* namespace kaleidoscope */
