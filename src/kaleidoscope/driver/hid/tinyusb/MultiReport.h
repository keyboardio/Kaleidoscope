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

#include <stdint.h>  // for uint8_t, int8_t, uint16_t

#include "Adafruit_TinyUSB.h"
#include "kaleidoscope/driver/hid/Base.h"
#include "kaleidoscope/driver/hid/base/Keyboard.h"
#include "kaleidoscope/driver/hid/apis/ConsumerControlAPI.h"
#include "kaleidoscope/driver/hid/apis/MouseAPI.h"
#include "kaleidoscope/driver/hid/apis/SystemControlAPI.h"
#include "HIDD.h"

// IWYU pragma: no_include "DeviceAPIs/AbsoluteMouseAPI.hpp"

namespace kaleidoscope {
namespace driver {
namespace hid {
namespace tinyusb {

enum MultiReportIDs {
  RID_CONSUMER_CONTROL = 1,
  RID_MOUSE,
  RID_SYSTEM_CONTROL,
};

class TUSBMultiReport_ : public HIDD {
 public:
  TUSBMultiReport_();
  void sendReport(uint8_t report_id, const void *data, uint8_t len) {
    (void)HIDD::sendReport(report_id, data, len);
  }
};

extern TUSBMultiReport_ &TUSBMultiReport();

class TUSBConsumerControl : public ConsumerControlAPI {
 public:
  TUSBConsumerControl() {
    (void)TUSBMultiReport();
  }
  void begin() {
    (void)TUSBMultiReport().begin();
    ConsumerControlAPI::begin();
  }

 protected:
  void sendReportUnchecked() {
    TUSBMultiReport().sendReport(RID_CONSUMER_CONTROL, &report_, sizeof(report_));
  }
};

class TUSBMouse : public MouseAPI {
 public:
  TUSBMouse() {
    (void)TUSBMultiReport();
  }

 protected:
  void sendReportUnchecked() {
    TUSBMultiReport().sendReport(RID_MOUSE, &report_, sizeof(report_));
  }
};

class TUSBSystemControl : public SystemControlAPI {
 public:
  TUSBSystemControl() {
    (void)TUSBMultiReport();
  }

 protected:
  void sendReport(void *data, int length) override {
    TUSBMultiReport().sendReport(RID_SYSTEM_CONTROL, data, length);
  }
  bool wakeupHost(uint8_t s) override {
    return false;
  }
};

struct MouseProps : public base::MouseProps {
  typedef TUSBMouse Mouse;
};

template<typename _Props>
class Mouse : public base::Mouse<_Props> {};

}  // namespace tinyusb
}  // namespace hid
}  // namespace driver
}  // namespace kaleidoscope

#endif /* USE_TINYUSB */
