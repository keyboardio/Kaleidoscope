/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2018  Keyboard.io, Inc.
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

// clang-format off

#pragma once

#include "kaleidoscope/macro_helpers.h"  // for __NL__

// Please note that due to a bug in older gcc versions the following
// warning suppressions mechanism does not work for gcc versions below 6.1.

//******************************************************************************
// Start for internal use only
//******************************************************************************
#define _GCC_DIAG_STR(s) #s
#define _GCC_DIAG_JOINSTR(x,y) _GCC_DIAG_STR(x ## y)
#define _GCC_DIAG_DO_PRAGMA(x) _Pragma (#x)
#define _GCC_DIAG_PRAGMA(x) _GCC_DIAG_DO_PRAGMA(GCC diagnostic x)
//******************************************************************************
// End for internal use only
//******************************************************************************

//******************************************************************************
// Use the following macros to temporarily silence compiler diagnostics (warnings).
// This enables suppress only those compiler warnings that are safe to
// silence without using global flags as -Wno...
//
// Use e.g. as
//
#if 0 // Not part of the example

// Suppress Warnings for unused variables
COMPILER_DIAG_OFF(unused - variable)

// Place code here that generates unavoidable unused variables

COMPILER_DIAG_ON(unused - variable)
// From this point onward, the compiler continues to warn for
// unused variables.

#endif // Not part of the example

//******************************************************************************
#define COMPILER_DIAG_OFF(x) _GCC_DIAG_PRAGMA(push)      __NL__ \
            _GCC_DIAG_PRAGMA(ignored _GCC_DIAG_JOINSTR(-W,x))
#define COMPILER_DIAG_ON(x) _GCC_DIAG_PRAGMA(pop)
