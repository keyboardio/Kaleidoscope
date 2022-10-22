/* -*- mode: c++ -*-
 * Splitography-Sketch -- A complete, functional sketch for Splitography
 * Copyright (C) 2018-2022  Keyboard.io, Inc
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
 *
 * Modeled after the default QMK layout:
 *  https://github.com/sdothum/qmk_firmware/blob/d865c82efa19beb7cb593e7d3affb2311017833e/keyboards/splitography/keymaps/default/keymap.c
 */

#include "Kaleidoscope.h"
#include "Kaleidoscope-Escape-OneShot.h"
#include "Kaleidoscope-DynamicMacros.h"
#include "Kaleidoscope-EEPROM-Settings.h"
#include "Kaleidoscope-EEPROM-Keymap.h"
#include "Kaleidoscope-FirmwareVersion.h"
#include "Kaleidoscope-FocusSerial.h"
#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-OneShot.h"
#include "Kaleidoscope-Ranges.h"
#include "Kaleidoscope-Qukeys.h"
#include "Kaleidoscope-SpaceCadet.h"
#include "Kaleidoscope-Steno.h"

// Layers
enum {
  _QWERTY,
  _BLUE,
  _ORANGE,
  _GREEN,
  _STENO,
  _PLOVER
};

// Custom keys
enum {
  QWERTY_1 = kaleidoscope::ranges::SAFE_START,
  QWERTY_2
};

#define QWERTY1   Key(QWERTY_1)
#define QWERTY2   Key(QWERTY_2)

#define MO(layer) ShiftToLayer(layer)
#define TO(layer) LockLayer(layer)

#define K_STP     Consumer_Stop
#define K_PRV     Consumer_ScanPreviousTrack
#define K_NXT     Consumer_ScanNextTrack
#define K_PLY     Consumer_PlaySlashPause

// Key aliases
#define Key_PgUp   Key_PageUp
#define Key_PageDn Key_PageDown
#define Key_PgDn   Key_PageDown
#define Key_Del    Key_Delete
#define Key_Grave  Key_Backtick
#define K_APP      Key_PcApplication
#define K_SCRLK    Key_ScrollLock
#define K_CPSLK    Key_CapsLock
#define K_PAUSE    Key_Pause
#define K_PRSCR    Key_PrintScreen
#define K_MUTE     Consumer_Mute
#define K_VUp      Consumer_VolumeIncrement
#define K_VDn      Consumer_VolumeDecrement
#define K_PST      LCTRL(Key_V)
#define K_CPY      LCTRL(Key_LeftControl)
#define K_CUT      LCTRL(Key_X)
#define K_UDO      LCTRL(Key_Z)

#define KP_0       Key_Keypad0
#define KP_1       Key_Keypad1
#define KP_2       Key_Keypad2
#define KP_3       Key_Keypad3
#define KP_4       Key_Keypad4
#define KP_5       Key_Keypad5
#define KP_6       Key_Keypad6
#define KP_7       Key_Keypad7
#define KP_8       Key_Keypad8
#define KP_9       Key_Keypad9

#define Key_Up     Key_UpArrow
#define Key_Dn     Key_DownArrow
#define Key_Left   Key_LeftArrow
#define Key_Rgt    Key_RightArrow
#define KP_SLS     Key_KeypadDivide
#define KP_STR     Key_KeypadMultiply
#define Key_Plus   Key_KeypadAdd

