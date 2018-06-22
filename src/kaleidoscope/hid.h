#pragma once
#include <Arduino.h>
#include "key_defs.h"

namespace kaleidoscope {
namespace hid {

// A facade on top of our HID implementation

extern void initializeKeyboard();

extern void pressToggledOnKey(Key mappedKey);

extern void pressKey(Key mappedKey);
extern void releaseKey(Key mappedKey);
extern void releaseAllKeys();
extern void pressRawKey(Key mappedKey);
extern void releaseRawKey(Key mappedKey);
/** Flushes any pending regular key switch events and sends them out */
extern void sendKeyboardReport();

extern boolean isModifierKeyActive(Key mappedKey);
extern boolean wasModifierKeyActive(Key mappedKey);

extern uint8_t getKeyboardLEDs();

extern void initializeConsumerControl();

extern void pressConsumerControl(Key mappedKey);
extern void releaseConsumerControl(Key mappedKey);

extern void initializeSystemControl();

extern void pressSystemControl(Key mappedKey);
extern void releaseSystemControl(Key mappedKey);

extern void initializeMouse();

extern void moveMouse(signed char x, signed char y, signed char vWheel = 0, signed char hWheel = 0);
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
