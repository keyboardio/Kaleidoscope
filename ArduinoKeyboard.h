#ifndef KeyboardIO_H_
#define KeyboardIO_H_
#include "Arduino.h"
//add your includes for the project KeyboardIO here


//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project KeyboardIO here
typedef struct {
    byte flags;
    byte rawKey;
} Key;


#include "key_defs.h"



boolean key_was_pressed (byte keyState)
{
    if ( byte((keyState >> 4)) ^ B00001111 ) {
        return false;
    } else {
        return true;
    }

}

boolean key_was_not_pressed (byte keyState)
{
    if ( byte((keyState >> 4)) ^ B00000000 ) {
        return false;
    } else {
        return true;
    }

}

boolean key_is_pressed (byte keyState)
{

    if ( byte((keyState << 4)) ^ B11110000 ) {
        return false;
    } else {
        return true;
    }
}
boolean key_is_not_pressed (byte keyState)
{

    if ( byte((keyState << 4)) ^ B00000000 ) {
        return false;
    } else {
        return true;
    }
}

boolean key_toggled_on(byte keyState)
{
    if (key_is_pressed(keyState) &&  key_was_not_pressed(keyState)) {
        return true;
    } else {
        return false;
    }
}


boolean key_toggled_off(byte keyState)
{
    if (key_was_pressed(keyState) && key_is_not_pressed(keyState)) {
        return true;
    } else {
        return false;
    }
}




//Do not add code below this line
#endif /* KeyboardIO_H_ */
