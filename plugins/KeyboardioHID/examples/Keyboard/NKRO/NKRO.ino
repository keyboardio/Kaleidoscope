/*
  Copyright (c) 2014-2015 NicoHood
  See the readme for credit to other people.

  NKRO Keyboard example

  Press a button to hold a lot of keys at the same time.
  NKRO can push 113 keys at the same time,
  the other Keyboards only 6 keys + 8 modifiers!


  See HID Project documentation for more information.
  https://github.com/NicoHood/HID/wiki/Keyboard-API#nkro-keyboard
*/

#include "KeyboardioHID.h"

const int pinLed    = LED_BUILTIN;
const int pinButton = 2;

void setup() {
  pinMode(pinLed, OUTPUT);
  pinMode(pinButton, INPUT_PULLUP);

  // Sends a clean report to the host. This is important on any Arduino type.
  Keyboard.begin();
}

void loop() {
  // Hold a lot of keys at the same time
  if (!digitalRead(pinButton)) {
    digitalWrite(pinLed, HIGH);

    // Do not press too many at once or some OS will have problems.
    // Note that the resulting pressed order might differ,
    // because all keys are pressed at the same time.
    Keyboard.press(HID_KEYBOARD_0_AND_RIGHT_PAREN);
    Keyboard.press(HID_KEYBOARD_1_AND_EXCLAMATION_POINT);
    Keyboard.press(HID_KEYBOARD_2_AND_AT);
    Keyboard.press(HID_KEYBOARD_3_AND_POUND);
    Keyboard.press(HID_KEYBOARD_4_AND_DOLLAR);
    Keyboard.press(HID_KEYBOARD_5_AND_PERCENT);
    Keyboard.press(HID_KEYBOARD_6_AND_CARAT);
    Keyboard.press(HID_KEYBOARD_7_AND_AMPERSAND);
    Keyboard.press(HID_KEYBOARD_8_AND_ASTERISK);
    Keyboard.press(HID_KEYBOARD_9_AND_LEFT_PAREN);
    Keyboard.sendReport();

    // Release all keys and hit enter
    Keyboard.releaseAll();
    Keyboard.press(HID_KEYBOARD_ENTER);
    Keyboard.sendReport();
    Keyboard.releaseAll();
    Keyboard.sendReport();

    // Simple debounce
    delay(300);
  }
}
