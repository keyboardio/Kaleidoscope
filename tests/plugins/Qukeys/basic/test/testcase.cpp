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
  ExpectedKeyboardReport(uint32_t timestamp,
                         const std::set<uint8_t> &keycodes,
                         std::string message) {
    timestamp_ = timestamp;
    keycodes_ = std::set<uint8_t>(keycodes);
    failure_message_ = message;
  }

  const std::set<uint8_t> & Keycodes() const {
    return keycodes_;
  }

  const uint32_t Timestamp() const {
    return timestamp_;
  }

  const std::string & Message() const {
    return failure_message_;
  }

 private:
  uint32_t timestamp_;
  std::set<uint8_t> keycodes_;
  std::string failure_message_ = "";
};

class QukeysBasic : public VirtualDeviceTest {
 protected:
  std::set<uint8_t> current_keyboard_keycodes_ = {};
  std::vector<ExpectedKeyboardReport> expected_reports_ = {};

  std::vector<uint32_t> input_timestamps_ = {};

  std::unique_ptr<State> output_state_ = nullptr;

  std::set<uint8_t> expected_keycodes_ = {};
  std::unique_ptr<State> state_ = nullptr;

  uint32_t reportTimestamp(size_t index) {
    return output_state_->HIDReports()->Keyboard(index).Timestamp();
  }

  uint32_t keyswitchTimestamp(size_t index) {
    return input_timestamps_[index];
  }

  uint32_t pressKey(uint32_t delay, KeyAddr addr) {
    sim_.RunForMillis(delay);
    sim_.Press(addr);
    input_timestamps_.push_back(Runtime.millisAtCycleStart());
    return Runtime.millisAtCycleStart();
  }
  uint32_t releaseKey(uint32_t delay, KeyAddr addr) {
    sim_.RunForMillis(delay);
    sim_.Release(addr);
    input_timestamps_.push_back(Runtime.millisAtCycleStart());
    return Runtime.millisAtCycleStart();
  }

  void addToReport(Key key) {
    current_keyboard_keycodes_.insert(key.getKeyCode());
  }
  void removeFromReport(Key key) {
    current_keyboard_keycodes_.erase(key.getKeyCode());
  }
  uint32_t expectReport(std::set<Key> added_keys = {},
                        std::set<Key> removed_keys = {},
                        std::string failure_message = "") {
    uint32_t report_timestamp = Runtime.millisAtCycleStart();
    for (Key key : added_keys) {
      addToReport(key);
    }
    for (Key key : removed_keys) {
      removeFromReport(key);
    }
    ExpectedKeyboardReport new_report(report_timestamp,
                                      current_keyboard_keycodes_,
                                      failure_message);
    expected_reports_.push_back(new_report);
    return report_timestamp;
  }
  uint32_t expectReportWith(Key key, std::string message = "") {
    return expectReport(std::set<Key> {key}, std::set<Key> {}, message);
  }
  uint32_t expectReportWithout(Key key, std::string message = "") {
    return expectReport(std::set<Key> {}, std::set<Key> {key}, message);
  }
  uint32_t expectReportAfterCycles(size_t n,
                                   std::set<Key> added_keys,
                                   std::set<Key> removed_keys,
                                   std::string message = "") {
    sim_.RunCycles(n);
    return expectReport(added_keys, removed_keys, message);
  }
  uint32_t expectReportAfterCycles(size_t n, std::string message = "") {
    return expectReportAfterCycles(n, std::set<Key> {}, std::set<Key> {},
                                   message);
  }
  uint32_t expectReportAfterCyclesWith(size_t n, Key key,
                                       std::string message = "") {
    sim_.RunCycles(n);
    return expectReportWith(key, message);
  }
  uint32_t expectReportAfterCyclesWithout(size_t n, Key key,
                                          std::string message = "") {
    sim_.RunCycles(n);
    return expectReportWithout(key, message);
  }
  uint32_t expectReportAfterMillis(uint32_t t,
                                   std::set<Key> added_keys,
                                   std::set<Key> removed_keys,
                                   std::string message = "") {
    sim_.RunForMillis(t);
    return expectReport(added_keys, removed_keys, message);
  }
  uint32_t expectReportAfterMillis(uint32_t t, std::string message = "") {
    return expectReportAfterMillis(t, std::set<Key> {}, std::set<Key> {},
                                   message);
  }
  uint32_t expectReportAfterMillisWith(uint32_t t, Key key,
                                       std::string message = "") {
    sim_.RunForMillis(t);
    return expectReportWith(key, message);
  }
  uint32_t expectReportAfterMillisWithout(uint32_t t, Key key,
                                          std::string message = "") {
    sim_.RunForMillis(t);
    return expectReportWithout(key, message);
  }
};

