#pragma once
// switch debouncing and status

#include <Arduino.h>

#define INJECTED    B10000000
#define IS_PRESSED  B00000010
#define WAS_PRESSED B00000001

#define keyWasPressed(keyState) ((keyState) & WAS_PRESSED)
#define keyIsPressed(keyState)  ((keyState) & IS_PRESSED)
#define keyToggledOn(keyState)  (keyIsPressed(keyState) && ! keyWasPressed(keyState))
#define keyToggledOff(keyState) (keyWasPressed(keyState) && ! keyIsPressed(keyState))


// Deprecated - Remove once the core has transitioned

#define key_was_pressed(keyState) ((keyState) & WAS_PRESSED)
#define key_is_pressed(keyState)  ((keyState) & IS_PRESSED)
#define key_toggled_on(keyState)  (key_is_pressed(keyState) && ! key_was_pressed(keyState))
#define key_toggled_off(keyState) (key_was_pressed(keyState) && ! key_is_pressed(keyState))
