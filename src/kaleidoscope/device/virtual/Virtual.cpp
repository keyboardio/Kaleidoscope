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

#ifdef KALEIDOSCOPE_VIRTUAL_BUILD

#include "kaleidoscope/device/virtual/Virtual.h"

// From Arduino:
#include <EEPROM.h>  // for EEPROMClass, EERef
// From KeyboardioHID:
#include <HIDReportObserver.h>  // for HIDReportObserver
// From system:
#include <stdint.h>      // for uint8_t, uint16_t
#include <stdlib.h>      // for exit, size_t
#include <virtual_io.h>  // for getLineOfInput, isInte...
#include <sstream>       // for operator<<, string
#include <string>        // for operator==, char_traits

// From Kaleidoscope:
#include "kaleidoscope/KeyAddr.h"                                  // for MatrixAddr, MatrixAddr...
#include "kaleidoscope/device/virtual/DefaultHIDReportConsumer.h"  // for DefaultHIDReportConsumer
#include "kaleidoscope/device/virtual/Logging.h"                   // for log_error, logging
#include "kaleidoscope/key_defs.h"                                 // for Key_NoKey
#include "kaleidoscope/keyswitch_state.h"                          // for IS_PRESSED, WAS_PRESSED

// FIXME: This relates to virtual/cores/arduino/EEPROM.h.
//        EEPROM static data must be defined here as only
//        the device knows about the EEPROM size.
//
//        It might be better to move the EEPROM.h header here as
//        well???

uint8_t EERef::eeprom_[kaleidoscope::DeviceProps::StorageProps::length];

EEPROMClass EEPROM;

uint16_t EEPROMClass::length() {
  return kaleidoscope::DeviceProps::StorageProps::length;
}

