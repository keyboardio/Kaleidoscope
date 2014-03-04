// Do not remove the include below
#include "ArduinoKeyboard.h"
// Copyright 2013 Jesse Vincent <jesse@fsck.com>
// All Rights Reserved. (To be licensed under an opensource license
// before the release of the keyboard.io model 01


/**
 *  TODO:

    add mouse inertia
    add series-of-character macros
    add series of keystroke macros
    use a lower-level USB API
 *
**/


#include <stdio.h>
#include <math.h>

#include "KeyboardConfig.h"
#include "keymaps_generated.h"
#include <EEPROM.h>  // Don't need this for CLI compilation, but do need it in the IDE

//extern int usbMaxPower;
#define DEBUG_SERIAL 0


 byte matrixState[ROWS][COLS];


byte charsBeingReported[KEYS_HELD_BUFFER]; // A bit vector for the 256 keys we might conceivably be holding down
byte charsReportedLastTime[KEYS_HELD_BUFFER]; // A bit vector for the 256 keys we might conceivably be holding down


long reporting_counter = 0;
byte primary_keymap = 0;
byte active_keymap = 0;

double mouseActiveForCycles = 0;
float carriedOverX = 0;
float carriedOverY = 0;


void setup_matrix()
{
  //blank out the matrix.
  for (byte col = 0; col < COLS; col++) {
    for (byte row = 0; row < ROWS; row++) {
      matrixState[row][col] = 0;
    }
  }
}
void reset_matrix()
{
  for (byte col = 0; col < COLS; col++) {
    for (byte row = 0; row < ROWS; row++) {
      matrixState[row][col] <<= 1;
    }
  }
}



void set_keymap(Key keymapEntry, byte matrixStateEntry) {
  if (keymapEntry.flags & SWITCH_TO_KEYMAP) {
    // this logic sucks. there is a better way TODO this
    if (! (keymapEntry.flags ^ ( MOMENTARY | SWITCH_TO_KEYMAP))) {
      if (key_is_pressed(matrixStateEntry)) {
        if ( keymapEntry.rawKey == KEYMAP_NEXT) {
          active_keymap++;
        } else if ( keymapEntry.rawKey == KEYMAP_PREVIOUS) {
          active_keymap--;
        } else {
          active_keymap = keymapEntry.rawKey;
        }
      }
    } else if (! (keymapEntry.flags ^ (  SWITCH_TO_KEYMAP))) {
      // switch keymap and stay there
      if (key_toggled_on(matrixStateEntry)) {
        active_keymap = primary_keymap = keymapEntry.rawKey;
        save_primary_keymap(primary_keymap);
#ifdef DEBUG_SERIAL
        Serial.print("keymap is now:");
        Serial.print(active_keymap);
#endif
      }
    }
  }
}

void scan_matrix()
{
  //scan the Keyboard matrix looking for connections
  for (byte row = 0; row < ROWS; row++) {
    digitalWrite(rowPins[row], LOW);
    for (byte col = 0; col < COLS; col++) {
      //If we see an electrical connection on I->J,

      if (digitalRead(colPins[col])) {
        matrixState[row][col] |= 0; // noop. just here for clarity
      } else {
        matrixState[row][col] |= 1; // noop. just here for clarity
      }
      // while we're inspecting the electrical matrix, we look
      // to see if the Key being held is a firmware level
      // metakey, so we can act on it, lest we only discover
      // that we should be looking at a seconary Keymap halfway
      // through the matrix scan


      set_keymap(keymaps[active_keymap][row][col], matrixState[row][col]);

    }
    digitalWrite(rowPins[row], HIGH);
  }
}


void setup()
{
  //usbMaxPower = 100;
  delay(2500);
  Keyboard.begin();
  Mouse.begin();
  setup_matrix();
  setup_pins();
  primary_keymap = load_primary_keymap();
}

void loop()
{
  active_keymap = primary_keymap;
  scan_matrix();
  send_key_events();
  reset_matrix();
  reset_key_report();
}













// switch debouncing and status
//
//


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



void save_primary_keymap(byte keymap)
{
  EEPROM.write(EEPROM_KEYMAP_LOCATION, keymap);
}

byte load_primary_keymap()
{
  byte keymap =  EEPROM.read(EEPROM_KEYMAP_LOCATION);
  if (keymap >= KEYMAPS ) {
    return 0; // undefined positions get saved as 255
  }
  return keymap;
}




// Debugging Reporting
//

void report_matrix()
{
#ifdef DEBUG_SERIAL
  if (reporting_counter++ % 100 == 0 ) {
    for (byte row = 0; row < ROWS; row++) {
      for (byte col = 0; col < COLS; col++) {
        Serial.print(matrixState[row][col], HEX);
        Serial.print(", ");

      }
      Serial.println("");
    }
    Serial.println("");
  }
#endif
}

