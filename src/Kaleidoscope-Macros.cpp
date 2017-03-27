#include "Kaleidoscope-Macros.h"

__attribute__((weak))
const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
    return MACRO_NONE;
}

byte Macros_::row, Macros_::col;

static void readKeyCodeAndPlay (const macro_t *macro_p, uint8_t flags, uint8_t keyStates) {
    Key key;
    key.flags = flags;
    key.keyCode = pgm_read_byte(macro_p++);

    if (keyStates & IS_PRESSED) {
        handle_key_event(key, UNKNOWN_KEYSWITCH_LOCATION, IS_PRESSED | INJECTED);
        Keyboard.sendReport();
    }
    if (keyStates & WAS_PRESSED) {
        handle_key_event(key, UNKNOWN_KEYSWITCH_LOCATION, WAS_PRESSED | INJECTED);
        Keyboard.sendReport();
    }
}

void Macros_::play(const macro_t *macro_p) {
    macro_t macro = END;
    uint8_t interval = 0;
    uint8_t flags;

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
            flags = pgm_read_byte(macro_p++);
            readKeyCodeAndPlay (macro_p++, flags, IS_PRESSED);
            break;
        case MACRO_ACTION_STEP_KEYUP:
            flags = pgm_read_byte(macro_p++);
            readKeyCodeAndPlay (macro_p++, flags, WAS_PRESSED);
            break;
        case MACRO_ACTION_STEP_TAP:
            flags = pgm_read_byte(macro_p++);
            readKeyCodeAndPlay (macro_p++, flags, IS_PRESSED | WAS_PRESSED);
            break;

        case MACRO_ACTION_STEP_KEYCODEDOWN:
            readKeyCodeAndPlay (macro_p++, 0, IS_PRESSED);
            break;
        case MACRO_ACTION_STEP_KEYCODEUP:
            readKeyCodeAndPlay (macro_p++, 0, WAS_PRESSED);
            break;
        case MACRO_ACTION_STEP_TAPCODE:
            readKeyCodeAndPlay (macro_p++, 0, IS_PRESSED | WAS_PRESSED);
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

    Macros_::row = row;
    Macros_::col = col;
    const macro_t *m = macroAction(mappedKey.keyCode, keyState);

    Macros.play(m);
    return Key_NoKey;
}

Macros_::Macros_ (void) {
}

void
Macros_::begin (void) {
    event_handler_hook_use (handleMacroEvent);
}

Macros_ Macros;
