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

constexpr KeyAddr key_addr_A{0, 0};
constexpr KeyAddr key_addr_B{0, 1};
constexpr KeyAddr key_addr_X{1, 0};
constexpr KeyAddr key_addr_Y{1, 1};

using ::testing::IsEmpty;

class QukeysIssue1107 : public VirtualDeviceTest {
 protected:
  std::set<uint8_t> expected_keycodes_ = {};
  std::unique_ptr<State> state_        = nullptr;

 public:
  void testDelayedTimeout() {
    state_ = RunCycle();

    // Press `X`
    sim_.Press(key_addr_X);
    state_ = RunCycle();

    ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 0)
      << "There should be no HID report after the qukey is pressed";

    // Record time at start
    uint32_t start = Kaleidoscope.millisAtCycleStart();

    uint8_t n = QUKEYS_HOLD_TIMEOUT + 1;
    while (n-- > 0)
      millis();

    // Release `X`
    sim_.Release(key_addr_X);
    state_ = RunCycle();

    // Record time at end
    uint32_t end = Kaleidoscope.millisAtCycleStart();

    ASSERT_EQ((end - start), (QUKEYS_HOLD_TIMEOUT + 2))
      << "Only one millisecond should be registered elapsed per cycle";

    ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 2)
      << "There should be two HID reports after the release of a timed-out qukey";
  }
};

TEST_F(QukeysIssue1107, DelayedTimeoutRelease) {
  for (int i = 0; i < 10; ++i)
    testDelayedTimeout();

  state_ = RunCycle();
  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 0);
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
