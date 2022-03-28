/* -*- mode: c++ -*-
 * Copyright (C) 2020  Eric Paniagua (epaniagua@google.com)
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

class KeyboardReports : public VirtualDeviceTest {};

TEST_F(KeyboardReports, HIDUsageTablev12KeycodesAdded) {
  sim_.Press(0, 0);  // VoiceCommand
  auto state = RunCycle();

  ASSERT_EQ(state->HIDReports()->ConsumerControl().size(), 1);
  EXPECT_THAT(
    state->HIDReports()->ConsumerControl(0).ActiveKeycodes(),
    Contains(Consumer_VoiceCommand));

  sim_.Release(0, 0);  // VoiceCommand
  state = RunCycle();

  ASSERT_EQ(state->HIDReports()->ConsumerControl().size(), 1);
  EXPECT_THAT(
    state->HIDReports()->ConsumerControl(0).ActiveKeycodes(),
    IsEmpty());

  state = RunCycle();

  // 2 cycles after releasing the key
  EXPECT_EQ(state->HIDReports()->ConsumerControl().size(), 0);
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
