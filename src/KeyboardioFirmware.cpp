#include "KeyboardioFirmware.h"

byte NUMPAD_KEYMAP = 0;

Keyboardio_::Keyboardio_(void) {
}

void
Keyboardio_::setup(const byte keymap_count, const byte numpad_layer) {
    event_handler_hook_add (handle_key_event_default);
    wdt_disable();
    delay(100);
    Keyboard.begin();
    Mouse.begin();
    AbsoluteMouse.begin();
    KeyboardHardware.setup();
    LEDControl.boot_animation();

    NUMPAD_KEYMAP = numpad_layer;
    temporary_keymap = primary_keymap = Storage.load_primary_keymap(keymap_count);
}

custom_loop_t loopHooks[HOOK_MAX] = {NULL};

void
Keyboardio_::loop(void) {
    for (byte i = 0; loopHooks[i] != NULL && i < HOOK_MAX; i++) {
        custom_loop_t hook = loopHooks[i];
        (*hook)();
    }

    KeyboardHardware.scan_matrix();
    LEDControl.update(temporary_keymap);
    Keyboard.sendReport();
    Keyboard.releaseAll();
}