TEST_F(QukeysBasic, TapQukeyAlone) {

  pressKey(10, key_addr_A);
  releaseKey(50, key_addr_A);
  expectReportAfterCyclesWith(
    1, Key_A, "Report should contain only `A`");
  expectReportAfterCyclesWithout(
    1, Key_A, "Report should be empty");

  sim_.RunForMillis(10);
  auto state = RunCycle();

  constexpr int expected_report_count = 2;
  ASSERT_EQ(state->HIDReports()->Keyboard().size(), expected_report_count)
      << "There should be two HID reports after a qukey is tapped";

  for (int i = 0; i < expected_report_count; ++i) {
    EXPECT_THAT(state->HIDReports()->Keyboard(i).ActiveKeycodes(),
                ::testing::ElementsAreArray(expected_reports_[i].Keycodes()))
        << expected_reports_[i].Message();
  }
}

TEST_F(QukeysBasic, HoldQukeyAlone) {
  constexpr uint32_t hold_time = 400;

  pressKey(10, key_addr_A);
  expectReportAfterMillisWith(
    QUKEYS_HOLD_TIMEOUT, Key_LeftGui,
    "The first report should include only `LeftGui`");
  releaseKey(hold_time, key_addr_A);
  expectReportAfterCyclesWithout(
    1, Key_LeftGui,
    "The second report should be empty");

  sim_.RunForMillis(10);
  output_state_ = RunCycle();

  constexpr int expected_report_count = 2;
  ASSERT_EQ(output_state_->HIDReports()->Keyboard().size(), expected_report_count)
      << "There should be two HID reports after a qukey is held and released";

  uint32_t measured_delay = reportTimestamp(0) - input_timestamps_[0];
  EXPECT_THAT(measured_delay, ::testing::Ge(QUKEYS_HOLD_TIMEOUT))
      << "The HID report should be sent after the hold timeout has elapsed";

  // std::cerr << keyswitchTimestamp(1) << std::endl;
  // std::cerr << reportTimestamp(1) << std::endl;

  EXPECT_THAT(reportTimestamp(1), ::testing::Ge(keyswitchTimestamp(1)))
      << "The key should stay active until release";

  for (int i = 0; i < expected_report_count; ++i) {
    EXPECT_THAT(output_state_->HIDReports()->Keyboard(i).ActiveKeycodes(),
                ::testing::ElementsAreArray(expected_reports_[i].Keycodes()))
        << expected_reports_[i].Message();
  }
}

TEST_F(QukeysBasic, FullOverlap) {

  pressKey(10, key_addr_F);
  pressKey(20, key_addr_X);
  releaseKey(50, key_addr_X);
  expectReportAfterCyclesWith(
    1, Key_LeftShift,
    "The first report should contain only `LeftShift`");
  expectReportAfterCyclesWith(
    1, Key_X,
    "The second report should add `X`");
  expectReportAfterCyclesWithout(
    1, Key_X,
    "The third report should remove `X`");
  releaseKey(10, key_addr_F);
  expectReportAfterCyclesWithout(
    1, Key_LeftShift,
    "The fourth report should remove `LeftShift`");

  sim_.RunForMillis(10);
  output_state_ = RunCycle();

  constexpr int expected_report_count = 4;
  ASSERT_EQ(output_state_->HIDReports()->Keyboard().size(), expected_report_count)
      << "There should be four HID reports total";

  for (int i = 0; i < expected_report_count; ++i) {
    EXPECT_THAT(output_state_->HIDReports()->Keyboard(i).ActiveKeycodes(),
                ::testing::ElementsAreArray(expected_reports_[i].Keycodes()))
        << expected_reports_[i].Message();
  }
}

TEST_F(QukeysBasic, RolloverPrimary) {

  pressKey(10, key_addr_F);
  pressKey(40, key_addr_X);
  releaseKey(20, key_addr_F);
  addToReport(Key_F);
  expectReportAfterMillis(30);
  addToReport(Key_X);
  expectReport(std::set<Key> {}, std::set<Key> {});
  removeFromReport(Key_F);
  expectReport(std::set<Key> {}, std::set<Key> {});
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
