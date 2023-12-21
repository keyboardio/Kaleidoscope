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

#include "testing/HIDState.h"

// IWYU pragma: no_include <__utility/move.h>

#include <utility>  // IWYU pragma: keep
#include <vector>   // for vector

#include "HID-Settings.h"      // for HID_REPORTID_CONSUMERCONTROL, HID_REPORTID_GAMEPAD, HID_RE...
#include "testing/iostream.h"  // for operator<<, char_traits, cout, ostream, basic_ostream

#define LOG(x) std::cout

namespace kaleidoscope {
namespace testing {

const std::vector<AbsoluteMouseReport> &HIDState::AbsoluteMouse() const {
  return absolute_mouse_reports_;
}

const AbsoluteMouseReport &HIDState::AbsoluteMouse(size_t i) const {
  return absolute_mouse_reports_.at(i);
}

const std::vector<ConsumerControlReport> &HIDState::ConsumerControl() const {
  return consumer_control_reports_;
}

const ConsumerControlReport &HIDState::ConsumerControl(size_t i) const {
  return consumer_control_reports_.at(i);
}

const std::vector<KeyboardReport> &HIDState::Keyboard() const {
  return keyboard_reports_;
}

const KeyboardReport &HIDState::Keyboard(size_t i) const {
  return keyboard_reports_.at(i);
}

const std::vector<MouseReport> &HIDState::Mouse() const {
  return mouse_reports_;
}

const MouseReport &HIDState::Mouse(size_t i) const {
  return mouse_reports_.at(i);
}

const std::vector<SystemControlReport> &HIDState::SystemControl() const {
  return system_control_reports_;
}

const SystemControlReport &HIDState::SystemControl(size_t i) const {
  return system_control_reports_.at(i);
}

namespace internal {

// static
void HIDStateBuilder::ProcessHidReport(
  uint8_t id, const void *data, int len, int result) {
  switch (id) {
  case HID_REPORTID_KEYBOARD: {
    ProcessKeyboardReport(KeyboardReport{data});
    break;
  }
  case HID_REPORTID_GAMEPAD: {
    LOG(ERROR) << "Dropped GamePadReport: unimplemented";
    break;
  }
  case HID_REPORTID_CONSUMERCONTROL: {
    ProcessConsumerControlReport(ConsumerControlReport{data});
    break;
  }
  case HID_REPORTID_SYSTEMCONTROL: {
    ProcessSystemControlReport(SystemControlReport{data});
    break;
  }
  case HID_REPORTID_MOUSE: {
    ProcessMouseReport(MouseReport{data});
    break;
  }
  case HID_REPORTID_MOUSE_ABSOLUTE: {
    ProcessAbsoluteMouseReport(AbsoluteMouseReport{data});
    break;
  }
  case HID_REPORTID_NKRO_KEYBOARD: {
    LOG(ERROR) << "Dropped NKROKeyboardReport: unimplemented";
    break;
  }
  default:
    LOG(ERROR) << "Encountered unknown HID report with id = " << id;
  }
}

// static
std::unique_ptr<HIDState> HIDStateBuilder::Snapshot() {
  auto hid_state = std::make_unique<HIDState>();
  // Populate state.
  // TODO: Grab a copy of current instantaneous state, like:
  //  key states, layer stack, led states
  hid_state->absolute_mouse_reports_   = std::move(absolute_mouse_reports_);
  hid_state->consumer_control_reports_ = std::move(consumer_control_reports_);
  hid_state->keyboard_reports_         = std::move(keyboard_reports_);
  hid_state->mouse_reports_            = std::move(mouse_reports_);
  hid_state->system_control_reports_   = std::move(system_control_reports_);

  Clear();  // Clear global state.
  return hid_state;
}

// static
void HIDStateBuilder::Clear() {
  absolute_mouse_reports_.clear();
  consumer_control_reports_.clear();
  keyboard_reports_.clear();
  mouse_reports_.clear();
  system_control_reports_.clear();
}

// static
void HIDStateBuilder::ProcessAbsoluteMouseReport(const AbsoluteMouseReport &report) {
  absolute_mouse_reports_.push_back(report);
}

// static
void HIDStateBuilder::ProcessConsumerControlReport(const ConsumerControlReport &report) {
  consumer_control_reports_.push_back(report);
}

// static
void HIDStateBuilder::ProcessKeyboardReport(const KeyboardReport &report) {
  keyboard_reports_.push_back(report);
}

// static
void HIDStateBuilder::ProcessMouseReport(const MouseReport &report) {
  mouse_reports_.push_back(report);
}

// static
void HIDStateBuilder::ProcessSystemControlReport(const SystemControlReport &report) {
  system_control_reports_.push_back(report);
}

// static
std::vector<AbsoluteMouseReport> HIDStateBuilder::absolute_mouse_reports_;
// static
std::vector<ConsumerControlReport> HIDStateBuilder::consumer_control_reports_;
// static
std::vector<KeyboardReport> HIDStateBuilder::keyboard_reports_;
// static
std::vector<MouseReport> HIDStateBuilder::mouse_reports_;
// static
std::vector<SystemControlReport> HIDStateBuilder::system_control_reports_;

}  // namespace internal
}  // namespace testing
}  // namespace kaleidoscope
