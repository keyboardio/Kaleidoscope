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

#include "kaleidoscope/simulator/interface/actions/Action_.h"
#include "kaleidoscope/simulator/interface/Simulator_.h"

namespace kaleidoscope {
namespace simulator {
namespace interface {
namespace actions {

/// @brief Asserts that that time that elapsed is greater than a given time in [ms].
///
class AssertElapsedTimeGreater {

 public:

  /// @brief Constructor.
  /// @param delta_t The amount of time that is asserted being elapsed.
  /// @param start_t An optional start point in time as reference (defaults to zero).
  ///
  AssertElapsedTimeGreater(Simulator_::TimeType delta_t, Simulator_::TimeType start_t = 0)
    :  AssertElapsedTimeGreater(DelegateConstruction{}, delta_t, start_t)
  {}

 private:

  class Action : public Action_ {

   public:

    Action(Simulator_::TimeType delta_t, Simulator_::TimeType start_t = 0)
      :  start_t_(start_t),
         delta_t_(delta_t)
    {}

    virtual void describe(const char *add_indent = "") const override {
      this->getSimulator()->log() << add_indent << "Time elapsed greater " << delta_t_ << " ms";
    }

    virtual void describeState(const char *add_indent = "") const {
      this->getSimulator()->log() << add_indent << "Actual time elapsed "
                                  << this->getSimulator()->getTime() << " ms";
    }

    virtual bool evalInternal() override {
      return this->getSimulator()->getTime() - start_t_ > delta_t_;
    }

   private:

    Simulator_::TimeType start_t_ = .0;
    Simulator_::TimeType delta_t_ = .0;
  };

  SIMULATOR_AUTO_DEFINE_ACTION_INVENTORY(AssertElapsedTimeGreater)
};

} // namespace actions
} // namespace interface
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
