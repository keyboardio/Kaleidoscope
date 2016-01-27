#pragma once
// switch debouncing and status

#include <Arduino.h>


boolean key_was_pressed (byte keyState);
boolean key_is_pressed (byte keyState);
boolean key_toggled_on(byte keyState);
boolean key_toggled_off(byte keyState);
