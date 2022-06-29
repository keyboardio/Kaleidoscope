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

#pragma once

#ifdef ARDUINO_SAMD_RAISE
#include <Arduino.h>

#include "kaleidoscope/device/dygma/raise/RaiseSide.h"

#define CRGB(r, g, b) \
  (cRGB) { b, g, r }

#include "kaleidoscope/device/Base.h"
#include "kaleidoscope/driver/bootloader/samd/Bossac.h"
#include "kaleidoscope/driver/hid/Keyboardio.h"
#include "kaleidoscope/driver/keyscanner/Base.h"
#include "kaleidoscope/driver/led/Base.h"
#include "kaleidoscope/driver/storage/Flash.h"
#include "kaleidoscope/util/flasher/KeyboardioI2CBootloader.h"

namespace kaleidoscope {
namespace device {
namespace dygma {

// LHK = Left Hand Keys
#define LHK 33

using kaleidoscope::driver::led::no_led;

struct RaiseLEDDriverProps : public kaleidoscope::driver::led::BaseProps {
  static constexpr uint8_t led_count = 132;
  // clang-format off
  static constexpr uint8_t key_led_map[] = {
    // ISO & ANSI (ANSI has no LED at 20, but this key can never be pressed so we can have just one map).
    0,  1,  2,  3,  4,  5,      6,      no_led,        no_led,  6 + LHK,  5 + LHK,  4 + LHK,  3 + LHK,  2 + LHK,  1 + LHK,  0 + LHK,
    7,  8,  9,  10, 11, 12,     no_led, no_led,      14 + LHK, 13 + LHK, 12 + LHK, 11 + LHK, 10 + LHK,  9 + LHK,  8 + LHK,  7 + LHK,
    13, 14, 15, 16, 17, 18,     no_led, no_led,        no_led, 21 + LHK, 20 + LHK, 19 + LHK, 18 + LHK, 17 + LHK, 16 + LHK, 15 + LHK,
    19, 20, 21, 22, 23, 24,     25,     no_led,        no_led,   no_led, 27 + LHK, 26 + LHK, 25 + LHK, 24 + LHK, 23 + LHK, 22 + LHK,
    26, 27, 28, 29, 30, no_led, 31,     32,          35 + LHK, 34 + LHK, 33 + LHK, 32 + LHK, 31 + LHK, 30 + LHK, 29 + LHK, 28 + LHK
  };  // clang-format on
};
#undef LHK

class RaiseLEDDriver : public kaleidoscope::driver::led::Base<RaiseLEDDriverProps> {
 public:
  static void setup();

  static void syncLeds();
  static void setCrgbAt(uint8_t i, cRGB crgb);
  static cRGB getCrgbAt(uint8_t i);
  static void setBrightness(uint8_t brightness);
  static uint8_t getBrightness();

  static void updateNeuronLED();

 private:
  static bool isLEDChangedNeuron;
  static uint8_t isLEDChangedLeft[LED_BANKS];
  static uint8_t isLEDChangedRight[LED_BANKS];
  static cRGB neuronLED;

