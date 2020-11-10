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

constexpr KeyAddr addr_macro_A{0, 0};
constexpr KeyAddr addr_macro_B{0, 1};
constexpr KeyAddr addr_macro_C{0, 2};
constexpr KeyAddr addr_macro_X{0, 3};
constexpr KeyAddr addr_X{1, 0};

class MacrosBasic : public VirtualDeviceTest {
 protected:
  std::set<uint8_t> expected_keycodes_ = {};
  std::unique_ptr<State> state_ = nullptr;
};

TEST_F(MacrosBasic, MacroIndex_0) {

  sim_.Press(addr_macro_A);
  state_ = RunCycle();
  expected_keycodes_.insert(Key_A.getKeyCode());

  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 1)
      << "There should be one HID report";
  EXPECT_THAT(state_->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes_))
      << "The report should include only `A`";

  sim_.Release(addr_macro_A);
  state_ = RunCycle();
  expected_keycodes_.erase(Key_A.getKeyCode());

  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 1)
      << "There should be one report after letter key release";
  EXPECT_THAT(state_->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes_))
      << "The report should be empty";
}

TEST_F(MacrosBasic, MacroIndex_1) {

  sim_.Press(addr_macro_B);
  state_ = RunCycle();
  expected_keycodes_.insert(Key_B.getKeyCode());

  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 1)
      << "There should be one HID report";
  EXPECT_THAT(state_->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes_))
      << "The report should include only `B`";

  sim_.Release(addr_macro_B);
  state_ = RunCycle();
  expected_keycodes_.erase(Key_B.getKeyCode());

  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 1)
      << "There should be one report after letter key release";
  EXPECT_THAT(state_->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes_))
      << "The report should be empty";
}

TEST_F(MacrosBasic, MacroIndex_255) {

  sim_.Press(addr_macro_C);
  state_ = RunCycle();
  expected_keycodes_.insert(Key_C.getKeyCode());

  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 1)
      << "There should be one HID report";
  EXPECT_THAT(state_->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes_))
      << "The report should include only `C`";

  state_ = RunCycle();
  expected_keycodes_.erase(Key_C.getKeyCode());

  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 1)
      << "There should be one report in the next cycle";
  EXPECT_THAT(state_->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes_))
      << "The report should be empty";

  sim_.Release(addr_macro_C);
  state_ = RunCycle();

  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 0)
      << "There should be no report after release";
}

TEST_F(MacrosBasic, NonMacrosKey) {

  sim_.Press(addr_X);
  state_ = RunCycle();
  expected_keycodes_.insert(Key_X.getKeyCode());

  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 1)
      << "There should be one HID report";
  EXPECT_THAT(state_->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes_))
      << "The report should include only `X`";

  sim_.Release(addr_X);
  state_ = RunCycle();
  expected_keycodes_.erase(Key_X.getKeyCode());

  ASSERT_EQ(state_->HIDReports()->Keyboard().size(), 1)
      << "There should be one report after letter key release";
  EXPECT_THAT(state_->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes_))
      << "The report should be empty";
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
