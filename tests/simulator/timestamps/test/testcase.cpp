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
#include "Kaleidoscope.h"

SETUP_GOOGLETEST();

namespace kaleidoscope {
namespace testing {
namespace {

constexpr KeyAddr key_addr_A{2, 1};

class ReportTimestamps : public VirtualDeviceTest {};

TEST_F(ReportTimestamps, Keyboard) {
  int delay        = 10;
  int current_time = 0;

  sim_.RunForMillis(delay);
  current_time += delay;

  sim_.Press(key_addr_A);
  auto state = RunCycle();
  current_time += sim_.CycleTime();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  EXPECT_THAT(state->HIDReports()->Keyboard(0).Timestamp(), current_time)
    << "Report timestamp is the same as the simulator clock as of the end of the last cycle";

  sim_.RunForMillis(delay);
  current_time += delay;

  sim_.Release(key_addr_A);
  state = RunCycle();
  current_time += sim_.CycleTime();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  EXPECT_THAT(state->HIDReports()->Keyboard(0).Timestamp(), current_time)
    << "Report timestamp is the same as the simulator clock as of the end of the last cycle";
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
