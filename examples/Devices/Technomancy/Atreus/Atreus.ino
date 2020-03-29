/* -*- mode: c++ -*-
 * Atreus -- A very basic Kaleidoscope example for the Atreus
 * Copyright (C) 2018  Keyboard.io, Inc
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
#include "Kaleidoscope-Macros.h"

#define MO(n) ShiftToLayer(n)
#define TG(n) LockLayer(n)

enum {
  QWERTY,
  FUN,
  UPPER
};

#define Key_Exclamation LSHIFT(Key_1)
#define Key_At LSHIFT(Key_2)
#define Key_Hash LSHIFT(Key_3)
#define Key_Dollar LSHIFT(Key_4)
#define Key_And LSHIFT(Key_7)
#define Key_Star LSHIFT(Key_8)
#define Key_Plus LSHIFT(Key_Equals)

/* *INDENT-OFF* */
KEYMAPS(
  [QWERTY] = KEYMAP_STACKED
  (
       Key_Q   ,Key_W   ,Key_E       ,Key_R         ,Key_T
      ,Key_A   ,Key_S   ,Key_D       ,Key_F         ,Key_G
      ,Key_Z   ,Key_X   ,Key_C       ,Key_V         ,Key_B
      ,Key_Esc ,Key_Tab ,Key_LeftGui ,Key_LeftShift ,Key_Backspace ,Key_LeftControl

                    ,Key_Y     ,Key_U ,Key_I     ,Key_O      ,Key_P
                    ,Key_H     ,Key_J ,Key_K     ,Key_L      ,Key_Semicolon
                    ,Key_N     ,Key_M ,Key_Comma ,Key_Period ,Key_Slash
       ,Key_LeftAlt ,Key_Space ,MO(FUN) ,Key_Minus ,Key_Quote  ,Key_Enter
  ),

  [FUN] = KEYMAP_STACKED
  (
       Key_Exclamation ,Key_At           ,Key_UpArrow   ,Key_LeftCurlyBracket ,Key_RightCurlyBracket
      ,Key_Hash        ,Key_LeftArrow    ,Key_DownArrow ,Key_RightArrow       ,Key_Dollar
      ,Key_LeftBracket ,Key_RightBracket ,Key_LeftParen ,Key_RightParen       ,Key_And
      ,TG(UPPER)       ,Key_Insert       ,Key_LeftGui   ,Key_LeftShift        ,Key_Backspace         ,Key_LeftControl

                   ,Key_PageUp   ,Key_7   ,Key_8      ,Key_9 ,Key_Star
                   ,Key_PageDown ,Key_4   ,Key_5      ,Key_6 ,Key_Plus
                   ,Key_Backtick ,Key_1   ,Key_2      ,Key_3 ,Key_Backslash
      ,Key_LeftAlt ,Key_Space    ,MO(FUN) ,Key_Period ,Key_0 ,Key_Equals
   ),

  [UPPER] = KEYMAP_STACKED
  (
       Key_Insert ,Key_Home                 ,Key_UpArrow   ,Key_End        ,Key_PageUp
      ,Key_Delete ,Key_LeftArrow            ,Key_DownArrow ,Key_RightArrow ,Key_PageDown
      ,XXX        ,Consumer_VolumeIncrement ,XXX           ,XXX            ,___
      ,XXX        ,Consumer_VolumeDecrement ,___           ,___            ,___          ,___

                ,Key_UpArrow   ,Key_F7   ,Key_F8          ,Key_F9         ,Key_F10
                ,Key_DownArrow ,Key_F4   ,Key_F5          ,Key_F6         ,Key_F11
                ,XXX           ,Key_F1   ,Key_F2          ,Key_F3         ,Key_F12
      ,___      ,___           ,M(QWERTY),Key_PrintScreen ,Key_ScrollLock ,Consumer_PlaySlashPause
   )
)
/* *INDENT-ON* */

KALEIDOSCOPE_INIT_PLUGINS(Macros);

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  switch (macroIndex) {
  case QWERTY:
    Layer.move(QWERTY);
    break;
  default:
    break;
  }

  return MACRO_NONE;
}

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}
