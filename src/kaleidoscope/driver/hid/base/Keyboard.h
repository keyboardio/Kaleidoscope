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

#include <stdint.h>  // for uint8_t

#include "kaleidoscope/key_defs.h"  // for Key, Key_LeftAlt, Key_LeftControl, Key_LeftGui, Key_L...

#ifndef HID_BOOT_PROTOCOL
#define HID_BOOT_PROTOCOL 0
#endif

namespace kaleidoscope {
namespace driver {
namespace hid {
namespace base {

class NoBootKeyboard {
 public:
  NoBootKeyboard() {}
  void begin() {}

  uint8_t getProtocol() {
    return 1;
  }
  void setProtocol(uint8_t protocol) {}
  void setDefaultProtocol(uint8_t protocol) {}

  void sendReport() {}

  void press(uint8_t code) {}
  void release(uint8_t code) {}
  void releaseAll() {}

  bool isModifierActive(uint8_t code) {
    return false;
  }
  bool wasModifierActive(uint8_t code) {
    return false;
  }
  bool isAnyModifierActive() {
    return false;
  }
  bool wasAnyModifierActive() {
    return false;
  }
  bool isKeyPressed(uint8_t code) {
    return false;
  }

  uint8_t getLeds() {
    return 0;
  }

  void onUSBReset() {}
};

class NoNKROKeyboard {
 public:
  NoNKROKeyboard() {}
  void begin() {}

  void sendReport() {}

  void press(uint8_t code) {}
  void release(uint8_t code) {}
  void releaseAll() {}

  bool isModifierActive(Key key) {
    return false;
  }
  bool wasModifierActive(Key key) {
    return false;
  }
  bool isAnyModifierActive() {
    return false;
  }
  bool wasAnyModifierActive() {
    return false;
  }
  bool isKeyPressed(uint8_t code) {
    return false;
  }

  uint8_t getLeds() {
    return 0;
  }
};

class NoConsumerControl {
 public:
  NoConsumerControl() {}
  void begin() {}

  void sendReport() {}
  void releaseAll() {}

  void press(uint8_t code) {}
  void release(uint8_t code) {}
};

class NoSystemControl {
 public:
  NoSystemControl() {}
  void begin() {}

  void press(uint8_t code) {}
  void release() {}
};

struct KeyboardProps {
  typedef NoBootKeyboard BootKeyboard;
  typedef NoNKROKeyboard NKROKeyboard;
  typedef NoConsumerControl ConsumerControl;
  typedef NoSystemControl SystemControl;
};

template<typename _Props>
class Keyboard {
 private:
  typename _Props::BootKeyboard boot_keyboard_;
  typename _Props::NKROKeyboard nkro_keyboard_;
  typename _Props::ConsumerControl consumer_control_;
  typename _Props::SystemControl system_control_;

 public:
  Keyboard() {}

  void setup() __attribute__((noinline)) {
    boot_keyboard_.begin();
    nkro_keyboard_.begin();
    consumer_control_.begin();
    system_control_.begin();
  }

  void sendReport() __attribute__((noinline)) {
    if (boot_keyboard_.getProtocol() == HID_BOOT_PROTOCOL) {
      boot_keyboard_.sendReport();
      return;
    }
    nkro_keyboard_.sendReport();
    consumer_control_.sendReport();
  }
  void releaseAllKeys() __attribute__((noinline)) {
    if (boot_keyboard_.getProtocol() == HID_BOOT_PROTOCOL) {
      boot_keyboard_.releaseAll();
    } else {
      nkro_keyboard_.releaseAll();
      consumer_control_.releaseAll();
    }
  }
  void pressConsumerControl(Key mapped_key) {
    consumer_control_.press(CONSUMER(mapped_key));
  }
  void releaseConsumerControl(Key mapped_key) {
    consumer_control_.release(CONSUMER(mapped_key));
  }
  void pressSystemControl(Key mapped_key) {
    uint8_t keycode = mapped_key.getKeyCode();
    system_control_.press(keycode);
    last_system_control_keycode_ = keycode;
  }
  void releaseSystemControl(Key mapped_key) {
    uint8_t keycode = mapped_key.getKeyCode();
    if (keycode == last_system_control_keycode_) {
      system_control_.release();
    }
  }

  void pressKey(Key pressed_key) __attribute__((noinline)) {
    pressModifiers(pressed_key);
    pressRawKey(pressed_key);
  }

  void pressModifiers(Key pressed_key) {
    pressModifiers(pressed_key.getFlags());
  }
  void releaseModifiers(Key released_key) {
    releaseModifiers(released_key.getFlags());
  }
  void clearModifiers() {
    releaseRawKey(Key_LeftShift);
    releaseRawKey(Key_LeftControl);
    releaseRawKey(Key_LeftAlt);
    releaseRawKey(Key_RightAlt);
    releaseRawKey(Key_LeftGui);
  }

