#pragma once

#include <KeyboardioFirmware.h>

#include "MacroKeyDefs.h"
#include "MacroSteps.h"

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState);

class Macros_ {
  public:
    Macros_(void);

    void play(const macro_t *macro_p);
};

static Macros_ Macros;
