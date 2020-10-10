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
  const KeyAddr KEYSWITCH_TOP_LEFT = KeyAddr{0, 0};   // layer-dependent key
  const KeyAddr KEYSWITCH_TOP_RIGHT = KeyAddr{0, 15}; // layer-dependent key
  const KeyAddr KEYSWITCH_LEFT_PALM = KeyAddr{3, 6};  // ShiftToLayer(1)
  const KeyAddr KEYSWITCH_RIGHT_PALM = KeyAddr{3, 9}; // ShiftToLayer(2)
  const KeyAddr KEYSWITCH_LEFT_THUMB_RIGHTMOST = KeyAddr{3, 7}; // L0 deactivate macro

  const Key LAYER0_KEY = Key_0;
  const Key LAYER1_KEY = Key_1;
  const Key LAYER2_KEY = Key_2;

  void pressKeyswitch(const KeyAddr& addr) {
    sim_.Press(addr.row(), addr.col());
  }

  void releaseKeyswitch(const KeyAddr& addr) {
    sim_.Release(addr.row(), addr.col());
  }

  auto pressKeyswitchAndRunCycle(const KeyAddr& addr) {
    pressKeyswitch(addr);
    return RunCycle();
  }

  auto releaseKeyswitchAndRunCycle(const KeyAddr& addr) {
    releaseKeyswitch(addr);
    return RunCycle();
  }

  void assertSingleKeyboardReportContaining(std::unique_ptr<State> &state, Key k) {
    ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
    EXPECT_THAT(
      state->HIDReports()->Keyboard(0).ActiveKeycodes(),
      Contains(k));
  }

  void assertSingleKeyboardReportNotContaining(std::unique_ptr<State> &state, Key k) {
    ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
    EXPECT_THAT(
      state->HIDReports()->Keyboard(0).ActiveKeycodes(),
      ::testing::Not(Contains(k)));
  }

  void assertSingleEmptyReport(std::unique_ptr<State> &state) {
    ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
    EXPECT_THAT(
      state->HIDReports()->Keyboard(0).ActiveKeycodes(),
      IsEmpty());
  }

  void assertNoReport(std::unique_ptr<State> &state) {
    ASSERT_EQ(state->HIDReports()->Keyboard().size(), 0);
  }

  void assertNoReportAfterCycle() {
    auto state = RunCycle();
    assertNoReport(state);
  }

  void TestPressAndRelease(const KeyAddr& addr, Key k) {
    auto state = pressKeyswitchAndRunCycle(addr);
    assertSingleKeyboardReportContaining(state, k);

    state = releaseKeyswitchAndRunCycle(addr);
    assertSingleEmptyReport(state);

    assertNoReportAfterCycle();
  }
};

TEST_F(LayerActivationOrder, BaseLayerHasNotRegressed) {
  TestPressAndRelease(KEYSWITCH_TOP_LEFT, LAYER0_KEY);
}

TEST_F(LayerActivationOrder, ShifToLayerOne) {
  // Pressing (KEYSWITCH_LEFT_PALM) shifts to Layer 1, and we stay there until release.
  auto state = pressKeyswitchAndRunCycle(KEYSWITCH_LEFT_PALM);
  TestPressAndRelease(KEYSWITCH_TOP_LEFT, LAYER1_KEY);

  // Releasing (KEYSWITCH_LEFT_PALM) gets us back to the base layer
  state = releaseKeyswitchAndRunCycle(KEYSWITCH_LEFT_PALM);
  TestPressAndRelease(KEYSWITCH_TOP_LEFT, LAYER0_KEY);
}

