/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Technomancy-Atreus -- Atreus hardware support for Kaleidoscope
 * Copyright (C) 2018  Keyboard.io, Inc
 *
 * Based on QMK (commit e9a67f8fd)
 *  (C) Jack Humbert, Phil Hagelberg, and others
 * Original QMK sources:
 *  - keyboards/atreus/atreus.h
 *  - keyboards/atreus/config.h
 *  - quantum/matrix.c
 *  - tmk_core/common/avr/bootloader.c
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

#include <Kaleidoscope.h>
#include <KeyboardioHID.h>
#include <avr/wdt.h>

static bool do_scan_ = true;

ISR(TIMER1_OVF_vect) {
  do_scan_ = true;
}

namespace kaleidoscope {
namespace hardware {

uint16_t Atreus::previousKeyState_[ROWS];
uint16_t Atreus::keyState_[ROWS];
uint16_t Atreus::masks_[ROWS];

uint8_t Atreus::debounce_matrix_[ROWS][COLS];
uint8_t Atreus::debounce = 3;

void Atreus::setup(void) {
  wdt_disable();
  delay(100);

  for (uint8_t i=0; i < ROWS; i++) {
    unselectRow(i);
    keyState_[i] = previousKeyState_[i] = 0;
  }

  // Initialize columns
  DDRB &= ~(_BV(5) | _BV(4) | _BV(6) | _BV(7));
  PORTB |= (_BV(5) | _BV(4) | _BV(6) | _BV(7));

  DDRD &= ~(_BV(7) | _BV(4) | _BV(6));
  PORTD |= (_BV(7) | _BV(4) | _BV(6));

  DDRC &= ~(_BV(6));
  PORTC |= (_BV(6));

  DDRE &= ~(_BV(6));
  PORTE |= (_BV(6));

  DDRF &= ~(_BV(6) | _BV(7));
  PORTF |= (_BV(6) | _BV(7));

  /* Set up Timer1 for 500usec */
  TCCR1B = _BV(WGM13);
  TCCR1A = 0;

  const unsigned long cycles = (F_CPU / 2000000) * 500;

  ICR1 = cycles;
  TCCR1B = _BV(WGM13) | _BV(CS10);
  TIMSK1 = _BV(TOIE1);
}

void Atreus::selectRow(uint8_t row)
{
  switch (row) {
    case 0:
      DDRD  |= (_BV(0));
      PORTD &= ~(_BV(0));
      break;
    case 1:
      DDRD  |= (_BV(1));
      PORTD &= ~(_BV(1));
      break;
    case 2:
      DDRD  |= (_BV(3));
      PORTD &= ~(_BV(3));
      break;
    case 3:
      DDRD  |= (_BV(2));
      PORTD &= ~(_BV(2));
      break;
    default:
      break;
  }
}

void Atreus::unselectRow(uint8_t row)
{
  switch (row) {
    case 0:
      DDRD  &= ~(_BV(0));
      PORTD |= (_BV(0));
      break;
    case 1:
      DDRD  &= ~(_BV(1));
      PORTD |= (_BV(1));
      break;
    case 2:
      DDRD  &= ~(_BV(3));
      PORTD |= (_BV(3));
      break;
    case 3:
      DDRD  &= ~(_BV(2));
      PORTD |= (_BV(2));
      break;
    default:
      break;
  }
}

uint16_t Atreus::readCols() {
  return ((PINB&(_BV(4)) ? 0 : (_BV(0))) |
          (PINB&(_BV(5)) ? 0 : (_BV(1))) |
          (PINB&(_BV(6)) ? 0 : (_BV(2))) |
          (PINB&(_BV(7)) ? 0 : (_BV(3))) |
          (PINC&(_BV(6)) ? 0 : (_BV(4))) |
          (PIND&(_BV(4)) ? 0 : (_BV(5))) |
          (PIND&(_BV(6)) ? 0 : (_BV(6))) |
          (PIND&(_BV(7)) ? 0 : (_BV(7))) |
          (PINE&(_BV(6)) ? 0 : (_BV(8))) |
          (PINF&(_BV(6)) ? 0 : (_BV(9))) |
          (PINF&(_BV(7)) ? 0 : (_BV(10))));
}

void Atreus::readMatrixRow(uint8_t current_row)
{
  uint16_t mask, cols;

  previousKeyState_[current_row] = keyState_[current_row];

  mask = debounceMaskForRow(current_row);

  selectRow(current_row);
  cols = (readCols() & mask) | (keyState_[current_row] & ~mask);
  unselectRow(current_row);
  debounceRow(cols ^ keyState_[current_row], current_row);
  keyState_[current_row] = cols;
}

void Atreus::readMatrix() {
  do_scan_ = false;

  for (uint8_t current_row = 0; current_row < ROWS; current_row++) {
    readMatrixRow(current_row);
  }
}

void Atreus::actOnMatrixScan() {
  for (byte row = 0; row < ROWS; row++) {
    for (byte col = 0; col < COLS; col++) {
      uint8_t keyState = (bitRead(previousKeyState_[row], col) << 0) |
                         (bitRead(keyState_[row], col) << 1);
      if (keyState) {
        handleKeyswitchEvent(Key_NoKey, row, col, keyState);
      }
    }
    previousKeyState_[row] = keyState_[row];
  }
}

void Atreus::scanMatrix() {
  if (!do_scan_)
    return;

  readMatrix();
  actOnMatrixScan();
}

void Atreus::maskKey(byte row, byte col) {
  if (row >= ROWS || col >= COLS)
    return;

  bitWrite(masks_[row], col, 1);
}

void Atreus::unMaskKey(byte row, byte col) {
  if (row >= ROWS || col >= COLS)
    return;

  bitWrite(masks_[row], col, 0);
}

bool Atreus::isKeyMasked(byte row, byte col) {
  if (row >= ROWS || col >= COLS)
    return false;

  return bitRead(masks_[row], col);
}

// Atreus-specific stuff

void Atreus::resetDevice() {
  cli();
  UDCON = 1;
  USBCON = (1 << FRZCLK);
  UCSR1B = 0;
  _delay_ms(5);

  EIMSK = 0;
  PCICR = 0;
  SPCR = 0;
  ACSR = 0;
  EECR = 0;
  ADCSRA = 0;
  TIMSK0 = 0;
  TIMSK1 = 0;
  TIMSK3 = 0;
  TIMSK4 = 0;
  UCSR1B = 0;
  TWCR = 0;
  DDRB = 0;
  DDRC = 0;
  DDRD = 0;
  DDRE = 0;
  DDRF = 0;
  TWCR = 0;
  PORTB = 0;
  PORTC = 0;
  PORTD = 0;
  PORTE = 0;
  PORTF = 0;
  asm volatile("jmp 0x7E00");
}

void Atreus::detachFromHost() {
  UDCON |= (1 << DETACH);
}

void Atreus::attachToHost() {
  UDCON &= ~(1 << DETACH);
}

bool Atreus::isKeyswitchPressed(byte row, byte col) {
  return (bitRead(keyState_[row], col) != 0);
}

bool Atreus::isKeyswitchPressed(uint8_t keyIndex) {
  keyIndex--;
  return isKeyswitchPressed(keyIndex / COLS, keyIndex % COLS);
}

uint8_t Atreus::pressedKeyswitchCount() {
  uint8_t count = 0;

  for (uint8_t r = 0; r < ROWS; r++) {
    count += __builtin_popcount(keyState_[r]);
  }
  return count;
}

uint16_t Atreus::debounceMaskForRow(uint8_t row) {
  uint16_t result = 0;

  for (uint16_t c = 0; c < COLS; ++c) {
    if (debounce_matrix_[row][c]) {
      --debounce_matrix_[row][c];
    } else {
      result |= (1 << c);
    }
  }
  return result;
}

void Atreus::debounceRow(uint16_t change, uint8_t row) {
  for (uint16_t i = 0; i < COLS; ++i) {
    if (change & (1 << i)) {
      debounce_matrix_[row][i] = debounce;
    }
  }
}

}
}

HARDWARE_IMPLEMENTATION KeyboardHardware;
kaleidoscope::hardware::Atreus &Atreus = KeyboardHardware;
