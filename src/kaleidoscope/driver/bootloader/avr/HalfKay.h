/* -*- mode: c++ -*-
 * kaleidoscope::driver::bootloader::avr::HalfKay -- Driver for the HalfKay bootloader
 * Copyright (C) 2019  Keyboard.io, Inc
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

#pragma once

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
#include <avr/wdt.h>
#endif                                            // ifndef KALEIDOSCOPE_VIRTUAL_BUILD
#include "kaleidoscope/driver/bootloader/Base.h"  // IWYU pragma: keep
#include "kaleidoscope/driver/bootloader/None.h"  // for None

namespace kaleidoscope {
namespace driver {
namespace bootloader {
namespace avr {

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
class HalfKay : public kaleidoscope::driver::bootloader::Base {
 public:
  // To reset a Teensy with the HalfKay bootloader, we need to disable all
  // interrupts, all peripherals we have attached, USB, the watchdog timer, etc.
  // Once done, we can jump to the bootloader address.
  //
  // Documentation: https://www.pjrc.com/teensy/jump_to_bootloader.html
  static void rebootBootloader() {
    cli();
    UDCON  = 1;
    USBCON = (1 << FRZCLK);
    UCSR1B = 0;
    _delay_ms(5);

    EIMSK  = 0;
    PCICR  = 0;
    SPCR   = 0;
    ACSR   = 0;
    EECR   = 0;
    ADCSRA = 0;
    TIMSK0 = 0;
    TIMSK1 = 0;
    TIMSK3 = 0;
    TIMSK4 = 0;
    UCSR1B = 0;
    TWCR   = 0;
    DDRB   = 0;
    DDRC   = 0;
    DDRD   = 0;
    DDRE   = 0;
    DDRF   = 0;
    TWCR   = 0;
    PORTB  = 0;
    PORTC  = 0;
    PORTD  = 0;
    PORTE  = 0;
    PORTF  = 0;
    asm volatile("jmp 0x7E00");
  }
};
#else
typedef bootloader::None HalfKay;
#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

}  // namespace avr
}  // namespace bootloader
}  // namespace driver
}  // namespace kaleidoscope
