/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2019  Keyboard.io, Inc.
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

#ifdef KALEIDOSCOPE_VIRTUAL_BUILD

#include <stdio.h>  // for fprintf, stderr, stdout
#include <utility>  // IWYU pragma: keep
// IWYU pragma: no_include <__utility/forward.h>

namespace kaleidoscope {
namespace logging {

extern void toggleVerboseOutput(bool state);
extern bool verboseOutputEnabled();

// Please note that we prefer stdio based logging against
// stream based output, to stay compatible with possible future global
// logging functions introduced by Kaleidoscope. Stream
// based logging is not available on the target platform due to restricted
// resources.

// Below, we use perfect forwarding to pass printf style log messages
// to different printf derivates. gcc warns when the the format string
// is not a literal, which is not the case here due to our forwarding.
// To silence the related warning, which is not desired in our application,
// we disable the format-security diagnostic until the end of this file.

// Disable diagnostic for literal format strings.
//
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"

#ifdef KALEIDOSCOPE_HARDWARE_VIRTUAL_NO_LOGGING

template<typename... Args__>
inline void log_debug(Args__ &&...args) {}

template<typename... Args__>
inline void log_info(Args__ &&...args) {}

#else  // #ifdef KALEIDOSCOPE_HARDWARE_VIRTUAL_NO_LOGGING

template<typename... Args__>
inline void log_debug(Args__ &&...args) {
  if (verboseOutputEnabled()) {
    fprintf(stdout, std::forward<Args__>(args)...);
  }
}

template<typename... Args__>
inline void log_info(Args__ &&...args) {
  if (verboseOutputEnabled()) {
    fprintf(stdout, std::forward<Args__>(args)...);
  }
}

#endif  // #ifdef KALEIDOSCOPE_HARDWARE_VIRTUAL_NO_LOGGING

template<typename... Args__>
inline void log_error(Args__ &&...args) {
  fprintf(stderr, std::forward<Args__>(args)...);
}

template<typename... Args__>
inline void log_critical(Args__ &&...args) {
  fprintf(stderr, std::forward<Args__>(args)...);
}

}  // namespace logging
}  // namespace kaleidoscope

// Re-enable diagnostic for literal format strings.
//
#pragma GCC diagnostic pop

#endif  // ifdef KALEIDOSCOPE_VIRTUAL_BUILD