TEST_F(LayerActivationOrder, ShiftingWithCaching) {
  // Pressing (KEYSWITCH_TOP_LEFT) will activate the key on layer 0
  auto state = pressKeyswitchAndRunCycle(KEYSWITCH_TOP_LEFT);
  assertSingleKeyboardReportContaining(state, LAYER0_KEY);

  // Pressing (KEYSWITCH_LEFT_PALM) will switch to Layer 1
  state = pressKeyswitchAndRunCycle(KEYSWITCH_LEFT_PALM);

  // ...since we're still pressing (KEYSWITCH_TOP_LEFT), and there was no change
  // in the HID states, we shouldn't emit a report.
  assertNoReport(state);

  // Pressing (KEYSWITCH_TOP_RIGHT), the report shall contain keys from both
  // layer 0 and layer1, because we started holding the layer 0 key prior to
  // switching layers, so it's code should remain cached.
  state = pressKeyswitchAndRunCycle(KEYSWITCH_TOP_RIGHT);
  assertSingleKeyboardReportContaining(state, LAYER0_KEY);
  assertSingleKeyboardReportContaining(state, LAYER1_KEY);

  // Releasing (KEYSWITCH_TOP_LEFT), the report should now contain the key from
  // layer1 only, and should not contain the layer0 key anymore.
  state = releaseKeyswitchAndRunCycle(KEYSWITCH_TOP_LEFT);
  assertSingleKeyboardReportContaining(state, LAYER1_KEY);
  assertSingleKeyboardReportNotContaining(state, LAYER0_KEY);

  // Release (KEYSWITCH_TOP_RIGHT)
  state = releaseKeyswitchAndRunCycle(KEYSWITCH_TOP_RIGHT);

  // Test the layer 1 key in isolation again
  TestPressAndRelease(KEYSWITCH_TOP_LEFT, LAYER1_KEY);

  // Release the layer key as well.
  state = releaseKeyswitchAndRunCycle(KEYSWITCH_LEFT_PALM);

  // Since the layer key release is internal to us, we shouldn't send a report.
  assertNoReport(state);
}

TEST_F(LayerActivationOrder, Ordering) {
  // Pressing (KEYSWITCH_RIGHT_PALM) will switch to Layer 2
  auto state = pressKeyswitchAndRunCycle(KEYSWITCH_RIGHT_PALM);

  // Pressing (KEYSWITCH_TOP_LEFT) will activate a key on layer 2
  state = pressKeyswitchAndRunCycle(KEYSWITCH_TOP_LEFT);
  assertSingleKeyboardReportContaining(state, LAYER2_KEY);

  // Pressing (KEYSWITCH_LEFT_PALM) will activate Layer 1
  state = pressKeyswitchAndRunCycle(KEYSWITCH_LEFT_PALM);

  // Pressing (KEYSWITCH_TOP_RIGHT) will activate the layer 1 key now, due to
  // activation ordering.
  state = pressKeyswitchAndRunCycle(KEYSWITCH_TOP_RIGHT);

  // We should have both the layer 1 and the layer 2 key active, because we're
  // holding both.
  assertSingleKeyboardReportContaining(state, LAYER1_KEY);
  assertSingleKeyboardReportContaining(state, LAYER2_KEY);

  // Releaseing all held keys, we should get an empty report.
  releaseKeyswitch(KEYSWITCH_TOP_LEFT);
  releaseKeyswitch(KEYSWITCH_TOP_RIGHT);
  releaseKeyswitch(KEYSWITCH_LEFT_PALM);
  releaseKeyswitch(KEYSWITCH_RIGHT_PALM);
  state = RunCycle();

  assertSingleEmptyReport(state);

  // One more cycle, and we should generate no report at all
  state = RunCycle();
  assertNoReport(state);
}

TEST_F(LayerActivationOrder, LayerZero) {
  // Pressing the rightmost of the left thumb keys should deactivate layer 0
  auto state = pressKeyswitchAndRunCycle(KEYSWITCH_LEFT_THUMB_RIGHTMOST);

  // Pressing KEYSWITCH_TOP_LEFT should fall back to activating the key on layer 0
  state = pressKeyswitchAndRunCycle(KEYSWITCH_TOP_LEFT);
  assertSingleKeyboardReportContaining(state, LAYER0_KEY);

  // Releasing all keys should generate a single empty report
  releaseKeyswitch(KEYSWITCH_TOP_LEFT);
  releaseKeyswitch(KEYSWITCH_LEFT_THUMB_RIGHTMOST);
  state = RunCycle();

  assertSingleEmptyReport(state);

  // Afterwards, we should generate no more reports.
  state = RunCycle();
  assertNoReport(state);
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
