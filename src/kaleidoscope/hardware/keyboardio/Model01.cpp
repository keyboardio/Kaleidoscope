/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Model01 -- Keyboard.io Model01 hardware support for Kaleidoscope
 * Copyright (C) 2017-2018  Keyboard.io, Inc
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

#include <Kaleidoscope.h>
#include <KeyboardioHID.h>
#include <avr/wdt.h>

namespace kaleidoscope {
namespace hardware {
namespace keyboardio {

KeyboardioScanner Model01::leftHand(0);
KeyboardioScanner Model01::rightHand(3);
bool Model01::isLEDChanged = true;
keydata_t Model01::leftHandMask;
keydata_t Model01::rightHandMask;

static constexpr int8_t key_led_map[Model01::led_count] PROGMEM = {
  3, 4, 11, 12, 19, 20, 26, 27,     36, 37, 43, 44, 51, 52, 59, 60,
  2, 5, 10, 13, 18, 21, 25, 28,     35, 38, 42, 45, 50, 53, 58, 61,
  1, 6, 9, 14, 17, 22, 24, 29,     34, 39, 41, 46, 49, 54, 57, 62,
  0, 7, 8, 15, 16, 23, 31, 30,     33, 32, 40, 47, 48, 55, 56, 63,
};

Model01::Model01(void) {

}

void Model01::enableScannerPower(void) {
  // PC7
  //pinMode(13, OUTPUT);
  //digitalWrite(13, HIGH);
  // Turn on power to the LED net
  DDRC |= _BV(7);
  PORTC |= _BV(7);

}

// This lets the keyboard pull up to 1.6 amps from
// the host. That violates the USB spec. But it sure
// is pretty looking
void Model01::enableHighPowerLeds(void) {
  // PE6
  //    pinMode(7, OUTPUT);
  //    digitalWrite(7, LOW);
  DDRE |= _BV(6);
  PORTE &= ~_BV(6);

  // Set B4, the overcurrent check to an input with an internal pull-up
  DDRB &= ~_BV(4);	// set bit, input
  PORTB &= ~_BV(4);	// set bit, enable pull-up resistor



}

void Model01::setup(void) {
  wdt_disable();
  delay(100);
  enableScannerPower();

  // Consider not doing this until 30s after keyboard
  // boot up, to make it easier to rescue things
  // in case of power draw issues.
  enableHighPowerLeds();
  leftHandState.all = 0;
  rightHandState.all = 0;

  TWBR = 12; // This is 400mhz, which is the fastest we can drive the ATTiny
}

void Model01::enableHardwareTestMode() {

  // Toggle the programming LEDS on
  PORTD |= (1 << 5);
  PORTB |= (1 << 0);

  // Disable the debouncer on the ATTinys
  KeyboardHardware.setKeyscanInterval(2);
}



void Model01::setCrgbAt(int8_t i, cRGB crgb) {
  if (i < 0) {
    return;
  }
  if (i < 32) {
    cRGB oldColor = getCrgbAt(i);
    isLEDChanged |= !(oldColor.r == crgb.r && oldColor.g == crgb.g && oldColor.b == crgb.b);

    leftHand.ledData.leds[i] = crgb;
  } else if (i < 64) {
    cRGB oldColor = getCrgbAt(i);
    isLEDChanged |= !(oldColor.r == crgb.r && oldColor.g == crgb.g && oldColor.b == crgb.b);

    rightHand.ledData.leds[i - 32] = crgb;
  } else {
    // TODO(anyone):
    // how do we want to handle debugging assertions about crazy user
    // code that would overwrite other memory?
  }
}

void Model01::setCrgbAt(KeyAddr key_addr, cRGB color) {
  setCrgbAt(getLedIndex(key_addr), color);
}

int8_t Model01::getLedIndex(KeyAddr key_addr) {
  return pgm_read_byte(&(key_led_map[key_addr.toInt()]));
}

cRGB Model01::getCrgbAt(int8_t i) {
  if (i < 0 || i > 64)
    return {0, 0, 0};

  if (i < 32) {
    return leftHand.ledData.leds[i];
  } else {
    return rightHand.ledData.leds[i - 32] ;
  }
}

void Model01::syncLeds() {
  if (!isLEDChanged)
    return;

  leftHand.sendLEDData();
  rightHand.sendLEDData();

  leftHand.sendLEDData();
  rightHand.sendLEDData();

  leftHand.sendLEDData();
  rightHand.sendLEDData();

  leftHand.sendLEDData();
  rightHand.sendLEDData();

  isLEDChanged = false;
}

boolean Model01::ledPowerFault() {
  if (PINB & _BV(4)) {
    return true;
  } else {
    return false;
  }
}

void debugKeyswitchEvent(keydata_t state, keydata_t previousState, uint8_t keynum, KeyAddr key_addr) {
  if (bitRead(state.all, keynum) != bitRead(previousState.all, keynum)) {
    Serial.print("Looking at row ");
    Serial.print(key_addr.row());
    Serial.print(", col ");
    Serial.print(key_addr.col());
    Serial.print(" key # ");
    Serial.print(keynum);
    Serial.print(" ");
    Serial.print(bitRead(previousState.all, keynum));
    Serial.print(" -> ");
    Serial.print(bitRead(state.all, keynum));
    Serial.println();
  }
}


void Model01::readMatrix() {
  //scan the Keyboard matrix looking for connections
  previousLeftHandState = leftHandState;
  previousRightHandState = rightHandState;

  if (leftHand.readKeys()) {
    leftHandState = leftHand.getKeyData();
  }

  if (rightHand.readKeys()) {
    rightHandState = rightHand.getKeyData();
  }
}

void Model01::actOnHalfRow(byte row, byte colState, byte colPrevState, byte startPos) {
  if ((colState != colPrevState) || (colState != 0)) {
    for (byte col = 0; col < 8; col++) {
      // Build up the key state for row, col
      uint8_t keyState = ((bitRead(colPrevState, 0) << 0) |
                          (bitRead(colState,     0) << 1));
      if (keyState)
        handleKeyswitchEvent(Key_NoKey, KeyAddr(row, startPos - col), keyState);

      // Throw away the data we've just used, so we can read the next column
      colState = colState >> 1;
      colPrevState = colPrevState >> 1;
    }
  }
}

void Model01::actOnMatrixScan() {
  for (byte row = 0; row < 4; row++) {
    actOnHalfRow(row, leftHandState.rows[row], previousLeftHandState.rows[row], 7);
    actOnHalfRow(row, rightHandState.rows[row], previousRightHandState.rows[row], 15);
  }
}


void Model01::scanMatrix() {
  readMatrix();
  actOnMatrixScan();
}

void Model01::rebootBootloader() {
  // Set the magic bits to get a Caterina-based device
  // to reboot into the bootloader and stay there, rather
  // than run move onward
  //
  // These values are the same as those defined in
  // Caterina.c

  uint16_t bootKey = 0x7777;
  uint16_t *const bootKeyPtr = reinterpret_cast<uint16_t *>(0x0800);

  // Stash the magic key
  *bootKeyPtr = bootKey;

  // Set a watchdog timer
  wdt_enable(WDTO_120MS);

  while (1) {} // This infinite loop ensures nothing else
  // happens before the watchdog reboots us
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

void Model01::maskKey(KeyAddr key_addr) {
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

void Model01::unMaskKey(KeyAddr key_addr) {
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

bool Model01::isKeyMasked(KeyAddr key_addr) {
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

void Model01::maskHeldKeys(void) {
  memcpy(leftHandMask.rows, leftHandState.rows, sizeof(leftHandMask));
  memcpy(rightHandMask.rows, rightHandState.rows, sizeof(rightHandMask));
}


void Model01::setKeyscanInterval(uint8_t interval) {
  leftHand.setKeyscanInterval(interval);
  rightHand.setKeyscanInterval(interval);
}

bool Model01::isKeyswitchPressed(KeyAddr key_addr) {
  auto row = key_addr.row();
  auto col = key_addr.col();
  if (col <= 7) {
    return (bitRead(leftHandState.rows[row], 7 - col) != 0);
  } else {
    return (bitRead(rightHandState.rows[row], 7 - (col - 8)) != 0);
  }
}

bool Model01::isKeyswitchPressed(uint8_t keyIndex) {
  keyIndex--;
  return isKeyswitchPressed(KeyAddr(keyIndex));
}


bool Model01::wasKeyswitchPressed(KeyAddr key_addr) {
  auto row = key_addr.row();
  auto col = key_addr.col();
  if (col <= 7) {
    return (bitRead(previousLeftHandState.rows[row], 7 - col) != 0);
  } else {
    return (bitRead(previousRightHandState.rows[row], 7 - (col - 8)) != 0);
  }
}

bool Model01::wasKeyswitchPressed(uint8_t keyIndex) {
  keyIndex--;
  return wasKeyswitchPressed(KeyAddr(keyIndex));
}

uint8_t Model01::pressedKeyswitchCount() {
  return __builtin_popcountl(leftHandState.all) + __builtin_popcountl(rightHandState.all);
}

uint8_t Model01::previousPressedKeyswitchCount() {
  return __builtin_popcountl(previousLeftHandState.all) + __builtin_popcountl(previousRightHandState.all);
}

}
}
}

HARDWARE_IMPLEMENTATION KeyboardHardware;

#endif
