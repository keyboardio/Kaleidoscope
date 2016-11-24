#include "keyswitch_state.h"
// switch debouncing and status

#define PRESSED B00000001

boolean key_was_pressed (uint8_t currentState, uint8_t previousState) {
    if (previousState & PRESSED) {
        return true;
    } else {
        return false;
    }
}

boolean key_is_pressed (uint8_t currentState, uint8_t previousState) {
    if (currentState & PRESSED) {
        return true;
    } else {
        return false;
    }
}

boolean key_toggled_on (uint8_t currentState, uint8_t previousState) {
    if ((currentState & PRESSED) && ! (previousState & PRESSED) ) {
        return true;
    } else {
        return false;
    }
}

boolean key_toggled_off (uint8_t currentState, uint8_t previousState) {
    if ((previousState & PRESSED) && ! (currentState & PRESSED) ) {
        return true;
    } else {
        return false;
    }
}
