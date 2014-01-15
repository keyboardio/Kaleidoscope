#ifndef KeyboardIO_H_
#define KeyboardIO_H_
#include "Arduino.h"
//add your includes for the project KeyboardIO here
#include <EEPROM.h>

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


// Switch status and debouncing
boolean key_was_pressed (byte keyState);
boolean key_was_not_pressed (byte keyState);
boolean key_is_pressed (byte keyState);
boolean key_is_not_pressed (byte keyState);
boolean key_toggled_off(byte keyState);
boolean key_toggled_on(byte keyState);


// EEPROM related 
void save_current_layer(byte layer);
byte load_current_layer();


// Keyboard debugging
void report(byte row, byte col, boolean value);
void report_matrix();


// Mouse-related methods

double mouse_accel (double cycles);
void handle_mouse_movement( char x, char y);

//Do not add code below this line
#endif /* KeyboardIO_H_ */
