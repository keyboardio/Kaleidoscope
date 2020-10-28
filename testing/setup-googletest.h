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

// NOTE: This should always be the last header file included in test source files.

#pragma once

#include "kaleidoscope/key_defs_keyboard.h"
#include "Kaleidoscope.h"

// Out of order because `fix-macros.h` clears the preprocessor environment for
// gtest and gmock.
#include "testing/fix-macros.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "testing/matchers.h"
#include "testing/VirtualDeviceTest.h"

#define SETUP_GOOGLETEST()                                              \
  void executeTestFunction() {                                          \
    setup(); /* setup Kaleidoscope */                                   \
    /* Turn off virtual_io's input. */                                  \
    Kaleidoscope.device().keyScanner().setEnableReadMatrix(false);      \
    testing::InitGoogleTest();                                          \
    int result = RUN_ALL_TESTS();                                       \
    exit(result);                                                       \
  }
