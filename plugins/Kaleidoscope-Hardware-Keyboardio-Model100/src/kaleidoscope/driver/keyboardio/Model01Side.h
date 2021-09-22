// -*- mode: c++ -*-
/* kaleidoscope::driver::keyboardio::Model01Side
 * Copyright (C) 2015-2020  Keyboard.io, Inc
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <Arduino.h>
#include "wire-protocol-constants.h"

// We allow cRGB/CRGB to be defined already when this is included.
//
#ifndef CRGB
struct cRGB {
  uint8_t b;
  uint8_t g;
  uint8_t r;
};
#define CRGB(r,g,b) (cRGB){b, g, r}
#endif

#define LED_BANKS 4

#define LEDS_PER_HAND 32
#define LED_BYTES_PER_BANK sizeof(cRGB)  * LEDS_PER_HAND/LED_BANKS

namespace kaleidoscope {
namespace driver {
namespace keyboardio {

typedef union {
  cRGB leds[LEDS_PER_HAND];
  byte bytes[LED_BANKS][LED_BYTES_PER_BANK];
} LEDData_t;

typedef union {
  uint8_t rows[4];
  uint32_t all;
} keydata_t;

// config options

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
// used to configure interrupts, configuration for a particular controller
class Model01Side {
 public:
  explicit Model01Side(byte setAd01);
  ~Model01Side() {}

  int readVersion();

  byte setKeyscanInterval(byte delay);
  int readKeyscanInterval();

  byte setLEDSPIFrequency(byte frequency);
  int readLEDSPIFrequency();

  void sendLEDData();
  void setOneLEDTo(byte led, cRGB color);
  void setAllLEDsTo(cRGB color);
  keydata_t getKeyData();
  bool readKeys();
  LEDData_t ledData;
  uint8_t controllerAddress();

  void setBrightness(uint8_t brightness) {
    brightness_adjustment_ = 255 - brightness;
  }
  uint8_t getBrightness() {
    return 255 - brightness_adjustment_;
  }

 private:
  uint8_t brightness_adjustment_ = 0;
  int addr;
  int ad01;
  keydata_t keyData;
  byte nextLEDBank = 0;
  void sendLEDBank(byte bank);
  int readRegister(uint8_t cmd);
};
#else // ifndef KALEIDOSCOPE_VIRTUAL_BUILD
class Model01Side;
#endif // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

} // namespace keyboardio
} // namespace driver
} // namespace kaleidoscope
