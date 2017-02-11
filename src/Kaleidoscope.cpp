#include "Kaleidoscope.h"
#include <stdarg.h>

Kaleidoscope_::Kaleidoscope_(void) {
    memset(eventHandlers, 0, HOOK_MAX * sizeof(custom_handler_t));
    memset(loopHooks, 0, HOOK_MAX * sizeof(custom_handler_t));
}

void
Kaleidoscope_::setup(const byte keymap_count) {
    wdt_disable();
    delay(100);
    Keyboard.begin();
    KeyboardHardware.setup();

    event_handler_hook_use (NULL);
    loop_hook_use (NULL);

    Layer.defaultLayer (KeyboardHardware.load_primary_layer (keymap_count));
}

custom_loop_t loopHooks[HOOK_MAX];

void
Kaleidoscope_::loop(void) {
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
Kaleidoscope_::use(KaleidoscopePlugin *plugin, ...) {
    va_list ap;
    KaleidoscopePlugin *p;

    plugin->begin();
    va_start(ap, plugin);
    while ((p = va_arg(ap, KaleidoscopePlugin*)) != NULL) {
        p->begin();
    };
    va_end(ap);
}

Kaleidoscope_ Kaleidoscope;
