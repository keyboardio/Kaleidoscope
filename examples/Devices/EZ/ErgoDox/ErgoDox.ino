/* -*- mode: c++ -*-
 * ErgoDox -- Chrysalis-enabled Sketch for ErgoDox-compatible boards
 * Copyright (C) 2019-2022  Keyboard.io, Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/*
 * This is based on the QMK factory firmware the ErgoDox EZ ships with. Modeled
 * after the layout in
 * https://configure.ergodox-ez.com/layouts/default/latest/0, as of 2019-01-04.
 */

#include "Kaleidoscope.h"
#include "Kaleidoscope-DynamicMacros.h"
#include "Kaleidoscope-Escape-OneShot.h"
#include "Kaleidoscope-EEPROM-Settings.h"
#include "Kaleidoscope-EEPROM-Keymap.h"
#include "Kaleidoscope-FirmwareVersion.h"
#include "Kaleidoscope-FocusSerial.h"
#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-OneShot.h"
#include "Kaleidoscope-Qukeys.h"
#include "Kaleidoscope-SpaceCadet.h"

// clang-format off
KEYMAPS(
  [0] = KEYMAP_STACKED
  (
      // left hand
      Key_Equals,      Key_1,     Key_2,               Key_3,         Key_4,          Key_5,   Key_LeftArrow,
      Key_Delete,      Key_Q,     Key_W,               Key_E,         Key_R,          Key_T,   LockLayer(1),
      Key_Backspace,   Key_A,     Key_S,               Key_D,         Key_F,          Key_G,
      Key_LeftShift,   Key_Z,     Key_X,               Key_C,         Key_V,          Key_B,   Key_Hyper,
      LT(1, Backtick), Key_Quote, LALT(Key_LeftShift), Key_LeftArrow, Key_RightArrow,

                     MT(LeftAlt, PcApplication), Key_LeftGui,
                                                 Key_Home,
      Key_Space,     Key_Backspace,              Key_End,

      // right hand
      Key_RightArrow, Key_6, Key_7,       Key_8,         Key_9,           Key_0,            Key_Minus,
      LockLayer(1),   Key_Y, Key_U,       Key_I,         Key_O,           Key_P,            Key_Backslash,
                      Key_H, Key_J,       Key_K,         Key_L,           LT(2, Semicolon), MT(LeftGui, Quote),
      Key_Meh,        Key_N, Key_M,       Key_Comma,     Key_Period,      Key_Slash,        Key_RightShift,
                             Key_UpArrow, Key_DownArrow, Key_LeftBracket, Key_RightBracket, ShiftToLayer(1),

      Key_LeftAlt,  MT(LeftControl, Esc),
      Key_PageUp,
      Key_PageDown, Key_Tab, Key_Enter
  ),
  [1] = KEYMAP_STACKED
  (
      // left hand
      Key_Esc, Key_F1,        Key_F2,        Key_F3,                  Key_F4,                   Key_F5,                XXX,
      XXX,     LSHIFT(Key_1), LSHIFT(Key_2), LSHIFT(Key_LeftBracket), LSHIFT(Key_RightBracket), LSHIFT(Key_Backslash), ___,
      XXX,     LSHIFT(Key_3), LSHIFT(Key_4), LSHIFT(Key_9),           LSHIFT(Key_0),            Key_Backtick,
      XXX,     LSHIFT(Key_5), LSHIFT(Key_6), Key_LeftBracket,         Key_RightBracket,         LSHIFT(Key_Backtick),  XXX,
      ___,     XXX,           XXX,           XXX,                     XXX,

           XXX, XXX,
                XXX,
      XXX, XXX, XXX,

      // right hand
      XXX, Key_F6,        Key_F7, Key_F8,     Key_F9, Key_F10,       Key_F11,
      ___, Key_UpArrow,   Key_7,  Key_8,      Key_9,  LSHIFT(Key_8), Key_F12,
           Key_DownArrow, Key_4,  Key_5,      Key_6,  XXX,           XXX,
      XXX, LSHIFT(Key_7), Key_1,  Key_2,      Key_3,  Key_Backslash, XXX,
                          XXX,    Key_Period, Key_0,  Key_Equals,    ___,

      XXX, XXX,
      XXX,
      XXX, XXX, XXX
  ),
  [2] = KEYMAP_STACKED
  (
      // left hand
      XXX, XXX, XXX,        XXX,           XXX,           XXX, XXX,
      XXX, XXX, XXX,        Key_mouseUp,   XXX,           XXX, XXX,
      XXX, XXX, Key_mouseL, Key_mouseDn,   Key_mouseR,    XXX,
      XXX, XXX, XXX,        XXX,           XXX,           XXX,  XXX,
      XXX, XXX, XXX,        Key_mouseBtnL, Key_mouseBtnR,

           XXX, XXX,
                XXX,
      XXX, XXX, XXX,

      // right hand
      XXX, XXX, XXX,                      XXX,                        XXX,                    XXX, XXX,
      XXX, XXX, XXX,                      XXX,                        XXX,                    XXX, XXX,
           XXX, XXX,                      XXX,                        XXX,                    ___, Consumer_PlaySlashPause,
      XXX, XXX, XXX,                      Consumer_ScanPreviousTrack, Consumer_ScanNextTrack, XXX, XXX,
                Consumer_VolumeIncrement, Consumer_VolumeDecrement,   Consumer_Mute,          XXX, XXX,

      XXX, XXX,
      XXX,
      XXX, XXX, XXX
  ),
)
// clang-format on

KALEIDOSCOPE_INIT_PLUGINS(
  EEPROMSettings,
  EEPROMKeymap,
  Focus,
  FocusEEPROMCommand,
  FocusSettingsCommand,
  Qukeys,
  SpaceCadet,
  OneShot,
  EscapeOneShot,
  EscapeOneShotConfig,
  DynamicMacros,
  MouseKeys,
  FirmwareVersion);

void blinkAllStatusLEDs() {
  for (auto i = 0; i <= 3; i++) {
    Kaleidoscope.device().setStatusLED(i, false);
  }

  for (auto i = 1; i <= 3; i++) {
    Kaleidoscope.device().setStatusLED(i, true);
    _delay_ms(50);
  }

  for (auto i = 1; i <= 3; i++) {
    Kaleidoscope.device().setStatusLED(i, false);
    _delay_ms(50);
  }
}

void setup() {
  Kaleidoscope.setup();

  EEPROMKeymap.setup(5);
  SpaceCadet.disable();
  DynamicMacros.reserve_storage(256);

  blinkAllStatusLEDs();

  Layer.move(EEPROMSettings.default_layer());
}

void loop() {
  Kaleidoscope.loop();
}
