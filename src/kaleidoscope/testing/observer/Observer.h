/* -*- mode: c++ -*-
 * Copyright (C) 2020  Eric Paniagua (epaniagua@google.com)
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

#pragma once

//#include "./Logging.h"
#include <iostream>
#define LOG(x) std::cout

#include "HID-Settings.h"
#include "HIDReportObserver.h"
#include "kaleidoscope/testing/reports/KeyboardReport.h"

namespace kaleidoscope {
namespace testing {

class Observer {
 public:
  Observer() {
    HIDReportObserver::resetHook(
        std::bind(&Observer::ProcessHidReport, this));
  }

  void Clear() {
    mouse_reports_.clear();
    boot_keyboard_reports_.clear();
    absolute_mouse_reports_.clear();
    keyboard_reports_.clear();
  }

  void ProcessHidReport(uint8_t id, const void* data, int len, int result) {
    switch (id) {
      case HID_REPORTID_MOUSE: {
        ProcessReport(MouseReport{data});
        break;
      }
      case HID_REPORTID_KEYBOARD: {
        ProcessReport(BootKeyboardReport{data});
        break;
      }
      case HID_REPORTID_GAMEPAD:
      case HID_REPORTID_CONSUMERCONTROL:
      case HID_REPORTID_SYSTEMCONTROL: {
        // TODO: React appropriately to these.
        LOG(INFO) << "Ignoring HID report with id = " << id;
        break;
      }
      case HID_REPORTID_MOUSE_ABSOLUTE: {
        ProcessReport(AbsoluteMouseReport{data});
        break;
      }
      case HID_REPORTID_NKRO_KEYBOARD: {
        ProcessReport(KeyboardReport{data});
        break;
      }
      default:
        LOG(ERROR) << "Encountered unknown HID report with id = " << id;
    }
  }

  const std::vector<KeyboardReport>& KeyboardReports() const {
    return keyboard_reports_;
  }

  const KeyboardReport& KeyboardReports(size_t i) const {
    return keyboard_reports_.at(i);
  }

 private:
  template <class R>
  void ProcessReport(const R& report) {
    observer_.RecordReport(report);
  }

  template <class R>
  void RecordReport(const R& report);

  std::vector<KeyboardReport> keyboard_reports_;
};

template <>
void Observer::RecordReport<KeyboardReport>(const KeyboardReport& report) {
  keyboard_reports_.push_back(report);
}

}  // namespace testing
}  // namespace kaleidoscope

#endif  // KALEIDOSCOPE_VIRTUAL_BUILD
