/* -*- mode: c++ -*-
 * Kaleidoscope - A Kaleidoscope example
 * Copyright (C) 2016-2019  Keyboard.io, Inc.
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

#define DEBUG_SERIAL false

#include "Kaleidoscope.h"
#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-NumPad.h"
#include "Kaleidoscope-HardwareTestMode.h"
#include "Kaleidoscope-MagicCombo.h"

#include "Kaleidoscope-LEDEffect-SolidColor.h"
#include "Kaleidoscope-LEDEffect-Breathe.h"
#include "Kaleidoscope-LEDEffect-Chase.h"
#include "Kaleidoscope-LEDEffect-Rainbow.h"

#define NUMPAD_KEYMAP 2


#define GENERIC_FN2  KEYMAP_STACKED ( \
         ___,      Key_F1,        Key_F2,      Key_F3,     Key_F4,          Key_F5,          XXX,            \
        Key_Tab,  Key_mouseBtnM, Key_mouseUp, ___,        Key_mouseWarpNW, Key_mouseWarpNE, Consumer_ScanNextTrack, \
        Key_Home, Key_mouseL,    Key_mouseDn, Key_mouseR, Key_mouseWarpSW, Key_mouseWarpSE, \
        Key_End,  Key_Z,         Key_X,       Key_C,      Key_V,           Key_mouseWarpEnd, ___,    \
                                                        Key_LeftControl, Key_mouseBtnL, Key_LeftGui, Key_LeftShift,                \
                                                                                    ___, \
\
        XXX,        Key_F6,         Key_F7,            Key_F8,                   Key_F9,              ___,                   ___, \
        Key_Delete, Consumer_PlaySlashPause,  Key_LeftCurlyBracket, Key_RightCurlyBracket, Key_LeftBracket, Key_RightBracket, System_Sleep, \
                    Key_LeftArrow,            Key_DownArrow,        Key_UpArrow,           Key_RightArrow,  Key_F11,                   Key_F12, \
        ___,        Consumer_VolumeDecrement, Consumer_VolumeIncrement,   Key_BacklightDown,   Key_BacklightUp,    Key_Backslash,      Key_Pipe, \
        Key_RightShift, Key_RightAlt, Key_mouseBtnR, Key_RightControl, \
        ___\
)




#define NUMPAD KEYMAP  (\
         ___, ___, ___, ___, ___, ___, ___,                           ___, ___, Key_Keypad7, Key_Keypad8,   Key_Keypad9,        Key_KeypadSubtract, ___, \
        ___, ___, ___, ___, ___, ___, ___,                            ___, ___, Key_Keypad4, Key_Keypad5,   Key_Keypad6,        Key_KeypadAdd,  ___, \
        ___, ___, ___, ___, ___, ___,                                      ___, Key_Keypad1, Key_Keypad2,   Key_Keypad3,        Key_Equals,      Key_Quote, \
        ___, ___, ___, ___, ___, ___, ___,                            ___, ___, Key_Keypad0, Key_KeypadDot, Key_KeypadMultiply, Key_KeypadDivide, Key_Enter, \
       Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,    Key_RightShift, Key_RightAlt, Key_Spacebar, Key_RightControl, \
                                 Key_Keymap1_Momentary,               Key_Keymap1_Momentary \
)

#define QWERTY KEYMAP ( \
        ___,          Key_1, Key_2, Key_3, Key_4, Key_5, Key_LEDEffectNext,         ___,        Key_6, Key_7, Key_8,     Key_9,      Key_0,         Key_KeypadNumLock, \
        Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,                   Key_Enter,  Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals, \
        Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,                                        Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote, \
        Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,                 ___,       Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus, \
                 Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,        Key_RightShift, Key_RightAlt, Key_Spacebar, Key_RightControl, \
                                              Key_KeymapNext_Momentary,         Key_KeymapNext_Momentary \
)

KEYMAPS(
  QWERTY,
  GENERIC_FN2,
  NUMPAD
)


static kaleidoscope::plugin::LEDSolidColor solidRed(60, 0, 0);
static kaleidoscope::plugin::LEDSolidColor solidOrange(60, 20, 0);
static kaleidoscope::plugin::LEDSolidColor solidYellow(40, 35, 0);
static kaleidoscope::plugin::LEDSolidColor solidGreen(0, 100, 0);
static kaleidoscope::plugin::LEDSolidColor solidBlue(0, 15, 100);
static kaleidoscope::plugin::LEDSolidColor solidIndigo(0, 0, 100);
static kaleidoscope::plugin::LEDSolidColor solidViolet(70, 0, 60);

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  if (macroIndex == 1 && keyToggledOn(keyState)) {
    Kaleidoscope.serialPort().print("Keyboard.IO keyboard driver v0.00");
    return MACRO(I(25),
                 D(LeftShift), T(M), U(LeftShift), T(O), T(D), T(E), T(L),
                 T(Spacebar),
                 W(100),
                 T(0), T(1));
  }
  return MACRO_NONE;
}

/**
 *  This enters the hardware test mode
 */
static void enterHardwareTestMode(uint8_t combo_index) {
  HardwareTestMode.runTests();
}


/** Magic combo list, a list of key combo and action pairs the firmware should
 * recognise.
 */
USE_MAGIC_COMBOS({
  .action = enterHardwareTestMode,
  // Left Fn + Prog + LED
  .keys = { R3C6, R0C0, R0C6 }
});

KALEIDOSCOPE_INIT_PLUGINS(HardwareTestMode,
                          LEDControl, LEDOff,
                          solidRed, solidOrange, solidYellow, solidGreen, solidBlue, solidIndigo, solidViolet,
                          LEDBreatheEffect, LEDRainbowEffect, LEDChaseEffect, NumPad,
                          Macros,
                          MouseKeys,
                          MagicCombo);

void setup() {
  Kaleidoscope.setup();

  NumPad.numPadLayer = NUMPAD_KEYMAP;
  LEDOff.activate();
}


void loop() {
  Kaleidoscope.loop();
}
