/* -*- mode: c++ -*-
 * kaleidoscope::device::dygma::Raise -- Kaleidoscope device plugin for Dygma Raise
 * Copyright (C) 2017-2020  Keyboard.io, Inc
 * Copyright (C) 2017-2020  Dygma Lab S.L.
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

#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-LEDControl.h>
#include <KeyboardioHID.h>
#include <Wire.h>

#include "kaleidoscope/Runtime.h"
#include "kaleidoscope/driver/color/GammaCorrection.h"
#include "kaleidoscope/driver/keyscanner/Base_Impl.h"
#include "kaleidoscope/util/crc16.h"

#define I2C_CLOCK_KHZ       100
#define I2C_FLASH_CLOCK_KHZ 100  // flashing doesn't work reliably at higher clock speeds

#define SIDE_POWER          1  // side power switch pa10

#define LAYOUT_ISO          0
#define LAYOUT_ANSI         1

namespace kaleidoscope {
namespace device {
namespace dygma {

/********* RaiseHands *********/

struct RaiseHands {
  static raise::RaiseSide leftHand;
  static raise::RaiseSide rightHand;

  static void setup();
  static void initializeSides();

  static uint8_t layout;

  static void setSidePower(bool power);
  static bool getSidePower() {
    return side_power_;
  }

  static void keyscanInterval(uint16_t interval);
  static uint16_t keyscanInterval() {
    return keyscan_interval_;
  }

 private:
  static uint16_t keyscan_interval_;
  static bool side_power_;
  static uint16_t settings_base_;
  static constexpr uint8_t iso_only_led_ = 19;
};

raise::RaiseSide RaiseHands::leftHand(0);
raise::RaiseSide RaiseHands::rightHand(1);
uint8_t RaiseHands::layout;
bool RaiseHands::side_power_;
uint16_t RaiseHands::settings_base_;
uint16_t RaiseHands::keyscan_interval_ = 50;

void RaiseHands::setSidePower(bool power) {
  digitalWrite(SIDE_POWER, power ? HIGH : LOW);
  side_power_ = power;
}

void RaiseHands::setup() {
  settings_base_ = ::EEPROMSettings.requestSlice(sizeof(keyscan_interval_));

  // If keyscan is max, assume that EEPROM is uninitialized, and store the
  // defaults.
  uint16_t interval;
  Runtime.storage().get(settings_base_, interval);
  if (interval == 0xffff) {
    Runtime.storage().put(settings_base_, keyscan_interval_);
    Runtime.storage().commit();
  }
  Runtime.storage().get(settings_base_, keyscan_interval_);
}

void RaiseHands::keyscanInterval(uint16_t interval) {
  leftHand.setKeyscanInterval(interval);
  rightHand.setKeyscanInterval(interval);
  keyscan_interval_ = interval;
  Runtime.storage().put(settings_base_, keyscan_interval_);
  Runtime.storage().commit();
}

void RaiseHands::initializeSides() {
  // key scan interval from eeprom
  leftHand.setKeyscanInterval(keyscan_interval_);
  rightHand.setKeyscanInterval(keyscan_interval_);

  // get ANSI/ISO at every side replug
  uint8_t l_layout = leftHand.readLayout();
  uint8_t r_layout = rightHand.readLayout();

  // setup layout variable, this will affect led mapping - defaults to ISO if
  // nothing reported
  // FIXME
  if (l_layout == 1 || r_layout == 1)
    layout = 1;
  else
    layout = 0;

  /*
   * if the neuron starts up with no sides connected, it will assume ISO. This
   * turns on an extra LED (hardware LED 19 on left side). If an ANSI left is
   * then plugged in, the keyboard will switch to ANSI, but LED 19 can't get
   * wiped because the ANSI LED map doesn't include this LED. It will be driven
   * from the SLED1735's memory with the same colour as before, which causes
   * weird looking colours to come on on other seemingly unrelated keys. So: on
   * a replug, set LED 19 to off to be safe.
   */
  leftHand.led_data.leds[iso_only_led_] = {0, 0, 0};

  // get activated LED plugin to refresh
  ::LEDControl.refreshAll();
}

/********* LED Driver *********/

bool RaiseLEDDriver::isLEDChangedNeuron;
uint8_t RaiseLEDDriver::isLEDChangedLeft[LED_BANKS];
uint8_t RaiseLEDDriver::isLEDChangedRight[LED_BANKS];
cRGB RaiseLEDDriver::neuronLED;
constexpr uint8_t RaiseLEDDriver::led_map[][RaiseLEDDriverProps::led_count + 1];

