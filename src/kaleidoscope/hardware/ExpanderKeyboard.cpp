/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-ExpanderKeyboard -- Base class for some keyboards with expanders
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

#if KALEIDOSCOPE_WITH_EXPANDER_KEYBOARD

#include "kaleidoscope/hardware/ExpanderKeyboard.h"
#include "kaleidoscope/hardware/common/i2cmaster.h"

#define I2C_ADDR        0b0100000
#define I2C_ADDR_WRITE  ( (I2C_ADDR<<1) | I2C_WRITE )
#define I2C_ADDR_READ   ( (I2C_ADDR<<1) | I2C_READ  )
#define IODIRA          0x00
#define GPPUA           0x0C
#define GPIOA           0x12
#define GPIOB           0x13

namespace kaleidoscope {
namespace hardware {

uint8_t ExpanderKeyboard::expander_error_;

uint8_t ExpanderKeyboard::initExpander() {
  Serial.println("initExpander");
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

void ExpanderKeyboard::setup(void) {
  ATMegaKeyboard::setup();

  expander_error_ = KeyboardHardware.initExpander();
}

uint16_t ExpanderKeyboard::readExpanderCols() {
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

void ExpanderKeyboard::selectExpanderRow(uint8_t row) {
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

void ExpanderKeyboard::readMatrix(void) {
  static const uint8_t expander_rows =
      KeyboardHardware.matrix_rows - KeyboardHardware.matrix_row_pin_cnt;

  KeyboardHardware.reattachExpanderOnError();
  ATMegaKeyboard::readMatrix();

  for (int8_t current_row = 0; current_row < expander_rows; current_row++) {
    uint16_t mask, cols;
    mask = KeyboardHardware.debounceMaskForRow(current_row);

    KeyboardHardware.selectExpanderRow(current_row);
    cols = (KeyboardHardware.readExpanderCols() & mask) | (KeyboardHardware.keyState_[current_row] & ~mask);

    KeyboardHardware.debounceRow(cols ^ KeyboardHardware.keyState_[current_row], current_row);
    KeyboardHardware.keyState_[current_row] = cols;
  }
}

void ExpanderKeyboard::reattachExpanderOnError() {
  static uint32_t start_time = millis();

  if (!expander_error_)
    return;

  if (millis() - start_time < 5000)
    return;

  expander_error_ = KeyboardHardware.initExpander();
#if 0
  Serial.print("reattachExpanderOnError=");
  Serial.println(expander_error_);
#endif
  start_time = millis();
}

}
}

#endif
