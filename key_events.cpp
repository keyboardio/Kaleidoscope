#include "key_events.h"

const Key keymaps[KEYMAPS][ROWS][COLS] = { KEYMAP_LIST };

void handle_synthetic_key_event(Key mappedKey, uint8_t currentState, uint8_t previousState) {
    if (key_toggled_on(currentState,previousState)) {
        if (akelaIF.register_code (&akelaIF.hid, &akelaIF.keyMap, mappedKey.keyCode))
            return;
    } else if (key_toggled_off(currentState,previousState)) {
        if (akelaIF.unregister_code (&akelaIF.hid, &akelaIF.keyMap, mappedKey.keyCode))
            return;
    }
    if (mappedKey.flags & IS_MOUSE_KEY && !( mappedKey.rawKey & KEY_MOUSE_WARP) ) {
        handle_mouse_key_event(mappedKey, currentState, previousState);
    } else if (! (mappedKey.flags & IS_INTERNAL)
               && (mappedKey.rawKey == KEY_MOUSE_BTN_L
                   || mappedKey.rawKey == KEY_MOUSE_BTN_M
                   || mappedKey.rawKey == KEY_MOUSE_BTN_R)) {
        if (key_toggled_on(currentState, previousState)) {
            MouseWrapper.press_button(
                (mappedKey.rawKey ==  KEY_MOUSE_BTN_L ?  KEY_MOUSE_BUTTON_LEFT   : 0x00) |
                (mappedKey.rawKey ==  KEY_MOUSE_BTN_M ?  KEY_MOUSE_BUTTON_MIDDLE : 0x00) |
                (mappedKey.rawKey ==  KEY_MOUSE_BTN_R ?  KEY_MOUSE_BUTTON_RIGHT  : 0x00) );
        } else if (key_toggled_off(currentState, previousState)) {
            MouseWrapper.release_button(
                (mappedKey.rawKey ==  KEY_MOUSE_BTN_L ?  KEY_MOUSE_BUTTON_LEFT   : 0x00) |
                (mappedKey.rawKey ==  KEY_MOUSE_BTN_M ?  KEY_MOUSE_BUTTON_MIDDLE : 0x00) |
                (mappedKey.rawKey ==  KEY_MOUSE_BTN_R ?  KEY_MOUSE_BUTTON_RIGHT  : 0x00) );
        }
    }


    else if (key_toggled_on(currentState,previousState)) {
        if (mappedKey.rawKey & KEY_MOUSE_WARP && mappedKey.flags & IS_MOUSE_KEY) {
            // we don't pass in the left and up values because those are the
            // default, "no-op" conditionals
            MouseWrapper.warp( ((mappedKey.rawKey & KEY_MOUSE_WARP_END) ? WARP_END : 0x00) |
                               ((mappedKey.rawKey & KEY_MOUSE_DOWN) ? WARP_DOWN : 0x00) |
                               ((mappedKey.rawKey & KEY_MOUSE_RIGHT) ? WARP_RIGHT : 0x00) );
        } else if (mappedKey.flags & IS_CONSUMER) {
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
void handle_key_event(byte row, byte col, uint8_t currentState, uint8_t previousState) {
    //for every newly pressed button, figure out what logical key it is and send a key down event
    // for every newly released button, figure out what logical key it is and send a key up event

    Key mappedKey = keymaps[temporary_keymap][row][col];

    if (keymaps[primary_keymap][row][col].flags & SWITCH_TO_KEYMAP) {
        handle_keymap_key_event(keymaps[primary_keymap][row][col], currentState, previousState);
    } else if (mappedKey.flags & SYNTHETIC_KEY) {
        handle_synthetic_key_event( mappedKey, currentState, previousState);
    } else if (key_is_pressed(currentState, previousState)) {
        press_key(mappedKey);
    }
}

void press_key(Key mappedKey) {
    if (mappedKey.flags & SHIFT_HELD) {
        Keyboard.press(Key_LShift.rawKey);
    }
    if (mappedKey.flags & CTRL_HELD) {
        Keyboard.press(Key_LCtrl.rawKey);
    }
    if (mappedKey.flags & ALT_HELD) {
        Keyboard.press(Key_RAlt.rawKey);
    }
    if (mappedKey.flags & GUI_HELD) {
        Keyboard.press(Key_LGUI.rawKey);
    }
    Keyboard.press(mappedKey.rawKey);
}


void handle_keymap_key_event(Key keymapEntry, uint8_t currentState, uint8_t previousState) {
    if (keymapEntry.flags & MOMENTARY ) {
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

void handle_mouse_key_event(Key mappedKey, uint8_t currentState, uint8_t previousState) {
    if (key_is_pressed(currentState,previousState)) {
        if (mappedKey.rawKey & KEY_MOUSE_UP) {
            MouseWrapper.move(0,-1);
        }
        if (mappedKey.rawKey & KEY_MOUSE_DOWN) {
            MouseWrapper.move(0,1);
        }
        if (mappedKey.rawKey & KEY_MOUSE_LEFT) {
            MouseWrapper.move(-1,0);
        }
        if (mappedKey.rawKey & KEY_MOUSE_RIGHT) {
            MouseWrapper.move(1,0);
        }
    }
}

