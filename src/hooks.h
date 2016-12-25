#pragma once

#include <Arduino.h>
#include "key_defs.h"

#define HOOK_MAX 64

typedef bool (*custom_handler_t)(Key mappedKey, byte row, byte col, uint8_t keyState);
extern custom_handler_t eventHandlers[HOOK_MAX];

void event_handler_hook_add (custom_handler_t hook);
void event_handler_hook_replace (custom_handler_t oldHook, custom_handler_t newHook);

typedef void (*custom_loop_t)(bool postClear);
extern custom_loop_t loopHooks[HOOK_MAX];

void loop_hook_add (custom_loop_t hook);
void loop_hook_replace (custom_loop_t oldHook, custom_loop_t newHook);
