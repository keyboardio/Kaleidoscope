/* -*- mode: c++ -*-
 * driver::MCU::ATmega32U4 -- ATmega32U4 MCU driver for Kaleidoscope
 * Copyright (C) 2019, 2020  Keyboard.io, Inc
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

#include "kaleidoscope/driver/mcu/Base.h"  // for Base, BaseProps

namespace kaleidoscope {
namespace driver {
namespace mcu {

struct ATmega32U4Props : public kaleidoscope::driver::mcu::BaseProps {
  static constexpr bool disable_jtag           = false;
  static constexpr bool disable_clock_division = false;
};

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
template<typename _Props>
class ATmega32U4 : public kaleidoscope::driver::mcu::Base<_Props> {
 public:
  void detachFromHost() {
    UDCON |= _BV(DETACH);
  }
  void attachToHost() {
    UDCON &= ~_BV(DETACH);
  }

  static void disableJTAG() {
    /* These two lines here are the result of many hours spent chasing ghosts.
     * These are great lines, and we love them dearly, for they make a set of
     * pins that would otherwise be reserved for JTAG accessible from the
     * firmware.
     *
     * Most AVR chips that get put into keyboards have the JTAG port disabled in
     * fuses, but some do not. When they're used for JTAG, then no matter what
     * we do in the firmware, they will not be affected. So in case JTAG is not
     * disabled in fuses, we want to do that in the firmware. Luckily for us,
     * that's doable, we just have to write the JTD bit into MCUCR twice within
     * four cycles. These two lines do just that.
     *
     * For more information, see the ATmega16U4/ATmega32U4 datasheet, the
     * following sections:
     *  - 2.2.7 (PIN Descriptions; PIN F)
     *  - 7.8.7 (On-chip Debug System)
     *  - 26.5.1 (MCU Control Register – MCUCR)
     */
    MCUCR |= (1 << JTD);
    MCUCR |= (1 << JTD);
  }

  static void disableClockDivision() {
    CLKPR = (1 << CLKPCE);
    CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
  }

  void setup() {
    if (_Props::disable_jtag)
      disableJTAG();
    if (_Props::disable_clock_division)
      disableClockDivision();
  }

  bool USBConfigured() {
    return USBDevice.configured();
  }
};
#else
template<typename _Props>
class ATmega32U4 : public kaleidoscope::driver::mcu::Base<_Props> {};
#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

}  // namespace mcu
}  // namespace driver
}  // namespace kaleidoscope
