// Do not remove the include below
#include "ArduinoKeyboard.h"
// Copyright 2013 Jesse Vincent <jesse@fsck.com>
// All Rights Reserved. (To be licensed under an opensource license
// before the release of the keyboard.io model 01


/**
 *  TODO:

    add mouse acceleration/deceleration
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

#include "KeyboardEEPROM.h"
#include "KeyboardMouse.h"
#include "KeyboardDebugging.h"
//extern int usbMaxPower;



byte matrixState[ROWS][COLS];


byte charsBeingReported[KEYS_HELD_BUFFER]; // A bit vector for the 256 keys we might conceivably be holding down
byte charsReportedLastTime[KEYS_HELD_BUFFER]; // A bit vector for the 256 keys we might conceivably be holding down


long reporting_counter = 0;
byte current_layer = 0;
byte active_layer = 0;



void release_keys_not_being_pressed()
{

  // we use charsReportedLastTime to figure out what we might not be holding anymore and can now release. this is destructive to charsReportedLastTime

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

void reset_matrix()
{
  for (byte col = 0; col < COLS; col++) {
    for (byte row = 0; row < ROWS; row++) {
      matrixState[row][col] <<= 1;
    }
  }
  for (byte i = 0; i < KEYS_HELD_BUFFER; i++) {
    charsReportedLastTime[i] = charsBeingReported[i];
    charsBeingReported[i] = 0x00;
  }
}




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
        Keyboard.print("Keyboard.IO keyboard driver v0.00");
      }
    }
  } else if (mappedKey.rawKey == KEY_MOUSE_BTN_L
             || mappedKey.rawKey == KEY_MOUSE_BTN_M
             || mappedKey.rawKey == KEY_MOUSE_BTN_R) {
    if (key_toggled_on (switchState)) {
      Mouse.press(mappedKey.rawKey);
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

void send_key_events(byte layer)
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
      Key mappedKey = keymaps[layer][row][col];
      if (mappedKey.flags & MOUSE_KEY ) {
        handle_mouse_key_press(matrixState[row][col], keymaps[layer][row][col], x, y);


      } else if (mappedKey.flags & SYNTHETIC_KEY) {
        handle_synthetic_key_press(matrixState[row][col], keymaps[layer][row][col]);
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

void setup_matrix()
{
  //blank out the matrix.
  for (byte col = 0; col < COLS; col++) {
    for (byte row = 0; row < ROWS; row++) {
      matrixState[row][col] = 0;
    }
  }

}

void scan_matrix()
{

  active_layer = current_layer;

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
      // that we should be looking at a seconary Keymap halfway through the matrix scan



      // this logic sucks. there is a better way TODO this
      if (! (keymaps[active_layer][row][col].flags ^ ( MOMENTARY | SWITCH_TO_LAYER))) {
        if (key_is_pressed(matrixState[row][col])) {


          if ( keymaps[current_layer][row][col].rawKey == LAYER_NEXT) {
            active_layer++;
          } else if ( keymaps[current_layer][row][col].rawKey == LAYER_PREVIOUS) {
            active_layer--;
          } else {
            active_layer = keymaps[current_layer][row][col].rawKey;
          }
        }
      } else if (! (keymaps[active_layer][row][col].flags ^ (  SWITCH_TO_LAYER))) {
        // switch layer and stay there
        if (key_toggled_on(matrixState[row][col])) {
          current_layer = active_layer =  keymaps[current_layer][row][col].rawKey;
          save_current_layer(current_layer);
        }
      }

    }
    digitalWrite(rowPins[row], HIGH);
  }
}


void setup()
{
  //usbMaxPower = 100;
  Serial.begin(115200);
  Keyboard.begin();
  Mouse.begin();
  //#ifdef DEBUG_SERIAL
  //#endif
  setup_matrix();
  setup_pins();
  Serial.println("loaded the matrix");
  current_layer = load_current_layer();
  active_layer = current_layer;
}

void loop()
{
  scan_matrix();
  send_key_events(active_layer);
  reset_matrix();
}

