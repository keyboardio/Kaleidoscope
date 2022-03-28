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

constexpr uint32_t t_0{0};
constexpr uint32_t t_1{1};
constexpr uint32_t t_100{100};
constexpr uint32_t t_500{500};
constexpr uint32_t t_100_000{100000};

using ::testing::IsEmpty;

class SimulatorTiming : public VirtualDeviceTest {
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

TEST_F(SimulatorTiming, SimulatorStart) {
  // Time doesn't advance in the simulator until after the first cycle runs
  ASSERT_EQ(Kaleidoscope.millisAtCycleStart(), t_0) << "Clock is at 0 ms before we do anything";
  RunCycle();
  ASSERT_EQ(Kaleidoscope.millisAtCycleStart(), t_1) << "Clock is at 1 ms after the first cycle";
}

TEST_F(SimulatorTiming, TimeElapses) {
  assertTimeElapses(t_1);
  assertTimeElapses(t_100);
  assertTimeElapses(t_500);
}

TEST_F(SimulatorTiming, ZeroTimeElapses) {
  assertTimeElapses(t_0);
}

TEST_F(SimulatorTiming, LongTimeElapses) {
  // Bigger number than uint16_t can hold
  assertTimeElapses(t_100_000);
}

TEST_F(SimulatorTiming, 3msPerCycleTestRunCycles) {


  sim_.SetCycleTime(3);
  // Record time at start
  uint32_t start = Kaleidoscope.millisAtCycleStart();
  // Run cycles until the given amount of time has elapsed
  sim_.RunCycles(1);
  // Record time at end
  uint32_t end = Kaleidoscope.millisAtCycleStart();
  ASSERT_EQ((end - start), 3) << "Start was " << start << " and end was " << end;


  // Record time at start
  start = Kaleidoscope.millisAtCycleStart();
  // Run cycles until the given amount of time has elapsed
  sim_.RunCycles(10);
  // Record time at end
  end = Kaleidoscope.millisAtCycleStart();
  ASSERT_EQ((end - start), 30) << "Start was " << start << " and end was " << end;
}


TEST_F(SimulatorTiming, 4msPerCycleTestRunForMillis) {
  sim_.SetCycleTime(4);
  // Record time at start
  uint32_t start = Kaleidoscope.millisAtCycleStart();
  // Run cycles until the given amount of time has elapsed
  sim_.RunForMillis(40);
  // Record time at end
  uint32_t end = Kaleidoscope.millisAtCycleStart();
  ASSERT_EQ((end - start), 40);
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
