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
constexpr KeyAddr key_addr_D{2, 3};

using ::testing::IsEmpty;

class Issue941 : public VirtualDeviceTest {};

TEST_F(Issue941, OneKeypressPerCycle) {

  std::unique_ptr<State> state{nullptr};
  std::set<uint8_t> expected_keycodes{};

  // Press `A`
  sim_.Press(key_addr_A);
  expected_keycodes.insert(Key_A.getKeyCode());

  state = VirtualDeviceTest::RunCycle();

  // Press `S`
  sim_.Press(key_addr_S);
  expected_keycodes.insert(Key_S.getKeyCode());

  state = VirtualDeviceTest::RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  EXPECT_THAT(
    state->HIDReports()->Keyboard(0).ActiveKeycodes(),
    ::testing::ElementsAreArray(expected_keycodes)
  );

  // Release `A`
  sim_.Release(key_addr_A);
  expected_keycodes.erase(Key_A.getKeyCode());

  // Release `S`
  sim_.Release(key_addr_S);
  expected_keycodes.erase(Key_S.getKeyCode());

  // Run one cycle with two keys toggled off
  state = VirtualDeviceTest::RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  EXPECT_THAT(
    state->HIDReports()->Keyboard(0).ActiveKeycodes(),
    IsEmpty()
  );
}

TEST_F(Issue941, SimultaneousKeypresses) {

  std::unique_ptr<State> state{nullptr};
  std::set<uint8_t> expected_keycodes{};

  // Press `A`
  sim_.Press(key_addr_A);
  expected_keycodes.insert(Key_A.getKeyCode());

  // state = VirtualDeviceTest::RunCycle();

  // Press `S`
  sim_.Press(key_addr_S);
  expected_keycodes.insert(Key_S.getKeyCode());

  // Press `D`
  sim_.Press(key_addr_D);
  expected_keycodes.insert(Key_D.getKeyCode());

  // Run one cycle with two keys toggled on
  state = VirtualDeviceTest::RunCycle();

  EXPECT_THAT(state->HIDReports()->Keyboard().size(), ::testing::Ge(1));
  int n = state->HIDReports()->Keyboard().size();
  EXPECT_THAT(
    state->HIDReports()->Keyboard(n - 1).ActiveKeycodes(),
    ::testing::ElementsAreArray(expected_keycodes)
  );

  // Release `A`
  sim_.Release(key_addr_A);
  expected_keycodes.erase(Key_A.getKeyCode());

  // Release `S`
  sim_.Release(key_addr_S);
  expected_keycodes.erase(Key_S.getKeyCode());

  // Release `D`
  sim_.Release(key_addr_D);
  expected_keycodes.erase(Key_D.getKeyCode());

  // Run one cycle with two keys toggled off
  state = VirtualDeviceTest::RunCycle();

  EXPECT_THAT(
    state->HIDReports()->Keyboard(0).ActiveKeycodes(),
    IsEmpty()
  );
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
