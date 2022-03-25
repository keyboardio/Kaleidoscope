// -*- mode: c++ -*-

// clang-format off

#pragma once

// We use `include-what-you-use`, which uses `clang` to help manage header
// includes.  If we don't guard this file with an #ifdef, it will cause that to
// fail on any file that includes this header.
#ifndef __clang__

#define KALEIDOSCOPE_AVR_GCC_MINIMAL_VERSION 5
#define KALEIDOSCOPE_AVR_GCC_MINIMAL_MINOR 4

#if   (__GNUC__  < KALEIDOSCOPE_AVR_GCC_MINIMAL_VERSION) \
  ||  (    (__GNUC__ == KALEIDOSCOPE_AVR_GCC_MINIMAL_VERSION) \
        && (__GNUC_MINOR__ < KALEIDOSCOPE_AVR_GCC_MINIMAL_MINOR) \
      )
#error Kaleidoscope requires Arduino version >= 1.8.6 to build. Please upgrade.
#endif

#endif // #ifndef __clang__
