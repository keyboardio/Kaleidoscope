/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2018  Keyboard.io, Inc.
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

#pragma once
#include <Arduino.h>
#include "kaleidoscope/key_defs.h"

namespace kaleidoscope {
namespace hid {

// A facade on top of our HID implementation

extern void initializeKeyboard();

extern void pressKey(Key mappedKey, boolean toggledOn = true);
extern void releaseKey(Key mappedKey);
extern void releaseAllKeys();
extern void pressRawKey(Key mappedKey);
extern void releaseRawKey(Key mappedKey);
/** Flushes any pending regular key switch events and sends them out */
extern void sendKeyboardReport();

// Store a copy of the current report so that a mid-cycle report can be sent based on the
// previous report instead of the incomplete current report
extern void stashKeyboardReport();
// Restore the saved copy of the keyboard report so that the current cycle can be resumed
// properly after sending a mid-cycle report
extern void restoreKeyboardReport();

extern boolean isModifierKeyActive(Key mappedKey);
extern boolean wasModifierKeyActive(Key mappedKey);

extern boolean isAnyModifierKeyActive();
extern boolean wasAnyModifierKeyActive();

extern uint8_t getKeyboardLEDs();

extern void initializeConsumerControl();

extern void pressConsumerControl(Key mappedKey);
extern void releaseConsumerControl(Key mappedKey);

extern void initializeSystemControl();

extern void pressSystemControl(Key mappedKey);
extern void releaseSystemControl(Key mappedKey);

extern void initializeMouse();

extern void moveMouse(signed char x, signed char y, signed char vWheel = 0, signed char hWheel = 0);
/** stopMouse() stops mouse and/or mouse wheel movement in given directions.
 *
 * Counterpart of moveMouse(), this function allows us to undo whatever movement
 * we were supposed to make. The intended use-case is one where we send multiple
 * reports per cycle, and want greater control over them, when we don't want to
 * clear the whole report, just parts of it.
 *
 * Any of the arguments that is set to true, will be cleared from the report to
 * be sent by the next call to sendMouseReport().
 */
extern void stopMouse(bool x, bool y, bool vWheel = false, bool hWheel = false);
extern void clickMouseButtons(uint8_t buttons);
extern void pressMouseButtons(uint8_t buttons);
extern void releaseMouseButtons(uint8_t buttons);
extern void releaseAllMouseButtons(void);
extern void sendMouseReport(void);

extern void initializeAbsoluteMouse();

extern void moveAbsoluteMouse(signed char x, signed char y, signed char wheel);
extern void moveAbsoluteMouseTo(uint16_t x, uint16_t y, signed char wheel);
extern void clickAbsoluteMouseButtons(uint8_t buttons);
extern void pressAbsoluteMouseButtons(uint8_t buttons);
extern void releaseAbsoluteMouseButtons(uint8_t buttons);

}
};
