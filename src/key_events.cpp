#include "Kaleidoscope.h"

static bool handle_synthetic_key_event(Key mappedKey, uint8_t keyState) {
    if (mappedKey.flags & RESERVED)
        return false;

    if (!(mappedKey.flags & SYNTHETIC))
        return false;

    if (mappedKey.flags & IS_INTERNAL) {
        return false;
    } else if (mappedKey.flags & IS_CONSUMER) {
      if (key_is_pressed(keyState))
        ConsumerControl.press(mappedKey.keyCode);
      else if (key_was_pressed(keyState))
        ConsumerControl.release(mappedKey.keyCode);
    } else if (mappedKey.flags & IS_SYSCTL) {
      if (key_is_pressed(keyState))
        SystemControl.press(mappedKey.keyCode);
      else if (key_was_pressed(keyState))
        SystemControl.release();
    } else if (mappedKey.flags & SWITCH_TO_KEYMAP) {
        // Should not happen, handled elsewhere.
    }

    return true;
}

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
        Keyboard.press(Key_LShift.keyCode);
    }
    if (mappedKey.flags & CTRL_HELD) {
        Keyboard.press(Key_LCtrl.keyCode);
    }
    if (mappedKey.flags & LALT_HELD) {
        Keyboard.press(Key_LAlt.keyCode);
    }
    if (mappedKey.flags & RALT_HELD) {
        Keyboard.press(Key_RAlt.keyCode);
    }
    if (mappedKey.flags & GUI_HELD) {
        Keyboard.press(Key_LGUI.keyCode);
    }
    Keyboard.press(mappedKey.keyCode);
}


void release_key(Key mappedKey) {
    if (mappedKey.flags & SHIFT_HELD) {
        Keyboard.release(Key_LShift.keyCode);
    }
    if (mappedKey.flags & CTRL_HELD) {
        Keyboard.release(Key_LCtrl.keyCode);
    }
    if (mappedKey.flags & LALT_HELD) {
        Keyboard.release(Key_LAlt.keyCode);
    }
    if (mappedKey.flags & RALT_HELD) {
        Keyboard.release(Key_RAlt.keyCode);
    }
    if (mappedKey.flags & GUI_HELD) {
        Keyboard.release(Key_LGUI.keyCode);
    }
    Keyboard.release(mappedKey.keyCode);
}

void handle_key_event(Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (!(keyState & INJECTED)) {
        mappedKey = Layer.lookup(row, col);
    }
    for (byte i = 0; Kaleidoscope.eventHandlers[i] != NULL && i < HOOK_MAX; i++) {
        Kaleidoscope_::eventHandlerHook handler = Kaleidoscope.eventHandlers[i];
        mappedKey = (*handler)(mappedKey, row, col, keyState);
        if (mappedKey.raw == Key_NoKey.raw)
            return;
    }
    mappedKey = Layer.eventHandler(mappedKey, row, col, keyState);
    if (mappedKey.raw == Key_NoKey.raw)
        return;
    handle_key_event_default(mappedKey, row, col, keyState);
}
