#pragma once

#include <KeyboardioFirmware.h>

#include "MacroKeyDefs.h"
#include "MacroSteps.h"

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState);

class Macros_ : public KeyboardioPlugin {
  public:
    Macros_(void);

    virtual void begin(void) final;

    void play(const macro_t *macro_p);
};

extern Macros_ Macros;
