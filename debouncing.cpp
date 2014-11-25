
#include <Arduino.h>
#include "debouncing.h"
// switch debouncing and status


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
