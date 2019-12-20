#pragma once

#define KALEIDOSCOPE_AVR_GCC_MINIMAL_VERSION 5
#define KALEIDOSCOPE_AVR_GCC_MINIMAL_MINOR 4

#if   (__GNUC__  < KALEIDOSCOPE_AVR_GCC_MINIMAL_VERSION) \
  ||  (    (__GNUC__ == KALEIDOSCOPE_AVR_GCC_MINIMAL_VERSION) \
        && (__GNUC_MINOR__ < KALEIDOSCOPE_AVR_GCC_MINIMAL_MINOR) \
      )
#error Kaleidoscope requires Arduino version >= 1.8.6 to build. Please upgrade.
#endif
