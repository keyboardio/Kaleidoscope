// Copyright 2013 Jesse Vincent <jesse@fsck.com>
// All Rights Reserved. (To be licensed under an opensource license
// before the release of the keyboard.io model 01

#include <stdio.h>
//#include <ps2.h>


#define ROW_A 0
#define ROW_B 1
#define ROW_C 2
#define ROW_D 3
#define ROW_E 4

#define COL_1 0
#define COL_2 1
#define COL_3 2
#define COL_4 3
#define COL_5 4
#define COL_6 5
#define COL_7 6
#define COL_8 7
#define COL_9 8
#define COL_10 9
#define COL_11 10
#define COL_12 11
#define COL_13 12
#define COL_14 13
#define NO_KEY false
#define KEY_RIGHT_FN2 false
#define KEY_LEFT_FN2 false


/* These are defined by the platform
 
 #define KEY_LEFT_CTRL           0x80
 #define KEY_LEFT_SHIFT          0x81
 #define KEY_LEFT_ALT            0x82
 #define KEY_LEFT_GUI            0x83
 #define KEY_RIGHT_CTRL          0x84
 #define KEY_RIGHT_SHIFT         0x85
 #define KEY_RIGHT_ALT           0x86
 #define KEY_RIGHT_GUI           0x87
 
 #define KEY_UP_ARROW            0xDA
 #define KEY_DOWN_ARROW          0xD9
 #define KEY_LEFT_ARROW          0xD8
 #define KEY_RIGHT_ARROW         0xD7
 #define KEY_BACKSPACE           0xB2
 #define KEY_TAB                         0xB3
 #define KEY_RETURN                      0xB0
 #define KEY_ESC                         0xB1
 #define KEY_INSERT                      0xD1
 #define KEY_DELETE                      0xD4
 #define KEY_PAGE_UP                     0xD3
 #define KEY_PAGE_DOWN           0xD6
 #define KEY_HOME                        0xD2
 #define KEY_END                         0xD5
 #define KEY_CAPS_LOCK           0xC1
 #define KEY_F1                          0xC2
 #define KEY_F2                          0xC3
 #define KEY_F3                          0xC4
 #define KEY_F4                          0xC5
 #define KEY_F5                          0xC6
 #define KEY_F6                          0xC7
 #define KEY_F7                          0xC8
 #define KEY_F8                          0xC9
 #define KEY_F9                          0xCA
 #define KEY_F10                         0xCB
 #define KEY_F11                         0xCC
 #define KEY_F12                         0xCD
 
 */


#define KEY_0 '0'
#define KEY_1 '1'
#define KEY_2 '2'
#define KEY_3 '3'
#define KEY_4 '4'
#define KEY_5 '5'
#define KEY_6 '6'
#define KEY_7 '7'
#define KEY_8 '8'
#define KEY_9 '9'
#define KEY_A 'a'
#define KEY_B 'b'
#define KEY_C 'c'
#define KEY_D 'd'
#define KEY_E 'e'
#define KEY_F 'f'
#define KEY_G 'g'
#define KEY_H 'h'
#define KEY_I 'i'
#define KEY_J 'j'
#define KEY_K 'k'
#define KEY_L 'l'
#define KEY_M 'm'
#define KEY_N 'n'
#define KEY_O 'o'
#define KEY_P 'p'
#define KEY_Q 'q'
#define KEY_R 'r'
#define KEY_S 's'
#define KEY_T 't'
#define KEY_U 'u'
#define KEY_V 'v'
#define KEY_W 'w'
#define KEY_X 'x'
#define KEY_Y 'y'
#define KEY_Z 'z'

#define KEY_BACKTICK  '`'
#define KEY_MINUS  '-'
#define KEY_EQUALS '='
#define KEY_LEFT_BRACKET '['
#define KEY_RIGHT_BRACKET ']'
#define KEY_BACKSLASH '\\'
#define KEY_SEMICOLON ';'
#define KEY_QUOTE '\''
#define KEY_COMMA ','
#define KEY_PERIOD '.'
#define KEY_SPACE ' '
#define KEY_SLASH '/'


static const int COLS = 14;
static const int ROWS = 5;

static int	colPins[COLS] = {

  16,0,1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12
};
static int	rowPins[ROWS] = {
  A2, A3, A4,A5,15


};









boolean		previousMatrix[COLS][ROWS];
boolean		currentMatrix[COLS][ROWS];
char keymap[ROWS][COLS];

void 
setup()
{

  Serial.begin(115200);
  setup_matrix();
  init_keymap();
  Keyboard.begin();
  //set up the col pins as inputs


  // initialize the previous button matrix
  // initialize the current button matrix
}


void 
loop()
{
  scan_matrix();
  send_key_event();
  reset_matrix();
}




void 
reset_matrix()
{
  //TODO:be smarter and just move pointers around

  for (int col = 0; col < COLS; col++) {
    for (int row = 0; row < ROWS; row++) {
      previousMatrix[col][row] = currentMatrix[col][row];
      currentMatrix[col][row] = 0;
    }
  }
}

void 
send_key_event()
{
  //for every newly pressed button, figure out what logical key it is and send a key down event
  // for every newly released button, figure out what logical key it is and send a key up event

    // TODO:switch to sending raw HID packets

  // set last_matrix to matrix and clear matrix


}

void 
setup_matrix()
{
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
  //blank out the matrix.blank out the previous version of the matrix

  for (int col = 0; col < COLS; col++) {
    for (int row = 0; row < ROWS; row++) {
      previousMatrix[col][col] = 0;
      currentMatrix[col][row] = 0;
    }
  }

}

