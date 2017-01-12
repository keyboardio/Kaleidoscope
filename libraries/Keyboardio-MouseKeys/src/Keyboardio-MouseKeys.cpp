#include <Arduino.h>

#include "Keyboardio-MouseKeys.h"
#include "MouseWrapper.h"
#include "KeyboardioFirmware.h"

static void handle_mouse_key_event(Key mappedKey, uint8_t keyState) {
    if (key_toggled_off(keyState)) {
	    if (mappedKey.keyCode & KEY_MOUSE_UP || mappedKey.keyCode & KEY_MOUSE_DOWN) {
		MouseWrapper.mouseActiveForCyclesY=0;
	    }
	    if (mappedKey.keyCode & KEY_MOUSE_LEFT || mappedKey.keyCode & KEY_MOUSE_RIGHT) {
		MouseWrapper.mouseActiveForCyclesX=0;
	    } 
    }

    if (!key_is_pressed(keyState))
        return;

    if (mappedKey.keyCode & KEY_MOUSE_UP) {
        MouseWrapper.move(0,-1);
    } else if (mappedKey.keyCode & KEY_MOUSE_DOWN) {
        MouseWrapper.move(0,1);
    }

    if (mappedKey.keyCode & KEY_MOUSE_LEFT) {
        MouseWrapper.move(-1,0);
    } else if (mappedKey.keyCode & KEY_MOUSE_RIGHT) {
        MouseWrapper.move(1,0);
    }
}

static Key handleMouseKeys(Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (mappedKey.flags != (SYNTHETIC | IS_MOUSE_KEY))
        return mappedKey;

    if (mappedKey.keyCode & KEY_MOUSE_BUTTON) {
        uint8_t button = mappedKey.keyCode & ~KEY_MOUSE_BUTTON;

        if (key_toggled_on(keyState)) {
            MouseWrapper.press_button(button);
        } else if (key_toggled_off(keyState)) {
            MouseWrapper.release_button(button);
        }
    } else if (!(mappedKey.keyCode & KEY_MOUSE_WARP)) {
        handle_mouse_key_event(mappedKey, keyState);
    } else if (key_toggled_on(keyState)) {
        if (mappedKey.keyCode & KEY_MOUSE_WARP && mappedKey.flags & IS_MOUSE_KEY) {
            // we don't pass in the left and up values because those are the
            // default, "no-op" conditionals
            MouseWrapper.warp( ((mappedKey.keyCode & KEY_MOUSE_WARP_END) ? WARP_END : 0x00) |
                               ((mappedKey.keyCode & KEY_MOUSE_DOWN) ? WARP_DOWN : 0x00) |
                               ((mappedKey.keyCode & KEY_MOUSE_RIGHT) ? WARP_RIGHT : 0x00) );
        }
    }

    return Key_NoKey;
}

MouseKeys_::MouseKeys_(void) {
}

void
MouseKeys_::begin (void) {
    event_handler_hook_add (handleMouseKeys);
}

MouseKeys_ MouseKeys;
