#ifndef debouncing_h
#define debouncing_h

// switch debouncing and status


boolean key_was_pressed (byte keyState);
boolean key_was_not_pressed (byte keyState);
boolean key_is_pressed (byte keyState);
boolean key_is_not_pressed (byte keyState);
boolean key_toggled_on(byte keyState);
boolean key_toggled_off(byte keyState);

#endif
