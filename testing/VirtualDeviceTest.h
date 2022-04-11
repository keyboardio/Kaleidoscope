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

#pragma once

// IWYU pragma: no_include <__memory/unique_ptr.h>

#include <cstddef>           // for size_t
#include <cstdint>           // for uint32_t, int8_t, uint8_t
#include <initializer_list>  // for initializer_list
#include <memory>            // IWYU pragma: keep
#include <set>               // for set
#include <vector>            // for vector

#include "kaleidoscope/KeyAddr.h"            // for KeyAddr
#include "kaleidoscope/key_defs.h"           // for Key
#include "testing/ExpectedKeyboardReport.h"  // for ExpectedKeyboardReport
#include "testing/ExpectedMouseReport.h"     // for ExpectedMouseReport
#include "testing/HIDState.h"                // for HIDState
#include "testing/SimHarness.h"              // for SimHarness
#include "testing/State.h"                   // for State
#include "testing/gtest.h"                   // for Test
#include "testing/iostream.h"                // for string

namespace kaleidoscope {
namespace testing {

// -----------------------------------------------------------------------------
// Utility classes for use in `PressKey()`/`ReleaseKey()` & `ExpectReport()`
// method invocations. These make those calls simpler by providing
// differentiated types for those polymorphic functions.
class AddKeycodes : public std::set<Key> {
 public:
  AddKeycodes(std::initializer_list<Key> list)
    : std::set<Key>(list) {}
};
class RemoveKeycodes : public std::set<Key> {
 public:
  RemoveKeycodes(std::initializer_list<Key> list)
    : std::set<Key>(list) {}
};

class Keycodes : public std::set<Key> {
 public:
  Keycodes(std::initializer_list<Key> list)
    : std::set<Key>(list) {}
};


// -----------------------------------------------------------------------------
// The base class for testcases
class VirtualDeviceTest : public ::testing::Test {
 protected:
  void SetUp();

  std::unique_ptr<State> RunCycle();

  SimHarness sim_;

  // ---------------------------------------------------------------------------
  // A representation of the set of observed HID reports accumulated by the
  // simulator as a testcase executes. It starts out empty, and a call to
  // `LoadState()` is required in order to load those reports into this
  // `output_state_` variable so that they can be used.
  std::unique_ptr<State> output_state_ = nullptr;

  // Load any accumulated observed HID reports from the simulator (since the
  // previous call to this function).
  void LoadState();

  // Clear previous state. This should be used at the beginning of each
  // `TEST_F()` call to clear out any accumulated state in the input and output
  // queues.
  void ClearState();

  // Get a pointer to the current list of observed HID reports
  const HIDState *HIDReports() const;

  // Get the timestamp of a logged Keyboard HID report
  uint32_t ReportTimestamp(size_t index) const;

  // ---------------------------------------------------------------------------
  // A vector of timestamps for input events. Calls to `PressKey()` &
  // `ReleaseKey()` append timestamps to it when called.
  std::vector<uint32_t> input_timestamps_ = {};

  // Get the timestamp of a logged input event from `output_state_`
  uint32_t EventTimestamp(size_t index) const;

  // ---------------------------------------------------------------------------
  // Press/release a keyswitch & log the input event timestamp
  void PressKey(KeyAddr addr);
  void ReleaseKey(KeyAddr addr);


  // ---------------------------------------------------------------------------
  // The following functions all add an expected value of HID report to the
  // queue. They specify modifications to the current (expected) report, and a
  // message to display if verification fails. Some versions allow multiple
  // keycode changes in a single report; others only a single keycode. Some run
  // the simulator for a specified number of milliseconds or cycles first. These
  // expected-value reports are all stored in a vector:
  std::vector<ExpectedKeyboardReport> expected_keyboard_reports_ = {};

  void ExpectKeyboardReport(AddKeycodes added_keys,
                            RemoveKeycodes removed_keys,
                            std::string description);

  void ExpectKeyboardReport(Keycodes added_keys, std::string description);
  void ExpectKeyboardReport(AddKeycodes added_keys, std::string description);
  void ExpectKeyboardReport(RemoveKeycodes removed_keys, std::string description);

  std::vector<ExpectedMouseReport> expected_mouse_reports_ = {};

  void ExpectMouseReport(uint8_t buttons, int8_t x, int8_t y, int8_t v, int8_t h, std::string description);

  // ---------------------------------------------------------------------------
  std::set<uint8_t> current_keyboard_keycodes_ = {};
  // Manage the set of keycodes expected in the next report
  void ClearKeyboardReport();
  void AddToKeyboardReport(Key key);
  void RemoveFromKeyboardReport(Key key);

  // ---------------------------------------------------------------------------
  // Compare accumulated observed and expected reports, matching both timestamps
  // and keycodes.
  void CheckReports() const;
  void CheckKeyboardReports() const;
  void CheckMouseReports() const;
};

}  // namespace testing
}  // namespace kaleidoscope
