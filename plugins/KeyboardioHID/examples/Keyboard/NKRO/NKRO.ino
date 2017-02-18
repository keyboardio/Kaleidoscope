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

const int pinLed = LED_BUILTIN;
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

    // Do not press to many at once or some OS will have problems.
    // Note that the resulting pressed order might differ,
    // because all keys are pressed at the same time.
    Keyboard.press(KEY_0);
    Keyboard.press(KEY_1);
    Keyboard.press(KEY_2);
    Keyboard.press(KEY_3);
    Keyboard.press(KEY_4);
    Keyboard.press(KEY_5);
    Keyboard.press(KEY_6);
    Keyboard.press(KEY_7);
    Keyboard.press(KEY_8);
    Keyboard.press(KEY_9);
    Keyboard.sendReport();

    // Release all keys and hit enter
    Keyboard.releaseAll();
    Keyboard.press(KEY_ENTER);
    Keyboard.sendReport();
    Keyboard.releaseAll();
    Keyboard.sendReport();

    // Simple debounce
    delay(300);
  }
}

