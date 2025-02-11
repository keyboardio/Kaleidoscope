/* Kaleidoscope-Colormap-Overlay -- Per key colors overlaying active LED effect
 * Copyright (C) 2017-2020  Bart Nagel
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * Additional Permissions:
 * As an additional permission under Section 7 of the GNU General Public
 * License Version 3, you may link this software against a Vendor-provided
 * Hardware Specific Software Module under the terms of the MCU Vendor
 * Firmware Library Additional Permission Version 1.0.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LED-Palette-Theme.h>
#include <Kaleidoscope-Colormap-Overlay.h>

// clang-format off

KEYMAPS(
  [0] = KEYMAP_STACKED
  (XXX,          Key_1, Key_2, Key_3, Key_4, Key_5, Key_LEDEffectNext,
   Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
   Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,

   Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
   XXX,

   XXX,          Key_6, Key_7, Key_8,     Key_9,         Key_0,         XXX,
   Key_Enter,    Key_Y, Key_U, Key_I,     Key_O,         Key_P,         Key_Equals,
                 Key_H, Key_J, Key_K,     Key_L,         Key_Semicolon, Key_Quote,
   Key_RightAlt, Key_N, Key_M, Key_Comma, Key_Period,    Key_Slash,     Key_Minus,

   Key_RightShift, Key_LeftAlt, Key_Spacebar, Key_RightControl,
   XXX)
) // KEYMAPS(

// clang-format on

enum {
  PALETTE_RED,
  PALETTE_GREEN,
  PALETTE_BLUE,
  PALETTE_BLACK,
  PALETTE_UNUSED_1,
  PALETTE_UNUSED_2,
  PALETTE_UNUSED_3,
  PALETTE_UNUSED_4,
  PALETTE_UNUSED_5,
  PALETTE_UNUSED_6,
  PALETTE_UNUSED_7,
  PALETTE_UNUSED_8,
  PALETTE_UNUSED_9,
  PALETTE_UNUSED_10,
  PALETTE_UNUSED_11,
  PALETTE_UNUSED_12,
};

PALETTE(
  [PALETTE_RED]   = CRGB(0xff, 0x00, 0x00),
  [PALETTE_GREEN] = CRGB(0x00, 0xff, 0x00),
  [PALETTE_BLUE]  = CRGB(0x00, 0x00, 0xff),
  [PALETTE_BLACK] = CRGB(0x00, 0x00, 0x00),
  // unused
  [PALETTE_UNUSED_1]  = CRGB(0x00, 0x00, 0x00),
  [PALETTE_UNUSED_2]  = CRGB(0x00, 0x00, 0x00),
  [PALETTE_UNUSED_3]  = CRGB(0x00, 0x00, 0x00),
  [PALETTE_UNUSED_4]  = CRGB(0x00, 0x00, 0x00),
  [PALETTE_UNUSED_5]  = CRGB(0x00, 0x00, 0x00),
  [PALETTE_UNUSED_6]  = CRGB(0x00, 0x00, 0x00),
  [PALETTE_UNUSED_7]  = CRGB(0x00, 0x00, 0x00),
  [PALETTE_UNUSED_8]  = CRGB(0x00, 0x00, 0x00),
  [PALETTE_UNUSED_9]  = CRGB(0x00, 0x00, 0x00),
  [PALETTE_UNUSED_10] = CRGB(0x00, 0x00, 0x00),
  [PALETTE_UNUSED_11] = CRGB(0x00, 0x00, 0x00),
  [PALETTE_UNUSED_12] = CRGB(0x00, 0x00, 0x00))

KALEIDOSCOPE_INIT_PLUGINS(EEPROMSettings,
                          LEDControl,
                          ColormapOverlay,
                          DefaultPalette);

void setup() {
  Kaleidoscope.setup();

  COLORMAP_OVERLAYS(
    // Make the any key green
    kaleidoscope::plugin::Overlay(0, KeyAddr(0, 9), PALETTE_GREEN),
    // Make numlock red
    kaleidoscope::plugin::Overlay(0, KeyAddr(0, 15), PALETTE_RED),
    // Make escape blue
    kaleidoscope::plugin::Overlay(0, KeyAddr(2, 6), PALETTE_BLUE),
    // Disable leds on the palm keys
    kaleidoscope::plugin::Overlay(0, KeyAddr(3, 6), PALETTE_BLACK),
    kaleidoscope::plugin::Overlay(0, KeyAddr(3, 9), PALETTE_BLACK))

  ColormapOverlay.setup();
  DefaultPalette.setup();
}

void loop() {
  Kaleidoscope.loop();
}
