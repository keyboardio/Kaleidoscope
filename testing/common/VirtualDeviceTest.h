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

#include "HID-Settings.h"
#include "kaleidoscope/sim/Sim.h"
#include "kaleidoscope/testing/observer/Observer.h"

namespace kaleidoscope {
namespace testing {

class VirtualDeviceTest : public ::testing::Test {
 protected:
  void RunCycle() {
    observer_.Clear();
    sim_.RunCycle();
  }
  void RunCycles(size_t n) {
    if (n == 0) return;
    observer_.Clear();
    sim_.RunCycles(n);
  }
  Observer& Result() { return observer_; }

  Sim sim_;

 private:
  Observer observer_;
};

}  // namespace testing
}  // namespace kaleidoscope

#endif  // KALEIDOSCOPE_VIRTUAL_BUILD