void report(byte row, byte col, boolean value)
{
#ifdef DEBUG_SERIAL
  Serial.print("Detected a change on ");
  Serial.print(col);
  Serial.print(" ");
  Serial.print(row);
  Serial.print(" to ");
  Serial.print(value);
  Serial.println(".");
#endif
}


// Mouse-related methods
//
//


 void _draw_warp_section(long x_origin, long y_origin, long x_end, long y_end, int tracing_scale) {

    if ( x_origin != x_end )   { // it's a horizontal line

        if (x_origin > x_end)  { // right to left
//            tracing_scale =  (x_origin-x_end) /100;
            for (long x = x_origin ; x>= x_end; x=x-tracing_scale) {
                Mouse.moveAbsolute(x,y_origin);
            }

        } else { // left to right
  //         tracing_scale =  (x_end-x_origin) /100;
            for (long x = x_origin ; x<= x_end; x=x+tracing_scale) {
                Mouse.moveAbsolute(x,y_origin);
            }

        }

    } else { // it's a vertical line
    
        if (y_origin > y_end)  { // bottom to top
    //       tracing_scale =  (y_origin-y_end) /100;
            for (long y = y_origin ; y>= y_end; y=y-tracing_scale) {
                Mouse.moveAbsolute(x_origin,y);
            }

        } else { // top to bottom
      //     tracing_scale =  (y_end-y_origin) /100;
            for (long y = y_origin ; y<= y_end; y=y+tracing_scale) {
                Mouse.moveAbsolute(x_origin, y);
            }

        }

    
    
    }
}

#define CLOVER_TRACE_SCALE 50

void _warp_clover(long left, long top, long height, long width) {
    long x_center = left + width/2;
    long y_center = top + height/2;
    long right = left + width;
    long bottom = top + height;
    _draw_warp_section(x_center, y_center, left, y_center, CLOVER_TRACE_SCALE);
    _draw_warp_section(left, y_center, left,top, CLOVER_TRACE_SCALE);
    _draw_warp_section(left, top , x_center,top, CLOVER_TRACE_SCALE);
    _draw_warp_section( x_center,top, x_center, y_center, CLOVER_TRACE_SCALE);
    _draw_warp_section(x_center, y_center, x_center, bottom, CLOVER_TRACE_SCALE);
    _draw_warp_section( x_center, bottom, right, bottom, CLOVER_TRACE_SCALE);
    _draw_warp_section( right, bottom, right, y_center, CLOVER_TRACE_SCALE);
    _draw_warp_section( right, y_center, x_center, y_center, CLOVER_TRACE_SCALE);
    _draw_warp_section( x_center, y_center, left, y_center, CLOVER_TRACE_SCALE);
    
    _draw_warp_section( left,y_center,left, bottom, CLOVER_TRACE_SCALE);
    _draw_warp_section(left, bottom, x_center, bottom, CLOVER_TRACE_SCALE);
    _draw_warp_section(  x_center, bottom, x_center, y_center, CLOVER_TRACE_SCALE);
    _draw_warp_section(  x_center, y_center, x_center,top, CLOVER_TRACE_SCALE);
    _draw_warp_section(  x_center,top, right, top, CLOVER_TRACE_SCALE);
    _draw_warp_section(  right, top, right, y_center, CLOVER_TRACE_SCALE);
    _draw_warp_section(   right, y_center, x_center, y_center, CLOVER_TRACE_SCALE);



}


void _warp_cross(long left, long top, long height, long width) {
    long x_center = left + width/2;
    long y_center = top + height/2;
    long right = left + width;
    long bottom = top + height;

    _draw_warp_section(x_center, y_center, x_center, top,40);
    _draw_warp_section(x_center, top, x_center, bottom,40);
    _draw_warp_section( x_center, bottom, x_center, y_center,40);
    _draw_warp_section( x_center, y_center,left,y_center,40);
    _draw_warp_section(left, y_center,  right, y_center,40);
    _draw_warp_section(right, y_center,  x_center, y_center,40);
}

void _warp_jump(long left, long top, long height, long width) {
    long x_center = left + width/2;
    long y_center = top + height/2;
                Mouse.moveAbsolute(x_center,y_center);
}



int last_x;
int last_y;

// apparently, the mac discards 15% of the value space for mouse movement.
// need to test this on other platforms
//

#define HALF_WIDTH 16384
#define HALF_HEIGHT 16384



int abs_left = 0;
int abs_top = 0;

int next_width;
int next_height;
int section_top;
int section_left;
boolean is_warping = false;