  static constexpr uint8_t lph = LEDS_PER_HAND;
  // led_count + 1, to account for the Neuron's LED. The last one is the
  // Neuron's LED, never send that to SLED.
  static constexpr uint8_t led_map[][RaiseLEDDriverProps::led_count + 1] = {
    // clang-format off
    // ISO
    {
      // left side - 33 keys includes LP
      0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 68, 69,

      // right side - 36 keys includes LP
      0 + LPH, 1 + LPH, 2 + LPH, 3 + LPH, 4 + LPH, 5 + LPH, 6 + LPH, 7 + LPH, 8 + LPH, 9 + LPH, 10 + LPH, 11 + LPH, 12 + LPH, 13 + LPH, 14 + LPH, 15 + LPH, 16 + LPH, 17 + LPH, 18 + LPH, 19 + LPH,
      20 + LPH, 21 + LPH, 22 + LPH, 23 + LPH, 24 + LPH, 25 + LPH, 26 + LPH, 27 + LPH, 28 + LPH, 29 + LPH, 30 + LPH, 31 + LPH, 32 + LPH, 33 + LPH, 68 + LPH, 69 + LPH,

      // left under glow - 30
      34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,

      // right underglow - 32
      34 + LPH, 35 + LPH, 36 + LPH, 37 + LPH, 38 + LPH, 39 + LPH, 40 + LPH, 41 + LPH, 42 + LPH, 43 + LPH, 44 + LPH, 45 + LPH, 46 + LPH, 47 + LPH, 48 + LPH, 49 + LPH, 50 + LPH, 51 + LPH,
      52 + LPH, 53 + LPH, 54 + LPH, 55 + LPH, 56 + LPH, 57 + LPH, 58 + LPH, 59 + LPH, 60 + LPH, 61 + LPH, 62 + LPH, 63 + LPH, 64 + LPH, 65 + LPH, 0xff
    },
    // ANSI
    {
      // left side - 32 keys includes LP: key 19 is missing for ANSI layout
      0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 20, 0xff, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 68, 69,

      // right side - 36 keys includes LP
      0 + LPH, 1 + LPH, 2 + LPH, 3 + LPH, 4 + LPH, 5 + LPH, 6 + LPH, 15 + LPH, 8 + LPH, 9 + LPH, 10 + LPH, 11 + LPH, 12 + LPH, 13 + LPH, 14 + LPH, 7 + LPH, 16 + LPH, 17 + LPH, 18 + LPH, 19 + LPH,
      20 + LPH, 21 + LPH, 22 + LPH, 23 + LPH, 24 + LPH, 25 + LPH, 26 + LPH, 27 + LPH, 28 + LPH, 29 + LPH, 30 + LPH, 31 + LPH, 32 + LPH, 33 + LPH, 68 + LPH, 69 + LPH,

      // left under glow - 30
      34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,

      // right underglow - 32
      34 + LPH, 35 + LPH, 36 + LPH, 37 + LPH, 38 + LPH, 39 + LPH, 40 + LPH, 41 + LPH, 42 + LPH, 43 + LPH, 44 + LPH, 45 + LPH, 46 + LPH, 47 + LPH, 48 + LPH, 49 + LPH, 50 + LPH, 51 + LPH,
      52 + LPH, 53 + LPH, 54 + LPH, 55 + LPH, 56 + LPH, 57 + LPH, 58 + LPH, 59 + LPH, 60 + LPH, 61 + LPH, 62 + LPH, 63 + LPH, 64 + LPH, 65 + LPH, 0xff
    }
  };  // clang-format on
};

struct RaiseKeyScannerProps : public kaleidoscope::driver::keyscanner::BaseProps {
  static constexpr uint8_t matrix_rows    = 5;
  static constexpr uint8_t matrix_columns = 16;
  typedef MatrixAddr<matrix_rows, matrix_columns> KeyAddr;

  static constexpr uint8_t left_columns  = 8;
  static constexpr uint8_t right_columns = matrix_columns - left_columns;
};

class RaiseKeyScanner : public kaleidoscope::driver::keyscanner::Base<RaiseKeyScannerProps> {
 private:
  typedef RaiseKeyScanner ThisType;
  typedef RaiseKeyScannerProps Props_;

 public:
  static void setup();
  static void scanMatrix();
  static void readMatrix();
  static void actOnMatrixScan();

  static bool isKeyswitchPressed(KeyAddr key_addr);
  static uint8_t pressedKeyswitchCount();

  static bool wasKeyswitchPressed(KeyAddr key_addr);
  static uint8_t previousPressedKeyswitchCount();

  static void setKeyscanInterval(uint8_t interval);

  static void reset();

 protected:
  static raise::keydata_t leftHandState;
  static raise::keydata_t rightHandState;
  static raise::keydata_t previousLeftHandState;
  static raise::keydata_t previousRightHandState;

  static bool lastLeftOnline;
  static bool lastRightOnline;
};

struct RaiseStorageProps : public kaleidoscope::driver::storage::FlashProps {
  static constexpr uint16_t length = EEPROM_EMULATION_SIZE;
};

struct RaiseSideFlasherProps : public kaleidoscope::util::flasher::BaseProps {};

struct RaiseProps : kaleidoscope::device::BaseProps {
  typedef kaleidoscope::driver::hid::KeyboardioProps HIDProps;
  typedef kaleidoscope::driver::hid::Keyboardio<HIDProps> HID;
  typedef RaiseLEDDriverProps LEDDriverProps;
  typedef RaiseLEDDriver LEDDriver;
  typedef RaiseKeyScannerProps KeyScannerProps;
  typedef RaiseKeyScanner KeyScanner;
  typedef RaiseStorageProps StorageProps;
  typedef kaleidoscope::driver::storage::Flash<StorageProps> Storage;
  typedef kaleidoscope::driver::bootloader::samd::Bossac Bootloader;

  typedef RaiseSideFlasherProps SideFlasherProps;
  typedef kaleidoscope::util::flasher::KeyboardioI2CBootloader<SideFlasherProps> SideFlasher;
  static constexpr const char *short_name = "raise";
};

class Raise : public kaleidoscope::device::Base<RaiseProps> {
 private:
  static RaiseProps::SideFlasher SideFlasher;

 public:
  static void setup();

  auto serialPort() -> decltype(SerialUSB) & {
    return SerialUSB;
  }

