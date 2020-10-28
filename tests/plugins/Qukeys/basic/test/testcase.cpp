/* -*- mode: c++ -*-
 * Copyright (C) 2020  Keyboard.io, Inc.
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

#include "testing/setup-googletest.h"

#include "../shared.h"

SETUP_GOOGLETEST();

namespace kaleidoscope {
namespace testing {
namespace {

constexpr KeyAddr key_addr_A{2, 1};
constexpr KeyAddr key_addr_S{2, 2};
constexpr KeyAddr key_addr_D{2, 3};
constexpr KeyAddr key_addr_F{2, 4};
constexpr KeyAddr key_addr_X{3, 2};

using ::testing::IsEmpty;

class ExpectedKeyboardReport {
 public:
  ExpectedKeyboardReport(uint32_t timestamp,
                         const std::set<uint8_t> &keycodes) {
    timestamp_ = timestamp;
    keycodes_ = std::set<uint8_t>(keycodes);
  }

  const std::set<uint8_t> & Keycodes() const {
    return keycodes_;
  }

 private:
  uint32_t timestamp_;
  std::set<uint8_t> keycodes_;
};

class QukeysBasic : public VirtualDeviceTest {
 protected:
  std::set<uint8_t> current_keyboard_keycodes_ = {};
  std::vector<ExpectedKeyboardReport> expected_reports_ = {};

  std::set<uint8_t> expected_keycodes_ = {};
  std::unique_ptr<State> state_ = nullptr;
  uint32_t last_event_timestamp_ = 0;

  uint32_t pressKey(uint32_t delay, KeyAddr addr) {
    sim_.RunForMillis(delay);
    sim_.Press(addr);
    last_event_timestamp_ = Runtime.millisAtCycleStart();
    return last_event_timestamp_;
  }
  uint32_t releaseKey(uint32_t delay, KeyAddr addr) {
    sim_.RunForMillis(delay);
    sim_.Release(addr);
    last_event_timestamp_ = Runtime.millisAtCycleStart();
    return last_event_timestamp_;
  }

  void addToReport(Key key) {
    current_keyboard_keycodes_.insert(key.getKeyCode());
  }
  void removeFromReport(Key key) {
    current_keyboard_keycodes_.erase(key.getKeyCode());
  }
  uint32_t expectReport() {
    uint32_t report_timestamp = Runtime.millisAtCycleStart();
    ExpectedKeyboardReport new_report(report_timestamp,
                                      current_keyboard_keycodes_);
    expected_reports_.push_back(new_report);
    return report_timestamp;
  }
  uint32_t expectReportAfterCycles(size_t n) {
    sim_.RunCycles(n);
    return expectReport();
  }
  uint32_t expectReportAfterMillis(uint32_t t) {
    sim_.RunForMillis(t);
    return expectReport();
  }
};

TEST_F(QukeysBasic, TapQukeyAlone) {

  pressKey(10, key_addr_A);
  releaseKey(50, key_addr_A);
  addToReport(Key_A);
  expectReportAfterCycles(1);
  removeFromReport(Key_A); 
  expectReportAfterCycles(1);

  sim_.RunForMillis(100);
  auto state = RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 2)
      << "There should be two HID reports after a qukey is tapped";

  EXPECT_THAT(state->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_reports_[0].Keycodes()))
      << "The first report should include only `A`";

  EXPECT_THAT(state->HIDReports()->Keyboard(1).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_reports_[1].Keycodes()))
      << "The second report should be empty";
}

TEST_F(QukeysBasic, HoldQukeyAlone) {
  constexpr uint32_t hold_time = 400;

  
  uint32_t t0 = pressKey(10, key_addr_A);
  addToReport(Key_LeftGui);
  uint32_t t1 = expectReportAfterMillis(QUKEYS_HOLD_TIMEOUT);
  releaseKey(hold_time, key_addr_A);
  removeFromReport(Key_LeftGui);
  expectReportAfterCycles(1);

  sim_.RunForMillis(10);
  auto state = RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 2)
      << "There should be two HID reports after a qukey is held and released";

  uint32_t measured_delay = state->HIDReports()->Keyboard(0).Timestamp() - t0;
  EXPECT_THAT(measured_delay, ::testing::Ge(QUKEYS_HOLD_TIMEOUT))
      << "The HID report should be sent after the hold timeout has elapsed";
  EXPECT_THAT(state->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_reports_[0].Keycodes()))
      << "The first report should include only `LeftGui`";

  measured_delay = state->HIDReports()->Keyboard(1).Timestamp() - t1;
  EXPECT_THAT(measured_delay, ::testing::Ge(hold_time))
      << "The key should stay active until release";
  EXPECT_THAT(state->HIDReports()->Keyboard(1).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_reports_[1].Keycodes()))
      << "The second report should be empty";
}

TEST_F(QukeysBasic, FullOverlap) {

  pressKey(10, key_addr_F);
  pressKey(20, key_addr_X);
  releaseKey(50, key_addr_X);
  addToReport(Key_LeftShift);
  expectReportAfterCycles(1);
  addToReport(Key_X);
  expectReportAfterCycles(1);
  removeFromReport(Key_X);
  expectReportAfterCycles(1);
  releaseKey(10, key_addr_F);
  removeFromReport(Key_LeftShift);
  expectReportAfterCycles(1);

  sim_.RunForMillis(10);
  auto state = RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 4)
      << "There should be four HID reports total";
  EXPECT_THAT(state->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_reports_[0].Keycodes()))
      << "The first report should contain only `LeftShift`";
  EXPECT_THAT(state->HIDReports()->Keyboard(1).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_reports_[1].Keycodes()))
      << "The second report should add `X`";
  EXPECT_THAT(state->HIDReports()->Keyboard(2).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_reports_[2].Keycodes()))
      << "The third report should remove `X`";
  EXPECT_THAT(state->HIDReports()->Keyboard(3).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_reports_[3].Keycodes()))
      << "The fourth report should remove `LeftShift`";
}

TEST_F(QukeysBasic, RolloverPrimary) {

  pressKey(10, key_addr_F);
  pressKey(40, key_addr_X);
  releaseKey(20, key_addr_F);
  addToReport(Key_F);
  expectReportAfterMillis(30);
  addToReport(Key_X);
  expectReport();
  removeFromReport(Key_F);
  expectReport();
  releaseKey(40, key_addr_X);
  removeFromReport(Key_X);
  expectReportAfterCycles(1);

  sim_.RunForMillis(10);
  auto state = RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 4)
      << "There should be four HID reports total";
  EXPECT_THAT(state->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_reports_[0].Keycodes()))
      << "The first report should contain only `F`";
  EXPECT_THAT(state->HIDReports()->Keyboard(1).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_reports_[1].Keycodes()))
      << "The second report should add `X`";
  EXPECT_THAT(state->HIDReports()->Keyboard(2).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_reports_[2].Keycodes()))
      << "The third report should remove `F`";
  EXPECT_THAT(state->HIDReports()->Keyboard(3).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_reports_[3].Keycodes()))
      << "The fourth report should remove `X`";
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
