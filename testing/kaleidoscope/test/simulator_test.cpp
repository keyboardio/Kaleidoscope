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
  std::cout << "epan 1" << std::endl;
  sim_.Press(2, 1); // A
  std::cout << "epan 2" << std::endl;
  RunCycle();
  std::cout << "epan 3" << std::endl;

  EXPECT_EQ(Result().KeyboardReports().size(), 1);
  std::cout << "epan 4" << std::endl;
  EXPECT_THAT(
      Result().KeyboardReports(0).ActiveKeycodes(),
      ContainsKey(Key_A));
  std::cout << "epan 5" << std::endl;

  sim_.Release(2, 1);  // A
  std::cout << "epan 6" << std::endl;
  RunCycles(2);
  std::cout << "epan 7" << std::endl;

  std::cout << "epan 8" << std::endl;
  EXPECT_THAT(Result().KeyboardReports(), IsEmpty());
  std::cout << "epan 9" << std::endl;
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
