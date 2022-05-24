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

// For some reason, the `MATCHER_P` macro confuses IWYU into trying to include
// this file in itself, so we need to block it with the following:
// IWYU pragma: no_include "testing/matchers.h"

#include "kaleidoscope/key_defs.h"  // for Key
#include "testing/gtest.h"          // for GMOCK_PP_INTERNAL_FOR_EACH_IMPL_1, GMOCK_PP_INTERNAL_...

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
