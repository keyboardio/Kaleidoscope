#pragma once
#include <Arduino.h>
#include "key_defs.h"

namespace kaleidoscope {
namespace hid {

// A facade on top of our HID implementation

void initializeKeyboard();

void pressKey(Key mappedKey);
void releaseKey(Key mappedKey);
void releaseAllKeys();
void pressRawKey(Key mappedKey);
void releaseRawKey(Key mappedKey);
/** Flushes any pending regular key switch events and sends them out */
void sendKeyboardReport();

boolean isModifierKeyActive(Key mappedKey);
boolean wasModifierKeyActive(Key mappedKey);

uint8_t getKeyboardLEDs();

void initializeConsumerControl();

void pressConsumerControl(Key mappedKey);
void releaseConsumerControl(Key mappedKey);

void initializeSystemControl();

void pressSystemControl(Key mappedKey);
void releaseSystemControl(Key mappedKey);

void initializeMouse();

void moveMouse(signed char x, signed char y, signed char wheel);
void clickMouseButtons(uint8_t buttons);
void pressMouseButtons(uint8_t buttons);
void releaseMouseButtons(uint8_t buttons);
void releaseAllMouseButtons(void);
void sendMouseReport(void);

void initializeAbsoluteMouse();

void moveAbsoluteMouse(signed char x, signed char y, signed char wheel);
void moveAbsoluteMouseTo(uint16_t x, uint16_t y, signed char wheel);
void clickAbsoluteMouseButtons(uint8_t buttons);
void pressAbsoluteMouseButtons(uint8_t buttons);
void releaseAbsoluteMouseButtons(uint8_t buttons);

}
};
