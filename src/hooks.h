#pragma once

#include <Arduino.h>
#include "key_defs.h"

#define HOOK_MAX 64

typedef Key (*custom_handler_t)(Key mappedKey, byte row, byte col, uint8_t keyState);
extern custom_handler_t eventHandlers[HOOK_MAX];

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
