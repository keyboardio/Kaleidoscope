/* This file originated in QMK. https://github.com/qmk/qmk_firmware
 * quantum/config_common.h 73e634482ea8f57d1f1a5f1e16bc3ffd74f84b8e
 *
 * Original copyright statement:
 *
 * Copyright 2015-2018 Jack Humbert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
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

#if !defined(__ASSEMBLER__) && !defined(KALEIDOSCOPE_VIRTUAL_BUILD)
//#include <avr/io.h>
#endif
#define PORT_SHIFTER 4 // this may be 4 for all AVR chips

// If you want to add more to this list, reference the PINx definitions in these header
// files: https://github.com/vancegroup-mirrors/avr-libc/tree/master/avr-libc/include/avr

#if defined(__SAMD21__) || defined(__SAMD51__)
// TODO - these numbers aren't right
#define ADDRESS_BASE 0x00
#define PINA_ADDRESS 0x9
#define PINB_ADDRESS 0x3
#define PINC_ADDRESS 0x6
#define PIND_ADDRESS 0x9
#define PINE_ADDRESS 0xC
#define PINF_ADDRESS 0xF
#elif !defined(KALEIDOSCOPE_VIRTUAL_BUILD)
#error "Pins are not defined"
#endif

/* I/O pins */
#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
#define PINDEF(port, pin) ((PIN##port##_ADDRESS << PORT_SHIFTER) | pin)
#else // ifndef KALEIDOSCOPE_VIRTUAL_BUILD
#define PINDEF(port, pin) 0
#define PORTA
#define PORTB
#define PORTC
#define PORTD
#define PORTE
#define PORTF
#endif // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

#ifdef PORTA
#define PIN_A0 PINDEF(A, 0)
#define PIN_A1 PINDEF(A, 1)
#define PIN_A2 PINDEF(A, 2)
#define PIN_A3 PINDEF(A, 3)
#define PIN_A4 PINDEF(A, 4)
#define PIN_A5 PINDEF(A, 5)
#define PIN_A6 PINDEF(A, 6)
#define PIN_A7 PINDEF(A, 7)
#endif
#ifdef PORTB
#define PIN_B0 PINDEF(B, 0)
#define PIN_B1 PINDEF(B, 1)
#define PIN_B2 PINDEF(B, 2)
#define PIN_B3 PINDEF(B, 3)
#define PIN_B4 PINDEF(B, 4)
#define PIN_B5 PINDEF(B, 5)
#define PIN_B6 PINDEF(B, 6)
#define PIN_B7 PINDEF(B, 7)
#endif
#ifdef PORTC
#define PIN_C0 PINDEF(C, 0)
#define PIN_C1 PINDEF(C, 1)
#define PIN_C2 PINDEF(C, 2)
#define PIN_C3 PINDEF(C, 3)
#define PIN_C4 PINDEF(C, 4)
#define PIN_C5 PINDEF(C, 5)
#define PIN_C6 PINDEF(C, 6)
#define PIN_C7 PINDEF(C, 7)
#endif
#ifdef PORTD
#define PIN_D0 PINDEF(D, 0)
#define PIN_D1 PINDEF(D, 1)
#define PIN_D2 PINDEF(D, 2)
#define PIN_D3 PINDEF(D, 3)
#define PIN_D4 PINDEF(D, 4)
#define PIN_D5 PINDEF(D, 5)
#define PIN_D6 PINDEF(D, 6)
#define PIN_D7 PINDEF(D, 7)
#endif
#ifdef PORTE
#define PIN_E0 PINDEF(E, 0)
#define PIN_E1 PINDEF(E, 1)
#define PIN_E2 PINDEF(E, 2)
#define PIN_E3 PINDEF(E, 3)
#define PIN_E4 PINDEF(E, 4)
#define PIN_E5 PINDEF(E, 5)
#define PIN_E6 PINDEF(E, 6)
#define PIN_E7 PINDEF(E, 7)
#endif
#ifdef PORTF
#define PIN_F0 PINDEF(F, 0)
#define PIN_F1 PINDEF(F, 1)
#define PIN_F2 PINDEF(F, 2)
#define PIN_F3 PINDEF(F, 3)
#define PIN_F4 PINDEF(F, 4)
#define PIN_F5 PINDEF(F, 5)
#define PIN_F6 PINDEF(F, 6)
#define PIN_F7 PINDEF(F, 7)
#endif

/* converting pins to ports */
enum { PIN_OFFSET, DDR_OFFSET, PORT_OFFSET};

#define PIN_ADDRESS_MASK 0xF

#define PIN_ADDRESS(p, offset) _SFR_IO8(ADDRESS_BASE + (p >> PORT_SHIFTER) + offset)
#define PIN_REG_FOR_PIN(pin) PIN_ADDRESS(pin, PIN_OFFSET )
#define DDR_REG_FOR_PIN(pin) PIN_ADDRESS(pin, DDR_OFFSET )
#define PORT_REG_FOR_PIN(pin) PIN_ADDRESS(pin, PORT_OFFSET )
#define PIN_NUM_FOR_PIN(pin) ( pin & PIN_ADDRESS_MASK )
#define PIN_MASK_FOR_PIN(pin) _BV(PIN_NUM_FOR_PIN(pin))

#define DDR_INPUT(pin) (DDR_REG_FOR_PIN(pin) &= ~(PIN_MASK_FOR_PIN(pin)))
#define DDR_OUTPUT(pin) (DDR_REG_FOR_PIN(pin) |= (PIN_MASK_FOR_PIN(pin)))


#define ENABLE_PULLUP(pin) (PORT_REG_FOR_PIN(pin) |= (PIN_MASK_FOR_PIN(pin)))
#define DISABLE_PULLUP(pin) (PORT_REG_FOR_PIN(pin) &= ~(PIN_MASK_FOR_PIN(pin)))


#define OUTPUT_HIGH(pin) (PORT_REG_FOR_PIN(pin) |= (PIN_MASK_FOR_PIN(pin)))
#define OUTPUT_LOW(pin) (PORT_REG_FOR_PIN(pin) &= ~(PIN_MASK_FOR_PIN(pin)))
#define OUTPUT_TOGGLE(pin) (PORT_REG_FOR_PIN(pin) ^= (PIN_MASK_FOR_PIN(pin)))

#define READ_PIN(pin) (!!(PIN_REG_FOR_PIN(pin) & PIN_MASK_FOR_PIN(pin)))

