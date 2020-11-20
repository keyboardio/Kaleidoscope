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

// This example demonstrates how different host keymaps can conveniently be used
// in one firmware. The example assumes that the host system supports layout
// switching between US english standard layout and german standard layout.
//
// We define dedicated keymap layers for both host keymaps and define
// a key (defined as llg below) that locks the german keymap layer.
//
// Once the layers are changed on the keyboard, the host keymap must be
// changed as well for the changes to take effect.

#include <Kaleidoscope.h>

#include "kaleidoscope/host_keymap/linux/us/standard/keymap.h"
#include "kaleidoscope/host_keymap/linux/de/standard/keymap.h"

// Define the us standard keymap as default.
//
USE_HOST_KEYMAP(linux, us, standard)

// Note: All unicode character strings like e.g. L"⌫" or L"ß" require to
//       be prefixed with an L to make C++ store them as wide characters.

enum Layers {
  US,
  German
};

// *INDENT-OFF*

constexpr Key llg = LockLayer(German);

START_KEYMAPS
  [US] = KEYMAP_STACKED
  (
      XXX,   "1",   "2",   "3",   "4",   "5",   XXX,
      "`",   "q",   "w",   "e",   "r",   "t",   "\t",
     L"⇞",   "a",   "s",   "d",   "f",   "g",
     L"⇟",   "z",   "x",   "c",   "v",   "b",   L"⎋",

     L"⌃", L"⌫", L"⌘", L"⇧",
      XXX,

      XXX,   "6",   "7",   "8",   "9",   "0",   XXX,
     L"⎆",   "y",   "u",   "i",   "o",   "p",   "=",
             "h",   "j",   "k",   "l",   ";",  "\"",
      XXX,   "n",   "m",   ",",   ".",   "/",   "-",

      llg, L"r⌥",  L"␣", L"r⌃",
      XXX
  ),

  // Reset the standard keymap to german
  //
  #undef CONVERT_TO_KEY
  #define CONVERT_TO_KEY(INPUT) MAP_WITH_HOST_KEYMAP(linux, de, standard, INPUT)

  [German] = KEYMAP_STACKED
  (
      XXX,   "1",   "2",   "3",   "4",   "5",   XXX,
      XXX,   "q",   "w",   "e",   "r",   "t",   "\t", // The backtick key does not exist on the german keyboard
     L"⇞",   "a",   "s",   "d",   "f",   "g",
     L"⇟",   "y",   "x",   "c",   "v",   "b",   L"⎋",

     L"⌃", L"⌫", L"⌘", L"⇧",
      XXX,

      XXX,   "6",   "7",   "8",   "9",   "0",   L"ß",
     L"⎆",   "z",   "u",   "i",   "o",   "p",   L"ü",
             "h",   "j",   "k",   "l",  L"ö",   L"ä",
      XXX,   "n",   "m",   ",",   ".",   "-",   XXX,

      ___, L"r⌥",  L"␣", L"r⌃",
      XXX
  )
END_KEYMAPS
// *INDENT-ON*

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}

// The following is just a test (not necessary to define a keymap).
//
// An auxiliary macro D is used to define keys of the german (de)
// keymap.
//
#define D(KEY_STRING) MAP_WITH_HOST_KEYMAP(linux, de, standard, L##KEY_STRING)

// For testing purposes, we define an auxiliary macro for the US keymap as
// well.
//
#define U(KEY_STRING) MAP_WITH_HOST_KEYMAP(linux, us, standard, L##KEY_STRING)

// The y and z keys of the german keymap exchange change positions with
// repect to the US keymap.
//
static_assert(D("y") == U("z"), "The letter y on the german keymap must "
              "generate the same Kaleidoscope Key value as the letter z on the US "
              "keymap");
static_assert(D("z") == U("y"), "The letter z on the german keymap must "
              "generate the same Kaleidoscope Key value as the letter y on the US "
              "keymap");
