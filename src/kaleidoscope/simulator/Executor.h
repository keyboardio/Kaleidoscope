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

#include "kaleidoscope/simulator/interface/Simulator_.h"

/// @namespace kaleidoscope
///
namespace kaleidoscope {

/// @namespace simulator
///
namespace simulator {

/// @brief A Kaleidoscope specific simulator class.
///
class Executor : public interface::Simulator_ {
 public:

  /// @brief Access the global simulator singleton.
  ///
  static Executor &getInstance();

 private:

  Executor(std::ostream &out);

  static void processHIDReport(uint8_t id, const void* data,
                               int len, int result);
};

} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
