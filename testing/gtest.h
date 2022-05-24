/* -*- mode: c++ -*-
 * Copyright (C) 2022  Keyboardio, Inc.
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

#undef TEST

// In order to include gtest files, we need to undefine some macros that
// Arduino.h unwisely exports.  Rahter than including "gtest/gtest.h" (et al)
// directly, any simulator code should instead include "testing/gtest.h".
#undef min
#undef max

// The headers listed here other than "gtest/gtest.h" and "gmock/gmock.h" are
// only included to prevent IWYU from inserting them directly into simulator
// source files.  This seems mildly preferable to modifying the gtest files
// themselves.
#include "gmock/gmock-matchers.h"   // IWYU pragma: export
#include "gmock/gmock.h"            // IWYU pragma: export
#include "gtest/gtest-message.h"    // IWYU pragma: export
#include "gtest/gtest-test-part.h"  // IWYU pragma: export
#include "gtest/gtest.h"            // IWYU pragma: export
#include "gtest/gtest_pred_impl.h"  // IWYU pragma: export
