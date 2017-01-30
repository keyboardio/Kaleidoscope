#include "hooks.h"

void
event_handler_hook_replace (custom_handler_t oldHook, custom_handler_t newHook) {
    for (byte i = 0; i < HOOK_MAX; i++) {
        if (eventHandlers[i] == oldHook) {
            eventHandlers[i] = newHook;
            return;
        }
    }
}

void
event_handler_hook_append (custom_handler_t hook) {
    event_handler_hook_replace ((custom_handler_t)NULL, hook);
}

void
event_handler_hook_use (custom_handler_t hook) {
    for (byte i = 0; i < HOOK_MAX; i++) {
        if (eventHandlers[i] == hook)
            return;
    }
    event_handler_hook_append (hook);
}

void
loop_hook_replace (custom_loop_t oldHook, custom_loop_t newHook) {
    for (byte i = 0; i < HOOK_MAX; i++) {
        if (loopHooks[i] == oldHook) {
            loopHooks[i] = newHook;
            return;
        }
    }
}

void
loop_hook_append (custom_loop_t hook) {
    loop_hook_replace ((custom_loop_t)NULL, hook);
}

void
loop_hook_use (custom_loop_t hook) {
    for (byte i = 0; i < HOOK_MAX; i++) {
        if (loopHooks[i] == hook)
            return;
    }
    loop_hook_append (hook);
}