constexpr uint8_t RaiseLEDDriverProps::key_led_map[];

void RaiseLEDDriver::setBrightness(uint8_t brightness) {
  RaiseHands::leftHand.setBrightness(brightness);
  RaiseHands::rightHand.setBrightness(brightness);
  for (uint8_t i = 0; i < LED_BANKS; i++) {
    isLEDChangedLeft[i]  = true;
    isLEDChangedRight[i] = true;
  }
}

uint8_t RaiseLEDDriver::getBrightness() {
  return RaiseHands::leftHand.getBrightness();
}

void RaiseLEDDriver::syncLeds() {
  // left and right sides
  for (uint8_t i = 0; i < LED_BANKS; i++) {
    // only send the banks that have changed - try to improve jitter performance
    if (isLEDChangedLeft[i]) {
      RaiseHands::leftHand.sendLEDBank(i);
      isLEDChangedLeft[i] = false;
    }
    if (isLEDChangedRight[i]) {
      RaiseHands::rightHand.sendLEDBank(i);
      isLEDChangedRight[i] = false;
    }
  }

  if (isLEDChangedNeuron) {
    updateNeuronLED();
    isLEDChangedNeuron = false;
  }
}

void RaiseLEDDriver::updateNeuronLED() {
  static constexpr struct {
    uint8_t r, g, b;
  } pins                = {3, 5, 4};
  auto constexpr gamma8 = kaleidoscope::driver::color::gamma_correction;

  // invert as these are common anode, and make sure we reach 65535 to be able
  // to turn fully off.
  analogWrite(pins.r, ((256 - pgm_read_byte(&gamma8[neuronLED.r])) << 8) - 1);
  analogWrite(pins.g, ((256 - pgm_read_byte(&gamma8[neuronLED.g])) << 8) - 1);
  analogWrite(pins.b, ((256 - pgm_read_byte(&gamma8[neuronLED.b])) << 8) - 1);
}

void RaiseLEDDriver::setCrgbAt(uint8_t i, cRGB crgb) {
  // prevent reading off the end of the led_map array
  if (i >= Props_::led_count)
    return;

  // neuron LED
  if (i == Props_::led_count - 1) {
    isLEDChangedNeuron |= !(neuronLED.r == crgb.r &&
                            neuronLED.g == crgb.g &&
                            neuronLED.b == crgb.b);
    neuronLED = crgb;
    return;
  }

  // get the SLED index
  uint8_t sled_num = led_map[RaiseHands::layout][i];
  if (sled_num < LEDS_PER_HAND) {
    cRGB oldColor                                = RaiseHands::leftHand.led_data.leds[sled_num];
    RaiseHands::leftHand.led_data.leds[sled_num] = crgb;
    isLEDChangedLeft[uint8_t(sled_num / 8)] |= !(oldColor.r == crgb.r &&
                                                 oldColor.g == crgb.g &&
                                                 oldColor.b == crgb.b);
  } else if (sled_num < 2 * LEDS_PER_HAND) {
    cRGB oldColor                                                 = RaiseHands::rightHand.led_data.leds[sled_num - LEDS_PER_HAND];
    RaiseHands::rightHand.led_data.leds[sled_num - LEDS_PER_HAND] = crgb;
    isLEDChangedRight[uint8_t((sled_num - LEDS_PER_HAND) / 8)] |=
      !(oldColor.r == crgb.r &&
        oldColor.g == crgb.g &&
        oldColor.b == crgb.b);
  } else {
    // TODO(anyone):
    // how do we want to handle debugging assertions about crazy user
    // code that would overwrite other memory?
  }
}

cRGB RaiseLEDDriver::getCrgbAt(uint8_t i) {
  if (i >= Props_::led_count)
    return {0, 0, 0};

  uint8_t sled_num = led_map[RaiseHands::layout][i];
  if (sled_num < LEDS_PER_HAND) {
    return RaiseHands::leftHand.led_data.leds[sled_num];
  } else if (sled_num < 2 * LEDS_PER_HAND) {
    return RaiseHands::rightHand.led_data.leds[sled_num - LEDS_PER_HAND];
  } else {
    return {0, 0, 0};
  }
}