void 
scan_matrix()
{

  //scan the keyboard matrix looking for connections
  for (int row = 0; row < ROWS; row++) {

    digitalWrite(rowPins[row], LOW);
    for (int col = 0; col < COLS; col++) {

      //If we see an electrical connection on I->J,
      int value = !digitalRead(colPins[col]);

      if (previousMatrix[col][row] != value) {
        if (value) {
          Keyboard.press(keymap[row][col]);
        } 
        else { 
          Keyboard.release(keymap[row][col]);
        }

        Serial.print("Detected a change on ");
        Serial.print(col);
        Serial.print(" ");
        Serial.print(row);
        Serial.print(" to ");
        Serial.print(value);
        Serial.println(".");
      }
      currentMatrix[col][row] = value;

    }
    digitalWrite(rowPins[row], HIGH);

  }
}

void init_keymap()  {
  keymap[ROW_A][COL_1] = NO_KEY;
  keymap[ROW_A][COL_2] = KEY_1;
  keymap[ROW_A][COL_3] = KEY_2;
  keymap[ROW_A][COL_4] = KEY_3;
  keymap[ROW_A][COL_5] = KEY_4;
  keymap[ROW_A][COL_6] = KEY_5;
  keymap[ROW_A][COL_7] = NO_KEY;
  keymap[ROW_A][COL_8] = NO_KEY;
  keymap[ROW_A][COL_9] = KEY_6;
  keymap[ROW_A][COL_10] = KEY_7;
  keymap[ROW_A][COL_11] = KEY_8;
  keymap[ROW_A][COL_12] = KEY_9;
  keymap[ROW_A][COL_13] = KEY_0;
  keymap[ROW_A][COL_14] = NO_KEY;


  keymap[ROW_B][COL_1] = KEY_BACKTICK;
  keymap[ROW_B][COL_2] = KEY_Q;
  keymap[ROW_B][COL_3] = KEY_W;
  keymap[ROW_B][COL_4] = KEY_E;
  keymap[ROW_B][COL_5] = KEY_R;
  keymap[ROW_B][COL_6] = KEY_T;
  keymap[ROW_B][COL_7] = NO_KEY;
  keymap[ROW_B][COL_8] = NO_KEY;
  keymap[ROW_B][COL_9] = KEY_Y;
  keymap[ROW_B][COL_10] = KEY_U;
  keymap[ROW_B][COL_11] = KEY_I;
  keymap[ROW_B][COL_12] = KEY_O;
  keymap[ROW_B][COL_13] = KEY_P;
  keymap[ROW_B][COL_14] = KEY_EQUALS;
  keymap[ROW_C][COL_1] = KEY_PAGE_UP;
  keymap[ROW_C][COL_2] = KEY_A;
  keymap[ROW_C][COL_3] = KEY_S;
  keymap[ROW_C][COL_4] = KEY_D;
  keymap[ROW_C][COL_5] = KEY_F;
  keymap[ROW_C][COL_6] = KEY_G;
  keymap[ROW_C][COL_7] = KEY_TAB;
  keymap[ROW_C][COL_8] = KEY_RETURN;
  keymap[ROW_C][COL_9] = KEY_H;
  keymap[ROW_C][COL_10] = KEY_J;
  keymap[ROW_C][COL_11] = KEY_K;
  keymap[ROW_C][COL_12] = KEY_L;
  keymap[ROW_C][COL_13] = KEY_SEMICOLON;
  keymap[ROW_C][COL_14] = KEY_QUOTE;
  keymap[ROW_D][COL_1] = KEY_PAGE_DOWN;
  keymap[ROW_D][COL_2] = KEY_Z;
  keymap[ROW_D][COL_3] = KEY_X;
  keymap[ROW_D][COL_4] = KEY_C;
  keymap[ROW_D][COL_5] = KEY_V;
  keymap[ROW_D][COL_6] = KEY_B;
  keymap[ROW_D][COL_7] = KEY_ESC;
  keymap[ROW_D][COL_8] = KEY_RETURN;
  keymap[ROW_D][COL_9] = KEY_N;
  keymap[ROW_D][COL_10] = KEY_M;
  keymap[ROW_D][COL_11] = KEY_COMMA;
  keymap[ROW_D][COL_12] = KEY_PERIOD;
  keymap[ROW_D][COL_13] = KEY_SLASH;
  keymap[ROW_D][COL_14] = KEY_MINUS;
  keymap[ROW_E][COL_1] = KEY_LEFT_GUI;
  keymap[ROW_E][COL_2] = KEY_BACKSPACE;
  keymap[ROW_E][COL_3] = KEY_LEFT_SHIFT;
  keymap[ROW_E][COL_4] = KEY_LEFT_CTRL;
  keymap[ROW_E][COL_5] = KEY_LEFT_FN2;
  keymap[ROW_E][COL_6] = NO_KEY;
  keymap[ROW_E][COL_7] = NO_KEY;
  keymap[ROW_E][COL_8] = NO_KEY;
  keymap[ROW_E][COL_9] = NO_KEY;
  keymap[ROW_E][COL_10] = KEY_RIGHT_FN2;
  keymap[ROW_E][COL_11] = KEY_RIGHT_CTRL;
  keymap[ROW_E][COL_12] = KEY_RIGHT_SHIFT;
  keymap[ROW_E][COL_13] = KEY_SPACE;
  keymap[ROW_E][COL_14] = KEY_RIGHT_ALT;


}


