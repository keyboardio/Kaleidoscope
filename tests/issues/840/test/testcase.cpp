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

class Issue840 : public VirtualDeviceTest {};

TEST_F(Issue840, HasNotRegressed) {
  sim_.Press(2, 1);  // Press System_PowerDown
  auto state = RunCycle();

  ASSERT_EQ(state->HIDReports()->SystemControl().size(), 1);
  EXPECT_THAT(state->HIDReports()->SystemControl(0), Contains(System_PowerDown));

  sim_.Press(3, 5);  // Press System_Sleep
  state = RunCycle();

  ASSERT_EQ(state->HIDReports()->SystemControl().size(), 1);
  EXPECT_THAT(state->HIDReports()->SystemControl(0), Contains(System_Sleep));

  sim_.Release(2, 1);  // Release System_PowerDown
  state = RunCycle();

  EXPECT_EQ(state->HIDReports()->SystemControl().size(), 0);

  sim_.Release(3, 5);  // Release System_Sleep
  state = RunCycle();

  ASSERT_EQ(state->HIDReports()->SystemControl().size(), 1);
  EXPECT_THAT(state->HIDReports()->SystemControl(0), IsEmpty());

  state = RunCycle();

  EXPECT_EQ(state->HIDReports()->SystemControl().size(), 0);
}

// TEST_F(Issue840, Reproduces) {
//   sim_.Press(2, 1); // Press System_PowerDown
//   auto state = RunCycle();
//
//   ASSERT_EQ(state->SystemControlReports().size(), 1);
//   EXPECT_THAT(state->SystemControlReports(0), Contains(System_PowerDown));
//
//   sim_.Press(3, 5); // Press System_Sleep
//   state = RunCycle();
//
//   ASSERT_EQ(state->SystemControlReports().size(), 1);
//   EXPECT_THAT(state->SystemControlReports(0), Contains(System_Sleep));
//
//   sim_.Release(2, 1);  // Release System_PowerDown
//   state = RunCycle();
//
//   ASSERT_EQ(state->SystemControlReports().size(), 1);
//   EXPECT_THAT(state->SystemControlReports(0), IsNull());
//
//   sim_.Release(3, 5);  // Release System_Sleep
//   state = RunCycle();
//
//   ASSERT_EQ(state->SystemControlReports().size(), 1);
//   EXPECT_THAT(state->SystemControlReports(0), IsNull());
//
//   state = RunCycle();
//
//   EXPECT_EQ(state->SystemControlReports().size(), 0);
// }

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
