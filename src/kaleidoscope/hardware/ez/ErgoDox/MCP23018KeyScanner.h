/* -*- mode: c++ -*-
 * MCP23018KeyScanner -- MCP23018 Expander-based KeyScanner driver
 * Copyright (C) 2018, 2019  Keyboard.io, Inc
 *
 * Based on QMK (commit 8cdb4a915)
 *  (C) Jack Humbert, Erez Zukerman, Oleg Kostyuk
 * Original QMK sources:
 *  - keyboards/ergodox_ez/ergodox_ez.c
 *  - keyboards/ergodox_ez/ergodox_ez.h
 *  - keyboards/ergodox_ez/matrix.c
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <Arduino.h>

#include "kaleidoscope/driver/BaseKeyScanner.h"
#include "kaleidoscope/hardware/ez/ErgoDox/i2cmaster.h"

namespace kaleidoscope {
namespace hardware {
namespace ez {
namespace ergodox {

#define I2C_ADDR        0b0100000
#define I2C_ADDR_WRITE  ( (I2C_ADDR<<1) | I2C_WRITE )
#define I2C_ADDR_READ   ( (I2C_ADDR<<1) | I2C_READ  )
#define IODIRA          0x00
#define IODIRB          0x01
#define GPPUA           0x0C
#define GPPUB           0x0D
#define GPIOA           0x12
#define GPIOB           0x13
#define OLATA           0x14
#define OLATB           0x15

template <typename _KeyScannerDescription>
class MCP23018KeyScanner : public kaleidoscope::driver::BaseKeyScanner<_KeyScannerDescription> {
 public:
  void setup() {
    static_assert(
        sizeof(_KeyScannerDescription::matrix_rows) > 0,
        "The key scanner description has zero rows."
    );
    static_assert(
        sizeof(_KeyScannerDescription::matrix_columns) > 0,
        "The key scanner description has zero columns."
    );

    expander_error_ = initExpander();
  }

  void __attribute__((optimize(3))) readMatrix() {
    reattachExpanderOnError();

    for (uint8_t row = 0; row < _KeyScannerDescription::matrix_rows / 2; row++) {
      selectRow(row);
      readMatrixRow(row);
    }
  }

  void scanMatrix() {
    // We ALWAYS want to tell Kaleidoscope about the state of the matrix
    actOnMatrixScan();
  }

  void __attribute__((optimize(3))) actOnMatrixScan() {
    for (byte row = 0; row < _KeyScannerDescription::matrix_rows; row++) {
      for (byte col = 0; col < _KeyScannerDescription::matrix_columns; col++) {
        uint8_t keyState = (bitRead(previousKeyState_[row], col) << 0) |
                           (bitRead(keyState_[row], col) << 1);
        if (!keyState)
          continue;

        KeyAddr addr = KeyAddr(
            row + _KeyScannerDescription::row_offset,
            col + _KeyScannerDescription::column_offset
        );
        handleKeyswitchEvent(Key_NoKey, addr, keyState);
      }
      previousKeyState_[row] = keyState_[row];
    }
  }

  uint8_t pressedKeyswitchCount() {
    uint8_t count = 0;

    for (uint8_t r = 0; r < _KeyScannerDescription::matrix_rows; r++) {
      count += __builtin_popcount(keyState_[r]);
    }
    return count;
  }
  bool isKeyswitchPressed(KeyAddr key_addr) {
    return (bitRead(keyState_[key_addr.row()], key_addr.col()) != 0);
  }

  uint8_t previousPressedKeyswitchCount() {
    uint8_t count = 0;

    for (uint8_t r = 0; r < _KeyScannerDescription::matrix_rows; r++) {
      count += __builtin_popcount(previousKeyState_[r]);
    }
    return count;
  }
  bool wasKeyswitchPressed(KeyAddr key_addr) {
    return (bitRead(previousKeyState_[key_addr.row()], key_addr.col()) != 0);
  }

  void maskKey(KeyAddr key_addr) {
    if (!key_addr.isValid())
      return;

    bitWrite(masks_[key_addr.row()], key_addr.col(), 1);
  }
  void unMaskKey(KeyAddr key_addr) {
    if (!key_addr.isValid())
      return;

    bitWrite(masks_[key_addr.row()], key_addr.col(), 0);
  }
  bool isKeyMasked(KeyAddr key_addr) {
    if (!key_addr.isValid())
      return false;

    return bitRead(masks_[key_addr.row()], key_addr.col());
  }

 private:
  uint8_t expander_error_ = 0x20;
  typedef _KeyScannerDescription KeyScannerDescription_;
  static volatile uint16_t previousKeyState_[_KeyScannerDescription::matrix_rows];
  static volatile uint16_t keyState_[_KeyScannerDescription::matrix_rows];
  static uint16_t masks_[_KeyScannerDescription::matrix_rows];
  static uint8_t debounce_matrix_[_KeyScannerDescription::matrix_rows][_KeyScannerDescription::matrix_columns];

  uint8_t initExpander() {
    uint8_t status = 0x20;

    i2c_init();
    delay(1000);

    status = i2c_start(I2C_ADDR_WRITE);
    if (status)
      goto out;
    status = i2c_write(IODIRA);
    if (status)
      goto out;
    status = i2c_write(0b00000000);
    if (status)
      goto out;
    status = i2c_write(0b00111111);
    if (status)
      goto out;

    i2c_stop();

    status = i2c_start(I2C_ADDR_WRITE);
    if (status)
      goto out;
    status = i2c_write(GPPUA);
    if (status)
      goto out;
    status = i2c_write(0b00000000);
    if (status)
      goto out;
    status = i2c_write(0b00111111);
    if (status)
      goto out;

 out:
    i2c_stop();

    return status;
  }

  void reattachExpanderOnError() {
    static uint32_t start_time = millis();

    if (!expander_error_)
      return;

    if (millis() - start_time < 5000)
      return;

    expander_error_ = initExpander();
    start_time = millis();
  }

  void __attribute__((optimize(3))) selectRow(uint8_t row) {
    if (expander_error_)
      return;

    expander_error_ = i2c_start(I2C_ADDR_WRITE);
    if (expander_error_)
      goto out;
    expander_error_ = i2c_write(GPIOA);
    if (expander_error_)
      goto out;
    expander_error_ = i2c_write(0xFF & ~(1 << row));
    if (expander_error_)
      goto out;
 out:
    i2c_stop();
  }

  uint8_t __attribute__((optimize(3))) readCols(uint8_t row) {
    if (expander_error_) {
      return 0;
    }
    uint8_t data = 0;

    expander_error_ = i2c_start(I2C_ADDR_WRITE);
    if (expander_error_)
      goto out;
    expander_error_ = i2c_write(GPIOB);
    if (expander_error_)
      goto out;
    expander_error_ = i2c_start(I2C_ADDR_READ);
    if (expander_error_)
      goto out;

    data = i2c_readNak();
    data = ~data;
 out:
    i2c_stop();
    return data;
  }

  void __attribute__((optimize(3))) readMatrixRow(uint8_t row) {
    uint8_t mask, cols;

    mask = debounceMaskForRow(row);
    cols = (readCols(row) & mask) | (keyState_[row] & ~mask);
    debounceRow(cols ^ keyState_[row], row);
    keyState_[row] = cols;
  }

  uint8_t debounceMaskForRow(uint8_t row) {
    uint8_t result = 0;

    for (uint8_t c = 0; c < _KeyScannerDescription::matrix_columns; ++c) {
      if (debounce_matrix_[row][c]) {
        --debounce_matrix_[row][c];
      } else {
        result |= (1 << c);
      }
    }
    return result;
  }

  void debounceRow(uint8_t change, uint8_t row) {
    for (uint8_t i = 0; i < _KeyScannerDescription::matrix_columns; ++i) {
      if (change & (1 << i)) {
        debounce_matrix_[row][i] = _KeyScannerDescription::debounce;
      }
    }
  }

};

}
}
}
}
