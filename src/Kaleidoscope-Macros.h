#pragma once

#include <Kaleidoscope.h>

#include "MacroKeyDefs.h"
#include "MacroSteps.h"

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState);

class Macros_ : public KaleidoscopePlugin {
 public:
  Macros_(void);

  void begin(void) final;

  void play(const macro_t *macro_p);

  static byte row, col;
};

extern Macros_ Macros;
