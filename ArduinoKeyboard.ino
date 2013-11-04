// Do not remove the include below
#include "ArduinoKeyboard.h"

// Copyright 2013 Jesse Vincent <jesse@fsck.com>
// All Rights Reserved. (To be licensed under an opensource license
// before the release of the keyboard.io model 01

#include <stdio.h>



static const int COLS = 14;
static const int ROWS = 5;

static int colPins[COLS] = { 16, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
static int rowPins[ROWS] = { A2, A3, A4, A5, 15 };

byte		matrixState[ROWS][COLS];


long counter = 0;
static const int LAYERS = 2;

int current_keymap = 0;

static const Key keymaps[LAYERS][ROWS][COLS] = {
  {
    { 
      NoKey,	        Key_1,	        Key_2,	        Key_3,	        Key_4,	        Key_5,	 NoKey,	 NoKey,	 Key_6,	 Key_7,	        Key_8,	        Key_9,	            Key_0,	    NoKey                          }
    ,	
    { 
      Key_Backtick,	Key_Q,	        Key_W,	        Key_E,	        Key_R,	        Key_T,	 NoKey,	 NoKey,	 Key_Y,	 Key_U,	        Key_I,	        Key_O,	            Key_P,	    Key_Equals                      }
    ,
    { 
      Key_PageUp,	Key_A,	        Key_S,	        Key_D,	        Key_F,	        Key_G,	 Key_Tab,	 Key_Return, Key_H,	 Key_J,	        Key_K,	        Key_L,	            Key_Semicolon,  Key_Quote                       }
    ,	
    { 
      Key_PageDown,	Key_Z,	        Key_X,	        Key_C,	        Key_V,	        Key_B,	 Key_Esc,	 Key_Return, Key_N,	 Key_M,	        Key_Comma,	Key_Period,	    Key_Slash,	    Key_Minus                       }
    ,	
    { 
      Key_LeftGUI,	Key_Backspace,  Key_LeftShift,	Key_LeftCtrl,	Key_NextKeymapMomentary,  NoKey,	 NoKey,	 NoKey,	 NoKey, Key_NextKeymapMomentary,	Key_RightCtrl, Key_RightShift,    Key_Space,	    Key_RightAlt                   }
  }
  ,

  {
    { 
      NoKey,	        Key_4,	        Key_2,	        Key_3,	        Key_4,	        Key_5,	 NoKey,	 NoKey,	 Key_6,	 Key_7,	        Key_8,	        Key_9,	            Key_0,	    NoKey                          }
    ,	
    { 
      Key_Backtick,	Key_Q,	        Key_W,	        Key_E,	        Key_R,	        Key_T,	 NoKey,	 NoKey,	 Key_Y,	 Key_U,	        Key_I,	        Key_O,	            Key_P,	    Key_Equals                      }
    ,
    { 
      Key_PageUp,	Key_A,	        Key_S,	        Key_D,	        Key_F,	        Key_G,	 Key_Tab,	 Key_Return, Key_LeftArrow,Key_DownArrow,Key_UpArrow,Key_RightArrow,   Key_Semicolon,  Key_Quote                       }
    ,	
    { 
      Key_PageDown,	Key_Z,	        Key_X,	        Key_C,	        Key_V,	        Key_B,	 Key_Esc,	 Key_Return, Key_N,	 Key_M,	        Key_Comma,	Key_Period,	    Key_Slash,	    Key_Minus                       }
    ,	
    { 
      Key_LeftGUI,	Key_Backspace,  Key_LeftShift,	Key_LeftCtrl,	Key_NextKeymapMomentary,   NoKey,	 NoKey,	 NoKey,	 NoKey, Key_NextKeymapMomentary,	Key_RightCtrl, Key_RightShift,    Key_Space,	    Key_RightAlt                   }
  }

};


void reset_matrix() {
  for (int col = 0; col < COLS; col++) {
    for (int row = 0; row < ROWS; row++) {
      matrixState[row][col] <<= 1;
    }
  }
}

void send_key_event() {
  //for every newly pressed button, figure out what logical key it is and send a key down event
  // for every newly released button, figure out what logical key it is and send a key up event

    // TODO:switch to sending raw HID packets
  for (int row = 0; row < ROWS; row++) {

    for (int col = 0; col < COLS; col++) {
      if (key_toggled_on (matrixState[row][col])) {
        Keyboard.press(keymaps[current_keymap][row][col].rawKey);
      }
      else if (key_toggled_off (matrixState[row][col])) {
        Keyboard.release(keymaps[current_keymap][row][col].rawKey);
      }
    }
  }
}

boolean key_was_pressed (byte keyState) {
  if ( byte((keyState >> 4)) ^ B00001111 ){
    return false; 
  } 
  else {
    return true;
  }

}

boolean key_was_not_pressed (byte keyState) {
  if ( byte((keyState >> 4)) ^ B00000000 ){
    return false; 
  } 
  else {
    return true;
  }

}


boolean key_toggled_on(byte keyState) {
  if (key_is_pressed(keyState) && ( key_was_not_pressed(keyState))) {
    return true; 
  } 
  else {
    return false;
  }
}


boolean key_toggled_off(byte keyState) {
  if (key_was_pressed(keyState) && key_is_not_pressed(keyState)) {
    return true; 
  } 
  else {
    return false;
  }
}


boolean key_is_pressed (byte keyState) {
  
  if ( byte((keyState << 4)) ^ B11110000 ){
    return false; 
  } 
  else {
    return true;
  }
}
boolean key_is_not_pressed (byte keyState) {
  
  if ( byte((keyState << 4)) ^ B00000000 ){
    return false; 
  } 
  else {
    return true;
  }
}

void setup_matrix() {
  //set up the row pins as outputs
  for (int row = 0; row < ROWS; row++) {
    pinMode(rowPins[row], OUTPUT);
    digitalWrite(rowPins[row], HIGH);
  }

  for (int col = 0; col < COLS; col++) {
    pinMode(colPins[col], INPUT);
    digitalWrite(colPins[col], HIGH);
    //drive em high by default s it seems to be more reliable than driving em low

  }
  //blank out the matrix.
  for (int col = 0; col < COLS; col++) {
    for (int row = 0; row < ROWS; row++) {
      matrixState[row][col] = 0;
    }
  }

}

void scan_matrix() {
  //scan the keyboard matrix looking for connections
  for (int row = 0; row < ROWS; row++) {
    digitalWrite(rowPins[row], LOW);
    for (int col = 0; col < COLS; col++) {
      //If we see an electrical connection on I->J,

      if (digitalRead(colPins[col])) {
        matrixState[row][col] |= 0; // noop. just here for clarity
      } 
      else {
        matrixState[row][col] |= 1; // noop. just here for clarity
      }
      // while we're inspecting the electrical matrix, we look 
      // to see if the key being held is a firmware level 
      // metakey, so we can act on it, lest we only discover 
      // that we should be looking at a seconary keymap halfway through the matrix scan



      if (       keymaps[current_keymap][row][col].flags & MOMENTARY ) {

        if (key_toggled_on(matrixState[row][col])  ){
          current_keymap++;
        }
        else if (key_toggled_off(matrixState[row][col]) ){
          current_keymap--;
        }
      }

      // guard  against layer overflow
      if (current_keymap >= LAYERS) {
        current_keymap = 0;
      } 
      else if (current_keymap < 0) {
        current_keymap = LAYERS - 1;
      }
    }
    digitalWrite(rowPins[row], HIGH);
  }
}


void report_matrix() {
  if (counter++ %100 == 0 ) {
    for (int row = 0; row < ROWS; row++) {
      for (int col = 0; col < COLS; col++) {
        Serial.print(matrixState[row][col],HEX);
        Serial.print(", ");

      }
      Serial.println("");
    }
    Serial.println("");
  }
}

void report(int row, int col, boolean value) {
  Serial.print("Detected a change on ");
  Serial.print(col);
  Serial.print(" ");
  Serial.print(row);
  Serial.print(" to ");
  Serial.print(value);
  Serial.println(".");
}


void setup() {
  Keyboard.begin();

  Serial.begin(115200);
  setup_matrix();
}

void loop() {
  scan_matrix();
  //    report_matrix();
  send_key_event();
  reset_matrix();
}

