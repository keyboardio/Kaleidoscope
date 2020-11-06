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

#include "testing/VirtualDeviceTest.h"

#include "HIDReportObserver.h"
#include "testing/HIDState.h"

namespace kaleidoscope {
namespace testing {

void VirtualDeviceTest::SetUp() {
  HIDReportObserver::resetHook(&internal::HIDStateBuilder::ProcessHidReport);
}

std::unique_ptr<State> VirtualDeviceTest::RunCycle() {
  sim_.RunCycle();
  return State::Snapshot();
}

// =============================================================================
void VirtualDeviceTest::LoadState() {
  output_state_ = State::Snapshot();
}

void VirtualDeviceTest::ClearState() {
  output_state_ = nullptr;
  input_timestamps_.clear();
  expected_reports_.clear();
}

const HIDState* VirtualDeviceTest::HIDReports() const {
  if (output_state_ == nullptr) return nullptr;
  return output_state_->HIDReports();
}

uint32_t VirtualDeviceTest::ReportTimestamp(size_t index) const {
  uint32_t t = output_state_->HIDReports()->Keyboard(index).Timestamp();
  return t;
}

// -----------------------------------------------------------------------------
uint32_t VirtualDeviceTest::EventTimestamp(size_t index) const {
  return input_timestamps_[index];
}

// =============================================================================
void VirtualDeviceTest::PressKey(KeyAddr addr) {
  sim_.Press(addr);
  input_timestamps_.push_back(Runtime.millisAtCycleStart());
}
void VirtualDeviceTest::ReleaseKey(KeyAddr addr) {
  sim_.Release(addr);
  input_timestamps_.push_back(Runtime.millisAtCycleStart());
}

// =============================================================================
void VirtualDeviceTest::ExpectReport(AddKeycodes added_keys,
                                     RemoveKeycodes removed_keys,
                                     std::string description) {
  uint32_t report_timestamp = Runtime.millisAtCycleStart();
  for (Key key : added_keys) {
    AddToReport(key);
  }
  for (Key key : removed_keys) {
    RemoveFromReport(key);
  }
  ExpectedKeyboardReport new_report(report_timestamp,
                                    current_keyboard_keycodes_,
                                    description);
  expected_reports_.push_back(new_report);
}

// -----------------------------------------------------------------------------
void VirtualDeviceTest::ExpectReport(AddKeycodes added_keys,
                                     std::string description) {
  ExpectReport(added_keys, RemoveKeycodes{}, description);
}
void VirtualDeviceTest::ExpectReport(RemoveKeycodes removed_keys,
                                     std::string description) {
  ExpectReport(AddKeycodes{}, removed_keys, description);
}

// =============================================================================
void VirtualDeviceTest::AddToReport(Key key) {
  current_keyboard_keycodes_.insert(key.getKeyCode());
}
void VirtualDeviceTest::RemoveFromReport(Key key) {
  current_keyboard_keycodes_.erase(key.getKeyCode());
}

}  // namespace testing
}  // namespace kaleidoscope
