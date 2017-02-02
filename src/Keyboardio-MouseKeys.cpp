#include <Arduino.h>

#include "Keyboardio-MouseKeys.h"
#include "MouseWrapper.h"
#include "KeyboardioFirmware.h"

uint8_t MouseKeys_::mouseMoveIntent;

void MouseKeys_::loopHook(bool postClear) {
    if (postClear) {
        mouseMoveIntent = 0;
        return;
    }

    if (mouseMoveIntent == 0) {
        MouseWrapper.mouseActiveForCycles = 0;
        return;
    }

    if (MouseWrapper.mouseActiveForCycles < 255)
        MouseWrapper.mouseActiveForCycles++;

    if (mouseMoveIntent & KEY_MOUSE_UP)
        MouseWrapper.move(0, -1);
    else if (mouseMoveIntent & KEY_MOUSE_DOWN)
        MouseWrapper.move(0, 1);

    if (mouseMoveIntent & KEY_MOUSE_LEFT)
        MouseWrapper.move(-1, 0);
    else if (mouseMoveIntent & KEY_MOUSE_RIGHT)
        MouseWrapper.move(1, 0);
}

Key MouseKeys_::eventHandlerHook(Key mappedKey, byte row, byte col, uint8_t keyState) {
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
        if (key_is_pressed(keyState))
            mouseMoveIntent |= mappedKey.keyCode;
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
    event_handler_hook_use(eventHandlerHook);
    loop_hook_use(loopHook);
}

MouseKeys_ MouseKeys;
