/* Kaleidoscope-LEDControl - LED control plugin for Kaleidoscope
 * Copyright (C) 2017-2018  Keyboard.io, Inc.
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

#include "kaleidoscope/plugin/BootAnimation.h"
#include "Kaleidoscope-LEDControl.h"

#ifdef ARDUINO_AVR_MODEL01
static void
type_letter(uint8_t letter) {
  LEDControl.setCrgbAt(letter, {255, 0, 0});
  LEDControl.syncLeds();
  delay(250);
  LEDControl.setCrgbAt(letter, {0, 0, 0});
  LEDControl.syncLeds();
  delay(10);
}
#endif

void
bootAnimation(void) {
#ifdef ARDUINO_AVR_MODEL01
  LEDControl.set_all_leds_to(0, 0, 0);
  type_letter(LED_K);
  type_letter(LED_E);
  type_letter(LED_Y);
  type_letter(LED_B);
  type_letter(LED_O);
  type_letter(LED_A);
  type_letter(LED_R);
  type_letter(LED_D);
  type_letter(LED_I);
  type_letter(LED_O);
  type_letter(LED_SPACE);
  type_letter(LED_0);
  type_letter(LED_PERIOD);
  type_letter(LED_9);
#endif
}
