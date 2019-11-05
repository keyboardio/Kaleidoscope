/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-ATMegaKeyboard -- Base class for some ATMega-based boards
 * Copyright (C) 2018  Keyboard.io, Inc
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

#include "Kaleidoscope.h"

#if KALEIDOSCOPE_WITH_ATMEGA_KEYBOARD

#include "kaleidoscope/device/ATMegaKeyboard.h"

namespace kaleidoscope {
namespace device {

uint8_t ATMegaKeyboard::debounce = 3;

void ATMegaKeyboard::setup(void) {
  wdt_disable();

  for (uint8_t i = 0; i < Kaleidoscope.device().matrix_columns; i++) {
    DDR_INPUT(Kaleidoscope.device().matrix_col_pins[i]);
    ENABLE_PULLUP(Kaleidoscope.device().matrix_col_pins[i]);
  }

  for (uint8_t i = 0; i < Kaleidoscope.device().matrix_rows; i++) {
    DDR_OUTPUT(Kaleidoscope.device().matrix_row_pins[i]);
    OUTPUT_HIGH(Kaleidoscope.device().matrix_row_pins[i]);
  }

  /* Set up Timer1 for 1700usec */
  TCCR1B = _BV(WGM13);
  TCCR1A = 0;

  const uint32_t cycles = (F_CPU / 2000000) * 1700;

  ICR1 = cycles;
  TCCR1B = _BV(WGM13) | _BV(CS10);
  TIMSK1 = _BV(TOIE1);
}

void __attribute__((optimize(3))) ATMegaKeyboard::readMatrix(void) {
  for (uint8_t current_row = 0; current_row < Kaleidoscope.device().matrix_rows; current_row++) {
    uint16_t mask, cols;

    mask = Kaleidoscope.device().debounceMaskForRow(current_row);

    OUTPUT_TOGGLE(Kaleidoscope.device().matrix_row_pins[current_row]);
    cols = (Kaleidoscope.device().readCols() & mask) | (Kaleidoscope.device().keyState_[current_row] & ~mask);
    OUTPUT_TOGGLE(Kaleidoscope.device().matrix_row_pins[current_row]);
    Kaleidoscope.device().debounceRow(cols ^ Kaleidoscope.device().keyState_[current_row], current_row);
    Kaleidoscope.device().keyState_[current_row] = cols;
  }
}

uint8_t ATMegaKeyboard::pressedKeyswitchCount() {
  uint8_t count = 0;

  for (int8_t r = 0; r < Kaleidoscope.device().matrix_rows; r++) {
    count += __builtin_popcount(Kaleidoscope.device().keyState_[r]);
  }
  return count;
}

uint8_t ATMegaKeyboard::previousPressedKeyswitchCount() {
  uint8_t count = 0;

  for (int8_t r = 0; r < Kaleidoscope.device().matrix_rows; r++) {
    count += __builtin_popcount(Kaleidoscope.device().previousKeyState_[r]);
  }
  return count;
}

void __attribute__((optimize(3))) ATMegaKeyboard::actOnMatrixScan() {
  for (byte row = 0; row < Kaleidoscope.device().matrix_rows; row++) {
    for (byte col = 0; col < Kaleidoscope.device().matrix_columns; col++) {
      uint8_t keyState = (bitRead(Kaleidoscope.device().previousKeyState_[row], col) << 0) |
                         (bitRead(Kaleidoscope.device().keyState_[row], col) << 1);
      if (keyState) {
        handleKeyswitchEvent(Key_NoKey, ::KeyAddr(row, col), keyState);
      }
    }
    Kaleidoscope.device().previousKeyState_[row] = Kaleidoscope.device().keyState_[row];
  }
}

void ATMegaKeyboard::scanMatrix() {
  // We ALWAYS want to tell Kaleidoscope about the state of the matrix
  Kaleidoscope.device().actOnMatrixScan();
}

/*
 * This function has loop unrolling disabled on purpose: we want to give the
 * hardware enough time to produce stable PIN reads for us. If we unroll the
 * loop, we will not have that, because even with the NOP, the codepath is too
 * fast. If we don't have stable reads, then entire rows or columns will behave
 * erratically.
 *
 * For this reason, we ask the compiler to not unroll our loop, which in turn,
 * gives hardware enough time to produce stable reads, at the cost of a little
 * bit of speed.
 *
 * Do not remove the attribute!
 */
__attribute__((optimize("no-unroll-loops")))
uint16_t ATMegaKeyboard::readCols() {
  uint16_t results = 0x00 ;
  for (uint8_t i = 0; i < Kaleidoscope.device().matrix_columns; i++) {
    asm("NOP"); // We need to pause a beat before reading or we may read before the pin is hot
    results |= (!READ_PIN(Kaleidoscope.device().matrix_col_pins[i]) << i);
  }
  return results;
}

uint16_t ATMegaKeyboard::debounceMaskForRow(uint8_t row) {
  uint16_t result = 0;

  for (uint16_t c = 0; c < Kaleidoscope.device().matrix_columns; ++c) {
    if (Kaleidoscope.device().debounce_matrix_[row][c]) {
      --Kaleidoscope.device().debounce_matrix_[row][c];
    } else {
      result |= _BV(c);
    }
  }
  return result;
}

void ATMegaKeyboard::debounceRow(uint16_t change, uint8_t row) {
  for (uint16_t i = 0; i < Kaleidoscope.device().matrix_columns; ++i) {
    if (change & _BV(i)) {
      Kaleidoscope.device().debounce_matrix_[row][i] = debounce;
    }
  }
}

}
}

#endif
