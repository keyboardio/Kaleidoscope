/* -*- mode: c++ -*-
 * Imago.ino -- Example sketch for the Keyboardio Imago
 * Copyright (C) 2018, 2019, 2020  Keyboard.io, Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License,	or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTabILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not,	write to the Free Software Foundation,	Inc.,
 * 51 Franklin Street,	Fifth Floor,	Boston,	MA 02110-1301 USA.
 */

#include "Kaleidoscope.h"
#include "Kaleidoscope-Macros.h"

// Support for controlling the keyboard's LEDs
#include "Kaleidoscope-LEDControl.h"

// Support for the "Boot greeting" effect, which pulses the 'LED' button for 10s
// when the keyboard is connected to a computer (or that computer is powered on)
#include "Kaleidoscope-LEDEffect-BootGreeting.h"

// Support for LED modes that set all LEDs to a single color
#include "Kaleidoscope-LEDEffect-SolidColor.h"

// Support for an LED mode that makes all the LEDs 'breathe'
#include "Kaleidoscope-LEDEffect-Breathe.h"


#include "Kaleidoscope-LEDEffect-Chase.h"

// Support for LED modes that pulse the keyboard's LED in a rainbow pattern
#include "Kaleidoscope-LEDEffect-Rainbow.h"


// Support for host power management (suspend & wakeup)
#include "Kaleidoscope-HostPowerManagement.h"

// Support for magic combos (key chords that trigger an action)
#include "Kaleidoscope-MagicCombo.h"

// Support for USB quirks, like changing the key state report protocol
#include "Kaleidoscope-USB-Quirks.h"


enum {
  _QWERTY,
};


// clang-format off
KEYMAPS(

[_QWERTY] = KEYMAP(

Key_F1, Key_Escape,	Key_Backtick,	Key_1,		Key_2,	 	Key_3,		Key_4,	   Key_5, Key_6,	Key_7,		Key_8, 		Key_9,	    Key_0, 	Key_Minus,	Key_Equals,	Key_Backspace,
Key_F2, Key_Tab,	Key_Q,	 	Key_W,		Key_E,	 	Key_R,		Key_T,	   			Key_Y, 		Key_U,	 	Key_I,	    Key_O,      Key_P,	 	Key_LeftBracket, Key_RightBracket, Key_Backslash,
Key_F3, Key_Escape,	Key_A,	 	Key_S,	 	Key_D,	 	Key_F,		Key_G,	   			Key_H, 		Key_J,	 	Key_K,	      Key_L,	    Key_Semicolon,	Key_Quote, Key_Enter,
Key_F4, Key_LeftShift,	Key_Z,	 	Key_X,	 	Key_C,	 	Key_V,		Key_B,	    Key_UpArrow,   	Key_N, 		Key_M,	 	Key_Comma,    Key_Period,	Key_Slash,	Key_RightShift,	Key_LEDEffectNext,
Key_F5,	Key_LeftControl,Key_LeftAlt,	Key_LeftGui, 	Key_Backspace,     Key_LeftArrow, 		    Key_DownArrow,	Key_RightArrow, Key_Space, 	Key_RightAlt, Key_Menu, Key_RightControl, Key_LEDEffectNext


));
// clang-format on


// These 'solid' color effect definitions define a rainbow of
// LED color modes calibrated to draw 500mA or less on the
// Keyboardio Model 01.


static kaleidoscope::plugin::LEDSolidColor solidRed(160, 0, 0);
static kaleidoscope::plugin::LEDSolidColor solidOrange(140, 70, 0);
static kaleidoscope::plugin::LEDSolidColor solidYellow(130, 100, 0);
static kaleidoscope::plugin::LEDSolidColor solidGreen(0, 160, 0);
static kaleidoscope::plugin::LEDSolidColor solidBlue(0, 70, 130);
static kaleidoscope::plugin::LEDSolidColor solidIndigo(0, 0, 170);
static kaleidoscope::plugin::LEDSolidColor solidViolet(130, 0, 120);


/** toggleLedsOnSuspendResume toggles the LEDs off when the host goes to sleep,
 * and turns them back on when it wakes up.
 */
void toggleLedsOnSuspendResume(kaleidoscope::plugin::HostPowerManagement::Event event) {
  switch (event) {
  case kaleidoscope::plugin::HostPowerManagement::Suspend:
    LEDControl.disable();
    break;
  case kaleidoscope::plugin::HostPowerManagement::Resume:
    LEDControl.enable();
    break;
  case kaleidoscope::plugin::HostPowerManagement::Sleep:
    break;
  }
}

/** hostPowerManagementEventHandler dispatches power management events (suspend,
 * resume, and sleep) to other functions that perform action based on these
 * events.
 */
void hostPowerManagementEventHandler(kaleidoscope::plugin::HostPowerManagement::Event event) {
  toggleLedsOnSuspendResume(event);
}

/** This 'enum' is a list of all the magic combos used by the Model 01's
 * firmware The names aren't particularly important. What is important is that
 * each is unique.
 *
 * These are the names of your magic combos. They will be used by the
 * `USE_MAGIC_COMBOS` call below.
 */
enum {
  // Toggle between Boot (6-key rollover; for BIOSes and early boot) and NKRO
  // mode.
  COMBO_TOGGLE_NKRO_MODE
};

/** A tiny wrapper, to be used by MagicCombo.
 * This simply toggles the keyboard protocol via USBQuirks, and wraps it within
 * a function with an unused argument, to match what MagicCombo expects.
 */
static void toggleKeyboardProtocol(uint8_t combo_index) {
  USBQuirks.toggleKeyboardProtocol();
}

/** Magic combo list, a list of key combo and action pairs the firmware should
 * recognise.
 */
USE_MAGIC_COMBOS(
  {.action = toggleKeyboardProtocol,
   // Left Fn + Esc + Shift
   .keys = {R3C6, R2C6, R3C7}});


KALEIDOSCOPE_INIT_PLUGINS(
  Macros,

  // LEDControl provides support for other LED modes
  LEDControl,


  // The rainbow effect changes the color of all of the keyboard's keys at the same time
  // running through all the colors of the rainbow.
  LEDRainbowEffect,

  // The rainbow wave effect lights up your keyboard with all the colors of a rainbow
  // and slowly moves the rainbow across your keyboard
  LEDRainbowWaveEffect,

  // The chase effect follows the adventure of a blue pixel which chases a red pixel across
  // your keyboard. Spoiler: the blue pixel never catches the red pixel
  LEDChaseEffect,

  // These static effects turn your keyboard's LEDs a variety of colors
  solidRed,
  solidOrange,
  solidYellow,
  solidGreen,
  solidBlue,
  solidIndigo,
  solidViolet,

  // The breathe effect slowly pulses all of the LEDs on your keyboard
  LEDBreatheEffect,

  // The HostPowerManagement plugin allows us to turn LEDs off when then host
  // goes to sleep, and resume them when it wakes up.
  HostPowerManagement,

  // The MagicCombo plugin lets you use key combinations to trigger custom
  // actions - a bit like Macros, but triggered by pressing multiple keys at the
  // same time.
  MagicCombo,

  // The USBQuirks plugin lets you do some things with USB that we aren't
  // comfortable - or able - to do automatically, but can be useful
  // nevertheless. Such as toggling the key report protocol between Boot (used
  // by BIOSes) and Report (NKRO).
  USBQuirks);

void setup() {
  Kaleidoscope.setup();
  Serial.begin(9600);
}

void loop() {
  Kaleidoscope.loop();
}
