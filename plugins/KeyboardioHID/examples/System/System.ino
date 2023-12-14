/*
  Copyright (c) 2014-2015 NicoHood
  See the readme for credit to other people.

  System example
  Press a button to put pc into sleep/shut it down or wake it up again.


  See HID Project documentation for more Consumer keys.
  https://github.com/NicoHood/HID/wiki/System-API
*/

#include "KeyboardioHID.h"

const int pinLed     = LED_BUILTIN;
const int pinButtonS = 2;
const int pinButtonW = 3;

void setup() {
  // prepare led + buttons
  pinMode(pinLed, OUTPUT);
  pinMode(pinButtonS, INPUT_PULLUP);
  pinMode(pinButtonW, INPUT_PULLUP);

  // Sends a clean report to the host. This is important on any Arduino type.
  SystemControl.begin();
}

void loop() {
  if (!digitalRead(pinButtonS)) {
    digitalWrite(pinLed, HIGH);

    // Puts PC into sleep mode/shuts it down
    SystemControl.write(HID_SYSTEM_SLEEP);
    //SystemControl.write(HID_SYSTEM_POWER_DOWN);

    // Simple debounce
    delay(300);
    digitalWrite(pinLed, LOW);
  }

  if (!digitalRead(pinButtonW)) {
    digitalWrite(pinLed, HIGH);

    // Try to wake up the PC
    // This might fail on some PCs/Laptops where USB wakeup is not supported
    SystemControl.write(HID_SYSTEM_WAKE_UP);

    // Simple debounce
    delay(300);
    digitalWrite(pinLed, LOW);
  }
}
