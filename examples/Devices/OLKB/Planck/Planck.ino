/* -*- mode: c++ -*-
 * Planck -- A very basic Kaleidoscope example for the OLKB Planck keyboard
 * Copyright (C) 2018  Keyboard.io, Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTabILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "Kaleidoscope.h"
#include "Kaleidoscope-Macros.h"

enum {
  _QWERTY,
  _COLEMAK,
  _DVORAK,
  _LOWER,
  _RAISE,
  _PLOVER,
  _ADJUST
};


// clang-format off
KEYMAPS(

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Brite| Ctrl | Alt  | GUI  |Lower |    Space    |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */


[_QWERTY] = KEYMAP(
    Key_Tab,  Key_Q,    Key_W,    Key_E,    Key_R,    Key_T,    Key_Y,    Key_U,    Key_I,    Key_O,    Key_P,    Key_Backspace,
    Key_Escape,  Key_A,    Key_S,    Key_D,    Key_F,    Key_G,    Key_H,    Key_J,    Key_K,    Key_L,    Key_Semicolon, Key_Quote,
    Key_LeftShift, Key_Z,    Key_X,    Key_C,    Key_V,    Key_B,    Key_N,    Key_M,    Key_Comma, Key_Period,  Key_Slash, Key_Enter ,
    ___, Key_LeftControl, Key_LeftAlt, Key_LeftGui, LockLayer(_LOWER),   Key_Space,  Key_Space,  LockLayer(_RAISE),   Key_LeftArrow, Key_DownArrow, Key_UpArrow,   Key_RightArrow

),
/* Colemak
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   F  |   P  |   G  |   J  |   L  |   U  |   Y  |   ;  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   R  |   S  |   T  |   D  |   H  |   N  |   E  |   I  |   O  |  "   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   K  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Brite| Ctrl | Alt  | GUI  |Lower |    Space    |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_COLEMAK] = KEYMAP(
    Key_Tab,  Key_Q,    Key_W,    Key_F,    Key_P,    Key_G,    Key_J,    Key_L,    Key_U,    Key_Y,    Key_Semicolon, Key_Backspace,
    Key_Escape,  Key_A,    Key_R,    Key_S,    Key_T,    Key_D,    Key_H,    Key_N,    Key_E,    Key_I,    Key_O,    Key_Quote,
    Key_LeftShift, Key_Z,    Key_X,    Key_C,    Key_V,    Key_B,    Key_K,    Key_M,    Key_Comma, Key_Period,  Key_Slash, Key_Enter ,
    ___, Key_LeftControl, Key_LeftAlt, Key_LeftGui, ShiftToLayer(_LOWER),   Key_Space,  Key_Space,  ShiftToLayer(_RAISE),   Key_LeftArrow, Key_DownArrow, Key_UpArrow,   Key_RightArrow
),

/* Dvorak
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   "  |   ,  |   .  |   P  |   Y  |   F  |   G  |   C  |   R  |   L  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   O  |   E  |   U  |   I  |   D  |   H  |   T  |   N  |   S  |  /   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   ;  |   Q  |   J  |   K  |   X  |   B  |   M  |   W  |   V  |   Z  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Brite| Ctrl | Alt  | GUI  |Lower |    Space    |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_DVORAK] = KEYMAP(
    Key_Tab,  Key_Quote, Key_Comma, Key_Period,  Key_P,    Key_Y,    Key_F,    Key_G,    Key_C,    Key_R,    Key_L,    Key_Backspace,
    Key_Escape,  Key_A,    Key_O,    Key_E,    Key_U,    Key_I,    Key_D,    Key_H,    Key_T,    Key_N,    Key_S,    Key_Slash,
    Key_LeftShift, Key_Semicolon, Key_Q,    Key_J,    Key_K,    Key_X,    Key_B,    Key_M,    Key_W,    Key_V,    Key_Z,    Key_Enter ,
    ___, Key_LeftControl, Key_LeftAlt, Key_LeftGui, LockLayer(_LOWER),   Key_Space,  Key_Space,  LockLayer(_RAISE),   Key_LeftArrow, Key_DownArrow, Key_UpArrow,   Key_RightArrow
),
/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |   ~  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Del  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   _  |   +  |   {  |   }  |  |   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |ISO ~ |ISO | | Home | End  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      | Next | Vol- | Vol+ | Play |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = KEYMAP(
    LSHIFT(Key_Backtick),  LSHIFT(Key_1),    LSHIFT(Key_2),    LSHIFT(Key_3),    LSHIFT(Key_4),    LSHIFT(Key_5),    LSHIFT(Key_6),    LSHIFT(Key_7),    LSHIFT(Key_8),    LSHIFT(Key_9),    LSHIFT(Key_0), Key_Backspace,
    Key_Delete,  Key_F1,   Key_F2,   Key_F3,   Key_F4,   Key_F5,   Key_F6,   LSHIFT(Key_Minus),    LSHIFT(Key_Equals),    Key_LeftBracket, Key_RightBracket, Key_Pipe,
    ___, Key_F7,   Key_F8,   Key_F9,   Key_F10,  Key_F11,  Key_F12,  LSHIFT(Key_NonUsPound), LSHIFT(Key_NonUsBackslashAndPipe), Key_Home, Key_End,  ___,
    ___, ___, ___, ___, ___, ___, ___, ___,    Consumer_ScanNextTrack,    Consumer_VolumeDecrement, Consumer_VolumeIncrement, Consumer_PlaySlashPause
),

/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Del  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   -  |   =  |   [  |   ]  |  \   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |ISO # |ISO / |Pg Up |Pg Dn |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      | Next | Vol- | Vol+ | Play |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = KEYMAP(
    Key_Backtick,  Key_1,    Key_2,    Key_3,    Key_4,    Key_5,    Key_6,    Key_7,    Key_8,    Key_9,    Key_0,    Key_Backspace,
    Key_Delete,  Key_F1,   Key_F2,   Key_F3,   Key_F4,   Key_F5,   Key_F6,   Key_Minus, Key_Equals,  Key_LeftBracket, Key_RightBracket, Key_Backslash,
    ___, Key_F7,   Key_F8,   Key_F9,   Key_F10,  Key_F11,  Key_F12,  Key_NonUsPound, Key_NonUsBackslashAndPipe, Key_PageUp, Key_PageDown, ___,
    ___, ___, ___, ___, ___, ___, ___, ___, Consumer_ScanNextTrack, Consumer_VolumeDecrement, Consumer_VolumeIncrement, Consumer_PlaySlashPause
),

/* Plover layer (http://opensteno.org)
 * ,-----------------------------------------------------------------------------------.
 * |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |   S  |   T  |   P  |   H  |   *  |   *  |   F  |   P  |   L  |   T  |   D  |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |   S  |   K  |   W  |   R  |   *  |   *  |   R  |   B  |   G  |   S  |   Z  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Exit |      |      |   A  |   O  |             |   E  |   U  |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */

