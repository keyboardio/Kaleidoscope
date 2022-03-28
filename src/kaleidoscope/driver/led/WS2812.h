/*
 * Based on Light_WS2812, from:
 *  https://github.com/cpldcpu/light_ws2812
 *
 * Original copyright:
 *
 *  light weight WS2812 lib V2.1 - Arduino support
 *
 *  Controls WS2811/WS2812/WS2812B RGB-LEDs
 *  Author: Matthias Riegler
 *
 *  Mar 07 2014: Added Arduino and C++ Library
 *
 *   September 6, 2014:    Added option to switch between most popular color orders
 *                                               (RGB, GRB, and BRG) --  Windell H. Oskay
 *
 * License:
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTabILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#pragma once

#include "kaleidoscope/driver/led/Color.h"
#include "kaleidoscope/hardware/avr/pins_and_ports.h"
#include "ws2812/config.h"

namespace kaleidoscope {
namespace driver {
namespace led {

template<uint8_t pin, class Color, int8_t ledCount>
class WS2812 {
 public:
  WS2812()
    : pinmask_(_BV(pin & 0xF)) {}

  int8_t led_count() {
    return ledCount;
  }

  void sync() {
    if (!modified_)
      return;

    DDR_OUTPUT(pin);

    sendArrayWithMask(pinmask_);
    _delay_us(50);
    modified_ = false;
  }

  void setColorAt(int8_t index, Color color) {
    if (index >= ledCount)
      return;
    modified_    = true;
    leds_[index] = color;
  }
  void setColorAt(int8_t index, uint8_t r, uint8_t g, uint8_t b) {
    if (index >= ledCount)
      return;
    modified_    = true;
    leds_[index] = Color(r, g, b);
  }
  Color getColorAt(int8_t index) {
    if (index >= ledCount)
      return Color(0, 0, 0);
    return leds_[index];
  }

 private:
  Color leds_[ledCount];  // NOLINT(runtime/arrays)
  uint8_t pinmask_;
  bool modified_ = false;

  void sendArrayWithMask(uint8_t maskhi) {
    uint8_t *data    = reinterpret_cast<uint8_t *>(leds_);
    uint16_t datalen = ledCount * sizeof(Color);
    uint8_t curbyte, ctr, masklo;
    uint8_t sreg_prev;

    masklo = ~maskhi & PORT_REG_FOR_PIN(pin);
    maskhi |= PORT_REG_FOR_PIN(pin);

    sreg_prev = SREG;
    cli();

    while (datalen--) {
      curbyte = (*data++);

      asm volatile(
        "       ldi   %0,8  \n\t"
        "loop%=:            \n\t"
        "       out   %2,%3 \n\t"  //  '1' [01] '0' [01] - re
#if (w1_nops & 1)
        w_nop1
#endif
#if (w1_nops & 2)
          w_nop2
#endif
#if (w1_nops & 4)
            w_nop4
#endif
#if (w1_nops & 8)
              w_nop8
#endif
#if (w1_nops & 16)
                w_nop16
#endif
        "       sbrs  %1,7  \n\t"  //  '1' [03] '0' [02]
        "       out   %2,%4 \n\t"  //  '1' [--] '0' [03] - fe-low
        "       lsl   %1    \n\t"  //  '1' [04] '0' [04]
#if (w2_nops & 1)
        w_nop1
#endif
#if (w2_nops & 2)
          w_nop2
#endif
#if (w2_nops & 4)
            w_nop4
#endif
#if (w2_nops & 8)
              w_nop8
#endif
#if (w2_nops & 16)
                w_nop16
#endif
        "       out   %2,%4 \n\t"  //  '1' [+1] '0' [+1] - fe-high
#if (w3_nops & 1)
        w_nop1
#endif
#if (w3_nops & 2)
          w_nop2
#endif
#if (w3_nops & 4)
            w_nop4
#endif
#if (w3_nops & 8)
              w_nop8
#endif
#if (w3_nops & 16)
                w_nop16
#endif

        "       dec   %0    \n\t"  //  '1' [+2] '0' [+2]
        "       brne  loop%=\n\t"  //  '1' [+3] '0' [+4]
        : "=&d"(ctr)
        : "r"(curbyte), "I"(_SFR_IO_ADDR(PORT_REG_FOR_PIN(pin))), "r"(maskhi), "r"(masklo));
    }

    SREG = sreg_prev;
  }
};
}  // namespace led
}  // namespace driver
}  // namespace kaleidoscope
