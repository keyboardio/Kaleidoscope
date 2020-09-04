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

#include "testing/common/State.h"

#include "testing/common/fix-macros.h"

// TODO(epan): Add proper logging.
#include <iostream>
#define LOG(x) std::cout

namespace kaleidoscope {
namespace testing {

State::State() {
  HIDReportObserver::resetHook(
      [this](uint8_t id, const void* data, int len, int result) {
        this->ProcessHidReport(id, data, len, result);
      });
}

void State::Clear() {
  keyboard_reports_.clear();
}

void State::ProcessHidReport(
    uint8_t id, const void* data, int len, int result) {
  switch (id) {
    case HID_REPORTID_MOUSE: {
      LOG(ERROR) << "Dropped MouseReport: unimplemented";
      break;
    }
    case HID_REPORTID_KEYBOARD: {
      LOG(ERROR) << "Dropped BootKeyboardReport: unimplemented";
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
      LOG(ERROR) << "Dropped AbsoluteMouseReport: unimplemented";
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

const std::vector<KeyboardReport>& State::KeyboardReports() const {
  return keyboard_reports_;
}

const KeyboardReport& State::KeyboardReports(size_t i) const {
  return keyboard_reports_.at(i);
}

}  // namespace testing
}  // namespace kaleidoscope
