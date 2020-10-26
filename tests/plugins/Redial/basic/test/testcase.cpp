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

SETUP_GOOGLETEST();

namespace kaleidoscope {
namespace testing {
namespace {

constexpr KeyAddr key_addr_Redial{0, 9};
constexpr KeyAddr key_addr_A{2, 1};
constexpr KeyAddr key_addr_X{3, 2};

class RedialBasic : public VirtualDeviceTest {
 protected:
  std::set<uint8_t> expected_keycodes_ = {};
  std::unique_ptr<State> state_ = nullptr;
};

TEST_F(RedialBasic, RedialFirst) {

  // Press redial key
  sim_.Press(key_addr_Redial);

  state_ = RunCycle();

  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 0)
      << "There should be no HID report for a redial key without anything pressed first";

  // Release redial key
  sim_.Release(key_addr_Redial);
  sim_.RunCycle();
}

TEST_F(RedialBasic, RedialFirstKey) {

  // Press `A`
  sim_.Press(key_addr_A);
  state_ = RunCycle();

  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 1)
      << "There should be one report after letter key press";

  expected_keycodes_.insert(Key_A.getKeyCode());
  EXPECT_THAT(state_->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes_))
      << "The report should include only `A`";

  // Release `A`
  sim_.Release(key_addr_A);
  state_ = RunCycle();

  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 1)
      << "There should be one report after letter key release";

  expected_keycodes_.erase(Key_A.getKeyCode());
  EXPECT_THAT(state_->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes_))
      << "The report should be empty";

  sim_.RunCycle();

  // Press redial key
  sim_.Press(key_addr_Redial);

  state_ = RunCycle();

  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 1)
      << "There should now be a redial-key report";

  expected_keycodes_.insert(Key_A.getKeyCode());
  EXPECT_THAT(state_->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes_))
      << "The report should include only `A`";

  // Release redial key
  sim_.Release(key_addr_Redial);
  state_ = RunCycle();

  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 1)
      << "There should be one report after redial-key release";

  expected_keycodes_.erase(Key_A.getKeyCode());
  EXPECT_THAT(state_->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes_))
      << "The report should be empty";

  sim_.RunCycle();

  state_ = RunCycle();
  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 0);
}

TEST_F(RedialBasic, RedialNextKey) {

  // Press `X`
  sim_.Press(key_addr_X);
  state_ = RunCycle();

  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 1)
      << "There should be one report after letter key press";

  expected_keycodes_.insert(Key_X.getKeyCode());
  EXPECT_THAT(state_->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes_))
      << "The report should include only `X`";

  // Release `X`
  sim_.Release(key_addr_X);
  state_ = RunCycle();

  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 1)
      << "There should be one report after letter key release";

  expected_keycodes_.erase(Key_X.getKeyCode());
  EXPECT_THAT(state_->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes_))
      << "The report should be empty";

  sim_.RunCycle();

  // Press redial key
  sim_.Press(key_addr_Redial);

  state_ = RunCycle();

  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 1)
      << "There should now be a redial-key report";

  expected_keycodes_.insert(Key_X.getKeyCode());
  EXPECT_THAT(state_->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes_))
      << "The report should include only `X`";

  // Release redial key
  sim_.Release(key_addr_Redial);
  state_ = RunCycle();

  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 1)
      << "There should be one report after redial-key release";

  expected_keycodes_.erase(Key_X.getKeyCode());
  EXPECT_THAT(state_->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes_))
      << "The report should be empty";

  sim_.RunCycle();

  state_ = RunCycle();
  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 0);
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
