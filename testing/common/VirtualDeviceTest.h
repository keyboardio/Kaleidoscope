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

#pragma once

#include <cstddef>

#include "testing/common/SimHarness.h"
#include "testing/common/State.h"

// Out of order due to macro conflicts.
#include "testing/common/fix-macros.h"
#include "gtest/gtest.h"
#include <memory>

namespace kaleidoscope {
namespace testing {

class VirtualDeviceTest : public ::testing::Test {
 protected:
  void SetUp();

  std::unique_ptr<State> RunCycle();

  SimHarness sim_;
};

}  // namespace testing
}  // namespace kaleidoscope
