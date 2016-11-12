#include "KeyboardioFirmware.h"

Keyboardio_::Keyboardio_(void) {
}

void
Keyboardio_::setup(void) {
    wdt_disable();
    delay(100);
    Keyboard.begin();
    Mouse.begin();
    AbsoluteMouse.begin();
    KeyboardHardware.setup();
    LEDControl.boot_animation();

    temporary_keymap = primary_keymap = Storage.load_primary_keymap(KEYMAPS);
}

void
Keyboardio_::loop(void) {
    KeyboardHardware.scan_matrix();
    LEDControl.update(temporary_keymap);
    Keyboard.sendReport();
    Keyboard.releaseAll();
}

