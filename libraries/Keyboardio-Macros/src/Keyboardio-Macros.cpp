#include "Keyboardio-Macros.h"

__attribute__((weak))
void
macroAction(uint8_t macroIndex, uint8_t keyState) {
}

static bool handleMacroEvent(Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (!(mappedKey.flags & (SYNTHETIC|IS_MACRO)))
        return false;

    macroAction(mappedKey.rawKey, keyState);
    return true;
}

Macros_::Macros_ (void) {
    event_handler_hook_add (handleMacroEvent);
}

Macros_ Macros;
