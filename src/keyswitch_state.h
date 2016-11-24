#pragma once
// switch debouncing and status

#include <Arduino.h>

#define INJECTED    B10000000
#define IS_PRESSED  B00000010
#define WAS_PRESSED B00000001

#define key_was_pressed(state) (state & WAS_PRESSED)
#define key_is_pressed(state)  (state & IS_PRESSED)
#define key_toggled_on(state)  (key_is_pressed(state) && ! key_was_pressed(state))
#define key_toggled_off(state) (key_was_pressed(state) && ! key_is_pressed(state))