void begin_warping() {
  section_left = abs_left;
  section_top = abs_top;
  next_width = 32767;
  next_height = 32767;
  is_warping = true;
}

void end_warping() {
    is_warping= false;
}

void warp_mouse(Key ninth) {
  if (is_warping == false) {
    begin_warping();
    }


  if ( ninth.rawKey & MOUSE_END_WARP) {
     end_warping();
    return;
  }


  next_width = next_width / 2;
  next_height = next_height/2;

#ifdef DEBUG_SERIAL
  Serial.print("Current box: ");
  Serial.print(section_left);
  Serial.print(",");
  Serial.print(section_top);
  Serial.print(" to: ");
  Serial.print(section_left+next_width);
  Serial.print(",");
  Serial.print(section_top+next_height);
  



  Serial.print("\nwarping - the next box size is ");
  Serial.print(next_width);
  Serial.print(", ");
  Serial.print(next_height);
  Serial.print("\n");
#endif

  if (ninth.rawKey & MOUSE_UP) {
//    Serial.print(" - up ");
  } else if (ninth.rawKey & MOUSE_DN) {
 //   Serial.print(" - down ");
    section_top  = section_top + next_height;
  }

  if (ninth.rawKey & MOUSE_L) {
  //  Serial.print(" - left ");
  } else if (ninth.rawKey & MOUSE_R) {
   // Serial.print(" - right ");
    section_left  = section_left + next_width;
  }

#ifdef DEBUG_SERIAL
  Serial.print("\nShould end up at ");
  Serial.print(section_left + next_width/ 2);
  Serial.print(",");
  Serial.print(section_top + next_height / 2);
  Serial.print("That should be half way between ");
  Serial.print(section_left);
  Serial.print(",");
  Serial.print(section_top);
  Serial.print(" and ");
  Serial.print(section_left + next_width);
  Serial.print(",");
  Serial.print(section_top+next_height);
  Serial.print("\n");

#endif

    // the cloverleaf

//    _warp_cross(section_left, section_top, next_height,next_width);

    _warp_jump(section_left, section_top, next_height,next_width);

}


// we want the whole s curve, not just the bit 
// that's usually above the x and y axes;
#define ATAN_LIMIT 1.57079633
#define ACCELERATION_FLOOR 0.25
#define ACCELERATION_MULTIPLIER 5
#define  ACCELERATION_RUNWAY 5
// Climb speed is how fast we get to max speed
// 1 is "instant"
// 0.05 is just right
// 0.001 is insanely slow

#define ACCELERATION_CLIMB_SPEED  0.05

double mouse_accel (double cycles)
{
  double accel = (atan((cycles * ACCELERATION_CLIMB_SPEED)-ACCELERATION_RUNWAY) + ATAN_LIMIT) * ACCELERATION_MULTIPLIER;
  if (accel < ACCELERATION_FLOOR) {
    accel = ACCELERATION_FLOOR;
  }
  return accel;
}

void handle_mouse_movement( char x, char y)
{

  if (x != 0 || y != 0) {
    mouseActiveForCycles++;
    double accel = (double) mouse_accel(mouseActiveForCycles);
    float moveX = 0;
    float moveY = 0;
    if (x > 0) {
      moveX = (x * accel) + carriedOverX;
      carriedOverX = moveX - floor(moveX);
    } else if (x < 0) {
      moveX = (x * accel) - carriedOverX;
      carriedOverX = ceil(moveX) - moveX;
    }

    if (y > 0) {
      moveY = (y * accel) + carriedOverY;
      carriedOverY = moveY - floor(moveY);
    } else if (y < 0) {
      moveY = (y * accel) - carriedOverY;
      carriedOverY = ceil(moveY) - moveY;
    }
#ifdef DEBUG_SERIAL
    Serial.println();
    Serial.print("cycles: ");
    Serial.println(mouseActiveForCycles);
    Serial.print("Accel: ");
    Serial.print(accel);
    Serial.print("	moveX is ");
    Serial.print(moveX);
    Serial.print(" moveY is ");
    Serial.print(moveY);
    Serial.print(" carriedoverx is ");
    Serial.print(carriedOverX);
    Serial.print(" carriedOverY is ");
    Serial.println(carriedOverY);
#endif

    end_warping();

    Mouse.move(moveX, moveY, 0);
  } else {
    mouseActiveForCycles = 0;
  }

}


//
// Key Reports
//