  // pressRawKey takes a Key object and calles KeyboardioHID's ".press" method
  // with its keycode. It does no processing of any flags or modifiers on the key
  void pressRawKey(Key pressed_key) {
    if (boot_keyboard_.getProtocol() == HID_BOOT_PROTOCOL) {
      boot_keyboard_.press(pressed_key.getKeyCode());
      return;
    }

    nkro_keyboard_.press(pressed_key.getKeyCode());
  }

  void releaseRawKey(Key released_key) {
    if (boot_keyboard_.getProtocol() == HID_BOOT_PROTOCOL) {
      boot_keyboard_.release(released_key.getKeyCode());
      return;
    }

    nkro_keyboard_.release(released_key.getKeyCode());
  }

  void releaseKey(Key released_key) {
    releaseModifiers(released_key.getFlags());
    releaseRawKey(released_key);
  }

  bool isKeyPressed(Key key) {
    if (boot_keyboard_.getProtocol() == HID_BOOT_PROTOCOL) {
      return boot_keyboard_.isKeyPressed(key.getKeyCode());
    }
    return nkro_keyboard_.isKeyPressed(key.getKeyCode());
  }

  bool isModifierKeyActive(Key modifier_key) {
    if (boot_keyboard_.getProtocol() == HID_BOOT_PROTOCOL) {
      return boot_keyboard_.isModifierActive(modifier_key.getKeyCode());
    }

    return nkro_keyboard_.isModifierActive(modifier_key.getKeyCode());
  }

  bool wasModifierKeyActive(Key modifier_key) {
    if (boot_keyboard_.getProtocol() == HID_BOOT_PROTOCOL) {
      return boot_keyboard_.wasModifierActive(modifier_key.getKeyCode());
    }

    return nkro_keyboard_.wasModifierActive(modifier_key.getKeyCode());
  }

  bool isAnyModifierKeyActive() {
    if (boot_keyboard_.getProtocol() == HID_BOOT_PROTOCOL) {
      return boot_keyboard_.isAnyModifierActive();
    }

    return nkro_keyboard_.isAnyModifierActive();
  }

  bool wasAnyModifierKeyActive() {
    if (boot_keyboard_.getProtocol() == HID_BOOT_PROTOCOL) {
      return boot_keyboard_.wasAnyModifierActive();
    }

    return nkro_keyboard_.wasAnyModifierActive();
  }

  uint8_t getKeyboardLEDs() {
    if (boot_keyboard_.getProtocol() == HID_BOOT_PROTOCOL) {
      return boot_keyboard_.getLeds();
    }

    return nkro_keyboard_.getLeds();
  }

  uint8_t getProtocol() {
    return boot_keyboard_.getProtocol();
  }
  void setProtocol(uint8_t protocol) {
    boot_keyboard_.setProtocol(protocol);
  }
  void setDefaultProtocol(uint8_t protocol) {
    boot_keyboard_.setDefaultProtocol(protocol);
  }

  void onUSBReset() {
    boot_keyboard_.onUSBReset();
  }

 private:
  // To prevent premature release of a System Control key when rolling
  // over from one to another, we record the last System Control
  // keycode that was pressed. It's initialized to zero, which should
  // not be a valid System Control keycode.
  uint8_t last_system_control_keycode_ = 0;

  // pressModifiers takes a bitmap of modifier keys that must be included in
  // the upcoming USB HID report and passes them through to KeyboardioHID
  // immediately

  void pressModifiers(uint8_t flags) {
    if (flags & SHIFT_HELD) {
      pressRawKey(Key_LeftShift);
    }
    if (flags & CTRL_HELD) {
      pressRawKey(Key_LeftControl);
    }
    if (flags & LALT_HELD) {
      pressRawKey(Key_LeftAlt);
    }
    if (flags & RALT_HELD) {
      pressRawKey(Key_RightAlt);
    }
    if (flags & GUI_HELD) {
      pressRawKey(Key_LeftGui);
    }
  }

  // releaseModifiers takes a bitmap of modifier keys that must not be included in
  // the upcoming USB HID report and passes them through to KeyboardioHID
  // immediately

  void releaseModifiers(uint8_t flags) {
    if (flags & SHIFT_HELD) {
      releaseRawKey(Key_LeftShift);
    }
    if (flags & CTRL_HELD) {
      releaseRawKey(Key_LeftControl);
    }
    if (flags & LALT_HELD) {
      releaseRawKey(Key_LeftAlt);
    }
    if (flags & RALT_HELD) {
      releaseRawKey(Key_RightAlt);
    }
    if (flags & GUI_HELD) {
      releaseRawKey(Key_LeftGui);
    }
  }
};

}  // namespace base
}  // namespace hid
}  // namespace driver
}  // namespace kaleidoscope
