#include "key_events.h"

void handle_synthetic_key_event(Key mappedKey, uint8_t currentState, uint8_t previousState) {
    if (key_toggled_on(currentState,previousState)) {
        if (mappedKey.flags & IS_CONSUMER) {
            ConsumerControl.press(mappedKey.rawKey);
        } else if (mappedKey.flags & IS_INTERNAL) {
            if (mappedKey.rawKey == LED_TOGGLE) {
                LEDControl.next_mode();
            }
        } else if (mappedKey.flags & IS_SYSCTL) {
            SystemControl.press(mappedKey.rawKey);
        } else  if (mappedKey.flags & IS_MACRO) {
            if (mappedKey.rawKey == 1) {
                Serial.print("Keyboard.IO keyboard driver v0.00");
            }
        }

    }
}

custom_handler_t eventHandlers[HOOK_MAX];

Key lookup_key(byte keymap, byte row, byte col) {
    Key mappedKey;

    mappedKey.raw = pgm_read_word(&(keymaps[keymap][row][col]));

    return mappedKey;
}

void handle_key_event(Key mappedKey, byte row, byte col, uint8_t currentState, uint8_t previousState) {
    if (!(currentState & INJECTED)) {
        mappedKey = lookup_key(temporary_keymap, row, col);
    }
    for (byte i = 0; eventHandlers[i] != NULL && i < HOOK_MAX; i++) {
        custom_handler_t handler = eventHandlers[i];
        if ((*handler)(mappedKey, row, col, currentState, previousState))
            return;
    }
}

bool handle_key_event_default(Key mappedKey, byte row, byte col, uint8_t currentState, uint8_t previousState) {
    //for every newly pressed button, figure out what logical key it is and send a key down event
    // for every newly released button, figure out what logical key it is and send a key up event

    Key baseKey = Key_NoKey;
    if (!(currentState & INJECTED)) {
      baseKey = lookup_key(primary_keymap, row, col);
    }

    if ((baseKey.flags & SWITCH_TO_KEYMAP
      || baseKey.flags & SWITCH_TO_KEYMAP_MOMENTARY)) {
        handle_keymap_key_event(baseKey, currentState, previousState);
    } else if (mappedKey.flags & SYNTHETIC) {
        handle_synthetic_key_event( mappedKey, currentState, previousState);
    } else if (key_is_pressed(currentState, previousState)) {
        press_key(mappedKey);
    }
    return true;
}

void press_key(Key mappedKey) {
    if (mappedKey.flags & SHIFT_HELD) {
        Keyboard.press(Key_LShift.rawKey);
    }
    if (mappedKey.flags & CTRL_HELD) {
        Keyboard.press(Key_LCtrl.rawKey);
    }
    if (mappedKey.flags & LALT_HELD) {
        Keyboard.press(Key_LAlt.rawKey);
    }
    if (mappedKey.flags & RALT_HELD) {
        Keyboard.press(Key_RAlt.rawKey);
    }
    if (mappedKey.flags & GUI_HELD) {
        Keyboard.press(Key_LGUI.rawKey);
    }
    Keyboard.press(mappedKey.rawKey);
}


void handle_keymap_key_event(Key keymapEntry, uint8_t currentState, uint8_t previousState) {
    if (keymapEntry.flags & SWITCH_TO_KEYMAP_MOMENTARY ) {
        if (key_toggled_on(currentState, previousState)) {
            if ( keymapEntry.rawKey == KEYMAP_NEXT) {
                temporary_keymap++;
            } else if ( keymapEntry.rawKey == KEYMAP_PREVIOUS) {
                temporary_keymap--;
            } else {
                temporary_keymap = keymapEntry.rawKey;
            }
        }
        if (key_toggled_off(currentState, previousState)) {
            temporary_keymap = primary_keymap;
        }

        // switch keymap and stay there
    } else if (key_toggled_on(currentState, previousState)) {
        temporary_keymap = primary_keymap = keymapEntry.rawKey;
        Storage.save_primary_keymap(primary_keymap);
    }
}