void RaiseLEDDriver::setup() {
  pinMode(SIDE_POWER, OUTPUT);
  RaiseHands::setSidePower(false);

  // arduino zero analogWrite(255) isn't fully on as its actually working with a
  // 16bit counter and the mapping is a bit shift.
  // so change to 16 bit resolution to avoid the mapping and do the mapping
  // ourselves in updateHubleLED() to ensure LEDs can be set fully off
  analogWriteResolution(16);
  updateNeuronLED();

  delay(10);
  RaiseHands::setSidePower(true);
  delay(500);  // wait for sides to power up and finish bootloader
}

/********* Key scanner *********/

raise::keydata_t RaiseKeyScanner::leftHandState;
raise::keydata_t RaiseKeyScanner::rightHandState;
raise::keydata_t RaiseKeyScanner::previousLeftHandState;
raise::keydata_t RaiseKeyScanner::previousRightHandState;
bool RaiseKeyScanner::lastLeftOnline;
bool RaiseKeyScanner::lastRightOnline;

void RaiseKeyScanner::readMatrix() {
  previousLeftHandState  = leftHandState;
  previousRightHandState = rightHandState;

  if (RaiseHands::leftHand.readKeys()) {
    leftHandState = RaiseHands::leftHand.getKeyData();
    // if ANSI, then swap r3c0 and r3c1 to match the PCB
    if (RaiseHands::layout == LAYOUT_ANSI) {
      // only swap if bits are different
      if ((leftHandState.rows[3] & (1 << 0)) ^ leftHandState.rows[3] & (1 << 1)) {
        leftHandState.rows[3] ^= (1 << 0);  // flip the bit
        leftHandState.rows[3] ^= (1 << 1);  // flip the bit
      }
    }
  }

  if (RaiseHands::rightHand.readKeys()) {
    rightHandState = RaiseHands::rightHand.getKeyData();
    // if ANSI, then swap r1c0 and r2c0 to match the PCB
    if (RaiseHands::layout == LAYOUT_ANSI) {
      if ((rightHandState.rows[1] & (1 << 0)) ^ rightHandState.rows[2] & (1 << 0)) {
        rightHandState.rows[1] ^= (1 << 0);
        rightHandState.rows[2] ^= (1 << 0);
      }
    }
  }

  // if a side has just been replugged, initialise it
  if ((RaiseHands::leftHand.online && !lastLeftOnline) ||
      (RaiseHands::rightHand.online && !lastRightOnline))
    RaiseHands::initializeSides();

  // if a side has just been unplugged, wipe its state
  if (!RaiseHands::leftHand.online && lastLeftOnline)
    leftHandState.all = 0;

  if (!RaiseHands::rightHand.online && lastRightOnline)
    rightHandState.all = 0;

  // store previous state of whether the sides are plugged in
  lastLeftOnline  = RaiseHands::leftHand.online;
  lastRightOnline = RaiseHands::rightHand.online;
}

void RaiseKeyScanner::actOnMatrixScan() {
  for (uint8_t row = 0; row < Props_::matrix_rows; row++) {
    for (uint8_t col = 0; col < Props_::left_columns; col++) {
      uint8_t keynum = (row * Props_::left_columns) + col;
      uint8_t keyState;

      // left
      keyState = (bitRead(previousLeftHandState.all, keynum) << 0) |
                 (bitRead(leftHandState.all, keynum) << 1);
      if (keyState)
        ThisType::handleKeyswitchEvent(Key_NoKey, KeyAddr(row, col), keyState);

      // right
      keyState = (bitRead(previousRightHandState.all, keynum) << 0) |
                 (bitRead(rightHandState.all, keynum) << 1);
      if (keyState)
        ThisType::handleKeyswitchEvent(Key_NoKey, KeyAddr(row, (Props_::matrix_columns - 1) - col), keyState);
    }
  }
}

void RaiseKeyScanner::scanMatrix() {
  readMatrix();
  actOnMatrixScan();
}

bool RaiseKeyScanner::isKeyswitchPressed(KeyAddr key_addr) {
  auto row = key_addr.row();
  auto col = key_addr.col();

  if (col >= Props_::left_columns) {
    return (bitRead(rightHandState.rows[row], (Props_::matrix_columns - 1) - col) != 0);
  } else {
    return (bitRead(leftHandState.rows[row], col) != 0);
  }
}

