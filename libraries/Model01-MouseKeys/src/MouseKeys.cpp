#include <Arduino.h>

#include "MouseKeys.h"
#include "MouseWrapper.h"
#include "KeyboardioFirmware.h"

static void handle_mouse_key_event(Key mappedKey, uint8_t state) {
    if (!key_is_pressed(state))
        return;

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

static bool handleMouseKeys(Key mappedKey, byte row, byte col, uint8_t state) {
    if (! (mappedKey.flags & IS_INTERNAL)
            && (mappedKey.rawKey == KEY_MOUSE_BTN_L
                || mappedKey.rawKey == KEY_MOUSE_BTN_M
                || mappedKey.rawKey == KEY_MOUSE_BTN_R)) {
        if (key_toggled_on(state)) {
            MouseWrapper.press_button(mappedKey.rawKey);
        } else if (key_toggled_off(state)) {
            MouseWrapper.release_button(mappedKey.rawKey);
        }
        return true;
    }

    if (!(mappedKey.flags & IS_MOUSE_KEY))
        return false;

    if (!(mappedKey.rawKey & KEY_MOUSE_WARP)) {
        handle_mouse_key_event(mappedKey, state);
    } else if (key_toggled_on(state)) {
        if (mappedKey.rawKey & KEY_MOUSE_WARP && mappedKey.flags & IS_MOUSE_KEY) {
            // we don't pass in the left and up values because those are the
            // default, "no-op" conditionals
            MouseWrapper.warp( ((mappedKey.rawKey & KEY_MOUSE_WARP_END) ? WARP_END : 0x00) |
                               ((mappedKey.rawKey & KEY_MOUSE_DOWN) ? WARP_DOWN : 0x00) |
                               ((mappedKey.rawKey & KEY_MOUSE_RIGHT) ? WARP_RIGHT : 0x00) );
        }
    }

    return true;
}

MouseKeys_::MouseKeys_(void) {
    event_handler_hook_add (handleMouseKeys);
}

MouseKeys_ MouseKeys;
