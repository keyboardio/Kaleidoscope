#pragma once
// switch debouncing and status

#include <Arduino.h>

#define INJECTED B10000000

boolean key_was_pressed (byte keyState);
boolean key_is_pressed (byte keyState);
boolean key_toggled_on(byte keyState);
boolean key_toggled_off(byte keyState);

boolean key_was_pressed (uint8_t currentState, uint8_t previousState);
boolean key_is_pressed (uint8_t currentState, uint8_t previousState);
boolean key_toggled_on(uint8_t currentState, uint8_t previousState);
boolean key_toggled_off(uint8_t currentState, uint8_t previousState);
