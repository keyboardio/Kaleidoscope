#pragma once

#include <Arduino.h>
#include "key_defs.h"

#define HOOK_MAX 64

typedef Key (*custom_handler_t)(Key mappedKey, byte row, byte col, uint8_t keyState);
extern custom_handler_t eventHandlers[HOOK_MAX];

/*
 * In most cases, one only wants a single copy of a hook. On the other hand,
 * plugins that depend on other plugins, may want to make it easier for the
 * end-user to use the plugin, and call the setup function of the dependent
 * plugins too. In case the end-user calls the same setup function, we'd end up
 * with hooks registered multiple times.
 *
 * To avoid this, protection against double-registration has been introduced.
 * The `event_handler_hook_use` and `loop_hook_use` functions will only allow
 * one copy of the hook. The `event_handler_hook_append` and `loop_hook_append`
 * functions will, on the other hand, just append the hooks, and not care about
 * protection.
 *
 * The `event_handler_hook_add` and `loop_hook_add` functions are deprecated,
 * but for the time being, they are aliases to the `_use` functions, until all
 * plugins have been updated, and the aliases can be removed.
 */

void event_handler_hook_use (custom_handler_t hook);
void event_handler_hook_append (custom_handler_t hook);
void event_handler_hook_replace (custom_handler_t oldHook, custom_handler_t newHook);
#define event_handler_hook_add event_handler_hook_use

typedef void (*custom_loop_t)(bool postClear);
extern custom_loop_t loopHooks[HOOK_MAX];

void loop_hook_use (custom_loop_t hook);
void loop_hook_append (custom_loop_t hook);
void loop_hook_replace (custom_loop_t oldHook, custom_loop_t newHook);
#define loop_hook_add loop_hook_use
