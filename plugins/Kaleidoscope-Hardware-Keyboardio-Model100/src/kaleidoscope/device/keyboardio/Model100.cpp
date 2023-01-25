/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Model100 -- Keyboardio Model 100 hardware support for Kaleidoscope
 * Copyright (C) 2021  Keyboard.io, Inc
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

#ifdef ARDUINO_keyboardio_model_100

#include "kaleidoscope/device/keyboardio/Model100.h"

#include <Arduino.h>  // for PROGMEM
#include <Wire.h>     // for Wire

#include "kaleidoscope/driver/keyscanner/Base_Impl.h"  // For Base<>

namespace kaleidoscope {
namespace device {
namespace keyboardio {

constexpr uint8_t Model100LEDDriverProps::key_led_map[] PROGMEM;

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD

/********* Model100Hands *********/

struct Model100Hands {
  static driver::keyboardio::Model100Side leftHand;
  static driver::keyboardio::Model100Side rightHand;

  static void setup();
};

driver::keyboardio::Model100Side Model100Hands::leftHand(0);
driver::keyboardio::Model100Side Model100Hands::rightHand(3);

void Model100Hands::setup() {
  Model100KeyScanner::enableScannerPower();
  delay(70);
  Wire.begin();
  Wire.setClock(400000);
}

/********* LED Driver *********/
bool Model100LEDDriver::isLEDChanged = true;

void Model100LEDDriver::setBrightness(uint8_t brightness) {
  Model100Hands::leftHand.setBrightness(brightness);
  Model100Hands::rightHand.setBrightness(brightness);
  isLEDChanged = true;
}

uint8_t Model100LEDDriver::getBrightness() {
  return Model100Hands::leftHand.getBrightness();
}

void Model100LEDDriver::setCrgbAt(uint8_t i, cRGB crgb) {
  if (i < 32) {
    cRGB oldColor = getCrgbAt(i);
    isLEDChanged |= !(oldColor.r == crgb.r && oldColor.g == crgb.g && oldColor.b == crgb.b);

    Model100Hands::leftHand.ledData.leds[i] = crgb;
  } else if (i < 64) {
    cRGB oldColor = getCrgbAt(i);
    isLEDChanged |= !(oldColor.r == crgb.r && oldColor.g == crgb.g && oldColor.b == crgb.b);

    Model100Hands::rightHand.ledData.leds[i - 32] = crgb;
  } else {
    // TODO(anyone):
    // how do we want to handle debugging assertions about crazy user
    // code that would overwrite other memory?
  }
}

cRGB Model100LEDDriver::getCrgbAt(uint8_t i) {
  if (i >= 64)
    return {0, 0, 0};

  if (i < 32) {
    return Model100Hands::leftHand.ledData.leds[i];
  } else {
    return Model100Hands::rightHand.ledData.leds[i - 32];
  }
}

void Model100LEDDriver::syncLeds() {
  if (!isLEDChanged)
    return;
  // LED Data is stored in four "banks" for each side
  // We send it all at once to make it look nicer.
  // We alternate left and right hands because otherwise
  // we run into a race condition with updating the next bank
  // on an ATTiny before it's done writing the previous one to memory

  Model100Hands::leftHand.sendLEDData();
  Model100Hands::rightHand.sendLEDData();

  Model100Hands::leftHand.sendLEDData();
  Model100Hands::rightHand.sendLEDData();

  Model100Hands::leftHand.sendLEDData();
  Model100Hands::rightHand.sendLEDData();

  Model100Hands::leftHand.sendLEDData();
  Model100Hands::rightHand.sendLEDData();

  isLEDChanged = false;
}

/********* Key scanner *********/

driver::keyboardio::keydata_t Model100KeyScanner::leftHandState;
driver::keyboardio::keydata_t Model100KeyScanner::rightHandState;
driver::keyboardio::keydata_t Model100KeyScanner::previousLeftHandState;
driver::keyboardio::keydata_t Model100KeyScanner::previousRightHandState;

void Model100KeyScanner::enableScannerPower() {
  // Turn on the switched 5V network.
  //  make sure this happens at least 100ms after USB connect
  // to satisfy inrush limits
  //
  pinMode(PB9, OUTPUT_OPEN_DRAIN);
  digitalWrite(PB9, LOW);

  pinMode(PB14, INPUT);
  digitalWrite(PB14, LOW);
  pinMode(PB15, INPUT);
  digitalWrite(PB15, LOW);
}

void Model100KeyScanner::disableScannerPower() {
  // Turn on power to the 5V net
  //
  pinMode(PB9, OUTPUT_OPEN_DRAIN);
  digitalWrite(PB9, HIGH);
}


void Model100KeyScanner::setup() {
  Model100Hands::setup();
}

void Model100KeyScanner::readMatrix() {
  //scan the Keyboard matrix looking for connections
  previousLeftHandState  = leftHandState;
  previousRightHandState = rightHandState;

  if (Model100Hands::leftHand.readKeys()) {
    leftHandState = Model100Hands::leftHand.getKeyData();
  }

  if (Model100Hands::rightHand.readKeys()) {
    rightHandState = Model100Hands::rightHand.getKeyData();
  }
}

void Model100KeyScanner::actOnHalfRow(uint8_t row, uint8_t colState, uint8_t colPrevState, uint8_t startPos) {
  if ((colState != colPrevState) || (colState != 0)) {
    for (uint8_t col = 0; col < 8; col++) {
      // Build up the key state for row, col
      uint8_t keyState = ((bitRead(colPrevState, 0) << 0) |
                          (bitRead(colState, 0) << 1));
      if (keyState)
        ThisType::handleKeyswitchEvent(Key_NoKey, KeyAddr(row, startPos - col), keyState);

      // Throw away the data we've just used, so we can read the next column
      colState     = colState >> 1;
      colPrevState = colPrevState >> 1;
    }
  }
}

void Model100KeyScanner::actOnMatrixScan() {
  for (uint8_t row = 0; row < 4; row++) {
    actOnHalfRow(row, leftHandState.rows[row], previousLeftHandState.rows[row], 7);
    actOnHalfRow(row, rightHandState.rows[row], previousRightHandState.rows[row], 15);
  }
}


void Model100KeyScanner::scanMatrix() {
  readMatrix();
  actOnMatrixScan();
}

void Model100KeyScanner::setKeyscanInterval(uint8_t interval) {
  Model100Hands::leftHand.setKeyscanInterval(interval);
  Model100Hands::rightHand.setKeyscanInterval(interval);
}

bool Model100KeyScanner::isKeyswitchPressed(KeyAddr key_addr) {
  auto row = key_addr.row();
  auto col = key_addr.col();
  if (col <= 7) {
    return (bitRead(leftHandState.rows[row], 7 - col) != 0);
  } else {
    return (bitRead(rightHandState.rows[row], 7 - (col - 8)) != 0);
  }
}

bool Model100KeyScanner::wasKeyswitchPressed(KeyAddr key_addr) {
  auto row = key_addr.row();
  auto col = key_addr.col();
  if (col <= 7) {
    return (bitRead(previousLeftHandState.rows[row], 7 - col) != 0);
  } else {
    return (bitRead(previousRightHandState.rows[row], 7 - (col - 8)) != 0);
  }
}

uint8_t Model100KeyScanner::pressedKeyswitchCount() {
  return __builtin_popcountl(leftHandState.all) + __builtin_popcountl(rightHandState.all);
}

uint8_t Model100KeyScanner::previousPressedKeyswitchCount() {
  return __builtin_popcountl(previousLeftHandState.all) + __builtin_popcountl(previousRightHandState.all);
}

/********* Hardware plugin *********/

void Model100::enableHardwareTestMode() {
  // Toggle the programming LEDS on
  // TODO(anyone): PORTD |= (1 << 5);
  // TOOD(anyone): PORTB |= (1 << 0);

  // Disable the debouncer on the ATTinys
  KeyScanner::setKeyscanInterval(2);
}

void Model100::rebootBootloader() {
  USBCore().disconnect();
  NVIC_SystemReset();
}

#endif

}  // namespace keyboardio
}  // namespace device
}  // namespace kaleidoscope

#endif
