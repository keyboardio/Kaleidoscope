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

using ::testing::IsEmpty;

class LayerActivationOrder : public VirtualDeviceTest {
 public:
  void SingleKeyTest(Key k) {
    sim_.Press(0, 0); // k
    auto state = RunCycle();

    ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
    EXPECT_THAT(
      state->HIDReports()->Keyboard(0).ActiveKeycodes(),
      Contains(k));

    sim_.Release(0, 0);  // k
    state = RunCycle();

    ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
    EXPECT_THAT(
      state->HIDReports()->Keyboard(0).ActiveKeycodes(),
      IsEmpty());

    state = RunCycle();

    // 2 cycles after releasing k
    EXPECT_EQ(state->HIDReports()->Keyboard().size(), 0);
  }
};

TEST_F(LayerActivationOrder, BaseLayerHasNotRegressed) {
  SingleKeyTest(Key_A);
}

TEST_F(LayerActivationOrder, ShifToLayerOne) {
  // Pressing (3,6) shifts to Layer 1, and we stay there until release.

  sim_.Press(3, 6); // ShiftToLayer(1)
  auto state = RunCycle();

  SingleKeyTest(Key_B);

  // Releasing (3,6) gets us back to the base layer
  sim_.Release(3, 6); // ShiftToLayer(1)
  state = RunCycle();

  SingleKeyTest(Key_A);
}

TEST_F(LayerActivationOrder, ShiftingWithCaching) {
  // Pressing (0, 0) will activate A
  sim_.Press(0, 0);
  auto state = RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  EXPECT_THAT(
    state->HIDReports()->Keyboard(0).ActiveKeycodes(),
    Contains(Key_A));

  // Pressing (3, 6) will switch to Layer 1
  sim_.Press(3, 6);
  state = RunCycle();

  // ...since we're still pressing (0, 0), and there was no change in the HID
  // states, we shouldn't emit a report.
  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 0);

  // Pressing (0, 1), the report shall contain 'A' _and_ 'B'.
  sim_.Press(0, 1);
  state = RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  EXPECT_THAT(
    state->HIDReports()->Keyboard(0).ActiveKeycodes(),
    Contains(Key_A));
  EXPECT_THAT(
    state->HIDReports()->Keyboard(0).ActiveKeycodes(),
    Contains(Key_B));

  // Releasing (0, 0), the report should now contain B only
  sim_.Release(0, 0);
  state = RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  EXPECT_THAT(
    state->HIDReports()->Keyboard(0).ActiveKeycodes(),
    Contains(Key_B));

  // Release (0, 1)
  sim_.Release(0, 1);
  state = RunCycle();

  // Test B in isolation again
  SingleKeyTest(Key_B);

  // Release the layer key as well.
  sim_.Release(3, 6);
  state = RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 0);
}

TEST_F(LayerActivationOrder, Ordering) {
  // Pressing (3, 9) will switch to Layer 2
  sim_.Press(3, 9);
  auto state = RunCycle();

  // Pressing (0, 0) will activate 'C'
  sim_.Press(0, 0);
  state = RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  EXPECT_THAT(
    state->HIDReports()->Keyboard(0).ActiveKeycodes(),
    Contains(Key_C));

  // Pressing (3, 6) will activate Layer 1
  sim_.Press(3, 6);
  state = RunCycle();

  // Pressing (0, 1) will activate 'B' now, due to activation ordering.
  sim_.Press(0, 1);
  state = RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  EXPECT_THAT(
    state->HIDReports()->Keyboard(0).ActiveKeycodes(),
    Contains(Key_C));
  EXPECT_THAT(
    state->HIDReports()->Keyboard(0).ActiveKeycodes(),
    Contains(Key_B));

  sim_.Release(0, 0);
  sim_.Release(0, 1);
  sim_.Release(3, 6);
  sim_.Release(3, 9);
  state = RunCycle();
}

TEST_F(LayerActivationOrder, LayerZero) {
  sim_.Press(3, 7); // Macro#0: Layer.deactivate(0)
  auto state = RunCycle();

  sim_.Press(0, 0); // A
  state = RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  EXPECT_THAT(
    state->HIDReports()->Keyboard(0).ActiveKeycodes(),
    Contains(Key_A));
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