// clang-format off
KEYMAPS(
/* QWERTY
 * ,-------------------------.      ,--------------------------.
 * | Esc | Q | W | E | R | T |      | Y | U | I | O | P | Bspc |
 * |-----+---+---+---+---+---|      |---+---+---+---+---+------|
 * | Alt | A | S | D | F | G |      | H | J | K | L | ; | Entr |
 * |-----+---+---+---+---+---|      |---+---+---+---+---+------|
 * | Sft | Z | X | C | V | B |      | N | M | , | . | / | Gui  |
 * `-------------+---+---+---'      `-+---+---+----------------'
 *               |ORG|BLU|            |SPC|CTR|
 *               `-------'            `-------'
 */
 [_QWERTY] = KEYMAP(
    Key_Esc       ,Key_Q ,Key_W ,Key_E ,Key_R ,Key_T    ,Key_Y ,Key_U ,Key_I     ,Key_O      ,Key_P         ,Key_Backspace
   ,Key_LeftAlt   ,Key_A ,Key_S ,Key_D ,Key_F ,Key_G    ,Key_H ,Key_J ,Key_K     ,Key_L      ,Key_Semicolon ,Key_Enter
   ,Key_LeftShift ,Key_Z ,Key_X ,Key_C ,Key_V ,Key_B    ,Key_N ,Key_M ,Key_Comma ,Key_Period ,Key_Slash     ,Key_LeftGui
                             ,MO(_ORANGE) ,MO(_BLUE)    ,Key_Space ,Key_LeftControl
  ),

/* Blue
 * ,-----------------------------------------.      ,----------------------------.
 * |  `  |  1   |  2   |  3   |   4   |  5   |      |  6   | 7 | 8 | 9 | 0 |     |
 * |-----+------+------+------+-------+------|      |------+---+---+---+---+-----|
 * | Alt | Stop | Prev | Play | Next  | Vol+ |      |      |   |   | [ | ] |  '  |
 * |-----+------+------+------+-------+------|      |------+---+---+---+---+-----|
 * | Sft | Undo | Cut  | Copy | Paste | Vol- |      | Mute |   |   | - | = | Gui |
 * `----------------------+------+------+----'      `-+---+---+------------------'
 *                        | GRN  |  []  |             |Del|CTR|
 *                        `-------------'             `-------'
 */
 [_BLUE] = KEYMAP(
    Key_Grave     ,Key_1 ,Key_2 ,Key_3 ,Key_4 ,Key_5    ,Key_6  ,Key_7 ,Key_8 ,Key_9           ,Key_0            ,XXX
   ,Key_LeftAlt   ,K_STP ,K_PRV ,K_PLY ,K_NXT ,K_VUp    ,XXX    ,XXX   ,XXX   ,Key_LeftBracket ,Key_RightBracket ,Key_Quote
   ,Key_LeftShift ,K_UDO ,K_CUT ,K_CPY ,K_PST ,K_VDn    ,K_MUTE ,XXX   ,XXX   ,Key_Minus       ,Key_Equals       ,Key_LeftGui
                              ,MO(_GREEN) ,___           ,Key_Del ,Key_LeftControl
  ),

/* Orange
 * ,------------------------------------------.      ,-----------------------------------------.
 * | Plvr |  F1  |  F2  |  F3  |  F4   |      |      |  App | PrScr | ScrLck | Pause |   |     |
 * |------+------+------+------+-------+------|      |------+-------+--------+-------+---+-----|
 * | Alt  |  F5  |  F6  |  F7  |  F8   |      |      |      |  Ins  |  Home  | PgUp  |   |     |
 * |------+------+------+------+-------+------|      |------+-------+--------+-------+---+-----|
 * | Sft  |  F9  |  F10 |  F11 |  F12  |      |      |      |  Del  |  End   | PgDn  | \ | Gui |
 * `----------------------+------+------+-----'      `-+---+---+-------------------------------'
 *                        |  []  | GRN  |              |Tab|CTR|
 *                        `-------------'              `-------'
 */
 [_ORANGE] = KEYMAP(
    TO(_PLOVER)   ,Key_F1 ,Key_F2  ,Key_F3  ,Key_F4  ,XXX    ,K_APP ,K_PRSCR    ,K_SCRLK  ,K_PAUSE    ,XXX           ,XXX
   ,Key_LeftAlt   ,Key_F5 ,Key_F6  ,Key_F7  ,Key_F8  ,XXX    ,XXX   ,Key_Insert ,Key_Home ,Key_PageUp ,XXX           ,XXX
   ,Key_LeftShift ,Key_F9 ,Key_F10 ,Key_F11 ,Key_F12 ,XXX    ,XXX   ,Key_Delete ,Key_End  ,Key_PageDn ,Key_Backslash ,Key_LeftGui
                                     ,___   ,MO(_GREEN)          ,Key_Tab ,Key_LeftControl
  ),

/* Green
 * ,----------------------------------------.      ,--------------------------.
 * | STENO |      |    |     |      | ScrLk |      | / | 7 | 8 | 9 | - |      |
 * |-------+------+----+-----+------+-------|      |---+---+---+---+---+------|
 * | Alt   | Home | Up | End | PgUp | CpsLk |      | * | 4 | 5 | 6 | + | Entr |
 * |-------+------+----+-----+------+-------|      |---+---+---+---+---+------|
 * | Sft   | Left | Dn | Rgt | PgDn |       |      | 0 | 1 | 2 | 3 |   | Gui  |
 * `---------------------------+----+----+--'      `-+---+---+----------------'
 *                             | [] | [] |           |   |CTR|
 *                             `---------'           `-------'
 */

 [_GREEN] = KEYMAP(
    TO(_STENO)    ,XXX      ,XXX    ,XXX     ,XXX      ,K_SCRLK    ,KP_SLS ,KP_7  ,KP_8 ,KP_9 ,Key_Minus     ,XXX
   ,Key_LeftAlt   ,Key_Home ,Key_Up ,Key_End ,Key_PgUp ,K_CPSLK    ,KP_STR ,KP_4  ,KP_5 ,KP_6 ,Key_Plus      ,Key_Enter
   ,Key_LeftShift ,Key_Left ,Key_Dn ,Key_Rgt ,Key_PgDn ,XXX        ,KP_0   ,KP_1  ,KP_2 ,KP_3 ,XXX           ,Key_LeftGui
                                       ,___         ,___             ,XXX  ,Key_LeftControl
  ),

/* Steno (GeminiPR)
 * ,-----------------------.      ,-----------------------.
 * | # | # | # | # | # | # |      | # | # | # | # | # | # |
 * |---+---+---+---+---+---|      |---+---+---+---+---+---|
 * |QWR| S | T | P | H | * |      | * | F | P | L | T | D |
 * |---+---+---+---+---+---|      |---+---+---+---+---+---|
 * |QWR| S | K | W | R | * |      | * | R | B | G | S | Z |
 * `-------------+---+---+-'      `-+---+---+-------------'
 *               | A | O |          | E | U |
 *               `-------'          `-------'
 */
 [_STENO] = KEYMAP(
    S(N1)    ,S(N2) ,S(N3) ,S(N4) ,S(N5) ,S(N6)    ,S(N7)  ,S(N8) ,S(N9) ,S(NA) ,S(NB) ,S(NC)
   ,QWERTY1  ,S(S1) ,S(TL) ,S(PL) ,S(HL) ,S(ST1)   ,S(ST3) ,S(FR) ,S(PR) ,S(LR) ,S(TR) ,S(DR)
   ,QWERTY2  ,S(S2) ,S(KL) ,S(WL) ,S(RL) ,S(ST2)   ,S(ST4) ,S(RR) ,S(BR) ,S(GR) ,S(SR) ,S(ZR)
                                   ,S(A)  ,S(O)     ,S(E)   ,S(U)
  ),

/* Steno (Keyboard, QWERTY)
 * ,-----------------------.      ,-----------------------.
 * | 1 | 1 | 1 | 1 | 1 | 1 |      | 1 | 1 | 1 | 1 | 1 | 1 |
 * |---+---+---+---+---+---|      |---+---+---+---+---+---|
 * |QWR| S | T | P | H | * |      | * | F | P | L | T | D |
 * |---+---+---+---+---+---|      |---+---+---+---+---+---|
 * |QWR| S | K | W | R | * |      | * | R | B | G | S | Z |
 * `-------------+---+---+-'      `-+---+---+-------------'
 *               | A | O |          | E | U |
 *               `-------'          `-------'
 */
 [_PLOVER] = KEYMAP(
    Key_1    ,Key_1 ,Key_1 ,Key_1 ,Key_1 ,Key_1    ,Key_1 ,Key_1 ,Key_1 ,Key_1 ,Key_1         ,Key_1
   ,QWERTY1  ,Key_Q ,Key_W ,Key_E ,Key_R ,Key_T    ,Key_Y ,Key_U ,Key_I ,Key_O ,Key_P         ,Key_LeftBracket
   ,QWERTY2  ,Key_A ,Key_S ,Key_D ,Key_F ,Key_G    ,Key_H ,Key_J ,Key_K ,Key_L ,Key_Semicolon ,Key_Quote
                                   ,Key_C ,Key_V    ,Key_N ,Key_M
  )
);
// clang-format on

