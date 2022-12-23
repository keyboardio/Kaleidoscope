/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Model100u -- Keyboardio Model100 hardware support for Kaleidoscope
 * Copyright (C) 2017-2021  Keyboard.io, Inc
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

#ifdef ARDUINO_keyboardio_model_100u

#include <Arduino.h>

#define CRGB(r, g, b) \
  (cRGB) {            \
    b, g, r           \
  }

struct cRGB {
  uint8_t b;
  uint8_t g;
  uint8_t r;
};


#include "kaleidoscope/device/Base.h"
#include "kaleidoscope/driver/bootloader/gd32/Base.h"
#include "kaleidoscope/driver/hid/Base.h"
#include "kaleidoscope/driver/hid/Keyboardio.h"
#include "kaleidoscope/driver/keyscanner/Base.h"
#include "kaleidoscope/driver/led/Base.h"
#include "kaleidoscope/driver/mcu/GD32.h"
#include "kaleidoscope/driver/storage/GD32Flash.h"


namespace kaleidoscope {
namespace device {
namespace keyboardio {

struct Model100uStorageProps : public kaleidoscope::driver::storage::GD32FlashProps {
  static constexpr uint16_t length = 16384;
};


struct Model100uKeyScannerProps : public kaleidoscope::driver::keyscanner::BaseProps {
  static constexpr uint8_t matrix_rows    = 4;
  static constexpr uint8_t matrix_columns = 16;
  typedef MatrixAddr<matrix_rows, matrix_columns> KeyAddr;
};

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
class Model100uKeyScanner : public kaleidoscope::driver::keyscanner::Base<Model100uKeyScannerProps> {
 private:
  typedef Model100uKeyScanner ThisType;

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

 protected:
};
#else   // ifndef KALEIDOSCOPE_VIRTUAL_BUILD
class Model100uKeyScanner;
#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD


// If we need to override HID props:
struct Model100uHIDProps : public kaleidoscope::driver::hid::KeyboardioProps {
  //typedef kaleidoscope::driver::hid::base::AbsoluteMouseProps AbsoluteMouseProps;
  //typedef kaleidoscope::driver::hid::base::AbsoluteMouse<AbsoluteMouseProps> AbsoluteMouse;
};


struct Model100uProps : public kaleidoscope::device::BaseProps {
  typedef Model100uHIDProps HIDProps;
  typedef kaleidoscope::driver::hid::Keyboardio<HIDProps> HID;

  typedef Model100uKeyScannerProps KeyScannerProps;
  typedef Model100uKeyScanner KeyScanner;

  typedef Model100uStorageProps StorageProps;
  typedef kaleidoscope::driver::storage::GD32Flash<StorageProps> Storage;

  typedef kaleidoscope::driver::bootloader::gd32::Base Bootloader;
  static constexpr const char *short_name = "kbio100u";

  typedef kaleidoscope::driver::mcu::GD32Props MCUProps;
  typedef kaleidoscope::driver::mcu::GD32<MCUProps> MCU;
};

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD

class Model100u : public kaleidoscope::device::Base<Model100uProps> {
 public:
  void setup();

  auto serialPort() -> decltype(Serial) & {
    return Serial;
  }
  static void rebootBootloader();
  static void enableHardwareTestMode();
};

#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

}  // namespace keyboardio
}  // namespace device

EXPORT_DEVICE(kaleidoscope::device::keyboardio::Model100u)

}  // namespace kaleidoscope

// clang-format off

#define PER_KEY_DATA_STACKED(dflt,                                    \
               r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6,                \
               r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6,                \
               r2c0, r2c1, r2c2, r2c3, r2c4, r2c5,                      \
               r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r2c6,                \
               r0c7, r1c7, r2c7, r3c7,                                  \
               r3c6,                                                    \
                                                                        \
               r0c9,  r0c10, r0c11, r0c12, r0c13, r0c14, r0c15,         \
               r1c9,  r1c10, r1c11, r1c12, r1c13, r1c14, r1c15,         \
                      r2c10, r2c11, r2c12, r2c13, r2c14, r2c15,         \
               r2c9,  r3c10, r3c11, r3c12, r3c13, r3c14, r3c15,         \
               r3c8,  r2c8,  r1c8, r0c8,                                \
               r3c9, ...)                                               \
                                                                        \
    r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6, r0c7, r0c8, r0c9, r0c10, r0c11, r0c12, r0c13, r0c14, r0c15, \
    r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6, r1c7, r1c8, r1c9, r1c10, r1c11, r1c12, r1c13, r1c14, r1c15, \
    r2c0, r2c1, r2c2, r2c3, r2c4, r2c5, r2c6, r2c7, r2c8, r2c9, r2c10, r2c11, r2c12, r2c13, r2c14, r2c15, \
    r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r3c6, r3c7, r3c8, r3c9, r3c10, r3c11, r3c12, r3c13, r3c14, RESTRICT_ARGS_COUNT((r3c15), 64, KEYMAP_STACKED, ##__VA_ARGS__)

#define PER_KEY_DATA(dflt,                                                                                 \
  r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6,        r0c9,  r0c10, r0c11, r0c12, r0c13, r0c14, r0c15, \
  r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6,        r1c9,  r1c10, r1c11, r1c12, r1c13, r1c14, r1c15, \
  r2c0, r2c1, r2c2, r2c3, r2c4, r2c5,                     r2c10, r2c11, r2c12, r2c13, r2c14, r2c15, \
  r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r2c6,        r2c9,  r3c10, r3c11, r3c12, r3c13, r3c14, r3c15, \
              r0c7, r1c7, r2c7, r3c7,                             r3c8,  r2c8,  r1c8, r0c8,         \
                          r3c6,                                          r3c9, ...)                      \
    r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6, r0c7, r0c8, r0c9, r0c10, r0c11, r0c12, r0c13, r0c14, r0c15, \
    r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6, r1c7, r1c8, r1c9, r1c10, r1c11, r1c12, r1c13, r1c14, r1c15, \
    r2c0, r2c1, r2c2, r2c3, r2c4, r2c5, r2c6, r2c7, r2c8, r2c9, r2c10, r2c11, r2c12, r2c13, r2c14, r2c15, \
    r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r3c6, r3c7, r3c8, r3c9, r3c10, r3c11, r3c12, r3c13, r3c14, RESTRICT_ARGS_COUNT((r3c15), 64, KEYMAP, ##__VA_ARGS__)

#endif
