#pragma once

#include <Arduino.h>
#include "key_defs.h"

#define HOOK_MAX 64

typedef bool (*custom_handler_t)(Key mappedKey, byte row, byte col, uint8_t currentState, uint8_t previousState);
extern custom_handler_t eventHandlers[HOOK_MAX];

void event_handler_hook_add (custom_handler_t hook);

typedef void (*custom_loop_t)(void);
extern custom_loop_t loopHooks[HOOK_MAX];

void loop_hook_add (custom_loop_t hook);
