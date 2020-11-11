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

#include "../common.h"

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

class QukeysBasic : public VirtualDeviceTest {
 protected:
  std::set<uint8_t> expected_keycodes_ = {};
  std::unique_ptr<State> state_ = nullptr;
};

TEST_F(QukeysBasic, TapQukeyAlone) {
  // Press `A`
  sim_.Press(key_addr_A);

  state_ = RunCycle();

  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 0)
      << "There should be no HID report after the qukey is pressed";
  sim_.RunForMillis(20);
  // Release `A`
  sim_.Release(key_addr_A);

  sim_.RunCycles(2);
  state_ = RunCycle();

  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 2)
      << "There should be two HID reports after the release of a tapped qukey";

  expected_keycodes_.insert(Key_A.getKeyCode());
  EXPECT_THAT(state_->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes_))
      << "The first report should include only `A`";

  expected_keycodes_.erase(Key_A.getKeyCode());
  EXPECT_THAT(state_->HIDReports()->Keyboard(1).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes_))
      << "The second report should be empty";

  state_ = RunCycle();
  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 0);
}

TEST_F(QukeysBasic, MinPriorIntervalOverflow) {
  // Wait until integer overflow causes the timestamp to catch up
  sim_.RunForMillis(65535);

  // Press `A`
  sim_.Press(key_addr_A);

  state_ = RunCycle();

  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 0);

  uint32_t t0 = Kaleidoscope.millisAtCycleStart();

  // To test the hold timeout, we check after every cycle to see if there's new
  // HID report. We can't just call `RunForMillis()` because we care about when
  // that report was sent.
  do {
    state_ = RunCycle();
  } while (state_->HIDReports()->Keyboard().size() == 0 &&
           (Kaleidoscope.millisAtCycleStart() - t0 < QUKEYS_HOLD_TIMEOUT + 1));

  uint32_t t1 = Kaleidoscope.millisAtCycleStart();

  EXPECT_THAT(t1 - t0, ::testing::Ge(QUKEYS_HOLD_TIMEOUT))
      << "The HID report should be sent after the hold timeout has elapsed";

  expected_keycodes_.insert(Key_LeftGui.getKeyCode());
  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 1)
      << "There should be only one HID report";
  EXPECT_THAT(state_->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes_))
      << "The HID report should contain just `Key_LeftGui`";

  sim_.RunForMillis(100);

  sim_.Release(key_addr_A);
  sim_.RunCycles(2);
  state_ = RunCycle();

  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 1)
      << "There should be a HID report immediately after the key is released";

  expected_keycodes_.erase(Key_LeftGui.getKeyCode());
  EXPECT_THAT(state_->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes_))
      << "The HID report should be empty at this point";

  state_ = RunCycle();
  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 0);
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
