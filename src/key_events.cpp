#include "key_events.h"
#include "layers.h"

static bool handle_synthetic_key_event(Key mappedKey, uint8_t keyState) {
    if (mappedKey.flags & RESERVED)
        return false;

    if (!(mappedKey.flags & SYNTHETIC))
        return false;

    if (!key_toggled_on(keyState))
        return true;

    if (mappedKey.flags & IS_INTERNAL) {
        if (mappedKey.flags & LED_TOGGLE) {
            LEDControl.next_mode();
        } else {
            return false;
        }
    } else if (mappedKey.flags & IS_CONSUMER) {
        ConsumerControl.press(mappedKey.rawKey);
    } else if (mappedKey.flags & IS_SYSCTL) {
        SystemControl.press(mappedKey.rawKey);
    } else if (mappedKey.flags & SWITCH_TO_KEYMAP) {
        // Should not happen, handled elsewhere.
    }

    return true;
}

custom_handler_t eventHandlers[HOOK_MAX];

static bool handle_key_event_default(Key mappedKey, byte row, byte col, uint8_t keyState) {
    //for every newly pressed button, figure out what logical key it is and send a key down event
    // for every newly released button, figure out what logical key it is and send a key up event

    if (mappedKey.flags & SYNTHETIC) {
        handle_synthetic_key_event( mappedKey, keyState);
    } else if (key_is_pressed(keyState)) {
        press_key(mappedKey);
    } else if (key_toggled_off(keyState) && (keyState & INJECTED)) {
        release_key(mappedKey);
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


void release_key(Key mappedKey) {
    if (mappedKey.flags & SHIFT_HELD) {
        Keyboard.release(Key_LShift.rawKey);
    }
    if (mappedKey.flags & CTRL_HELD) {
        Keyboard.release(Key_LCtrl.rawKey);
    }
    if (mappedKey.flags & LALT_HELD) {
        Keyboard.release(Key_LAlt.rawKey);
    }
    if (mappedKey.flags & RALT_HELD) {
        Keyboard.release(Key_RAlt.rawKey);
    }
    if (mappedKey.flags & GUI_HELD) {
        Keyboard.release(Key_LGUI.rawKey);
    }
    Keyboard.release(mappedKey.rawKey);
}

void handle_key_event(Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (!(keyState & INJECTED)) {
        mappedKey = Layer.lookup(row, col);
    }
    for (byte i = 0; eventHandlers[i] != NULL && i < HOOK_MAX; i++) {
        custom_handler_t handler = eventHandlers[i];
        mappedKey = (*handler)(mappedKey, row, col, keyState);
        if (mappedKey.raw == Key_NoKey.raw)
            return;
    }
    handle_key_event_default(mappedKey, row, col, keyState);
}
