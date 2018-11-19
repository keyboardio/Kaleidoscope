/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-EZ-ErgoDox -- ErgoDox hardware support for Kaleidoscope
 * Copyright (C) 2018  Keyboard.io, Inc
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

#ifdef ARDUINO_AVR_ERGODOX

#include "kaleidoscope/hardware/ez/ErgoDox/ErgoDoxScanner.h"
#include <avr/wdt.h>
#include "kaleidoscope/hardware/ez/ErgoDox/i2cmaster.h"

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

namespace kaleidoscope {
namespace hardware {
namespace ez {

uint8_t ErgoDoxScanner::initExpander() {
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

void
ErgoDoxScanner::initCols() {
  DDRF  &= ~(1 << 7 | 1 << 6 | 1 << 5 | 1 << 4 | 1 << 1 | 1 << 0);
  PORTF |= (1 << 7 | 1 << 6 | 1 << 5 | 1 << 4 | 1 << 1 | 1 << 0);
}

void
ErgoDoxScanner::begin() {
  expander_error_ = initExpander();

  unselectRows();
  initCols();
}

void
ErgoDoxScanner::selectRow(int row) {
  if (row < 7) {
    if (!expander_error_) {
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
  } else {
    switch (row) {
    case 7:
      DDRB  |= (1 << 0);
      PORTB &= ~(1 << 0);
      break;
    case 8:
      DDRB  |= (1 << 1);
      PORTB &= ~(1 << 1);
      break;
    case 9:
      DDRB  |= (1 << 2);
      PORTB &= ~(1 << 2);
      break;
    case 10:
      DDRB  |= (1 << 3);
      PORTB &= ~(1 << 3);
      break;
    case 11:
      DDRD  |= (1 << 2);
      PORTD &= ~(1 << 3);
      break;
    case 12:
      DDRD  |= (1 << 3);
      PORTD &= ~(1 << 3);
      break;
    case 13:
      DDRC  |= (1 << 6);
      PORTC &= ~(1 << 6);
      break;
    }
  }
}

void
ErgoDoxScanner::unselectRows() {
  DDRB  &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
  PORTB &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
  DDRD  &= ~(1 << 2 | 1 << 3);
  PORTD &= ~(1 << 2 | 1 << 3);
  DDRC  &= ~(1 << 6);
  PORTC &= ~(1 << 6);
}

uint8_t
ErgoDoxScanner::readCols(int row) {
  if (row < 7) {
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
  } else {
    return (~((PINF & 0x03) | ((PINF & 0xF0) >> 2))) & ~0b11000000;
  }
}

void
ErgoDoxScanner::reattachExpanderOnError() {
  static uint32_t start_time = millis();

  if (!expander_error_)
    return;

  if (millis() - start_time < 5000)
    return;

  expander_error_ = initExpander();
  start_time = millis();
}

}
}
}

#endif
