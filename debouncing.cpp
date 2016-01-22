
#include <Arduino.h>
#include "debouncing.h"
// switch debouncing and status


boolean key_was_pressed (byte keyState) {
    return (! key_was_not_pressed(keyState));
}

boolean key_was_not_pressed (byte keyState) {
    if ( keyState & B00001000 || keyState & B00000100) {
        return false;
    } else {
        return true;
    }
}

boolean key_is_pressed (byte keyState) {
    if ( keyState & B00000001 && keyState & B00000010 ) {
        return true;
    } else {
        return false;
    }
}

boolean key_is_not_pressed (byte keyState) {
    return !key_is_pressed(keyState);
}

boolean key_toggled_on(byte keyState) {
    if (key_is_pressed(keyState) && key_was_not_pressed(keyState)) {
        return true;
    } else {
        return false;
    }
}


boolean key_toggled_off(byte keyState) {
    if (key_was_pressed(keyState) && key_is_not_pressed(keyState)) {
        return true;
    } else {
        return false;
    }
}
