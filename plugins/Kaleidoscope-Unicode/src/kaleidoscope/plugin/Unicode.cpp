/* -*- mode: c++ -*-
 * Kaleidoscope-Unicode -- Unicode input helpers
 * Copyright (C) 2016, 2017, 2018  Keyboard.io, Inc
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

#include "kaleidoscope/plugin/Unicode.h"

#include <Arduino.h>              // for delay
#include <Kaleidoscope-HostOS.h>  // for HostOS, LINUX, MACOS, WINDOWS, OSX
#include <stdint.h>               // for uint8_t, uint32_t, int8_t

#include "kaleidoscope/Runtime.h"                         // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"                   // for Base<>::HID, VirtualProps::HID
#include "kaleidoscope/driver/hid/keyboardio/Keyboard.h"  // for Keyboard
#include "kaleidoscope/key_defs.h"                        // for Key, Key_LeftAlt, KEY_FLAGS, Key_A

namespace kaleidoscope {
namespace plugin {

uint8_t Unicode::input_delay_;
Key Unicode::linux_key_ = Key_U;

void Unicode::start() {
  switch (::HostOS.os()) {
  case hostos::LINUX:
    kaleidoscope::Runtime.hid().keyboard().pressRawKey(Key_LeftControl);
    kaleidoscope::Runtime.hid().keyboard().pressRawKey(Key_LeftShift);
    kaleidoscope::Runtime.hid().keyboard().pressRawKey(linux_key_);
    kaleidoscope::Runtime.hid().keyboard().sendReport();
    kaleidoscope::Runtime.hid().keyboard().releaseRawKey(Key_LeftControl);
    kaleidoscope::Runtime.hid().keyboard().releaseRawKey(Key_LeftShift);
    kaleidoscope::Runtime.hid().keyboard().releaseRawKey(linux_key_);
    kaleidoscope::Runtime.hid().keyboard().sendReport();
    break;
  case hostos::WINDOWS:
    kaleidoscope::Runtime.hid().keyboard().pressRawKey(Key_LeftAlt);
    kaleidoscope::Runtime.hid().keyboard().sendReport();
    kaleidoscope::Runtime.hid().keyboard().pressRawKey(Key_KeypadAdd);
    kaleidoscope::Runtime.hid().keyboard().sendReport();
    kaleidoscope::Runtime.hid().keyboard().releaseRawKey(Key_KeypadAdd);
    kaleidoscope::Runtime.hid().keyboard().sendReport();
    break;
  case hostos::MACOS:
    kaleidoscope::Runtime.hid().keyboard().pressRawKey(Key_LeftAlt);
    break;
  default:
    unicodeCustomStart();
    break;
  }
}

void Unicode::input() {
  switch (::HostOS.os()) {
  case hostos::LINUX:
    break;
  case hostos::WINDOWS:
  case hostos::MACOS:
    kaleidoscope::Runtime.hid().keyboard().pressRawKey(Key_LeftAlt);
    break;
  default:
    unicodeCustomInput();
    break;
  }
  delay(input_delay_);
}

void Unicode::end() {
  switch (::HostOS.os()) {
  case hostos::LINUX:
    kaleidoscope::Runtime.hid().keyboard().pressRawKey(Key_Spacebar);
    kaleidoscope::Runtime.hid().keyboard().sendReport();
    kaleidoscope::Runtime.hid().keyboard().releaseRawKey(Key_Spacebar);
    kaleidoscope::Runtime.hid().keyboard().sendReport();
    break;
  case hostos::WINDOWS:
  case hostos::MACOS:
    kaleidoscope::Runtime.hid().keyboard().releaseRawKey(Key_LeftAlt);
    kaleidoscope::Runtime.hid().keyboard().sendReport();
    break;
  default:
    unicodeCustomEnd();
    break;
  }
}

void Unicode::typeCode(uint32_t unicode) {
  bool on_zero_start = true;
  for (int8_t i = 7; i >= 0; i--) {
    if (i <= 3) {
      on_zero_start = false;
    }
    uint8_t digit = ((unicode >> (i * 4)) & 0xF);
    if (digit == 0) {
      if (on_zero_start == false) {
        Key key;
        if (::HostOS.os() != hostos::OSX) {
          key = hexToKeysWithNumpad(digit);
        } else {
          key = hexToKey(digit);
        }
        input();
        kaleidoscope::Runtime.hid().keyboard().pressRawKey(key);
        kaleidoscope::Runtime.hid().keyboard().sendReport();
        input();
        kaleidoscope::Runtime.hid().keyboard().releaseRawKey(key);
        kaleidoscope::Runtime.hid().keyboard().sendReport();
      }
    } else {
      Key key;
      if (::HostOS.os() != hostos::OSX) {
        key = hexToKeysWithNumpad(digit);
      } else {
        key = hexToKey(digit);
      }
      input();
      kaleidoscope::Runtime.hid().keyboard().pressRawKey(key);
      kaleidoscope::Runtime.hid().keyboard().sendReport();
      input();
      kaleidoscope::Runtime.hid().keyboard().releaseRawKey(key);
      kaleidoscope::Runtime.hid().keyboard().sendReport();
      on_zero_start = false;
    }
    delay(5);
  }
}

void Unicode::type(uint32_t unicode) {
  start();
  typeCode(unicode);
  end();
}

}  // namespace plugin
}  // namespace kaleidoscope

__attribute__((weak)) Key hexToKey(uint8_t hex) {
  uint8_t m;
  if (hex == 0x0) {
    return Key_0;
  }
  if (hex < 0xA) {
    m = Key_1.getKeyCode() + (hex - 0x1);
  } else {
    m = Key_A.getKeyCode() + (hex - 0xA);
  }
  return {m, KEY_FLAGS};
}

__attribute__((weak)) Key hexToKeysWithNumpad(uint8_t hex) {
  uint8_t m;
  if (hex == 0x0) {
    return Key_Keypad0;
  }
  if (hex < 0xA) {
    m = Key_Keypad1.getKeyCode() + (hex - 0x1);
  } else {
    switch (hex) {
    case 0xA:
      m = Key_A.getKeyCode();
      break;
    case 0xB:
      m = Key_B.getKeyCode();
      break;
    case 0xC:
      m = Key_C.getKeyCode();
      break;
    case 0xD:
      m = Key_D.getKeyCode();
      break;
    case 0xE:
      m = Key_E.getKeyCode();
      break;
    case 0xF:
      m = Key_F.getKeyCode();
      break;
    default:
      m = Key_NoKey.getKeyCode();
      break;
    }
  }
  return {m, KEY_FLAGS};
}

__attribute__((weak)) void unicodeCustomStart() {
}

__attribute__((weak)) void unicodeCustomEnd() {
}

__attribute__((weak)) void unicodeCustomInput() {
}

kaleidoscope::plugin::Unicode Unicode;
