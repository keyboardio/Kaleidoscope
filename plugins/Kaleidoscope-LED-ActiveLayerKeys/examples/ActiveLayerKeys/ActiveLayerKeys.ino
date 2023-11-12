// -*- mode: c++ -*-
/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2017-2020  Bart Nagel
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

#include "Kaleidoscope.h"
#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-LED-ActiveLayerKeys.h"

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
   XXX),

  [1] =  KEYMAP_STACKED
  (___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___,
   ___,

   M(MACRO_VERSION_INFO),  ___, Key_7, Key_8,      Key_9,              Key_KeypadSubtract, ___,
   ___,                    ___, Key_4, Key_5,      Key_6,              Key_KeypadAdd,      ___,
                           ___, Key_1, Key_2,      Key_3,              Key_Equals,         ___,
   ___,                    ___, Key_0, Key_Period, Key_KeypadMultiply, Key_KeypadDivide,   Key_Enter,
   ___, ___, ___, ___,
   ___),

  [2] =  KEYMAP_STACKED
  (___,      Key_F1,           Key_F2,      Key_F3,     Key_F4,        Key_F5,           Key_CapsLock,
   Key_Tab,  ___,              Key_mouseUp, ___,        Key_mouseBtnR, Key_mouseWarpEnd, Key_mouseWarpNE,
   Key_Home, Key_mouseL,       Key_mouseDn, Key_mouseR, Key_mouseBtnL, Key_mouseWarpNW,
   Key_End,  Key_PrintScreen,  Key_Insert,  ___,        Key_mouseBtnM, Key_mouseWarpSW,  Key_mouseWarpSE,
   ___, Key_Delete, ___, ___,
   ___,

   Consumer_ScanPreviousTrack, Key_F6,                 Key_F7,                   Key_F8,                   Key_F9,          Key_F10,          Key_F11,
   Consumer_PlaySlashPause,    Consumer_ScanNextTrack, Key_LeftCurlyBracket,     Key_RightCurlyBracket,    Key_LeftBracket, Key_RightBracket, Key_F12,
                               Key_LeftArrow,          Key_DownArrow,            Key_UpArrow,              Key_RightArrow,  ___,              ___,
   Key_PcApplication,          Consumer_Mute,          Consumer_VolumeDecrement, Consumer_VolumeIncrement, ___,             Key_Backslash,    Key_Pipe,
   ___, ___, Key_Enter, ___,
   ___)
) // KEYMAPS(

// clang-format on

KALEIDOSCOPE_INIT_PLUGINS(
  // LEDControl provides support for other LED modes
  LEDControl,

  // Support for ActiveLayerKeys
  LEDActiveLayerKeysEffect);


void setup() {
  static const cRGB layerColormap[] PROGMEM = {
    CRGB(0xff, 0x00, 0x00),  // red, for the first layer
    CRGB(0x00, 0xff, 0x00),  // green, for the second layer
    CRGB(0x00, 0x00, 0xff),  // blue, for the third layer
  };

  Kaleidoscope.setup();
  LEDActiveLayerKeysEffect.setColormap(layerColormap);
  // Light up leds of keys on lower layers as well
  LEDActiveLayerKeysEffect.lightLowerLayers(true);
  LEDActiveLayerKeysEffect.activate();
}

void loop() {
  Kaleidoscope.loop();
}