bool RaiseKeyScanner::wasKeyswitchPressed(KeyAddr key_addr) {
  auto row = key_addr.row();
  auto col = key_addr.col();

  if (col >= Props_::left_columns) {
    return (bitRead(previousRightHandState.rows[row], (Props_::matrix_columns - 1) - col) != 0);
  } else {
    return (bitRead(previousLeftHandState.rows[row], col) != 0);
  }
}

uint8_t RaiseKeyScanner::pressedKeyswitchCount() {
  return __builtin_popcountll(leftHandState.all) + __builtin_popcountll(rightHandState.all);
}

uint8_t RaiseKeyScanner::previousPressedKeyswitchCount() {
  return __builtin_popcountll(previousLeftHandState.all) + __builtin_popcountll(previousRightHandState.all);
}

void RaiseKeyScanner::setKeyscanInterval(uint8_t interval) {
  RaiseHands::leftHand.setKeyscanInterval(interval);
  RaiseHands::rightHand.setKeyscanInterval(interval);
}

void RaiseKeyScanner::setup() {
  // clang-format off
  static constexpr uint8_t keyscanner_pins[] = {
    2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, 26, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
  };  // clang-format on
  for (int i = 0; i < sizeof(keyscanner_pins); i++) {
    pinMode(keyscanner_pins[i], OUTPUT);
    digitalWrite(keyscanner_pins[i], LOW);
  }
}

void RaiseKeyScanner::reset() {
  leftHandState.all  = 0;
  rightHandState.all = 0;
  Runtime.hid().keyboard().releaseAllKeys();
  Runtime.hid().keyboard().sendReport();
}

/********* Hardware plugin *********/
void Raise::setup() {
  RaiseHands::setup();
  KeyScanner::setup();
  LEDDriver::setup();

  // initialise Wire of scanner - have to do this here to avoid problem with
  // static object intialisation ordering
  Wire.begin();
  Wire.setClock(I2C_CLOCK_KHZ * 1000);

  RaiseHands::initializeSides();
}

void Raise::side::prepareForFlash() {
  Wire.end();

  setPower(LOW);
  // also turn off i2c pins to stop attiny from getting enough current through i2c to stay on
  pinMode(SCL, OUTPUT);
  pinMode(SDA, OUTPUT);
  digitalWrite(SCL, false);
  digitalWrite(SDA, false);

  // wipe key states, to prevent accidental key repeats
  RaiseKeyScanner::reset();

  setPower(HIGH);

  Wire.begin();
  Wire.setClock(I2C_FLASH_CLOCK_KHZ * 1000);
  // wait for side bootloader to be ready
  delay(100);
}

uint8_t Raise::side::getPower() {
  return RaiseHands::getSidePower();
}
void Raise::side::setPower(uint8_t power) {
  RaiseHands::setSidePower(power);
}

uint8_t Raise::side::leftVersion() {
  return RaiseHands::leftHand.readVersion();
}
uint8_t Raise::side::rightVersion() {
  return RaiseHands::rightHand.readVersion();
}

uint8_t Raise::side::leftCRCErrors() {
  return RaiseHands::leftHand.crc_errors();
}
uint8_t Raise::side::rightCRCErrors() {
  return RaiseHands::rightHand.crc_errors();
}

uint8_t Raise::side::leftSLEDVersion() {
  return RaiseHands::leftHand.readSLEDVersion();
}
uint8_t Raise::side::rightSLEDVersion() {
  return RaiseHands::rightHand.readSLEDVersion();
}

uint8_t Raise::side::leftSLEDCurrent() {
  return RaiseHands::leftHand.readSLEDCurrent();
}
uint8_t Raise::side::rightSLEDCurrent() {
  return RaiseHands::rightHand.readSLEDCurrent();
}
void Raise::side::setSLEDCurrent(uint8_t current) {
  RaiseHands::rightHand.setSLEDCurrent(current);
  RaiseHands::leftHand.setSLEDCurrent(current);
}

Raise::settings::Layout Raise::settings::layout() {
  return RaiseHands::layout == LAYOUT_ANSI ? Layout::ANSI : Layout::ISO;
}
uint8_t Raise::settings::joint() {
  return RaiseHands::rightHand.readJoint();
}

uint16_t Raise::settings::keyscanInterval() {
  return RaiseHands::keyscanInterval();
}
void Raise::settings::keyscanInterval(uint16_t interval) {
  RaiseHands::keyscanInterval(interval);
}

}  // namespace dygma
}  // namespace device
}  // namespace kaleidoscope

#endif
