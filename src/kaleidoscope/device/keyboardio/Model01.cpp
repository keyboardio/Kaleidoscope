/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Model01 -- Keyboard.io Model01 hardware support for Kaleidoscope
 * Copyright (C) 2017-2020  Keyboard.io, Inc
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef ARDUINO_AVR_MODEL01

#include "Arduino.h"                                 // for PROGMEM
#include "kaleidoscope/device/keyboardio/Model01.h"  // for Model01LEDDriver...
#include "kaleidoscope/key_events.h"
#include "kaleidoscope/driver/keyscanner/Base_Impl.h"

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
#include <KeyboardioHID.h>
#include <avr/wdt.h>
#endif // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

namespace kaleidoscope {
namespace device {
namespace keyboardio {

constexpr uint8_t Model01LEDDriverProps::key_led_map[] PROGMEM;

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD

/********* Model01Hands *********/

struct Model01Hands {
  static KeyboardioScanner leftHand;
  static KeyboardioScanner rightHand;

  static void setup();
};

KeyboardioScanner Model01Hands::leftHand(0);
KeyboardioScanner Model01Hands::rightHand(3);

void Model01Hands::setup(void) {
  // This lets the keyboard pull up to 1.6 amps from the host.
  // That violates the USB spec. But it sure is pretty looking
  DDRE |= _BV(6);
  PORTE &= ~_BV(6);

  // Set B4, the overcurrent check to an input with an internal pull-up
  DDRB &= ~_BV(4);	// set bit, input
  PORTB &= ~_BV(4);	// set bit, enable pull-up resistor
}

/********* LED Driver *********/
bool Model01LEDDriver::isLEDChanged = true;

void Model01LEDDriver::setBrightness(uint8_t brightness) {
  Model01Hands::leftHand.setBrightness(brightness);
  Model01Hands::rightHand.setBrightness(brightness);
  isLEDChanged = true;
}

uint8_t Model01LEDDriver::getBrightness() {
  return Model01Hands::leftHand.getBrightness();
}

void Model01LEDDriver::setCrgbAt(uint8_t i, cRGB crgb) {
  if (i < 32) {
    cRGB oldColor = getCrgbAt(i);
    isLEDChanged |= !(oldColor.r == crgb.r && oldColor.g == crgb.g && oldColor.b == crgb.b);

    Model01Hands::leftHand.ledData.leds[i] = crgb;
  } else if (i < 64) {
    cRGB oldColor = getCrgbAt(i);
    isLEDChanged |= !(oldColor.r == crgb.r && oldColor.g == crgb.g && oldColor.b == crgb.b);

    Model01Hands::rightHand.ledData.leds[i - 32] = crgb;
  } else {
    // TODO(anyone):
    // how do we want to handle debugging assertions about crazy user
    // code that would overwrite other memory?
  }
}

cRGB Model01LEDDriver::getCrgbAt(uint8_t i) {
  if (i >= 64)
    return {0, 0, 0};

  if (i < 32) {
    return Model01Hands::leftHand.ledData.leds[i];
  } else {
    return Model01Hands::rightHand.ledData.leds[i - 32];
  }
}

void Model01LEDDriver::syncLeds() {
  if (!isLEDChanged)
    return;

  // LED Data is stored in four "banks" for each side
  // We send it all at once to make it look nicer.
  // We alternate left and right hands because otherwise
  // we run into a race condition with updating the next bank
  // on an ATTiny before it's done writing the previous one to memory

  Model01Hands::leftHand.sendLEDData();
  Model01Hands::rightHand.sendLEDData();

  Model01Hands::leftHand.sendLEDData();
  Model01Hands::rightHand.sendLEDData();

  Model01Hands::leftHand.sendLEDData();
  Model01Hands::rightHand.sendLEDData();

  Model01Hands::leftHand.sendLEDData();
  Model01Hands::rightHand.sendLEDData();

  isLEDChanged = false;
}

boolean Model01LEDDriver::ledPowerFault() {
  if (PINB & _BV(4)) {
    return true;
  } else {
    return false;
  }
}

/********* Key scanner *********/

keydata_t Model01KeyScanner::leftHandState;
keydata_t Model01KeyScanner::rightHandState;
keydata_t Model01KeyScanner::previousLeftHandState;
keydata_t Model01KeyScanner::previousRightHandState;
keydata_t Model01KeyScanner::leftHandMask;
keydata_t Model01KeyScanner::rightHandMask;

void Model01KeyScanner::enableScannerPower(void) {
  // Turn on power to the LED net
  DDRC |= _BV(7);
  PORTC |= _BV(7);
}

void Model01KeyScanner::setup() {
  wdt_disable();
  delay(100);
  enableScannerPower();
}

void Model01KeyScanner::readMatrix() {
  //scan the Keyboard matrix looking for connections
  previousLeftHandState = leftHandState;
  previousRightHandState = rightHandState;

  if (Model01Hands::leftHand.readKeys()) {
    leftHandState = Model01Hands::leftHand.getKeyData();
  }

  if (Model01Hands::rightHand.readKeys()) {
    rightHandState = Model01Hands::rightHand.getKeyData();
  }
}

void Model01KeyScanner::actOnHalfRow(byte row, byte colState, byte colPrevState, byte startPos) {
  if ((colState != colPrevState) || (colState != 0)) {
    for (byte col = 0; col < 8; col++) {
      // Build up the key state for row, col
      uint8_t keyState = ((bitRead(colPrevState, 0) << 0) |
                          (bitRead(colState,     0) << 1));
      if (keyState)
        ThisType::handleKeyswitchEvent(Key_NoKey, KeyAddr(row, startPos - col), keyState);

      // Throw away the data we've just used, so we can read the next column
      colState = colState >> 1;
      colPrevState = colPrevState >> 1;
    }
  }
}

void Model01KeyScanner::actOnMatrixScan() {
  for (byte row = 0; row < 4; row++) {
    actOnHalfRow(row, leftHandState.rows[row], previousLeftHandState.rows[row], 7);
    actOnHalfRow(row, rightHandState.rows[row], previousRightHandState.rows[row], 15);
  }
}


void Model01KeyScanner::scanMatrix() {
  readMatrix();
  actOnMatrixScan();
}

// In the maskKey(), unMaskKey(), and isKeyMasked() functions, we read and write bits in
// two bitfields -- one for each half of the keyboard. The fourth bit of the column number
// tells us which bitfield (right or left) to access, thus the "8" (B00001000). The row
// number tells us which element of the array to access. The last three bits of the column
// number tell us which of the eight bits to access, thus the "7" (B00000111), and we
// shift a bit starting from the left (B10000000, or 128) by that many places to get
// there. This is all nice and convenient because the keyboard has 64 keys, in symmetric
// halves, with eight keys per logical row.

constexpr byte HIGH_BIT = B10000000;
constexpr byte HAND_BIT = B00001000;
constexpr byte ROW_BITS = B00110000;
constexpr byte COL_BITS = B00000111;

void Model01KeyScanner::maskKey(KeyAddr key_addr) {
  if (!key_addr.isValid())
    return;

  auto row = key_addr.row();
  auto col = key_addr.col();
  if (col & HAND_BIT) {
    rightHandMask.rows[row] |= (HIGH_BIT >> (col & COL_BITS));
  } else {
    leftHandMask.rows[row] |= (HIGH_BIT >> (col & COL_BITS));
  }
}

void Model01KeyScanner::unMaskKey(KeyAddr key_addr) {
  if (!key_addr.isValid())
    return;

  auto row = key_addr.row();
  auto col = key_addr.col();
  if (col & HAND_BIT) {
    rightHandMask.rows[row] &= ~(HIGH_BIT >> (col & COL_BITS));
  } else {
    leftHandMask.rows[row] &= ~(HIGH_BIT >> (col & COL_BITS));
  }
}

bool Model01KeyScanner::isKeyMasked(KeyAddr key_addr) {
  if (!key_addr.isValid())
    return false;

  auto row = key_addr.row();
  auto col = key_addr.col();
  if (col & HAND_BIT) {
    return rightHandMask.rows[row] & (HIGH_BIT >> (col & COL_BITS));
  } else {
    return leftHandMask.rows[row] & (HIGH_BIT >> (col & COL_BITS));
  }
}

void Model01KeyScanner::maskHeldKeys() {
  memcpy(leftHandMask.rows, leftHandState.rows, sizeof(leftHandMask));
  memcpy(rightHandMask.rows, rightHandState.rows, sizeof(rightHandMask));
}


void Model01KeyScanner::setKeyscanInterval(uint8_t interval) {
  Model01Hands::leftHand.setKeyscanInterval(interval);
  Model01Hands::rightHand.setKeyscanInterval(interval);
}

bool Model01KeyScanner::isKeyswitchPressed(KeyAddr key_addr) {
  auto row = key_addr.row();
  auto col = key_addr.col();
  if (col <= 7) {
    return (bitRead(leftHandState.rows[row], 7 - col) != 0);
  } else {
    return (bitRead(rightHandState.rows[row], 7 - (col - 8)) != 0);
  }
}

bool Model01KeyScanner::wasKeyswitchPressed(KeyAddr key_addr) {
  auto row = key_addr.row();
  auto col = key_addr.col();
  if (col <= 7) {
    return (bitRead(previousLeftHandState.rows[row], 7 - col) != 0);
  } else {
    return (bitRead(previousRightHandState.rows[row], 7 - (col - 8)) != 0);
  }
}

uint8_t Model01KeyScanner::pressedKeyswitchCount() {
  return __builtin_popcountl(leftHandState.all) + __builtin_popcountl(rightHandState.all);
}

uint8_t Model01KeyScanner::previousPressedKeyswitchCount() {
  return __builtin_popcountl(previousLeftHandState.all) + __builtin_popcountl(previousRightHandState.all);
}

/********* Hardware plugin *********/

void Model01::setup() {
  Model01Hands::setup();
  kaleidoscope::device::Base<Model01Props>::setup();

  TWBR = 12; // This is 400mhz, which is the fastest we can drive the ATTiny
}

void Model01::enableHardwareTestMode() {
  // Toggle the programming LEDS on
  PORTD |= (1 << 5);
  PORTB |= (1 << 0);

  // Disable the debouncer on the ATTinys
  KeyScanner::setKeyscanInterval(2);
}

#endif

}
}
}
#endif
