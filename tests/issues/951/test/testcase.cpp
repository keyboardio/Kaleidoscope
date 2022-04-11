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

#include "Kaleidoscope.h"

#include "testing/setup-googletest.h"

SETUP_GOOGLETEST();

namespace kaleidoscope {
namespace testing {
namespace {

using ::testing::IsEmpty;

class Issue951 : public VirtualDeviceTest {};

TEST_F(Issue951, InitialLayerState) {
  EXPECT_THAT(Layer.isActive(0), true)
    << "Layer 0 should be active when the keyboard starts up.";
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
