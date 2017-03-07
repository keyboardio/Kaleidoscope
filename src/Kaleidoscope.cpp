#include "Kaleidoscope.h"
#include <stdarg.h>

Kaleidoscope_::eventHandlerHook Kaleidoscope_::eventHandlers[HOOK_MAX];
Kaleidoscope_::loopHook Kaleidoscope_::loopHooks[HOOK_MAX];

Kaleidoscope_::Kaleidoscope_(void) {
}

void
Kaleidoscope_::setup(void) {
    wdt_disable();
    delay(100);
    Keyboard.begin();
    KeyboardHardware.setup();

    // A workaround, so that the compiler does not optimize this out...
    handle_key_event (Key_NoKey, 255, 255, 0);
}

void
Kaleidoscope_::runLoopHooks (bool postClear) {
  for (byte i = 0; loopHooks[i] != NULL && i < HOOK_MAX; i++) {
    loopHook hook = loopHooks[i];
    (*hook)(postClear);
  }
}

void
Kaleidoscope_::loop(void) {
    KeyboardHardware.scan_matrix();

    runLoopHooks(false);

    Keyboard.sendReport();
    Keyboard.releaseAll();

    runLoopHooks(true);
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

void
Kaleidoscope_::replaceEventHandlerHook(eventHandlerHook oldHook, eventHandlerHook newHook) {
  for (byte i = 0; i < HOOK_MAX; i++) {
    if (eventHandlers[i] == oldHook) {
      eventHandlers[i] = newHook;
      return;
    }
  }
}

void
Kaleidoscope_::appendEventHandlerHook (eventHandlerHook hook) {
  replaceEventHandlerHook((eventHandlerHook)NULL, hook);
}

void
Kaleidoscope_::useEventHandlerHook (eventHandlerHook hook) {
  for (byte i = 0; i < HOOK_MAX; i++) {
    if (eventHandlers[i] == hook)
      return;
  }
  appendEventHandlerHook(hook);
}

void
Kaleidoscope_::replaceLoopHook(loopHook oldHook, loopHook newHook) {
  for (byte i = 0; i < HOOK_MAX; i++) {
    if (loopHooks[i] == oldHook) {
      loopHooks[i] = newHook;
      return;
    }
  }
}

void
Kaleidoscope_::appendLoopHook(loopHook hook) {
  replaceLoopHook((loopHook)NULL, hook);
}

void
Kaleidoscope_::useLoopHook(loopHook hook) {
  for (byte i = 0; i < HOOK_MAX; i++) {
    if (loopHooks[i] == hook)
      return;
  }
  appendLoopHook (hook);
}

Kaleidoscope_ Kaleidoscope;
