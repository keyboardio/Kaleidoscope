/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Model01 -- Keyboard.io Model01 hardware support for Kaleidoscope
 * Copyright (C) 2017-2019  Keyboard.io, Inc
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

#ifdef ARDUINO_AVR_MODEL01
#ifndef KALEIDOSCOPE_VIRTUAL_BUILD

#include <Kaleidoscope.h>
#include <avr/wdt.h>

namespace kaleidoscope {
namespace hardware {
namespace keyboardio {

bool Model01Leds::isLEDChanged = true;

static constexpr int8_t key_led_map[Model01::led_count] PROGMEM = {
  3, 4, 11, 12, 19, 20, 26, 27,     36, 37, 43, 44, 51, 52, 59, 60,
  2, 5, 10, 13, 18, 21, 25, 28,     35, 38, 42, 45, 50, 53, 58, 61,
  1, 6, 9, 14, 17, 22, 24, 29,     34, 39, 41, 46, 49, 54, 57, 62,
  0, 7, 8, 15, 16, 23, 31, 30,     33, 32, 40, 47, 48, 55, 56, 63,
};

void Model01Leds::enableHighPowerLeds(void) {
  // This lets the keyboard pull up to 1.6 amps from the host.
  // That violates the USB spec. But it sure is pretty looking
  DDRE |= _BV(6);
  PORTE &= ~_BV(6);

  // Set B4, the overcurrent check to an input with an internal pull-up
  DDRB &= ~_BV(4);	// set bit, input
  PORTB &= ~_BV(4);	// set bit, enable pull-up resistor
}

void Model01Leds::setup() {
  // TODO: Consider not doing this until 30s after keyboard
  // boot up, to make it easier to rescue things in case of power draw issues.
  enableHighPowerLeds();
}

void Model01Leds::setCrgbAt(int8_t i, cRGB crgb) {
  if (i < 0) {
    return;
  }
  if (i < 32) {
    cRGB oldColor = getCrgbAt(i);
    isLEDChanged |= !(oldColor.r == crgb.r && oldColor.g == crgb.g && oldColor.b == crgb.b);

    Model01KeyScanner::leftHand.ledData.leds[i] = crgb;
  } else if (i < 64) {
    cRGB oldColor = getCrgbAt(i);
    isLEDChanged |= !(oldColor.r == crgb.r && oldColor.g == crgb.g && oldColor.b == crgb.b);

    Model01KeyScanner::rightHand.ledData.leds[i - 32] = crgb;
  } else {
    // TODO(anyone):
    // how do we want to handle debugging assertions about crazy user
    // code that would overwrite other memory?
  }
}

int8_t Model01Leds::getLedIndex(KeyAddr key_addr) {
  return pgm_read_byte(&(key_led_map[key_addr.toInt()]));
}

cRGB Model01Leds::getCrgbAt(int8_t i) {
  if (i < 0 || i >= 64)
    return {0, 0, 0};

  if (i < 32) {
    return Model01KeyScanner::leftHand.ledData.leds[i];
  } else {
    return Model01KeyScanner::rightHand.ledData.leds[i - 32];
  }
}

void Model01Leds::syncLeds() {
  if (!isLEDChanged)
    return;

  // LED Data is stored in four "banks" for each side
  // We send it all at once to make it look nicer.
  // We alternate left and right hands because otherwise
  // we run into a race condition with updating the next bank
  // on an ATTiny before it's done writing the previous one to memory

  Model01KeyScanner::leftHand.sendLEDData();
  Model01KeyScanner::rightHand.sendLEDData();

  Model01KeyScanner::leftHand.sendLEDData();
  Model01KeyScanner::rightHand.sendLEDData();

  Model01KeyScanner::leftHand.sendLEDData();
  Model01KeyScanner::rightHand.sendLEDData();

  Model01KeyScanner::leftHand.sendLEDData();
  Model01KeyScanner::rightHand.sendLEDData();

  isLEDChanged = false;
}

boolean Model01Leds::ledPowerFault() {
  if (PINB & _BV(4)) {
    return true;
  } else {
    return false;
  }
}

}
}
}

#endif
#endif
