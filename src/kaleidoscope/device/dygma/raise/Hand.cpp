/* -*- mode: c++ -*-
 * kaleidoscope::device::dygma::Raise -- Kaleidoscope device plugin for Dygma Raise
 * Copyright (C) 2017-2019  Keyboard.io, Inc
 * Copyright (C) 2017-2019  Dygma Lab S.L.
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

#ifdef ARDUINO_SAMD_RAISE

#include <Arduino.h>
#include "Hand.h"

#include "kaleidoscope/driver/color/GammaCorrection.h"

namespace kaleidoscope {
namespace device {
namespace dygma {
namespace raise {

#define TWI_CMD_NONE 0x00
#define TWI_CMD_VERSION 0x01
#define TWI_CMD_KEYSCAN_INTERVAL 0x02
#define TWI_CMD_LED_SET_ALL_TO 0x03
#define TWI_CMD_LED_SET_ONE_TO 0x04
#define TWI_CMD_COLS_USE_PULLUPS 0x05
#define TWI_CMD_LED_SPI_FREQUENCY 0x06
#define TWI_CMD_LED_GLOBAL_BRIGHTNESS 0x07

#define TWI_CMD_SLED_STATUS 0x08
#define TWI_CMD_LED_OPEN 0x09
#define TWI_CMD_LED_SHORT 0x0A
#define TWI_CMD_JOINED 0x0B
#define TWI_CMD_LAYOUT 0x0C
#define TWI_CMD_SLED_CURRENT 0x0D
#define TWI_CMD_SLED_SELF_TEST 0x0E

#define LED_SPI_FREQUENCY_4MHZ      0x07
#define LED_SPI_FREQUENCY_2MHZ      0x06
#define LED_SPI_FREQUENCY_1MHZ      0x05
#define LED_SPI_FREQUENCY_512KHZ    0x04
#define LED_SPI_FREQUENCY_256KHZ    0x03
#define LED_SPI_FREQUENCY_128KHZ    0x02
#define LED_SPI_FREQUENCY_64KHZ     0x01
#define LED_SPI_OFF                 0x00

// 512KHZ seems to be the sweet spot in early testing
// so make it the default
#define LED_SPI_FREQUENCY_DEFAULT LED_SPI_FREQUENCY_512KHZ

#define TWI_CMD_LED_BASE 0x80

#define TWI_REPLY_NONE 0x00
#define TWI_REPLY_KEYDATA 0x01

#define ELEMENTS(arr)  (sizeof(arr) / sizeof((arr)[0]))

// Returns the relative controller addresss. The expected range is 0-3
uint8_t Hand::controllerAddress() {
  return ad01_;
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
byte Hand::setKeyscanInterval(byte delay) {
  uint8_t data[] = {TWI_CMD_KEYSCAN_INTERVAL, delay};
  return twi_.writeTo(data, ELEMENTS(data));
}

// returns -1 on error, otherwise returns the scanner version integer
int Hand::readVersion() {
  return readRegister(TWI_CMD_VERSION);
}

// returns -1 on error, otherwise returns the sled version integer
int Hand::readSLEDVersion() {
  return readRegister(TWI_CMD_SLED_STATUS);
}
// returns -1 on error, otherwise returns the sled current settings
int Hand::readSLEDCurrent() {
  return readRegister(TWI_CMD_SLED_CURRENT);
}

byte Hand::setSLEDCurrent(byte current) {
  uint8_t data[] = {TWI_CMD_SLED_CURRENT, current};
  return twi_.writeTo(data, ELEMENTS(data));
}

// returns -1 on error, otherwise returns the scanner keyscan interval
int Hand::readKeyscanInterval() {
  return readRegister(TWI_CMD_KEYSCAN_INTERVAL);
}

// returns -1 on error, otherwise returns the layout (ANSI/ISO) setting
int Hand::readLayout() {
  return readRegister(TWI_CMD_LAYOUT);
}

// returns -1 on error, otherwise returns the LED SPI Frequncy
int Hand::readLEDSPIFrequency() {
  return readRegister(TWI_CMD_LED_SPI_FREQUENCY);
}

// Set the LED SPI Frequency. See wire-protocol-constants.h for
// values.
//
// returns the Wire.endTransmission code (0 = success)
// https://www.arduino.cc/en/Reference/WireEndTransmission
byte Hand::setLEDSPIFrequency(byte frequency) {
  uint8_t data[] = {TWI_CMD_LED_SPI_FREQUENCY, frequency};
  return twi_.writeTo(data, ELEMENTS(data));
}

// returns -1 on error, otherwise returns the value of the hall sensor integer
int Hand::readJoint() {
  byte return_value = 0;

  uint8_t data[] = {TWI_CMD_JOINED};
  uint8_t result = twi_.writeTo(data, ELEMENTS(data));
  if (result != 0)
    return -1;

  // needs to be long enough for the slave to respond
  delayMicroseconds(40);

  uint8_t rxBuffer[2];

  // perform blocking read into buffer
  uint8_t read = twi_.readFrom(rxBuffer, ELEMENTS(rxBuffer));
  if (read == 2) {
    return rxBuffer[0] + (rxBuffer[1] << 8);
  } else {
    return -1;
  }
}

int Hand::readRegister(uint8_t cmd) {
  byte return_value = 0;

  uint8_t data[] = {cmd};
  uint8_t result = twi_.writeTo(data, ELEMENTS(data));
  if (result != 0)
    return -1;

  // needs to be long enough for the slave to respond
  delayMicroseconds(40);

  uint8_t rxBuffer[1];

  // perform blocking read into buffer
  uint8_t read = twi_.readFrom(rxBuffer, ELEMENTS(rxBuffer));
  if (read > 0) {
    return rxBuffer[0];
  } else {
    return -1;
  }
}

// gives information on the key that was just pressed or released.
bool Hand::readKeys() {
  uint8_t rxBuffer[6] = {0, 0, 0, 0, 0, 0};

  // perform blocking read into buffer
  uint8_t result = twi_.readFrom(rxBuffer, ELEMENTS(rxBuffer));
  // if result isn't 6? this can happens if slave nacks while trying to read
  Hand::online = (result == 6) ? true : false;

  if (result != 6)
    // could also try reset pressed keys here
    return false;

  if (rxBuffer[0] == TWI_REPLY_KEYDATA) {
    key_data_.rows[0] = rxBuffer[1];
    key_data_.rows[1] = rxBuffer[2];
    key_data_.rows[2] = rxBuffer[3];
    key_data_.rows[3] = rxBuffer[4];
    key_data_.rows[4] = rxBuffer[5];
    return true;
  } else {
    return false;
  }
}

keydata_t Hand::getKeyData() {
  return key_data_;
}

void Hand::sendLEDData() {
  sendLEDBank(next_led_bank_++);
  if (next_led_bank_ == LED_BANKS) {
    next_led_bank_ = 0;
  }
}

auto constexpr gamma8 = kaleidoscope::driver::color::gamma_correction;

void Hand::sendLEDBank(uint8_t bank) {
  uint8_t data[LED_BYTES_PER_BANK + 1]; // + 1 for the update LED command itself
  data[0]  = TWI_CMD_LED_BASE + bank;
  for (uint8_t i = 0 ; i < LED_BYTES_PER_BANK; i++) {
    data[i + 1] = pgm_read_byte(&gamma8[led_data.bytes[bank][i]]);
  }
  uint8_t result = twi_.writeTo(data, ELEMENTS(data));
}

void Hand::setAllLEDsTo(cRGB color) {
  uint8_t data[] = {TWI_CMD_LED_SET_ALL_TO,
                    pgm_read_byte(&gamma8[color.r]),
                    pgm_read_byte(&gamma8[color.g]),
                    pgm_read_byte(&gamma8[color.b])
                   };
  twi_.writeTo(data, ELEMENTS(data));
}

void Hand::setOneLEDTo(byte led, cRGB color) {
  uint8_t data[] = {TWI_CMD_LED_SET_ONE_TO,
                    led,
                    pgm_read_byte(&gamma8[color.r]),
                    pgm_read_byte(&gamma8[color.g]),
                    pgm_read_byte(&gamma8[color.b])
                   };
  twi_.writeTo(data, ELEMENTS(data));
}

}
}
}
}
#endif
