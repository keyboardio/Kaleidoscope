// -*- mode: c++ -*-
/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2021  Keyboard.io, Inc.
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

#include <Arduino.h>
#include <USBComposite.h>

#include "kaleidoscope/driver/hid/base/Keyboard.h"

namespace kaleidoscope {
namespace driver {
namespace hid {
namespace rcmcomposite {

extern HIDKeyboard RCMKeyboard;
extern HIDConsumer RCMConsumer;

class KeyboardWrapper {
 public:
  KeyboardWrapper() {}

  void begin() {}

  uint8_t getProtocol() {
    return 1;
  }
  void setProtocol(uint8_t protocol) {}
  void setDefaultProtocol(uint8_t protocol) {}

  void sendReport() {
    RCMKeyboard.sendReport();
  }

  void press(uint8_t code) {
    RCMKeyboard.press(code);
  }
  void release(uint8_t code) {
    RCMKeyboard.release(code);
  }
  void releaseAll() {
    RCMKeyboard.releaseAll();
  }

  bool isKeyPressed(uint8_t code) {
    return RCMKeyboard.isKeyPressed(code);
  }
  bool wasKeyPressed(uint8_t code) {
    return RCMKeyboard.wasKeyPressed(code);
  }
  bool isModifierActive(uint8_t code) {
    return RCMKeyboard.isModifierActive(code);
  }
  bool wasModifierActive(uint8_t code) {
    return RCMKeyboard.wasModifierActive(code);
  }
  bool isAnyModifierActive() {
    return RCMKeyboard.isAnyModifierActive();
  }
  bool wasAnyModifierActive() {
    return RCMKeyboard.wasAnyModifierActive();
  }

  uint8_t getLeds() {
    return RCMKeyboard.getLEDs();
  }
};

class ConsumerControlWrapper {
 public:
  ConsumerControlWrapper() {}

  void begin() {
    RCMConsumer.begin();
  }

  void sendReport() {
  }
  void releaseAll() {
    RCMConsumer.release();
  }

  void press(uint16_t code) {
    RCMConsumer.press(code);
  }
  void release(uint16_t code) {
    RCMConsumer.release();
  }
};

struct KeyboardProps: public base::KeyboardProps {
  typedef KeyboardWrapper NKROKeyboard;
  typedef ConsumerControlWrapper ConsumerControl;
};

template <typename _Props>
class Keyboard: public base::Keyboard<_Props> {};

} // namespace rcmcomposite
} // namespace hid
} // namespace driver
} // namespace keyboardio