[_PLOVER] = KEYMAP(
    LSHIFT(Key_1),    LSHIFT(Key_1),    LSHIFT(Key_1),    LSHIFT(Key_1),    LSHIFT(Key_1),    LSHIFT(Key_1),    LSHIFT(Key_1),    LSHIFT(Key_1),    LSHIFT(Key_1),    LSHIFT(Key_1),    LSHIFT(Key_1),    LSHIFT(Key_1)   ,
    XXX, Key_Q,    Key_W,    Key_E,    Key_R,    Key_T,    Key_Y,    Key_U,    Key_I,    Key_O,    Key_P,    Key_LeftBracket,
    XXX, Key_A,    Key_S,    Key_D,    Key_F,    Key_G,    Key_H,    Key_J,    Key_K,    Key_L,    Key_Semicolon, Key_Quote,
    LockLayer(_QWERTY), XXX, XXX, Key_C,    Key_V,    XXX, XXX, Key_N,    Key_M,    XXX, XXX, XXX
),

/* Adjust (Lower + Raise)
 * ,-----------------------------------------------------------------------------------.
 * |      | Reset|      |      |      |      |      |      |      |      |      |  Del |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |      |      |Aud on|Audoff|AGnorm|AGswap|Qwerty|Colemk|Dvorak|Plover|      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |Voice-|Voice+|Mus on|Musoff|MIDIon|MIDIof|      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
//[_ADJUST] = KEYMAP(
//    ___, RESET,   DEBUG,    RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, Key_Delete ,
//    ___, ___, MU_MOD,  AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, LockLayer(_QWERTY),  LockLayer(_COLEMAK), LockLayer(_DVORAK),  LockLayer(_PLOVER),  ___,
//    ___, MUV_DE,  MUV_IN,  MU_ON,   MU_OFF,  MI_ON,   MI_OFF,  TERM_ON, TERM_OFF, ___, ___, ___,
//    ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___
//)
);
// clang-format on

KALEIDOSCOPE_INIT_PLUGINS(Macros);

void setup() {
  Kaleidoscope.setup();
  Kaleidoscope.serialPort().begin(9600);
}

void loop() {
  Kaleidoscope.loop();
}
