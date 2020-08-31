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

#include "./Logging.h"

#include "HID-Settings.h"

namespace kaleidoscope {
namespace testing {

class Observer {
 public:
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

  template <class R>
  void RecordReport(const R& report);

  template <>
  void RecordReport<MouseReport>(const MouseReport& report) {
    mouse_reports_.push_back(report);
  }

  template <>
  void RecordReport<BootKeyboardReport>(const BootKeyboardReport& report) {
    boot_keyboard_reports_.push_back(report);
  }

  template <>
  void RecordReport<AbsoluteMouseReport>(const AbsoluteMouseReport& report) {
    absolute_mouse_reports_.push_back(report);
  }

  template <>
  void RecordReport<KeyboardReport>(const KeyboardReport& report) {
    keyboard_reports_.push_back(report);
  }

  std::vector<MouseReport> mouse_reports_;
  std::vector<BootKeyboardReport> boot_keyboard_reports_;
  std::vector<AbsoluteMouseReport> absolute_mouse_reports_;
  std::vector<KeyboardReport> keyboard_reports_;
};

}  // namespace testing
}  // namespace kaleidoscope
