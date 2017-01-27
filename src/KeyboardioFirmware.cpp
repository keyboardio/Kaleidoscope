#include "KeyboardioFirmware.h"
#include <stdarg.h>

Keyboardio_::Keyboardio_(void) {
    memset(eventHandlers, 0, HOOK_MAX * sizeof(custom_handler_t));
    memset(loopHooks, 0, HOOK_MAX * sizeof(custom_handler_t));
}

void
Keyboardio_::setup(const byte keymap_count) {
    wdt_disable();
    delay(100);
    Keyboard.begin();
    KeyboardHardware.setup();

    event_handler_hook_add (NULL);
    loop_hook_add (NULL);

    Layer.defaultLayer (Storage.load_primary_keymap (keymap_count));
}

custom_loop_t loopHooks[HOOK_MAX];

void
Keyboardio_::loop(void) {
    KeyboardHardware.scan_matrix();

    for (byte i = 0; loopHooks[i] != NULL && i < HOOK_MAX; i++) {
      custom_loop_t hook = loopHooks[i];
      (*hook)(false);
    }

    Keyboard.sendReport();
    Keyboard.releaseAll();

    for (byte i = 0; loopHooks[i] != NULL && i < HOOK_MAX; i++) {
      custom_loop_t hook = loopHooks[i];
      (*hook)(true);
    }
}

void
Keyboardio_::use(KeyboardioPlugin *plugin, ...) {
    va_list ap;
    KeyboardioPlugin *p;

    plugin->begin();
    va_start(ap, plugin);
    while ((p = va_arg(ap, KeyboardioPlugin*)) != NULL) {
      p->begin();
    };
    va_end(ap);
}

Keyboardio_ Keyboardio;