  auto sideFlasher() -> decltype(SideFlasher) & {
    return SideFlasher;
  }

  struct side {
    uint8_t getPower();
    void setPower(uint8_t power);

    uint8_t leftVersion();
    uint8_t rightVersion();

    uint8_t leftCRCErrors();
    uint8_t rightCRCErrors();

    uint8_t leftSLEDVersion();
    uint8_t rightSLEDVersion();

    uint8_t leftSLEDCurrent();
    uint8_t rightSLEDCurrent();
    void setSLEDCurrent(uint8_t current);

    void prepareForFlash();

    // Side bootloader addresses
    static constexpr uint8_t left_boot_address  = 0x50;
    static constexpr uint8_t right_boot_address = 0x51;
  } side;

  struct settings {
    enum class Layout {
      ISO,
      ANSI
    };
    Layout layout();
    uint8_t joint();

    uint16_t keyscanInterval();
    void keyscanInterval(uint16_t interval);
  } settings;
};

}  // namespace dygma
}  // namespace device

typedef kaleidoscope::device::dygma::Raise Device;

}  // namespace kaleidoscope

// clang-format off

#define PER_KEY_DATA(dflt,                                                                                  \
  r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6,                r0c9,  r0c10, r0c11, r0c12, r0c13, r0c14, r0c15, \
  r1c0, r1c1, r1c2, r1c3, r1c4, r1c5,               r1c8,  r1c9,  r1c10, r1c11, r1c12, r1c13, r1c14, r1c15, \
  r2c0, r2c1, r2c2, r2c3, r2c4, r2c5,                      r2c9,  r2c10, r2c11, r2c12, r2c13, r2c14, r2c15, \
  r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r3c6,                       r3c10, r3c11, r3c12, r3c13, r3c14, r3c15, \
  r4c0, r4c1, r4c2, r4c3, r4c4,                                   r4c10, r4c11, r4c12, r4c13, r4c14, r4c15, \
                          r4c6, r4c7,                             r4c8,  r4c9                               \
  )                                                                                                         \
                                                                                                            \
    r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6, dflt, dflt, r0c9, r0c10, r0c11, r0c12, r0c13, r0c14, r0c15,   \
    r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, dflt, dflt, r1c8, r1c9, r1c10, r1c11, r1c12, r1c13, r1c14, r1c15,   \
    r2c0, r2c1, r2c2, r2c3, r2c4, r2c5, dflt, dflt, dflt, r2c9, r2c10, r2c11, r2c12, r2c13, r2c14, r2c15,   \
    r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r3c6, dflt, dflt, dflt, r3c10, r3c11, r3c12, r3c13, r3c14, r3c15,   \
    r4c0, r4c1, r4c2, r4c3, r4c4, dflt, r4c6, r4c7, r4c8, r4c9, r4c10, r4c11, r4c12, r4c13, r4c14, r4c15


#define PER_KEY_DATA_STACKED(dflt,                          \
  r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6,                 \
  r1c0, r1c1, r1c2, r1c3, r1c4, r1c5,                       \
  r2c0, r2c1, r2c2, r2c3, r2c4, r2c5,                       \
  r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r3c6,                 \
  r4c0, r4c1, r4c2, r4c3, r4c4,                             \
                          r4c6, r4c7,                       \
                                                            \
         r0c9,  r0c10, r0c11, r0c12, r0c13, r0c14, r0c15,   \
  r1c8,  r1c9,  r1c10, r1c11, r1c12, r1c13, r1c14, r1c15,   \
         r2c9,  r2c10, r2c11, r2c12, r2c13, r2c14, r2c15,   \
                r3c10, r3c11, r3c12, r3c13, r3c14, r3c15,   \
                r4c10, r4c11, r4c12, r4c13, r4c14, r4c15,   \
                r4c8,  r4c9                                 \
  )                                                         \
                                                            \
    r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6, dflt, dflt, r0c9, r0c10, r0c11, r0c12, r0c13, r0c14, r0c15,   \
    r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, dflt, dflt, r1c8, r1c9, r1c10, r1c11, r1c12, r1c13, r1c14, r1c15,   \
    r2c0, r2c1, r2c2, r2c3, r2c4, r2c5, dflt, dflt, dflt, r2c9, r2c10, r2c11, r2c12, r2c13, r2c14, r2c15,   \
    r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r3c6, dflt, dflt, dflt, r3c10, r3c11, r3c12, r3c13, r3c14, r3c15,   \
    r4c0, r4c1, r4c2, r4c3, r4c4, dflt, r4c6, r4c7, r4c8, r4c9, r4c10, r4c11, r4c12, r4c13, r4c14, r4c15

#endif
