/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Planck -- OLKB Planck hardware support for Kaleidoscope
 * Copyright (C) 2018  Keyboard.io, Inc
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of version 3 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef ARDUINO_AVR_PLANCK

#include <Kaleidoscope.h>
#include <KeyboardioHID.h>
#include <avr/wdt.h>

static bool do_scan_ = true;

ISR(TIMER1_OVF_vect) {
  do_scan_ = true;
}

namespace kaleidoscope {
namespace hardware {
namespace olkb {

constexpr uint8_t Planck::matrix_row_pins[matrix_rows];
constexpr uint8_t Planck::matrix_col_pins[matrix_columns];
uint16_t Planck::previousKeyState_[matrix_rows];
uint16_t Planck::keyState_[matrix_rows];
uint16_t Planck::masks_[matrix_rows];
uint8_t Planck::debounce_matrix_[matrix_rows][matrix_columns];

void Planck::setup(void) {
  wdt_disable();

  for (uint8_t i = 0; i < sizeof(matrix_col_pins); i++) {
    DDR_INPUT(matrix_col_pins[i]);
    ENABLE_PULLUP(matrix_col_pins[i]);
  }

  for (uint8_t i = 0; i < sizeof(matrix_row_pins); i++) {
    DDR_OUTPUT(matrix_row_pins[i]);
    OUTPUT_HIGH(matrix_row_pins[i]);
  }

  /* Set up Timer1 for 500usec */
  TCCR1B = _BV(WGM13);
  TCCR1A = 0;

  const uint32_t cycles = (F_CPU / 2000000) * 500;

  ICR1 = cycles;
  TCCR1B = _BV(WGM13) | _BV(CS10);
  TIMSK1 = _BV(TOIE1);
}


uint16_t Planck::readCols() {
  uint16_t results = 0x00 ;
  for (uint8_t i = 0; i < matrix_columns; i++) {
    results |= (!READ_PIN(matrix_col_pins[i]) << i);
  }
  return results;
}

void Planck::readMatrix() {

  for (uint8_t current_row = 0; current_row < sizeof(matrix_row_pins); current_row++) {
    uint16_t mask, cols;

    previousKeyState_[current_row] = keyState_[current_row];

    mask = debounceMaskForRow(current_row);

    OUTPUT_TOGGLE(matrix_row_pins[current_row]);
    cols = (readCols() & mask) | (keyState_[current_row] & ~mask);
    OUTPUT_TOGGLE(matrix_row_pins[current_row]);
    debounceRow(cols ^ keyState_[current_row], current_row);
    keyState_[current_row] = cols;


  }
}

void Planck::actOnMatrixScan() {
  for (byte row = 0; row < matrix_rows; row++) {
    for (byte col = 0; col < matrix_columns; col++) {
      uint8_t keyState = (bitRead(previousKeyState_[row], col) << 0) |
                         (bitRead(keyState_[row], col) << 1);
      if (keyState) {
        handleKeyswitchEvent(Key_NoKey, row, col, keyState);
      }
    }
    previousKeyState_[row] = keyState_[row];
  }
}

void Planck::scanMatrix() {
  if (!do_scan_)
    return;

  do_scan_ = false;

  readMatrix();
  actOnMatrixScan();
}

void Planck::maskKey(byte row, byte col) {
  if (row >= matrix_rows || col >= matrix_columns)
    return;

  bitWrite(masks_[row], col, 1);
}

void Planck::unMaskKey(byte row, byte col) {
  if (row >= matrix_rows || col >= matrix_columns)
    return;

  bitWrite(masks_[row], col, 0);
}

bool Planck::isKeyMasked(byte row, byte col) {
  if (row >= matrix_rows || col >= matrix_columns)
    return false;

  return bitRead(masks_[row], col);
}


void Planck::detachFromHost() {
  UDCON |= _BV(DETACH);
}

void Planck::attachToHost() {
  UDCON &= ~_BV(DETACH);
}

bool Planck::isKeyswitchPressed(byte row, byte col) {
  return (bitRead(keyState_[row], col) != 0);
}

bool Planck::isKeyswitchPressed(uint8_t keyIndex) {
  keyIndex--;
  return isKeyswitchPressed(keyIndex / matrix_columns, keyIndex % matrix_columns);
}

uint8_t Planck::pressedKeyswitchCount() {
  uint8_t count = 0;

  for (uint8_t r = 0; r < matrix_rows; r++) {
    count += __builtin_popcount(keyState_[r]);
  }
  return count;
}

uint16_t Planck::debounceMaskForRow(uint8_t row) {
  uint16_t result = 0;

  for (uint16_t c = 0; c < matrix_columns; ++c) {
    if (debounce_matrix_[row][c]) {
      --debounce_matrix_[row][c];
    } else {
      result |= _BV(c);
    }
  }
  return result;
}

void Planck::debounceRow(uint16_t change, uint8_t row) {
  for (uint16_t i = 0; i < matrix_columns; ++i) {
    if (change & _BV(i)) {
      debounce_matrix_[row][i] = debounce;
    }
  }
}

}
}
}

HARDWARE_IMPLEMENTATION KeyboardHardware;
kaleidoscope::hardware::olkb::Planck &Planck = KeyboardHardware;

#endif
