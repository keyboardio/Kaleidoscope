/* -*- mode: c++ -*-
 * kaleidoscope::simulator - Testing API for Kaleidoscope
 * Copyright (C) 2019  noseglasses (shinynoseglasses@gmail.com)
 * Copyright (C) 2020  Keyboard.io, Inc
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

#include <sstream>
#include <exception>

namespace kaleidoscope {
namespace simulator {
namespace executor {

/// @private
///
struct OStringStreamWrapper {
  template<typename _T>
  OStringStreamWrapper &operator<<(const _T &t) {
    osstream_ << t;
    return *this;
  }

  operator const char *() {
    return osstream_.str().c_str();
  }

  std::ostringstream osstream_;
};

#define KS_T_EXCEPTION(...)                                                    \
   throw std::runtime_error(OStringStreamWrapper() << __VA_ARGS__);

} // namespace executor
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
