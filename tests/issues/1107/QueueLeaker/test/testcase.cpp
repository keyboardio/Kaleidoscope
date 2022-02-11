/* -*- mode: c++ -*-
 * Copyright (C) 2022  Keyboard.io, Inc.
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

constexpr uint32_t t_0{0};
constexpr uint32_t t_1{1};
constexpr uint32_t t_10{10};

using ::testing::IsEmpty;

class KeyAddrEventQueueLeak : public VirtualDeviceTest {
 public:
  void assertTimeElapses(uint32_t interval) {
    // Record time at start
    uint32_t start = Kaleidoscope.millisAtCycleStart();

    // Run cycles until the given amount of time has elapsed
    sim_.RunForMillis(interval);

    // Record time at end
    uint32_t end = Kaleidoscope.millisAtCycleStart();

    ASSERT_EQ((end - start), interval);
  }
};

TEST_F(KeyAddrEventQueueLeak, TimeIsConsistent) {
  // Basic test every cycle
  for (int i = 0; i < 10; ++i)
    assertTimeElapses(1);

  // Guarantee underflow in case of signed integer length
  for (int i = 0; i < 50; ++i)
    assertTimeElapses(10);
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