namespace kaleidoscope {
namespace plugin {
class MultiSwitcher : public kaleidoscope::Plugin {
 public:
  MultiSwitcher() {}

  EventHandlerResult onKeyEvent(KeyEvent &event) {
    if (event.key < QWERTY_1 || event.key > QWERTY_2)
      return EventHandlerResult::OK;

    uint8_t bit = event.key.getRaw() - QWERTY_1;

    if (keyToggledOn(event.state)) {
      switch_state_ |= (1 << bit);

      if (switch_state_ == (1 << 0 | 1 << 1)) {
        Layer.move(_QWERTY);
      }
    } else {
      switch_state_ &= ~(1 << bit);
    }

    return EventHandlerResult::EVENT_CONSUMED;
  }

 private:
  uint8_t switch_state_ = 0;
};
}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::MultiSwitcher MultiSwitcher;

KALEIDOSCOPE_INIT_PLUGINS(
  EscapeOneShot,
  GeminiPR,
  MultiSwitcher,
  Focus,
  EEPROMSettings,
  EEPROMKeymap,
  FocusEEPROMCommand,
  FocusSettingsCommand,
  Qukeys,
  SpaceCadet,
  OneShot,
  MouseKeys,
  EscapeOneShotConfig,
  DynamicMacros,
  FirmwareVersion);

void setup() {
  Kaleidoscope.setup();
  EEPROMKeymap.setup(6);
  SpaceCadet.disable();
  DynamicMacros.reserve_storage(256);
  Layer.move(EEPROMSettings.default_layer());
}

void loop() {
  Kaleidoscope.loop();
}
