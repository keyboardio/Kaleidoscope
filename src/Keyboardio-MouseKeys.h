#pragma once

#include "KeyboardioFirmware.h"
#include "MouseKeyDefs.h"

class MouseKeys_ : public KeyboardioPlugin {
  public:
    MouseKeys_ (void);

    virtual void begin(void) final;
};

extern MouseKeys_ MouseKeys;
