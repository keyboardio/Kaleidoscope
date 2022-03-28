/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-EZ-ErgoDox -- ErgoDox hardware support for Kaleidoscope
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

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
#ifdef ARDUINO_AVR_ERGODOX

#include "kaleidoscope/device/ez/ErgoDox/ErgoDoxScanner.h"

#include <Arduino.h>
#include <avr/wdt.h>

#include "kaleidoscope/device/avr/pins_and_ports.h"
#include "kaleidoscope/device/ez/ErgoDox/i2cmaster.h"

#define I2C_ADDR       0b0100000
#define I2C_ADDR_WRITE ((I2C_ADDR << 1) | I2C_WRITE)
#define I2C_ADDR_READ  ((I2C_ADDR << 1) | I2C_READ)
#define IODIRA         0x00
#define IODIRB         0x01
#define GPPUA          0x0C
#define GPPUB          0x0D
#define GPIOA          0x12
#define GPIOB          0x13
#define OLATA          0x14
#define OLATB          0x15

namespace kaleidoscope {
namespace device {
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

void ErgoDoxScanner::begin() {
  expander_error_ = initExpander();

  // Init columns
  DDRF &= ~(1 << 7 | 1 << 6 | 1 << 5 | 1 << 4 | 1 << 1 | 1 << 0);
  PORTF |= (1 << 7 | 1 << 6 | 1 << 5 | 1 << 4 | 1 << 1 | 1 << 0);

  // Init rows
  DDRB |= (1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
  PORTB |= (1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
  DDRD |= (1 << 2 | 1 << 3);
  PORTD |= (1 << 2 | 1 << 3);
  DDRC |= (1 << 6);
  PORTC |= (1 << 6);
}

void __attribute__((optimize(3))) ErgoDoxScanner::selectExtenderRow(int row) {
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
}

void __attribute__((optimize(3))) ErgoDoxScanner::toggleATMegaRow(int row) {
  static uint8_t row_pins[] = {PIN_B0, PIN_B1, PIN_B2, PIN_B3, PIN_D2, PIN_D3, PIN_C6};
  OUTPUT_TOGGLE(row_pins[row]);
}

uint8_t __attribute__((optimize(3)))
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

void ErgoDoxScanner::reattachExpanderOnError() {
  static uint32_t start_time = millis();

  if (!expander_error_)
    return;

  if (millis() - start_time < 5000)
    return;

  expander_error_ = initExpander();
  start_time      = millis();
}

}  // namespace ez
}  // namespace device
}  // namespace kaleidoscope

#endif
#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD
