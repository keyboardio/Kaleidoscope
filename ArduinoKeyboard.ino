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




static const int COLS = 14;
static const int ROWS = 5;

static int colPins[COLS] = { 16, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
static int rowPins[ROWS] = { A2, A3, A4, A5, 15 };

byte matrixState[ROWS][COLS];

// if we're sticking to boot protocol, these could all be 6 + mods
// but *mumble*
//


#define KEYS_HELD_BUFFER 12
byte charsBeingReported[KEYS_HELD_BUFFER]; // A bit vector for the 256 keys we might conceivably be holding down
byte charsReportedLastTime[KEYS_HELD_BUFFER]; // A bit vector for the 256 keys we might conceivably be holding down


long reporting_counter = 0;
static const int LAYERS = 2;
int current_layer = 0;
double mouseActiveForCycles = 0;
float carriedOverX =0;
float carriedOverY =0;


static const Key keymaps[LAYERS][ROWS][COLS] = {
    {
        { NoKey,     Key_1,  Key_2, Key_3, Key_4, Key_5,  NoKey, NoKey,  Key_6,  Key_7, Key_8, Key_9,  Key_0, NoKey  },
        { Key_Backtick, Key_Q,  Key_W, Key_E, Key_R, Key_T,  NoKey, NoKey,  Key_Y,  Key_U, Key_I, Key_O, Key_P, Key_Equals},
        { Key_PageUp,   Key_A,  Key_S, Key_D, Key_F, Key_G,  Key_Tab, Key_Return, Key_H,  Key_J, Key_K, Key_L, Key_Semicolon, Key_Quote },
        { Key_PageDn,   Key_Z,  Key_X, Key_C, Key_V, Key_B,  Key_Esc, Key_Return, Key_N,  Key_M, Key_Comma, Key_Period, Key_Slash,  Key_Minus},
        { Key_LGUI,     Key_Backspace, Key_LShift, Key_LCtrl, NextKeymap, NoKey,  NoKey, NoKey, NoKey,  NextKeymap, Key_RCtrl, Key_RShift, Key_Space,  Key_RAlt }
    },
    {
        { NoKey,     Key_1,  Key_2, Key_3, Key_4, Key_5,  NoKey,      NoKey,  Key_6,  Key_7, Key_8, Key_9,  Key_0, NoKey  },
        { Key_Backtick, Key_Q,  mouseBtnL, mouseBtnM, mouseBtnR, Key_T,  NoKey,      NoKey,  Key_LCurlyBracket,  Key_RCurlyBracket, Key_LSquareBracket, Key_RSquareBracket, Key_P, Key_Equals},
        { Key_PageUp,   mouseL, mouseUp, mouseDn, mouseR, Key_G,  Key_Tab,       Key_Return, Key_LArrow, Key_DnArrow,  Key_UpArrow, Key_RArrow, Key_Semicolon, Key_Quote},
        { Key_PageDn,   Key_Z,  mouseBtnL, mouseBtnM, mouseBtnR, Key_B,  Key_Esc,       Key_Return, Key_Pipe,  Key_M, Key_Comma, Key_Period, Key_Backslash,  Key_Minus},
        { Key_LGUI,     Key_Backspace, Key_LShift, Key_LCtrl, NextKeymap, NoKey,    NoKey, NoKey, NoKey,  NextKeymap, Key_RCtrl, Key_RShift, Key_Space,  Key_RAlt }
    }


};


void release_keys_not_being_pressed() {

    // we use charsReportedLastTime to figure out what we might not be holding anymore and can now release. this is destructive to charsReportedLastTime

    for (int i=0; i<KEYS_HELD_BUFFER; i++) {
        // for each key we were holding as of the end of the last cycle
        // see if we're still holding it
        // if we're not, call an explicit Release

        if (charsReportedLastTime[i] != 0x00) {
            // if there _was_ a character in this slot, go check the
            // currently held characters
            for (int j=0; j<KEYS_HELD_BUFFER; j++) {
                if (charsReportedLastTime[i] == charsBeingReported[j]) {
                    // if's still held, we don't need to do anything.
                    charsReportedLastTime[i] = 0x00;
                    break;
                }
            }

        }
    }
    for (int i=0; i<KEYS_HELD_BUFFER; i++) {
        if (charsReportedLastTime[i] != 0x00) {
            Keyboard.release(charsReportedLastTime[i]);
        }
    }
}

void record_key_being_pressed(byte character) {
    for (int i=0; i<KEYS_HELD_BUFFER; i++) {
        // todo - deal with overflowing the 12 key buffer here
        if (charsBeingReported[i] == 0x00) {
            charsBeingReported[i] = character;
            break;
        }
    }
}

boolean key_was_pressed (byte keyState) {
    if ( byte((keyState >> 4)) ^ B00001111 ) {
        return false;
    }
    else {
        return true;
    }

}

boolean key_was_not_pressed (byte keyState) {
    if ( byte((keyState >> 4)) ^ B00000000 ) {
        return false;
    }
    else {
        return true;
    }

}

boolean key_is_pressed (byte keyState) {

    if ( byte((keyState << 4)) ^ B11110000 ) {
        return false;
    }
    else {
        return true;
    }
}
boolean key_is_not_pressed (byte keyState) {

    if ( byte((keyState << 4)) ^ B00000000 ) {
        return false;
    }
    else {
        return true;
    }
}

boolean key_toggled_on(byte keyState) {
    if (key_is_pressed(keyState) &&  key_was_not_pressed(keyState)) {
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


void reset_matrix() {
    for (int col = 0; col < COLS; col++) {
        for (int row = 0; row < ROWS; row++) {
            matrixState[row][col] <<= 1;
        }
    }
    for (int i=0; i<KEYS_HELD_BUFFER; i++) {
        charsReportedLastTime[i] = charsBeingReported[i];
        charsBeingReported[i] = 0x00;
    }
}
double mouse_accel (double cycles) {
    double accel = atan((cycles/25)-5);
    accel += 1.5707963267944; // we want the whole s curve, not just the bit that's usually above the x and y axes;
    accel = accel *0.85;
	if (accel<0.25) {
		accel =0.25;
	}
    return accel;
}

void send_key_events(int layer) {
    //for every newly pressed button, figure out what logical key it is and send a key down event
    // for every newly released button, figure out what logical key it is and send a key up event


    // TODO:switch to sending raw HID packets


    bool mouseActiveThisCycle = false;
    int x = 0;
    int y = 0;

    for (int row = 0; row < ROWS; row++) {

        for (int col = 0; col < COLS; col++) {
            byte switchState = matrixState[row][col];
            Key mappedKey = keymaps[layer][row][col];
            if (mappedKey.flags & MOUSE_KEY ) {
                if (key_is_pressed(switchState)) {
                    mouseActiveThisCycle = true;
                    if (mappedKey.rawKey & MOUSE_UP) {
                        y-=1;
                    }
                    if (mappedKey.rawKey & MOUSE_DN) {
                        y+= 1;
                    }
                    if (mappedKey.rawKey & MOUSE_L) {
                        x-= 1;
                    }

                    if (mappedKey.rawKey & MOUSE_R) {
                        x+= 1 ;
                    }
                }
            } else if (mappedKey.flags & SYNTHETIC_KEY) {
                if (mappedKey.rawKey == KEY_MOUSE_BTN_L || mappedKey.rawKey == KEY_MOUSE_BTN_M|| mappedKey.rawKey == KEY_MOUSE_BTN_R) {
                    if (key_toggled_on (switchState)) {
                        Mouse.press(mappedKey.rawKey);
                    } else if (key_is_pressed(switchState)) {
                    } else if (Mouse.isPressed(mappedKey.rawKey) ) {
                        Mouse.release(mappedKey.rawKey);
                    }
                }
            } else {
                if (key_is_pressed(switchState)) {
                    record_key_being_pressed(mappedKey.rawKey);
                    if (key_toggled_on (switchState)) {
                        Keyboard.press(mappedKey.rawKey);
                    }
                }
                else if (key_toggled_off (switchState)) {
                    Keyboard.release(mappedKey.rawKey);
                }
            }
        }
    }
    if (mouseActiveThisCycle) {
        mouseActiveForCycles++;
        double accel = (double) mouse_accel(mouseActiveForCycles);
        float moveX=0;
        float moveY=0;
        if (x>0) {
            moveX = (x*accel) + carriedOverX;
            carriedOverX = moveX - floor(moveX);
        }
        else if(x<0) {
            moveX = (x*accel) - carriedOverX;
            carriedOverX = ceil(moveX) - moveX;
        }

        if (y >0) {
            moveY = (y*accel) + carriedOverY;
            carriedOverY = moveY - floor(moveY);
        } else if (y<0) {
            moveY = (y*accel) - carriedOverY;
            carriedOverY = ceil(moveY) - moveY;
        }
	Serial.println();
	Serial.print("cycles: ");
	Serial.println(mouseActiveForCycles);
	Serial.print("Accel: ");
	Serial.print(accel); Serial.print("	moveX is "); Serial.print(moveX); Serial.print(" moveY is "); Serial.print(moveY); Serial.print(" carriedoverx is "); Serial.print(carriedOverX); Serial.print(" carriedOverY is "); Serial.println(carriedOverY);
        Mouse.move(moveX,moveY, 0);
    } else {
        mouseActiveForCycles=0;
    }
    release_keys_not_being_pressed();
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

    int active_layer = current_layer;

    //scan the Keyboard matrix looking for connections
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
            // to see if the Key being held is a firmware level
            // metakey, so we can act on it, lest we only discover
            // that we should be looking at a seconary Keymap halfway through the matrix scan



            if (! (keymaps[active_layer][row][col].flags ^ ( MOMENTARY | SWITCH_TO_LAYER))) { // this logic sucks. there is a better way TODO this

                if (key_is_pressed(matrixState[row][col])) {
                    active_layer = keymaps[current_layer][row][col].rawKey;
                }
            }
        }
        digitalWrite(rowPins[row], HIGH);
    }
    send_key_events(active_layer);
}


void report_matrix() {
    if (reporting_counter++ %100 == 0 ) {
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
    Mouse.begin();
    Serial.begin(115200);
    setup_matrix();
}

void loop() {
    scan_matrix();
    //    report_matrix();
    reset_matrix();
}

