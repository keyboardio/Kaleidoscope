/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Colormap -- Per-layer colormap effect
 * Copyright (C) 2017-2022  Keyboard.io, Inc
 *
 * This program is free software: you can redistribute it and/or modify it under it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <Kaleidoscope.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-EEPROM-Keymap.h>
#include <Kaleidoscope-Colormap.h>
#include <Kaleidoscope-FocusSerial.h>
#include <Kaleidoscope-LED-Palette-Theme.h>
#include <Kaleidoscope-LEDControl.h>

// clang-format off
KEYMAPS(
  [0] = KEYMAP_STACKED
  (Key_NoKey,    Key_1, Key_2, Key_3, Key_4, Key_5, Key_LEDEffectNext,
   Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
   Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,

   Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
   Key_NoKey,

   Key_skip,  Key_6, Key_7, Key_8,     Key_9,      Key_0,         Key_skip,
   Key_Enter, Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals,
              Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,
   Key_skip,  Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus,

   Key_RightShift, Key_RightAlt, Key_Spacebar, Key_RightControl,
   Key_NoKey),
)

// Colors names of the EGA palette, for convenient use in colormaps. Should
// match the palette definition below. Optional, one can just use the indexes
// directly, too.
enum {
  BLACK,
  BLUE,
  GREEN,
  CYAN,
  RED,
  MAGENTA,
  BROWN,
  LIGHT_GRAY,
  DARK_GRAY,
  BRIGHT_BLUE,
  BRIGHT_GREEN,
  BRIGHT_CYAN,
  BRIGHT_RED,
  BRIGHT_MAGENTA,
  YELLOW,
  WHITE
};

// Define an EGA palette. Conveniently, that's exactly 16 colors, just like the
// limit of LEDPaletteTheme.
PALETTE(
    CRGB(0x00, 0x00, 0x00),  // [0x0] black
    CRGB(0x00, 0x00, 0xaa),  // [0x1] blue
    CRGB(0x00, 0xaa, 0x00),  // [0x2] green
    CRGB(0x00, 0xaa, 0xaa),  // [0x3] cyan
    CRGB(0xaa, 0x00, 0x00),  // [0x4] red
    CRGB(0xaa, 0x00, 0xaa),  // [0x5] magenta
    CRGB(0xaa, 0x55, 0x00),  // [0x6] brown
    CRGB(0xaa, 0xaa, 0xaa),  // [0x7] light gray
    CRGB(0x55, 0x55, 0x55),  // [0x8] dark gray
    CRGB(0x55, 0x55, 0xff),  // [0x9] bright blue
    CRGB(0x55, 0xff, 0x55),  // [0xa] bright green
    CRGB(0x55, 0xff, 0xff),  // [0xb] bright cyan
    CRGB(0xff, 0x55, 0x55),  // [0xc] bright red
    CRGB(0xff, 0x55, 0xff),  // [0xd] bright magenta
    CRGB(0xff, 0xff, 0x55),  // [0xe] yellow
    CRGB(0xff, 0xff, 0xff)   // [0xf] white
)

COLORMAPS(
    [0] = COLORMAP_STACKED
    (BLACK,   GREEN, GREEN, GREEN, GREEN, GREEN, BLUE,
     MAGENTA, CYAN,  CYAN,  CYAN,  CYAN,  CYAN,  RED,
     BROWN,   CYAN,  CYAN,  CYAN,  CYAN,  CYAN,
     BROWN,   CYAN,  CYAN,  CYAN,  CYAN,  CYAN,  RED,

     LIGHT_GRAY, RED, LIGHT_GRAY, LIGHT_GRAY,
     BLACK,

     BLACK,      BRIGHT_GREEN, BRIGHT_GREEN, BRIGHT_GREEN, BRIGHT_GREEN, BRIGHT_GREEN, BLACK,
     BRIGHT_RED, BRIGHT_CYAN,  BRIGHT_CYAN,  BRIGHT_CYAN,  BRIGHT_CYAN,  BRIGHT_CYAN,  YELLOW,
                 BRIGHT_CYAN,  BRIGHT_CYAN,  BRIGHT_CYAN,  BRIGHT_CYAN,  BRIGHT_RED,   BRIGHT_RED,
     BLACK,      BRIGHT_CYAN,  BRIGHT_CYAN,  BRIGHT_RED,   BRIGHT_RED,   BRIGHT_RED,   BRIGHT_RED,

     DARK_GRAY, BRIGHT_RED, DARK_GRAY, DARK_GRAY,
     BLACK)
)

// clang-format on

KALEIDOSCOPE_INIT_PLUGINS(EEPROMSettings,
                          EEPROMKeymap,
                          LEDControl,
                          LEDPaletteTheme,
                          LEDOff,
                          ColormapEffect,
                          DefaultColormap,
                          Focus,
                          FocusEEPROMCommand,
                          FocusSettingsCommand);

void setup() {
  Kaleidoscope.setup();

  EEPROMKeymap.setup(1);

  ColormapEffect.max_layers(1);
  ColormapEffect.activate();

  DefaultColormap.setup();
}

void loop() {
  Kaleidoscope.loop();
}
