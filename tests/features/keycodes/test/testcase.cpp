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

constexpr KeyAddr key_addr_A{2, 1};
constexpr KeyAddr key_addr_S{2, 2};
constexpr KeyAddr key_addr_LeftShift{3, 7};
constexpr uint8_t keycode_LeftShift{Key_LeftShift.getKeyCode()};

class Keycodes : public VirtualDeviceTest {};

TEST_F(Keycodes, KeyboardNonModifier) {

  std::set<uint8_t> expected_keycodes{};

  // Press `A`
  sim_.Press(key_addr_A);
  expected_keycodes.insert(Key_A.getKeyCode());

  auto state = VirtualDeviceTest::RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  EXPECT_THAT(state->HIDReports()->Keyboard(0).ActiveNonModifierKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes));
  EXPECT_THAT(state->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes));

  sim_.Release(key_addr_A);
  expected_keycodes.erase(Key_A.getKeyCode());

  state = VirtualDeviceTest::RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  EXPECT_THAT(state->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes));
}

TEST_F(Keycodes, KeyboardModifier) {

  std::set<uint8_t> expected_keycodes{};

  // Press `LeftShift`
  sim_.Press(key_addr_LeftShift);
  expected_keycodes.insert(keycode_LeftShift);

  auto state = VirtualDeviceTest::RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  EXPECT_THAT(state->HIDReports()->Keyboard(0).ActiveModifierKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes));
  EXPECT_THAT(state->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes));

  uint8_t bit_LeftShift = keycode_LeftShift - HID_KEYBOARD_FIRST_MODIFIER;
  uint8_t expected_modifiers = 1 << bit_LeftShift;

  sim_.Release(key_addr_LeftShift);
  expected_keycodes.erase(Key_LeftShift.getKeyCode());

  state = VirtualDeviceTest::RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  EXPECT_THAT(state->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes));
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
