#ifndef KeyboardIO_H_
#define KeyboardIO_H_
#include <Arduino.h>
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


#include <stdio.h>
#include <math.h>
#include <avr/wdt.h>
#include "key_defs.h"
#include "KeyboardConfig.h"

#include "keymaps_generated.h"
#include "debouncing.h"
#include "led_control.h"
#include "digitalWriteFast.h"
#include <Wire.h>
#include "sx1509_library.h"

//extern int usbMaxPower;
#define DEBUG_SERIAL 0

char x;
char y;

byte matrixState[ROWS][COLS];

byte charsBeingReported[KEYS_HELD_BUFFER]; // A bit vector for the 256 keys we might conceivably be holding down
byte charsReportedLastTime[KEYS_HELD_BUFFER]; // A bit vector for the 256 keys we might conceivably be holding down


long reporting_counter = 0;
byte primary_keymap = 0;
byte active_keymap = 0;



byte commandBuffer[32];
int commandBufferSize;
bool commandMode;
bool commandPromptPrinted;


// Console related 
void process_command_buffer();

// EEPROM related
void save_primary_keymap(byte keymap);
byte load_primary_keymap();


// Keyboard debugging
void report(byte row, byte col, boolean value);
void report_matrix();


// Mouse-related methods

double mouse_accel (double cycles);
void handle_mouse_movement( char x, char y);
void begin_warping();
void end_warping();
void warp_mouse(Key key);

// hardware keymap interaction
void setup_pins();
void setup_input_pins();
void setup_output_pins();
void scan_matrix();

// key matrix
void setup_matrix();
void reset_matrix();
void handle_immediate_action_during_matrix_scan(Key keymapEntry, byte matrixStateEntry);

// keymaps
void set_keymap_keymap(Key keymapEntry, byte matrixStateEntry);

// sending events to the computer
void record_key_being_pressed(byte character);
void release_keys_not_being_pressed();
void reset_key_report();
void handle_synthetic_key_press(byte switchState, Key mappedKey);
void handle_mouse_key_press(byte switchState, Key mappedKey, char &x, char &y);
void send_key_events();

void press_key(Key mappedKey);
void release_key(Key mappedKey);

int setup_sx1509(sx1509Class sx1509, int colpins[], int rowpins[]);

#ifndef VERSION
#define VERSION "locally-built"
#endif

//Do not add code below this line
#endif /* KeyboardIO_H_ */
