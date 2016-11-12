#include "hooks.h"

void
event_handler_hook_add (custom_handler_t hook) {
    byte i;

    for (i = 0; i < HOOK_MAX && eventHandlers[i] != NULL; i++) {
    }

    if (i == HOOK_MAX)
        return;

    eventHandlers[i] = hook;
}

void
loop_hook_add (custom_loop_t hook) {
    byte i;

    for (i = 0; i < HOOK_MAX && loopHooks[i] != NULL; i++) {
    }

    if (i == HOOK_MAX)
        return;

    loopHooks[i] = hook;
}
