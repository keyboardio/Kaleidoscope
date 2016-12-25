#include "Keyboardio-Macros.h"

__attribute__((weak))
const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
    return MACRO_NONE;
}

void Macros_::play(const macro_t *macro_p) {
    macro_t macro = END;
    uint8_t interval = 0;
    Key key;

    if (!macro_p)
        return;

    while (true) {
        switch (macro = pgm_read_byte(macro_p++)) {
        case MACRO_ACTION_STEP_INTERVAL:
            interval = pgm_read_byte(macro_p++);
            break;
        case MACRO_ACTION_STEP_WAIT: {
            uint8_t wait = pgm_read_byte(macro_p++);
            delay(wait);
            break;
        }
        case MACRO_ACTION_STEP_KEYDOWN:
            key.flags = pgm_read_byte(macro_p++);
            key.rawKey = pgm_read_byte(macro_p++);
            handle_key_event(key, 255, 255, IS_PRESSED | INJECTED);
            Keyboard.sendReport();
            break;
        case MACRO_ACTION_STEP_KEYUP:
            key.flags = pgm_read_byte(macro_p++);
            key.rawKey = pgm_read_byte(macro_p++);
            handle_key_event(key, 255, 255, WAS_PRESSED | INJECTED);
            Keyboard.sendReport();
            break;
        case END:
        default:
            return;
        }

        delay(interval);
    }
}

static Key handleMacroEvent(Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (mappedKey.flags != (SYNTHETIC | IS_MACRO))
        return mappedKey;

    if (!key_toggled_on(keyState))
      return Key_NoKey;

    const macro_t *m = macroAction(mappedKey.rawKey, keyState);

    Macros.play(m);
    return Key_NoKey;
}

Macros_::Macros_ (void) {
}

void
Macros_::begin (void) {
    event_handler_hook_add (handleMacroEvent);
}

Macros_ Macros;
