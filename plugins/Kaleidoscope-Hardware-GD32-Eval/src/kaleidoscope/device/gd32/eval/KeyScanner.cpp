/* -*- mode: c++ -*-
 * Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2021  Keyboard.io, Inc.
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

#ifdef ARDUINO_GD32F303ZE_EVAL

#include "Arduino.h"
#include "kaleidoscope/Runtime.h"
#include "kaleidoscope/device/gd32/eval/KeyScanner.h"

namespace kaleidoscope {
namespace device {
namespace gd32 {
namespace eval {

uint8_t KeyScanner::keyState;
uint8_t KeyScanner::previousKeyState;
int KeyScanner::prevPinState[2];

void KeyScanner::setup() {
  pinMode(PA3, INPUT);
  pinMode(PB13, INPUT);

  prevPinState[0] = HIGH;
  prevPinState[1] = HIGH;
}

void KeyScanner::scanMatrix() {
  readMatrix();
  actOnMatrixScan();
}

void KeyScanner::readMatrix() {
  int b1, b2;

  b1 = digitalRead(PA3);
  b2 = digitalRead(PB13);

  previousKeyState = keyState;

  if ((b1 != prevPinState[0]) && (b1 == HIGH)) {
    bitSet(keyState, 0);
  }
  prevPinState[0] = b1;

  if ((b2 != prevPinState[1]) && (b2 == HIGH)) {
    bitSet(keyState, 1);
  }
  prevPinState[1] = b2;
}

void KeyScanner::actOnMatrixScan() {
  if (bitRead(keyState, 0) != bitRead(previousKeyState, 0)) {
    ThisType::handleKeyswitchEvent(
      Key_NoKey,
      typename Props_::KeyAddr(0, 0),
      keyState
    );
  }
  if (bitRead(keyState, 1) != bitRead(previousKeyState, 1)) {
    ThisType::handleKeyswitchEvent(
      Key_NoKey,
      typename Props_::KeyAddr(0, 1),
      keyState
    );
  }
}

} // namespace eval
} // namespace gd32
} // namespace device
} // namespace kaleidoscope

#endif
