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

// IWYU pragma: no_include <__algorithm/max.h>
// IWYU pragma: no_include <__tree>

#include <bitset>  // for bitset
#include <vector>  // for vector

#include "HIDReportObserver.h"       // for HIDReportObserver
#include "kaleidoscope/Runtime.h"    // for Runtime, Runtime_
#include "testing/HIDState.h"        // for HIDState, HIDStateBuilder
#include "testing/KeyboardReport.h"  // for KeyboardReport
#include "testing/MouseReport.h"     // for MouseReport
#include "testing/gtest.h"           // for Message, TestPartResult, EXPECT_EQ, ElementsAreArray
#include "testing/iostream.h"        // for operator<<, basic_ostream, char_traits, string, cerr

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
  expected_keyboard_reports_.clear();
  expected_mouse_reports_.clear();
}

const HIDState *VirtualDeviceTest::HIDReports() const {
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
void VirtualDeviceTest::ExpectKeyboardReport(Keycodes keys,
                                             std::string description) {
  size_t report_timestamp{Runtime.millisAtCycleStart()};
  ClearKeyboardReport();
  for (Key key : keys) {
    AddToKeyboardReport(key);
  }
  ExpectedKeyboardReport new_report(report_timestamp,
                                    current_keyboard_keycodes_,
                                    description);
  expected_keyboard_reports_.push_back(new_report);
}

// =============================================================================
void VirtualDeviceTest::ExpectKeyboardReport(AddKeycodes added_keys,
                                             RemoveKeycodes removed_keys,
                                             std::string description) {
  uint32_t report_timestamp = Runtime.millisAtCycleStart();
  for (Key key : added_keys) {
    AddToKeyboardReport(key);
  }
  for (Key key : removed_keys) {
    RemoveFromKeyboardReport(key);
  }
  ExpectedKeyboardReport new_report(report_timestamp,
                                    current_keyboard_keycodes_,
                                    description);
  expected_keyboard_reports_.push_back(new_report);
}

// -----------------------------------------------------------------------------
void VirtualDeviceTest::ExpectKeyboardReport(AddKeycodes added_keys,
                                             std::string description) {
  ExpectKeyboardReport(added_keys, RemoveKeycodes{}, description);
}
void VirtualDeviceTest::ExpectKeyboardReport(RemoveKeycodes removed_keys,
                                             std::string description) {
  ExpectKeyboardReport(AddKeycodes{}, removed_keys, description);
}

// =============================================================================
void VirtualDeviceTest::ClearKeyboardReport() {
  current_keyboard_keycodes_.clear();
}
void VirtualDeviceTest::AddToKeyboardReport(Key key) {
  current_keyboard_keycodes_.insert(key.getKeyCode());
}
void VirtualDeviceTest::RemoveFromKeyboardReport(Key key) {
  current_keyboard_keycodes_.erase(key.getKeyCode());
}

// =============================================================================
void VirtualDeviceTest::ExpectMouseReport(uint8_t buttons,
                                          int8_t x,
                                          int8_t y,
                                          int8_t v,
                                          int8_t h,
                                          std::string description) {
  uint32_t report_timestamp = Runtime.millisAtCycleStart();
  ExpectedMouseReport new_report(report_timestamp,
                                 buttons,
                                 x,
                                 y,
                                 v,
                                 h,
                                 description);
  expected_mouse_reports_.push_back(new_report);
}


// =============================================================================
void VirtualDeviceTest::CheckReports() const {
  CheckKeyboardReports();
  CheckMouseReports();
}

void VirtualDeviceTest::CheckKeyboardReports() const {
  int observed_keyboard_report_count = HIDReports()->Keyboard().size();
  int expected_keyboard_report_count = expected_keyboard_reports_.size();

  EXPECT_EQ(observed_keyboard_report_count, expected_keyboard_report_count);

  int max_count = std::max(observed_keyboard_report_count,
                           expected_keyboard_report_count);

  for (int i = 0; i < observed_keyboard_report_count; ++i) {
    auto observed_report   = HIDReports()->Keyboard(i);
    auto observed_keycodes = observed_report.ActiveKeycodes();

    if (i < expected_keyboard_report_count) {
      auto expected_report   = expected_keyboard_reports_[i];
      auto expected_keycodes = expected_report.Keycodes();

      EXPECT_THAT(observed_keycodes,
                  ::testing::ElementsAreArray(expected_keycodes))
        << expected_keyboard_reports_[i].Message() << " (i=" << i << ")";
      EXPECT_EQ(observed_report.Timestamp(), expected_report.Timestamp())
        << "Report timestamps don't match (i=" << i << ")";

    } else {

      std::cerr << "Unexpected keyboard report at "
                << observed_report.Timestamp() << "ms: { " << std::hex;
      for (uint8_t keycode : observed_keycodes) {
        std::cerr << int(keycode) << " ";
      }
      std::cerr << "}" << std::dec << std::endl;
    }
  }
}

void VirtualDeviceTest::CheckMouseReports() const {
  int observed_mouse_report_count = HIDReports()->Mouse().size();
  int expected_mouse_report_count = expected_mouse_reports_.size();

  EXPECT_EQ(observed_mouse_report_count, expected_mouse_report_count);

  int max_count = std::max(observed_mouse_report_count,
                           expected_mouse_report_count);

  for (int i = 0; i < observed_mouse_report_count; ++i) {
    auto observed_report = HIDReports()->Mouse(i);

    if (i < expected_mouse_report_count) {
      auto expected_report = expected_mouse_reports_[i];

      EXPECT_EQ(observed_report.Buttons(), expected_report.Buttons())
        << expected_mouse_reports_[i].Message() << " (i=" << i << ")";
      EXPECT_EQ(observed_report.XAxis(), expected_report.XAxis())
        << expected_mouse_reports_[i].Message() << " (i=" << i << ")";
      EXPECT_EQ(observed_report.YAxis(), expected_report.YAxis())
        << expected_mouse_reports_[i].Message() << " (i=" << i << ")";
      EXPECT_EQ(observed_report.VWheel(), expected_report.VWheel())
        << expected_mouse_reports_[i].Message() << " (i=" << i << ")";
      EXPECT_EQ(observed_report.HWheel(), expected_report.HWheel())
        << expected_mouse_reports_[i].Message() << " (i=" << i << ")";
      EXPECT_EQ(observed_report.Timestamp(), expected_report.Timestamp())
        << "Report timestamps don't match (i=" << i << ")";

    } else {
      std::bitset<8> observed_buttons{observed_report.Buttons()};
      std::cerr << "Unexpected mouse report at "
                << observed_report.Timestamp() << "ms: {"
                << " buttons: " << observed_buttons
                << " x: " << int(observed_report.XAxis())
                << " y: " << int(observed_report.YAxis())
                << " v: " << int(observed_report.VWheel())
                << " h: " << int(observed_report.HWheel())
                << " }" << std::endl;
    }
  }
}

}  // namespace testing
}  // namespace kaleidoscope
