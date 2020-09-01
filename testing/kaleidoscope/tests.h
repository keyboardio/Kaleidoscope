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

#ifdef KALEIDOSCOPE_VIRTUAL_BUILD

#pragma once

#include "setup-googletest.h"

#include "kaleidoscope/key_defs_keyboard.h"
#include "../common/matchers.h"
#include "../common/VirtualDeviceTest.h"

SETUP_GOOGLETEST();

namespace kaleidoscope {
namespace testing {
namespace {

using ::testing::IsEmpty;

class KeyboardReports : public VirtualDeviceTest {};

TEST_F(KeyboardReports, KeysActiveWhenPressed) {
  sim_.Press(2, 1); // A
  RunCycle();

  EXPECT_EQ(Result().KeyboardReports().size(), 1);
  EXPECT_THAT(
      Result().KeyboardReports(0).ActiveKeyCodes(),
      ContainsKey(Key_A));

  sim_.Release(2, 1);  // A
  RunCycles(2);

  EXPECT_THAT(Result().KeyboardReports().size(), IsEmpty());
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope

#endif  // KALEIDOSCOPE_VIRTUAL_BUILD
