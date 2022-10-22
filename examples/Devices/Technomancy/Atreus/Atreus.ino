/* -*- mode: c++ -*-
 * Atreus -- Chrysalis-enabled Sketch for Technomancy's Atreus
 * Copyright (C) 2018-2022  Keyboard.io, Inc
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

#include "Kaleidoscope.h"
#include "Kaleidoscope-DynamicMacros.h"
#include "Kaleidoscope-EEPROM-Settings.h"
#include "Kaleidoscope-EEPROM-Keymap.h"
#include "Kaleidoscope-Escape-OneShot.h"
#include "Kaleidoscope-FirmwareVersion.h"
#include "Kaleidoscope-FocusSerial.h"
#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-OneShot.h"
#include "Kaleidoscope-Qukeys.h"
#include "Kaleidoscope-SpaceCadet.h"

#define MO(n) ShiftToLayer(n)
#define TG(n) LockLayer(n)

enum {
  RESET,
  QW
};

#define Key_Exclamation LSHIFT(Key_1)
#define Key_At          LSHIFT(Key_2)
#define Key_Hash        LSHIFT(Key_3)
#define Key_Dollar      LSHIFT(Key_4)
#define Key_And         LSHIFT(Key_7)
#define Key_Star        LSHIFT(Key_8)
#define Key_Plus        LSHIFT(Key_Equals)

enum {
  _QW,
  _RS,
  _LW
};

// clang-format off
KEYMAPS(
  [_QW] = KEYMAP_STACKED
  (
       Key_Q   ,Key_W   ,Key_E       ,Key_R         ,Key_T
      ,Key_A   ,Key_S   ,Key_D       ,Key_F         ,Key_G
      ,Key_Z   ,Key_X   ,Key_C       ,Key_V         ,Key_B
      ,Key_Esc ,Key_Tab ,Key_LeftGui ,Key_LeftShift ,Key_Backspace ,Key_LeftControl

                    ,Key_Y     ,Key_U   ,Key_I     ,Key_O      ,Key_P
                    ,Key_H     ,Key_J   ,Key_K     ,Key_L      ,Key_Semicolon
                    ,Key_N     ,Key_M   ,Key_Comma ,Key_Period ,Key_Slash
       ,Key_LeftAlt ,Key_Space ,MO(_RS) ,Key_Minus ,Key_Quote  ,Key_Enter
  ),

  [_RS] = KEYMAP_STACKED
  (
       Key_Exclamation ,Key_At           ,Key_UpArrow   ,Key_LeftCurlyBracket ,Key_RightCurlyBracket
      ,Key_Hash        ,Key_LeftArrow    ,Key_DownArrow ,Key_RightArrow       ,Key_Dollar
      ,Key_LeftBracket ,Key_RightBracket ,Key_LeftParen ,Key_RightParen       ,Key_And
      ,TG(_LW)         ,Key_Insert       ,Key_LeftGui   ,Key_LeftShift        ,Key_Backspace         ,Key_LeftControl

                   ,Key_PageUp   ,Key_7 ,Key_8      ,Key_9 ,Key_Star
                   ,Key_PageDown ,Key_4 ,Key_5      ,Key_6 ,Key_Plus
                   ,Key_Backtick ,Key_1 ,Key_2      ,Key_3 ,Key_Backslash
      ,Key_LeftAlt ,Key_Space    ,___   ,Key_Period ,Key_0 ,Key_Equals
   ),

  [_LW] = KEYMAP_STACKED
  (
       Key_Insert ,Key_Home                 ,Key_UpArrow   ,Key_End        ,Key_PageUp
      ,Key_Delete ,Key_LeftArrow            ,Key_DownArrow ,Key_RightArrow ,Key_PageDown
      ,XXX        ,Consumer_VolumeIncrement ,XXX           ,XXX            ,M(RESET)
      ,XXX        ,Consumer_VolumeDecrement ,___           ,___            ,___          ,___

                ,Key_UpArrow   ,Key_F7 ,Key_F8          ,Key_F9         ,Key_F10
                ,Key_DownArrow ,Key_F4 ,Key_F5          ,Key_F6         ,Key_F11
                ,XXX           ,Key_F1 ,Key_F2          ,Key_F3         ,Key_F12
      ,___      ,___           ,M(QW)  ,Key_PrintScreen ,Key_ScrollLock ,Consumer_PlaySlashPause
   )
)
// clang-format on

KALEIDOSCOPE_INIT_PLUGINS(
  EscapeOneShot,
  EEPROMSettings,
  EEPROMKeymap,
  Focus,
  FocusEEPROMCommand,
  FocusSettingsCommand,
  Macros,
  Qukeys,
  SpaceCadet,
  OneShot,
  MouseKeys,
  EscapeOneShotConfig,
  DynamicMacros,
  FirmwareVersion);

const macro_t *macroAction(uint8_t macro_id, KeyEvent &event) {
  switch (macro_id) {
  case RESET:
    Kaleidoscope.rebootBootloader();
    break;
  case QW:
    Layer.move(_QW);
    break;
  default:
    break;
  }

  return MACRO_NONE;
}

void setup() {
  Kaleidoscope.setup();

  EEPROMKeymap.setup(5);
  SpaceCadet.disable();
  DynamicMacros.reserve_storage(256);
  Layer.move(EEPROMSettings.default_layer());
}

void loop() {
  Kaleidoscope.loop();
}
