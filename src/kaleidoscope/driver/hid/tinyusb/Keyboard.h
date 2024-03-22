// -*- mode: c++ -*-
/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2024  Keyboard.io, Inc.
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

#ifdef USE_TINYUSB

#include <stdint.h>  // for uint8_t, uint16_t
#include "Adafruit_TinyUSB.h"

#include "kaleidoscope/driver/hid/apis/BootKeyboardAPI.h"

// From Kaleidoscope:
#include "kaleidoscope/driver/hid/base/Keyboard.h"  // for Keyboard, KeyboardProps
#include "kaleidoscope/driver/hid/tinyusb/AbsoluteMouse.h"
#include "kaleidoscope/driver/hid/tinyusb/MultiReport.h"

namespace kaleidoscope {
namespace driver {
namespace hid {
namespace tinyusb {

extern "C" {
/* TinyUSB callback on SetReport */
void tud_hid_set_protocol_cb(uint8_t instance, uint8_t protocol);

void boot_keyboard_set_report_cb(uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize);
}

extern class BootKeyboard_ &BootKeyboard();

class BootKeyboard_ : public BootKeyboardAPI, public HIDD {
 public:
  explicit BootKeyboard_(uint8_t bootkb_only_ = 0);
  void begin() {
    (void)HIDD::begin();
    BootKeyboardAPI::begin();
    inited = true;
  }
  void sendReport() {
    BootKeyboardAPI::sendReport();
  }
  void onUSBReset() override {}
  uint8_t getProtocol() override {
    return HIDD::getProtocol();
  }
  uint8_t getLeds() {
    return leds;
  }
  void setBootOnly(uint8_t bootonly) {
    BootKeyboardAPI::setBootOnly(bootonly);
    /*
     * Rebuild configuration descriptors, so the correct report descriptor
     * length ends up in the HID descriptor when switching to or from
     * boot-only.
     */
    if (!inited) {
      /*
       * Don't try to clear configuration before begin() has run, or it will
       * scramble some Adafruit_USBD_HID state.
       */
      return;
    }
    /*
     * This calls TinyUSBDevice.clearConfiguration() and also resets the string
     * descriptor. Calling clearConfiguration() directly leaves a dangling
     * string descriptor index.
     */
    Serial.end();
    /*
     * This calls TinyUSBDevice.addInterface(), adding the CDC descriptors
     * back. The baud rate unfortunately has to be hardcoded here.
     */
    Serial.begin(115200);
    TinyUSBDevice.addInterface(*this);
    TinyUSBDevice.addInterface(TUSBMultiReport());
    TinyUSBDevice.addInterface(TUSBAbsoluteMouse());
  }

 protected:
  int SendHIDReport(const void *data, int len) override {
    if (HIDD::sendReport(0, data, len)) {
      return len;
    } else {
      return -1;
    }
  }
  void setReportDescriptor(uint8_t bootkb_only) override;

  static uint8_t leds;

  friend void boot_keyboard_set_report_cb(uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize);

 private:
  bool inited = false;
};


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

struct KeyboardProps : public base::KeyboardProps {
  typedef BootKeyboardWrapper BootKeyboard;
  typedef TUSBConsumerControl ConsumerControl;
  typedef TUSBSystemControl SystemControl;
};

template<typename _Props>
class Keyboard : public base::Keyboard<_Props> {};

}  // namespace tinyusb
}  // namespace hid
}  // namespace driver
}  // namespace kaleidoscope

#endif
