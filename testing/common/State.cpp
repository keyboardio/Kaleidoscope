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

std::vector<KeyboardReport> State::keyboard_reports;

// static
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
      ProcessKeyboardReport(KeyboardReport{data});
      break;
    }
    default:
      LOG(ERROR) << "Encountered unknown HID report with id = " << id;
  }
}

// static
std::unique_ptr<State> State::Snapshot() {
  auto state = std::make_unique<State>();
  // Populate state.
  // TODO: Grab a copy of current instantaneous state, like:
  //  key states, layer stack, led states
  state->keyboard_reports_ = std::move(keyboard_reports);

  Clear();  // Clear global state.
  return state;
}

const std::vector<KeyboardReport>& State::KeyboardReports() const {
  return keyboard_reports_;
}

const KeyboardReport& State::KeyboardReports(size_t i) const {
  return keyboard_reports_.at(i);
}

// static
void State::Clear() {
  keyboard_reports.clear();
}

// static
void State::ProcessKeyboardReport(const KeyboardReport& report) {
  keyboard_reports.push_back(report);
}

}  // namespace testing
}  // namespace kaleidoscope