namespace kaleidoscope {
namespace device {
namespace virt {

using namespace kaleidoscope::logging;  // NOLINT(build/namespaces)

//##############################################################################
// VirtualKeyScanner
//##############################################################################

VirtualKeyScanner::VirtualKeyScanner()
  : n_pressed_switches_{0},
    n_previously_pressed_switches_{0},
    read_matrix_enabled_{true} {
}

void VirtualKeyScanner::setup() {

  HIDReportObserver::resetHook(&DefaultHIDReportConsumer::processHIDReport);

  for (auto key_addr : KeyAddr::all()) {
    keystates_[key_addr.toInt()]      = KeyState::NotPressed;
    keystates_prev_[key_addr.toInt()] = KeyState::NotPressed;
  }
}

enum Mode {
  M_TAP,
  M_DOWN,
  M_UP,
};

// FIXME: getRCfromPhysicalKey only works for the Model01 with default
//        layout. We might want to remove this as the virtual hardware
//        intents to simulate any type of keyboard.
//
// static KeyAddr getRCfromPhysicalKey(std::string keyname) {
//   if (keyname == "prog") return KeyAddr{0, 0};
//   else if (keyname == "1") return KeyAddr{0, 1};
//   else if (keyname == "2") return KeyAddr{0, 2};
//   else if (keyname == "3") return KeyAddr{0, 3};
//   else if (keyname == "4") return KeyAddr{0, 4};
//   else if (keyname == "5") return KeyAddr{0, 5};
//   else if (keyname == "led") return KeyAddr{0, 6};
//   else if (keyname == "any") return KeyAddr{0, 9};
//   else if (keyname == "6") return KeyAddr{0, 10};
//   else if (keyname == "7") return KeyAddr{0, 11};
//   else if (keyname == "8") return KeyAddr{0, 12};
//   else if (keyname == "9") return KeyAddr{0, 13};
//   else if (keyname == "0") return KeyAddr{0, 14};
//   else if (keyname == "num") return KeyAddr{0, 15};
//   else if (keyname == "`") return KeyAddr{1, 0};
//   else if (keyname == "q") return KeyAddr{1, 1};
//   else if (keyname == "w") return KeyAddr{1, 2};
//   else if (keyname == "e") return KeyAddr{1, 3};
//   else if (keyname == "r") return KeyAddr{1, 4};
//   else if (keyname == "t") return KeyAddr{1, 5};
//   else if (keyname == "tab") return KeyAddr{1, 6};
//   else if (keyname == "enter") return KeyAddr{1, 9};
//   else if (keyname == "y") return KeyAddr{1, 10};
//   else if (keyname == "u") return KeyAddr{1, 11};
//   else if (keyname == "i") return KeyAddr{1, 12};
//   else if (keyname == "o") return KeyAddr{1, 13};
//   else if (keyname == "p") return KeyAddr{1, 14};
//   else if (keyname == "=") return KeyAddr{1, 15};
//   else if (keyname == "pgup") return KeyAddr{2, 0};
//   else if (keyname == "a") return KeyAddr{2, 1};
//   else if (keyname == "s") return KeyAddr{2, 2};
//   else if (keyname == "d") return KeyAddr{2, 3};
//   else if (keyname == "f") return KeyAddr{2, 4};
//   else if (keyname == "g") return KeyAddr{2, 5};
//   else if (keyname == "h") return KeyAddr{2, 10};
//   else if (keyname == "j") return KeyAddr{2, 11};
//   else if (keyname == "k") return KeyAddr{2, 12};
//   else if (keyname == "l") return KeyAddr{2, 13};
//   else if (keyname == ";") return KeyAddr{2, 14};
//   else if (keyname == "'") return KeyAddr{2, 15};
//   else if (keyname == "pgdn") return KeyAddr{3, 0};
//   else if (keyname == "z") return KeyAddr{3, 1};
//   else if (keyname == "x") return KeyAddr{3, 2};
//   else if (keyname == "c") return KeyAddr{3, 3};
//   else if (keyname == "v") return KeyAddr{3, 4};
//   else if (keyname == "b") return KeyAddr{3, 5};
//   else if (keyname == "esc") return KeyAddr{2, 6};   // yes, row 2
//   else if (keyname == "fly") return KeyAddr{2, 9};   // yes, row 2
//   else if (keyname == "n") return KeyAddr{3, 10};
//   else if (keyname == "m") return KeyAddr{3, 11};
//   else if (keyname == ",") return KeyAddr{3, 12};
//   else if (keyname == ".") return KeyAddr{3, 13};
//   else if (keyname == "/") return KeyAddr{3, 14};
//   else if (keyname == "-") return KeyAddr{3, 15};
//   else if (keyname == "lctrl") return KeyAddr{0, 7};
//   else if (keyname == "bksp") return KeyAddr{1, 7};
//   else if (keyname == "cmd") return KeyAddr{2, 7};
//   else if (keyname == "lshift") return KeyAddr{3, 7};
//   else if (keyname == "rshift") return KeyAddr{3, 8};
//   else if (keyname == "alt") return KeyAddr{2, 8};
//   else if (keyname == "space") return KeyAddr{1, 8};
//   else if (keyname == "rctrl") return KeyAddr{0, 8};
//   else if (keyname == "lfn") return KeyAddr{3, 6};
//   else if (keyname == "rfn") return KeyAddr{3, 9};
//
//   return KeyAddr{}; // Invalid key
// }

void VirtualKeyScanner::readMatrix() {

  if (!read_matrix_enabled_) return;

  std::stringstream sline;
  sline << getLineOfInput(anythingHeld());
  Mode mode = M_TAP;

  while (true) {
    std::string token;
    std::getline(sline, token, ' ');

    if (token == "") {
      break;  // end of line
    } else if (token == "#") {
      break;  // skip the rest of the line
    } else if ((token == "?" || token == "help") && isInteractive()) {
      printHelp();
    } else if (token == "Q") {
      exit(0);
    } else if (token == "T") {
      mode = M_TAP;
    } else if (token == "D") {
      mode = M_DOWN;
    } else if (token == "U") {
      mode = M_UP;
    } else if (token == "C") {
      for (auto key_addr : KeyAddr::all()) {
        keystates_[key_addr.toInt()] = KeyState::NotPressed;
      }
    } else {
      KeyAddr key_addr;

      if (token.front() == '(' && token.back() == ')') {
        size_t commapos = token.find_first_of(',');

        if (commapos == std::string::npos) {
          log_error("Bad (r,c) pair: %s\n", token.c_str());
          continue;
        } else {
          key_addr = KeyAddr(
            (uint8_t)std::stoi(token.substr(1, commapos - 1)),
            (uint8_t)std::stoi(token.substr(commapos + 1, token.length() - commapos - 1)));

          if (!key_addr.isValid()) {
            log_error("Bad coordinates: %s\n", token.c_str());
            continue;
          }
        }
      } else {
        // TODO(anyone): Is there a device independent
        //       way to determine KeyAddr from key names?
        //         key_addr = getRCfromPhysicalKey(token);
        //
        //         if (!key_addr.isValid()) {
        //           log_error("Unrecognized command: %s\n", token.c_str());
        //           continue;
        //         }
      }

      keystates_[key_addr.toInt()] =
        (mode == M_DOWN) ? KeyState::Pressed : (mode == M_UP) ? KeyState::NotPressed
                                                              : KeyState::Tap;
    }
  }
}
void VirtualKeyScanner::actOnMatrixScan() {

  n_pressed_switches_            = 0;
  n_previously_pressed_switches_ = 0;

  for (auto key_addr : KeyAddr::all()) {
    uint8_t key_state = 0;

    switch (keystates_prev_[key_addr.toInt()]) {
    case KeyState::Pressed:
      key_state |= WAS_PRESSED;
      ++n_previously_pressed_switches_;
      break;

    case KeyState::Tap:
      log_error("Error: assertion failed, keystates_prev_ should not be KeyState::Tap\n");
      break;

    case KeyState::NotPressed:
    default:
      /* do nothing */
      break;
    }

    switch (keystates_[key_addr.toInt()]) {
    case KeyState::Pressed:
    case KeyState::Tap:
      key_state |= IS_PRESSED;
      ++n_pressed_switches_;
      break;

    case KeyState::NotPressed:
    default:
      /* do nothing */
      break;
    }

    if (key_state != 0)
      handleKeyswitchEvent(Key_NoKey, key_addr, key_state);
    keystates_prev_[key_addr.toInt()] = keystates_[key_addr.toInt()];

    if (keystates_[key_addr.toInt()] == KeyState::Tap) {
      key_state = WAS_PRESSED & ~IS_PRESSED;
      handleKeyswitchEvent(Key_NoKey, key_addr, key_state);
      keystates_[key_addr.toInt()]      = KeyState::NotPressed;
      keystates_prev_[key_addr.toInt()] = KeyState::NotPressed;
    }
  }
}

uint8_t VirtualKeyScanner::pressedKeyswitchCount() const {
  return n_pressed_switches_;
}
bool VirtualKeyScanner::isKeyswitchPressed(KeyAddr key_addr) const {
  if (keystates_[key_addr.toInt()] == KeyState::NotPressed) {
    return false;
  }
  return true;
}

uint8_t VirtualKeyScanner::previousPressedKeyswitchCount() const {
  return n_previously_pressed_switches_;
}
bool VirtualKeyScanner::wasKeyswitchPressed(KeyAddr key_addr) const {
  if (keystates_prev_[key_addr.toInt()] == KeyState::NotPressed) {
    return false;
  }

  return true;
}

void VirtualKeyScanner::setKeystate(KeyAddr keyAddr, KeyState ks) {
  keystates_[keyAddr.toInt()] = ks;
}

VirtualKeyScanner::KeyState VirtualKeyScanner::getKeystate(KeyAddr keyAddr) const {
  return keystates_[keyAddr.toInt()];
}

bool VirtualKeyScanner::anythingHeld() {
  for (auto key_addr : KeyAddr::all()) {
    if (keystates_[key_addr.toInt()] == KeyState::Pressed) return true;
  }

  return false;
}

//##############################################################################
// VirtualLEDDriver
//##############################################################################

void VirtualLEDDriver::setup() {
  for (int i = 0; i < led_count; i++) {
    led_states_[i] = CRGB(0, 0, 0);
  }
}

void VirtualLEDDriver::syncLeds() {
  // log format: red.green.blue where values are written in hex; followed by a space, followed by the next LED
  std::stringstream ss;
  ss << std::hex;

  for (int i = 0; i < led_count; i++) {
    const cRGB state = led_states_[i];
    ss << (unsigned int)state.r << "." << (unsigned int)state.g << "." << (unsigned int)state.b << " ";
  }

  ss << std::endl;
  logLEDStates(ss.str());
}

void VirtualLEDDriver::setCrgbAt(uint8_t i, cRGB color) {
  if (static_cast<int>(i) >= static_cast<int>(led_count)) {
    log_error("Virtual::setCrgbAt: Index %d out of bounds\n", i);
    return;
  }
  led_states_[i] = color;
}

cRGB VirtualLEDDriver::getCrgbAt(uint8_t i) const {
  if (static_cast<int>(i) >= static_cast<int>(led_count)) {
    log_error("Virtual::getCrgbAt: Index %d out of bounds\n", i);
    return CRGB(0, 0, 0);
  }
  return led_states_[i];
}

}  // namespace virt
}  // namespace device

}  // namespace kaleidoscope


#endif  // ifdef KALEIDOSCOPE_VIRTUAL_BUILD
