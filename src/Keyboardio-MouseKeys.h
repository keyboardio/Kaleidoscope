#pragma once

#include "KeyboardioFirmware.h"
#include "MouseKeyDefs.h"

class MouseKeys_ : public KeyboardioPlugin {
  public:
    MouseKeys_ (void);

    virtual void begin(void) final;

 private:
    static uint8_t mouseMoveIntent;

    static void loopHook(bool postClear);
    static Key eventHandlerHook(Key mappedKey, byte row, byte col, uint8_t keyState);
};

extern MouseKeys_ MouseKeys;
