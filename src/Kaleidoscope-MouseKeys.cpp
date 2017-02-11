#include <Arduino.h>

#include "Kaleidoscope-MouseKeys.h"
#include "MouseWrapper.h"
#include "Kaleidoscope.h"

uint8_t MouseKeys_::mouseMoveIntent;

uint8_t MouseKeys_::speed = 1;
uint16_t MouseKeys_::speedDelay = 0;

uint8_t MouseKeys_::accelSpeed = 1;
uint16_t MouseKeys_::accelDelay = 50;

uint32_t MouseKeys_::accelStartTime;
uint32_t MouseKeys_::startTime;

void MouseKeys_::loopHook(bool postClear) {
    if (postClear) {
        mouseMoveIntent = 0;
        return;
    }

    if (mouseMoveIntent == 0) {
        MouseWrapper.accelStep = 0;
        startTime = 0;
        accelStartTime = 0;
        return;
    }

    if ((millis() - startTime) < speedDelay)
        return;

    startTime = millis();

    int8_t moveX = 0, moveY = 0;

    if ((millis() - accelStartTime) >= (accelDelay * MouseWrapper.accelStep)) {
        if (MouseWrapper.accelStep < 255 - accelSpeed)
            MouseWrapper.accelStep += accelSpeed;
    }

    if (mouseMoveIntent & KEY_MOUSE_UP)
        moveY = -speed;
    else if (mouseMoveIntent & KEY_MOUSE_DOWN)
        moveY = speed;

    if (mouseMoveIntent & KEY_MOUSE_LEFT)
        moveX = -speed;
    else if (mouseMoveIntent & KEY_MOUSE_RIGHT)
        moveX = speed;

    MouseWrapper.move(moveX, moveY);
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
        if (key_toggled_on(keyState)) {
            if (!startTime)
                startTime = millis();
            if (!accelStartTime)
                accelStartTime = millis();
        }
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
