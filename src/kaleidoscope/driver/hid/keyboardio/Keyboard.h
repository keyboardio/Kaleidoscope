// -*- mode: c++ -*-
/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2019  Keyboard.io, Inc.
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

#include <KeyboardioHID.h>  // for BootKeyboard, BootKeyboard_, Keyboard
#include <stdint.h>         // for uint8_t, uint16_t

// From Kaleidoscope:
#include "kaleidoscope/driver/hid/base/Keyboard.h"  // for Keyboard, KeyboardProps

#if !BOOTKB_HYBRID
#error "This version of Kaleidoscope requires KeyboardioHID with BOOTKB_HYBRID"
#error "Please update your KeyboardioHID"
#endif

namespace kaleidoscope {
namespace driver {
namespace hid {
namespace keyboardio {

/*
 * We are wrapping a few keyboard-related objects here, instead of directly
 * using the classes in `KeyboardProps` below. We do this, because this lets the
 * linker optimize this whole thing out if it is unused. It can do that because
 * instantiating `Keyboard_` & co is in a separate compilation unit.
 *
 * While it would have been cleaner and shorter to instantiate them here, and
 * drop the global objects, that prevents optimizing them out, and that's a cost
 * we do not want to pay.
 */

class BootKeyboardWrapper {
 public:
  BootKeyboardWrapper() {}
  void begin() {
    BootKeyboard().begin();
  }

  uint8_t getProtocol() {
    return BootKeyboard().getProtocol();
  }

  uint8_t getBootOnly() {
    return BootKeyboard().getBootOnly();
  }
  void setBootOnly(uint8_t bootonly) {
    BootKeyboard().setBootOnly(bootonly);
  }

  void sendReport() {
    BootKeyboard().sendReport();
  }

  void press(uint8_t code) {
    BootKeyboard().press(code);
  }
  void release(uint8_t code) {
    BootKeyboard().release(code);
  }
  void releaseAll() {
    BootKeyboard().releaseAll();
  }

  bool isKeyPressed(uint8_t code) {
    return BootKeyboard().isKeyPressed(code);
  }
  bool isModifierActive(uint8_t code) {
    return BootKeyboard().isModifierActive(code);
  }
  bool wasModifierActive(uint8_t code) {
    return BootKeyboard().wasModifierActive(code);
  }
  bool isAnyModifierActive() {
    return BootKeyboard().isAnyModifierActive();
  }
  bool wasAnyModifierActive() {
    return BootKeyboard().wasAnyModifierActive();
  }

  uint8_t getLeds() {
    return BootKeyboard().getLeds();
  }

  void onUSBReset() {
    BootKeyboard().onUSBReset();
  }
};

class ConsumerControlWrapper {
 public:
  ConsumerControlWrapper() {}
  void begin() {
    ConsumerControl.begin();
  }

  void sendReport() {
    ConsumerControl.sendReport();
  }
  void releaseAll() {
    ConsumerControl.releaseAll();
  }

  void press(uint16_t code) {
    ConsumerControl.press(code);
  }
  void release(uint16_t code) {
    ConsumerControl.release(code);
  }
};

class SystemControlWrapper {
 public:
  SystemControlWrapper() {}
  void begin() {
    SystemControl.begin();
  }

  void press(uint8_t code) {
    SystemControl.press(code);
  }
  void release() {
    SystemControl.release();
  }
};

struct KeyboardProps : public base::KeyboardProps {
  typedef BootKeyboardWrapper BootKeyboard;
  typedef ConsumerControlWrapper ConsumerControl;
  typedef SystemControlWrapper SystemControl;
};

template<typename _Props>
class Keyboard : public base::Keyboard<_Props> {};

}  // namespace keyboardio
}  // namespace hid
}  // namespace driver
}  // namespace kaleidoscope