void release_keys_not_being_pressed()
{
  // we use charsReportedLastTime to figure out what we might
  // not be holding anymore and can now release. this is
  // destructive to charsReportedLastTime

  for (byte i = 0; i < KEYS_HELD_BUFFER; i++) {
    // for each key we were holding as of the end of the last cycle
    // see if we're still holding it
    // if we're not, call an explicit Release

    if (charsReportedLastTime[i] != 0x00) {
      // if there _was_ a character in this slot, go check the
      // currently held characters
      for (byte j = 0; j < KEYS_HELD_BUFFER; j++) {
        if (charsReportedLastTime[i] == charsBeingReported[j]) {
          // if's still held, we don't need to do anything.
          charsReportedLastTime[i] = 0x00;
          break;
        }
      }
    }
  }
  for (byte i = 0; i < KEYS_HELD_BUFFER; i++) {
    if (charsReportedLastTime[i] != 0x00) {
      Keyboard.release(charsReportedLastTime[i]);
    }
  }
}

void record_key_being_pressed(byte character)
{
  for (byte i = 0; i < KEYS_HELD_BUFFER; i++) {
    // todo - deal with overflowing the 12 key buffer here
    if (charsBeingReported[i] == 0x00) {
      charsBeingReported[i] = character;
      break;
    }
  }
}

void reset_key_report()
{
  for (byte i = 0; i < KEYS_HELD_BUFFER; i++) {
    charsReportedLastTime[i] = charsBeingReported[i];
    charsBeingReported[i] = 0x00;
  }
}


// Sending events to the usb host

void handle_synthetic_key_press(byte switchState, Key mappedKey) {
  if (mappedKey.flags & IS_CONSUMER) {
    if (key_toggled_on (switchState)) {
      Keyboard.consumerControl(mappedKey.rawKey);
    }
  }
  else if (mappedKey.flags & IS_SYSCTL) {
    if (key_toggled_on (switchState)) {
      Keyboard.systemControl(mappedKey.rawKey);
    }
  }
  else  if (mappedKey.flags & IS_MACRO) {
    if (key_toggled_on (switchState)) {
      if (mappedKey.rawKey == 1) {
        Serial.print("Keyboard.IO keyboard driver v0.00");
      }
    }
  } else if (mappedKey.rawKey == KEY_MOUSE_BTN_L
             || mappedKey.rawKey == KEY_MOUSE_BTN_M
             || mappedKey.rawKey == KEY_MOUSE_BTN_R) {
    if (key_toggled_on (switchState)) {
      Mouse.press(mappedKey.rawKey);
      end_warping();
    } else if (key_is_pressed(switchState)) {
    } else if (Mouse.isPressed(mappedKey.rawKey) ) {
      Mouse.release(mappedKey.rawKey);
    }
  }
}
void handle_mouse_key_press(byte switchState, Key mappedKey, char &x, char &y) {

  if (key_is_pressed(switchState)) {
    if (mappedKey.rawKey & MOUSE_UP) {
      y -= 1;
    }
    if (mappedKey.rawKey & MOUSE_DN) {
      y += 1;
    }
    if (mappedKey.rawKey & MOUSE_L) {
      x -= 1;
    }

    if (mappedKey.rawKey & MOUSE_R) {
      x += 1 ;
    }
  }
}

void send_key_events()
{
  //for every newly pressed button, figure out what logical key it is and send a key down event
  // for every newly released button, figure out what logical key it is and send a key up event


  // TODO:switch to sending raw HID packets

  // really, these are signed small ints
  char x = 0;
  char y = 0;

  for (byte row = 0; row < ROWS; row++) {

    for (byte col = 0; col < COLS; col++) {
      byte switchState = matrixState[row][col];
      Key mappedKey = keymaps[active_keymap][row][col];

      if (mappedKey.flags & MOUSE_KEY ) {
        if (mappedKey.rawKey & MOUSE_WARP) {
          if (key_toggled_on(switchState)) {
            warp_mouse(mappedKey);
          }
        } else {
          handle_mouse_key_press(switchState, mappedKey, x, y);
        }

      } else if (mappedKey.flags & SYNTHETIC_KEY) {
        handle_synthetic_key_press(switchState, mappedKey);
      }
      else {
        if (key_is_pressed(switchState)) {
          record_key_being_pressed(mappedKey.rawKey);
          if (key_toggled_on (switchState)) {
            Keyboard.press(mappedKey.rawKey);
          }
        } else if (key_toggled_off (switchState)) {
          Keyboard.release(mappedKey.rawKey);
        }
      }
    }
  }
  handle_mouse_movement(x, y);
  release_keys_not_being_pressed();
}


// Hardware initialization
void setup_pins()
{
  //set up the row pins as outputs
  for (byte row = 0; row < ROWS; row++) {
    pinMode(rowPins[row], OUTPUT);
    digitalWrite(rowPins[row], HIGH);
  }

  for (byte col = 0; col < COLS; col++) {
    pinMode(colPins[col], INPUT);
    digitalWrite(colPins[col], HIGH);
    //drive em high by default s it seems to be more reliable than driving em low

  }
}

