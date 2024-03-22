/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Model101 -- Keyboardio Model101 hardware support for Kaleidoscope
 * Copyright (C) 2017-2024  Keyboard.io, Inc
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

#ifdef ARDUINO_NRF52840_ITSYBITSY

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
#include "kaleidoscope/driver/bootloader/Base.h"
#include "kaleidoscope/driver/hid/Base.h"
#include "kaleidoscope/driver/hid/TinyUSB.h"
#include "kaleidoscope/driver/keyscanner/Simple.h"
#include "kaleidoscope/driver/led/Base.h"
#include "kaleidoscope/driver/mcu/TinyUSB.h"
#include "kaleidoscope/driver/storage/Base.h"


namespace kaleidoscope {
namespace device {
namespace keyboardio {

struct Model101StorageProps : public kaleidoscope::driver::storage::BaseProps {
  static constexpr uint16_t length = 16384;
};


struct Model101KeyScannerProps : public kaleidoscope::driver::keyscanner::SimpleProps {

  static constexpr uint8_t matrix_rows    = 4;
  static constexpr uint8_t matrix_columns = 16;
  typedef MatrixAddr<matrix_rows, matrix_columns> KeyAddr;
  static constexpr uint8_t matrix_row_pins[matrix_rows]    = {A0, A1, A2, A3};
  static constexpr uint8_t matrix_col_pins[matrix_columns] = {A4, A5, SCK, MOSI, MISO, 2, 0, 1, 21, 22, 7, 9, 10, 11, 12, 13};
};


// If we need to override HID props:
struct Model101HIDProps : public kaleidoscope::driver::hid::TinyUSBProps {
  //typedef kaleidoscope::driver::hid::base::AbsoluteMouseProps AbsoluteMouseProps;
  //typedef kaleidoscope::driver::hid::base::AbsoluteMouse<AbsoluteMouseProps> AbsoluteMouse;
};


struct Model101Props : public kaleidoscope::device::BaseProps {
  typedef Model101HIDProps HIDProps;
  typedef kaleidoscope::driver::hid::TinyUSB<HIDProps> HID;

  typedef kaleidoscope::driver::led::BaseProps LEDDriverProps;
  typedef kaleidoscope::driver::led::Base<LEDDriverProps> LEDDriver;

  typedef Model101KeyScannerProps KeyScannerProps;
  typedef kaleidoscope::driver::keyscanner::Simple<KeyScannerProps> KeyScanner;

  typedef Model101StorageProps StorageProps;
  typedef kaleidoscope::driver::storage::Base<StorageProps> Storage;

  typedef kaleidoscope::driver::bootloader::Base Bootloader;
  static constexpr const char *short_name = "kbio101";

  typedef kaleidoscope::driver::mcu::TinyUSBProps MCUProps;
  typedef kaleidoscope::driver::mcu::TinyUSB<MCUProps> MCU;
};

class Model101 : public kaleidoscope::device::Base<Model101Props> {
 public:
  auto serialPort() -> decltype(Serial) & {
    return Serial;
  }
  static void rebootBootloader() {}
};

}  // namespace keyboardio
}  // namespace device

EXPORT_DEVICE(kaleidoscope::device::keyboardio::Model101)

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
