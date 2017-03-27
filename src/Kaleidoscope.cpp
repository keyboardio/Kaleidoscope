#include "Kaleidoscope.h"
#include <stdarg.h>

Kaleidoscope_::listItem<Kaleidoscope_::eventHandlerHook> *Kaleidoscope_::eventHandlerRootNode;
Kaleidoscope_::listItem<Kaleidoscope_::loopHook> *Kaleidoscope_::loopHookRootNode;

Kaleidoscope_::Kaleidoscope_(void) {
}

void
Kaleidoscope_::setup(void) {
    wdt_disable();
    delay(100);
    Keyboard.begin();
    KeyboardHardware.setup();

    // A workaround, so that the compiler does not optimize this out...
    handle_keyswitch_event (Key_NoKey, 255, 255, 0);
}

void
Kaleidoscope_::runLoopHooks (bool postClear) {
    auto *node = loopHookRootNode;

    while (node) {
        (*(node->hook))(postClear);
        node = node->next;
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

template<typename T>
void Kaleidoscope_::appendHook(T **rootNode, T *newNode) {
    if (!*rootNode) {
        *rootNode = newNode;
        return;
    }

    T *node = *rootNode;

    while (node->next) {
        node = node->next;
    }
    node->next = newNode;
}

template<typename T>
void
Kaleidoscope_::useHook(T **rootNode, T *newNode) {
    if (!*rootNode) {
        *rootNode = newNode;
        return;
    }

    T *node = *rootNode;

    while (node->next) {
        if (node->hook == newNode->hook)
            return;
        node = node->next;
    }
    node->next = newNode;
}

void
Kaleidoscope_::useHook(listItem<eventHandlerHook> *newNode) {
    useHook(&eventHandlerRootNode, newNode);
}

void
Kaleidoscope_::useHook(listItem<loopHook> *newNode) {
    useHook(&loopHookRootNode, newNode);
}

void
Kaleidoscope_::appendHook(listItem<eventHandlerHook> *newNode) {
    appendHook(&eventHandlerRootNode, newNode);
}

void
Kaleidoscope_::appendHook(listItem<loopHook> *newNode) {
    appendHook(&loopHookRootNode, newNode);
}

void
Kaleidoscope_::prependHook(listItem<eventHandlerHook> *newNode) {
    newNode->next = eventHandlerRootNode;
    eventHandlerRootNode = newNode;
}

void
Kaleidoscope_::prependHook(listItem<loopHook> *newNode) {
    newNode->next = loopHookRootNode;
    loopHookRootNode = newNode;
}

Kaleidoscope_ Kaleidoscope;
