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

#include "kaleidoscope/key_defs_keyboard.h"

// Out of order because `fix-macros.h` clears the preprocessor environment for
// gtest and gmock.
#include "testing/common/fix-macros.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <iostream>

#include "testing/common/matchers.h"
#include "testing/common/VirtualDeviceTest.h"

SETUP_GOOGLETEST();

namespace kaleidoscope {
namespace testing {
namespace {

using ::testing::IsEmpty;

class KeyboardReports : public VirtualDeviceTest {};

TEST_F(KeyboardReports, KeysActiveWhenPressed) {
  sim_.Press(2, 1); // A
  auto state = RunCycle();

  ASSERT_EQ(state->KeyboardReports().size(), 1);
  EXPECT_THAT(
      state->KeyboardReports(0).ActiveKeycodes(),
      ContainsKey(Key_A));

  sim_.Release(2, 1);  // A
  state = RunCycle();

  ASSERT_EQ(state->KeyboardReports().size(), 1);
  EXPECT_THAT(
      state->KeyboardReports(0).ActiveKeycodes(),
      IsEmpty());

  state = RunCycle();  // 2 cycles later

  EXPECT_EQ(state->KeyboardReports().size(), 0);
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
