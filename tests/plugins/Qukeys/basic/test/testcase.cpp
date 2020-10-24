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

#include "../sketch.h"

SETUP_GOOGLETEST();

namespace kaleidoscope {
namespace testing {
namespace {

struct QukeyValues {
  Key primary{Key_NoKey};
  Key alternate{Key_NoKey};
};
constexpr KeyAddr key_addr_A{2, 1};
constexpr KeyAddr key_addr_S{2, 2};
constexpr KeyAddr key_addr_D{2, 3};
constexpr KeyAddr key_addr_F{2, 4};
constexpr KeyAddr key_addr_X{3, 2};

using ::testing::IsEmpty;

class QukeysBasic : public VirtualDeviceTest {};

TEST_F(QukeysBasic, TapQukeyAlone) {

  std::unique_ptr<State> state{nullptr};
  std::set<uint8_t> expected_keycodes{};

  // Press `A`
  sim_.Press(key_addr_A);

  state = VirtualDeviceTest::RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 0);

  // Release `A`
  sim_.Release(key_addr_A);
  expected_keycodes.insert(Key_A.getKeyCode());

  state = VirtualDeviceTest::RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  EXPECT_THAT(state->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes));

  expected_keycodes.erase(Key_A.getKeyCode());

  sim_.RunCycles(2);
  state = VirtualDeviceTest::RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  EXPECT_THAT(state->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes));

  state = VirtualDeviceTest::RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 0);
}

TEST_F(QukeysBasic, HoldQukeyAlone) {

  std::unique_ptr<State> state{nullptr};
  std::set<uint8_t> expected_keycodes{};

  // Press `A`
  sim_.Press(key_addr_A);

  state = VirtualDeviceTest::RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 0);

  uint32_t t0 = Kaleidoscope.millisAtCycleStart();

  do {
    state = VirtualDeviceTest::RunCycle();
    if (Kaleidoscope.millisAtCycleStart() > t0 + QUKEYS_HOLD_TIMEOUT)
      break;
  } while (state->HIDReports()->Keyboard().size() == 0);

  uint32_t t1 = Kaleidoscope.millisAtCycleStart();

  EXPECT_THAT(t1 - t0, ::testing::Ge(QUKEYS_HOLD_TIMEOUT));

  expected_keycodes.insert(Key_LeftGui.getKeyCode());

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  EXPECT_THAT(state->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes));

  sim_.Release(key_addr_A);
  expected_keycodes.erase(Key_LeftGui.getKeyCode());

  sim_.RunForMillis(100);
  state = VirtualDeviceTest::RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  EXPECT_THAT(state->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes));
}

TEST_F(QukeysBasic, FullOverlap) {

  std::set<uint8_t> expected_keycodes{};

  sim_.Press(key_addr_F);
  sim_.RunForMillis(20);
  sim_.Press(key_addr_X);
  auto state = RunCycle();
  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 0);

  sim_.RunForMillis(50);
  sim_.Release(key_addr_X);
  state = RunCycle();
  expected_keycodes.insert(Key_LeftShift.getKeyCode());
  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  EXPECT_THAT(state->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes));
  sim_.RunCycles(3);
  state = RunCycle();
  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 2);
  expected_keycodes.insert(Key_X.getKeyCode());
  EXPECT_THAT(state->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes));
  expected_keycodes.erase(Key_X.getKeyCode());
  EXPECT_THAT(state->HIDReports()->Keyboard(1).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes));
  sim_.Release(key_addr_F);
  sim_.RunCycles(3);
  state = RunCycle();
  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  expected_keycodes.erase(Key_LeftShift.getKeyCode());
  EXPECT_THAT(state->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes));
}

TEST_F(QukeysBasic, RolloverPrimary) {

  std::set<uint8_t> expected_keycodes{};

  sim_.Press(key_addr_F);
  sim_.RunForMillis(20);
  sim_.Press(key_addr_X);
  auto state = RunCycle();
  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 0);

  sim_.RunForMillis(50);
  sim_.Release(key_addr_F);
  sim_.RunForMillis(50);
  state = RunCycle();
  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 3);

  expected_keycodes.insert(Key_F.getKeyCode());
  EXPECT_THAT(state->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes));

  expected_keycodes.insert(Key_X.getKeyCode());
  EXPECT_THAT(state->HIDReports()->Keyboard(1).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes));

  expected_keycodes.erase(Key_F.getKeyCode());
  EXPECT_THAT(state->HIDReports()->Keyboard(2).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes));

  sim_.Release(key_addr_X);
  sim_.RunCycles(3);
  state = RunCycle();
  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  expected_keycodes.erase(Key_X.getKeyCode());
  EXPECT_THAT(state->HIDReports()->Keyboard(0).ActiveKeycodes(),
              ::testing::ElementsAreArray(expected_keycodes));
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
