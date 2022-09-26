/* kaleidoscope::driver::keyboardio::Model100Side
 * Copyright (C) 2021  Keyboard.io, Inc
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

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD

#include "kaleidoscope/driver/keyboardio/Model100Side.h"

#include <Arduino.h>
#include <Wire.h>
#include <utility/twi.h>

#include "kaleidoscope/driver/color/GammaCorrection.h"
#include "kaleidoscope/driver/keyboardio/wire-protocol-constants.h"

namespace kaleidoscope {
namespace driver {
namespace keyboardio {

#define SCANNER_I2C_ADDR_BASE 0x58
#define ELEMENTS(arr)         (sizeof(arr) / sizeof((arr)[0]))

uint8_t twi_uninitialized = 1;

Model100Side::Model100Side(uint8_t setAd01) {
  ad01 = setAd01;
  addr = SCANNER_I2C_ADDR_BASE | ad01;
  markDeviceUnavailable();
}

// Returns the relative controller addresss. The expected range is 0-3
uint8_t Model100Side::controllerAddress() {
  return ad01;
}

// Sets the keyscan interval. We currently do three reads.
// before declaring a key event debounced.
//
// Takes an integer value representing a counter.
//
// 0 - 0.1-0.25ms
// 1 - 0.125ms
// 10 - 0.35ms
// 25 - 0.8ms
// 50 - 1.6ms
// 100 - 3.15ms
//
// You should think of this as the _minimum_ keyscan interval.
// LED updates can cause a bit of jitter.
//
// returns the Wire.endTransmission code (0 = success)
// https://www.arduino.cc/en/Reference/WireEndTransmission
uint8_t Model100Side::setKeyscanInterval(uint8_t delay) {
  uint8_t data[] = {TWI_CMD_KEYSCAN_INTERVAL, delay};
  uint8_t result = writeData(data, ELEMENTS(data));
  return result;
}


// returns -1 on error, otherwise returns the scanner version integer
int Model100Side::readVersion() {
  return readRegister(TWI_CMD_VERSION);
}

// returns -1 on error, otherwise returns the scanner keyscan interval
int Model100Side::readKeyscanInterval() {
  return readRegister(TWI_CMD_KEYSCAN_INTERVAL);
}


// returns -1 on error, otherwise returns the LED SPI Frequncy
int Model100Side::readLEDSPIFrequency() {
  return readRegister(TWI_CMD_LED_SPI_FREQUENCY);
}

// Set the LED SPI Frequency. See wire-protocol-constants.h for
// values.
//
// returns the Wire.endTransmission code (0 = success)
// https://www.arduino.cc/en/Reference/WireEndTransmission
uint8_t Model100Side::setLEDSPIFrequency(uint8_t frequency) {
  uint8_t data[] = {TWI_CMD_LED_SPI_FREQUENCY, frequency};
  uint8_t result = writeData(data, ELEMENTS(data));

  return result;
}


// GD32 I2C implements timeouts which will cause a stall when a device does not answer.
// This method will verify that the device is around and ready to talk.
bool Model100Side::isDeviceAvailable() {
  return true;
  if (unavailable_device_check_countdown_ == 0) {
    // if the counter is zero, that's the special value that means "we know it's there"
    return true;
  } else if (--unavailable_device_check_countdown_ == 0) {
    // if the time to check counter was 1, check for the device
    uint8_t wire_result;
    Wire.beginTransmission(addr);
    wire_result = Wire.endTransmission();
    //if the check succeeds
    if (wire_result == 0) {
      // unavailable_device_check_countdown_ = 0; // TODO this is already true
      return true;
    } else {
      // set the time to check counter to max
      unavailable_device_check_countdown_ = UNAVAILABLE_DEVICE_COUNTDOWN_MAX;
      return false;
    }
  } else {
    // we've decremented the counter, but it's not time to probe for the device yet.
    return false;
  }
}

void Model100Side::markDeviceUnavailable() {
  unavailable_device_check_countdown_ = 1;  // We think there was a comms problem. Check on the next cycle
}

uint8_t Model100Side::writeData(uint8_t *data, uint8_t length) {
  if (isDeviceAvailable() == false) {
    return 1;
  }
  Wire.beginTransmission(addr);
  Wire.write(data, length);
  uint8_t result = Wire.endTransmission();
  if (result) {
    markDeviceUnavailable();
  }
  return result;
}

int Model100Side::readRegister(uint8_t cmd) {
  uint8_t return_value = 0;
  uint8_t data[]       = {cmd};
  uint8_t result       = writeData(data, ELEMENTS(data));

  // If the setup failed, return. This means there was a problem asking for the register
  if (result) {
    return -1;
  }

  delayMicroseconds(50);  // TODO(anyone): We may be able to drop this in the future
  // but will need to verify with correctly
  // sized pull-ups on both the left and right
  // hands' i2c SDA and SCL lines

  uint8_t rxBuffer[1] = {0};

  // perform blocking read into buffer

  Wire.requestFrom(addr, 1);  // request 1 byte from the keyscanner
  if (Wire.available()) {
    return Wire.read();
  } else {
    markDeviceUnavailable();
    return -1;
  }
}


// gives information on the key that was just pressed or released.
bool Model100Side::readKeys() {
  if (isDeviceAvailable() == false) {
    return false;
  }

  uint8_t row_counter = 0;
  // perform blocking read into buffer
  uint8_t read           = 0;
  uint8_t bytes_returned = 0;
  bytes_returned         = Wire.requestFrom(addr, 5);  // request 5 bytes from the keyscanner
  if (bytes_returned < 5) {
    return false;
  }
  if (Wire.available()) {
    read = Wire.read();
    if (TWI_REPLY_KEYDATA == read) {
      while (Wire.available()) {
        keyData.rows[row_counter++] = Wire.read();
      }
      return true;
    }
  }
  return false;
}

keydata_t Model100Side::getKeyData() {
  return keyData;
}

void Model100Side::sendLEDData() {
  sendLEDBank(nextLEDBank++);
  if (nextLEDBank == LED_BANKS) {
    nextLEDBank = 0;
  }
}

auto constexpr gamma8 = kaleidoscope::driver::color::gamma_correction;

void Model100Side::sendLEDBank(uint8_t bank) {
  uint8_t data[LED_BYTES_PER_BANK + 1];
  data[0] = TWI_CMD_LED_BASE + bank;
  for (uint8_t i = 0; i < LED_BYTES_PER_BANK; i++) {
    /* While the ATTiny controller does have a global brightness command, it is
     * limited to 32 levels, and those aren't nicely spread out either. For this
     * reason, we're doing our own brightness adjustment on this side, because
     * that results in a considerably smoother curve. */
    uint8_t c = ledData.bytes[bank][i];
    if (c > brightness_adjustment_)
      c -= brightness_adjustment_;
    else
      c = 0;

    data[i + 1] = pgm_read_byte(&gamma8[c]);
  }
  uint8_t result = writeData(data, ELEMENTS(data));
}

void Model100Side::setAllLEDsTo(cRGB color) {
  uint8_t data[] = {TWI_CMD_LED_SET_ALL_TO,
                    pgm_read_byte(&gamma8[color.b]),
                    pgm_read_byte(&gamma8[color.g]),
                    pgm_read_byte(&gamma8[color.r])};
  uint8_t result = writeData(data, ELEMENTS(data));
}

void Model100Side::setOneLEDTo(uint8_t led, cRGB color) {
  uint8_t data[] = {TWI_CMD_LED_SET_ONE_TO,
                    led,
                    pgm_read_byte(&gamma8[color.b]),
                    pgm_read_byte(&gamma8[color.g]),
                    pgm_read_byte(&gamma8[color.r])};
  uint8_t result = writeData(data, ELEMENTS(data));
}

}  // namespace keyboardio
}  // namespace driver
}  // namespace kaleidoscope

#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD
