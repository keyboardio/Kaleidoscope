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

#undef min
#undef max
#undef T
#undef U

#include "fake-gtest.h"
#include "gtest/gtest.h"

#include "Kaleidoscope-Simulator.h"

void executeTestFunction() {
  setup(); /* setup Kaleidoscope */
  testing::InitGoogleTest();
  RUN_ALL_TESTS();
}

namespace kaleidoscope {
namespace simulator {
namespace {

using namespace actions;
using namespace interface;
using namespace interface::actions;

class SimulatorTest : public ::testing::Test {
 protected:
  void SetUp() {
    sim_ = &Simulator::getInstance();
  }

  Simulator* sim_;
};

class KeyboardReports : public SimulatorTest {};

TEST_F(KeyboardReports, ActiveKeycodesAreAccurate) {
  // Assert that the next cycle generates exactly one keyboard report.
  //
  sim_->cycleActionsQueue().queue(AssertCycleGeneratesNReports<KeyboardReport_> {1});

  sim_->tapKey(2, 1); // A
  sim_->cycleExpectReports(AssertKeycodesActive{Key_A});

  sim_->cycleExpectReports(AssertReportEmpty{});
}

TEST(Test, ThatPasses) {
  EXPECT_TRUE(true);
}

TEST(Test, ThatFails) {
  EXPECT_TRUE(false);
}

}  // namespace
}  // namespace simulator
}  // namespace kaleidoscope

#endif  // KALEIDOSCOPE_VIRTUAL_BUILD
