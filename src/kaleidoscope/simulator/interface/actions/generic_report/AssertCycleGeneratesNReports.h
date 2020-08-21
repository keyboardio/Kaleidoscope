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

/// @brief Asserts that there was a specific number of reports of a given
///        type generated within a specific scan cycle.
///
template<typename _ReportType>
class AssertCycleGeneratesNReports {

 public:

  /// @brief Constructor.
  /// @param n_reports The number of reports that must have been
  ///        generated.
  ///
  AssertCycleGeneratesNReports(int n_reports)
    : AssertCycleGeneratesNReports(DelegateConstruction{}, n_reports)
  {}

 private:

  class Action : public Action_ {

   public:

    Action(int n_reports) : n_reports_(n_reports) {}

    virtual void describe(const char *add_indent = "") const override {
      this->getSimulator()->log() << add_indent << n_reports_ << " keyboard reports expected in cycle";
    }

    virtual void describeState(const char *add_indent = "") const {
      this->getSimulator()->log() << add_indent << this->getSimulator()->getNumTypedReportsInCycle<_ReportType>() << " keyboard reports encountered";
    }

    virtual bool evalInternal() override {
      return this->getSimulator()->getNumTypedReportsInCycle<_ReportType>() == n_reports_;
    }

   private:

    int n_reports_ = -1;
  };

  SIMULATOR_AUTO_DEFINE_ACTION_INVENTORY_TMPL(AssertCycleGeneratesNReports<_ReportType>)
};

} // namespace actions
} // namespace interface
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
