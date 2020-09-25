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

#include "kaleidoscope/key_defs.h"
#include "testing/SystemControlReport.h"

// Out of order because `fix-macros.h` clears the preprocessor environment for
// gtest and gmock.
#include "testing/fix-macros.h"
#include "gmock/gmock.h"

namespace kaleidoscope {
namespace testing {

MATCHER_P(Contains, key, negation ? "does not contain" : "contains") {
  return arg.Key() == key.getKeyCode();
}

auto Contains(Key key) {
  return ::testing::Contains(key.getKeyCode());
}

}  // namespace testing
}  // namespace kaleidoscope
