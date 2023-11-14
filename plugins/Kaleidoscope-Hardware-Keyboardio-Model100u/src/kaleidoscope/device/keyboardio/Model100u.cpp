/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Model100u -- Keyboardio Model 100 hardware support for Kaleidoscope
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

#ifdef ARDUINO_keyboardio_model_100u

#include "kaleidoscope/device/keyboardio/Model100u.h"

#include <Arduino.h>  // for PROGMEM

#include "kaleidoscope/driver/keyscanner/Base_Impl.h"  // For Base<>

namespace kaleidoscope {
namespace device {
namespace keyboardio {


#ifndef KALEIDOSCOPE_VIRTUAL_BUILD


/********* Key scanner *********/


void Model100uKeyScanner::setup() {
}

void Model100uKeyScanner::readMatrix() {
  //scan the Keyboard matrix looking for connections
}

void Model100uKeyScanner::actOnMatrixScan() {
}


void Model100uKeyScanner::scanMatrix() {
  readMatrix();
  actOnMatrixScan();
}

void Model100uKeyScanner::setKeyscanInterval(uint8_t interval) {
}

bool Model100uKeyScanner::isKeyswitchPressed(KeyAddr key_addr) {
}

bool Model100uKeyScanner::wasKeyswitchPressed(KeyAddr key_addr) {
}

uint8_t Model100uKeyScanner::pressedKeyswitchCount() {
}

uint8_t Model100uKeyScanner::previousPressedKeyswitchCount() {
}

/********* Hardware plugin *********/

void Model100u::setup() {
  Model100uKeyScanner::setup();
  kaleidoscope::device::Base<Model100uProps>::setup();
}

void Model100u::enableHardwareTestMode() {
  // Toggle the programming LEDS on
  // TODO(anyone): PORTD |= (1 << 5);
  // TOOD(anyone): PORTB |= (1 << 0);
}

void Model100u::rebootBootloader() {
  USBCore().disconnect();
  NVIC_SystemReset();
}

#endif

}  // namespace keyboardio
}  // namespace device
}  // namespace kaleidoscope

#endif
