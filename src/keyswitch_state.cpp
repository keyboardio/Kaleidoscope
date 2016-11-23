#include "keyswitch_state.h"
// switch debouncing and status


boolean key_was_pressed (byte keyState) {
    if ( keyState & B00001000 || keyState & B00000100) {
        return true;
    } else {
        return false;
    }
}

boolean key_was_pressed (uint8_t currentState, uint8_t previousState) {
    if (previousState & B00000001) {
        return true;
    } else {
        return false;
    }
}

boolean key_is_pressed (byte keyState) {
    if ( keyState & B00000001 && keyState & B00000010 ) {
        return true;
    } else {
        return false;
    }
}

boolean key_is_pressed (uint8_t currentState, uint8_t previousState) {
    if (currentState & B00000001) {
        return true;
    } else {
        return false;
    }
}

boolean key_toggled_on(byte keyState) {
    if (key_is_pressed(keyState) && ! key_was_pressed(keyState)) {
        return true;
    } else {
        return false;
    }
}

boolean key_toggled_on (uint8_t currentState, uint8_t previousState) {
    if ((currentState & B00000001) && ! (previousState & B00000001) ) {
        return true;
    } else {
        return false;
    }
}


boolean key_toggled_off(byte keyState) {
    if (key_was_pressed(keyState) && ! key_is_pressed(keyState)) {
        return true;
    } else {
        return false;
    }
}

boolean key_toggled_off (uint8_t currentState, uint8_t previousState) {
    if ((previousState & B00000001) && ! (currentState & B00000001) ) {
        return true;
    } else {
        return false;
    }
}
