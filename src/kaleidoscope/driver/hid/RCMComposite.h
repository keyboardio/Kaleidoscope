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

#include "kaleidoscope/driver/hid/Base.h"

#include "rcmcomposite/Keyboard.h"
#include "rcmcomposite/Mouse.h"

namespace kaleidoscope {
namespace driver {
namespace hid {

namespace rcmcomposite {

extern USBHID RCMHID;
extern USBCompositeSerial CompositeSerial;

const uint8_t report_description_[] = {
  HID_KEYBOARD_REPORT_DESCRIPTOR(),
  HID_CONSUMER_REPORT_DESCRIPTOR(),
  HID_MOUSE_REPORT_DESCRIPTOR()
};

}

struct RCMCompositeProps: public BaseProps {
  typedef rcmcomposite::KeyboardProps KeyboardProps;
  typedef rcmcomposite::Keyboard<KeyboardProps> Keyboard;
  typedef rcmcomposite::MouseProps MouseProps;
  typedef rcmcomposite::Mouse<MouseProps> Mouse;
};

template <typename _Props>
class RCMComposite: public Base<_Props> {
 public:
  void setup() {
    rcmcomposite::RCMHID.begin(rcmcomposite::CompositeSerial,
                               rcmcomposite::report_description_,
                               sizeof(rcmcomposite::report_description_));
    while(!USBComposite);

    rcmcomposite::RCMBootKeyboard.begin();
  }
};

} // namespace hid
} // namespace driver
} // namespace kaleidoscope
