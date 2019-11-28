/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2019  Keyboard.io, Inc.
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

#include <Kaleidoscope-HIDAdaptor-KeyboardioHID.h>
#include "kaleidoscope/key_defs.h"
#include "kaleidoscope_internal/deprecations.h"

namespace kaleidoscope {
namespace hid {

extern void initializeKeyboard() DEPRECATED(HID_FACADE);
extern void pressKey(Key mappedKey, boolean toggledOn = true) DEPRECATED(HID_FACADE);
extern void releaseKey(Key mappedKey) DEPRECATED(HID_FACADE);
extern void releaseAllKeys() DEPRECATED(HID_FACADE);
extern void pressRawKey(Key mappedKey) DEPRECATED(HID_FACADE);
extern void releaseRawKey(Key mappedKey) DEPRECATED(HID_FACADE);
/** Flushes any pending regular key switch events and sends them out */
extern void sendKeyboardReport() DEPRECATED(HID_FACADE);

extern boolean isModifierKeyActive(Key mappedKey) DEPRECATED(HID_FACADE);
extern boolean wasModifierKeyActive(Key mappedKey) DEPRECATED(HID_FACADE);

extern boolean isAnyModifierKeyActive() DEPRECATED(HID_FACADE);
extern boolean wasAnyModifierKeyActive() DEPRECATED(HID_FACADE);

extern uint8_t getKeyboardLEDs() DEPRECATED(HID_FACADE);

extern void initializeConsumerControl() DEPRECATED(HID_FACADE);

extern void pressConsumerControl(Key mappedKey) DEPRECATED(HID_FACADE);
extern void releaseConsumerControl(Key mappedKey) DEPRECATED(HID_FACADE);

extern void initializeSystemControl() DEPRECATED(HID_FACADE);

extern void pressSystemControl(Key mappedKey) DEPRECATED(HID_FACADE);
extern void releaseSystemControl(Key mappedKey) DEPRECATED(HID_FACADE);

extern void initializeMouse() DEPRECATED(HID_FACADE);

extern void moveMouse(signed char x, signed char y, signed char vWheel = 0, signed char hWheel = 0) DEPRECATED(HID_FACADE);
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
extern void stopMouse(bool x, bool y, bool vWheel = false, bool hWheel = false) DEPRECATED(HID_FACADE);
extern void clickMouseButtons(uint8_t buttons) DEPRECATED(HID_FACADE);
extern void pressMouseButtons(uint8_t buttons) DEPRECATED(HID_FACADE);
extern void releaseMouseButtons(uint8_t buttons) DEPRECATED(HID_FACADE);
extern void releaseAllMouseButtons(void) DEPRECATED(HID_FACADE);
extern void sendMouseReport(void) DEPRECATED(HID_FACADE);

extern void initializeAbsoluteMouse() DEPRECATED(HID_FACADE);

extern void moveAbsoluteMouse(signed char x, signed char y, signed char wheel) DEPRECATED(HID_FACADE);
extern void moveAbsoluteMouseTo(uint16_t x, uint16_t y, signed char wheel) DEPRECATED(HID_FACADE);
extern void clickAbsoluteMouseButtons(uint8_t buttons) DEPRECATED(HID_FACADE);
extern void pressAbsoluteMouseButtons(uint8_t buttons) DEPRECATED(HID_FACADE);
extern void releaseAbsoluteMouseButtons(uint8_t buttons) DEPRECATED(HID_FACADE);

}
};
