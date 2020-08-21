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

namespace kaleidoscope {
namespace simulator {
namespace interface {
namespace actions {

/// @brief Checks the number of overall reports of a specific type.
/// @details Asserts that there was a specific number of reports
///          generated since testing started.
///
template<typename _ReportType>
class AssertNumOverallReportsEquals {

 public:

  /// @brief Constructor.
  /// @param n_overall_reports The expected overall number of reports
  ///                          of type _ReportType
  ///
  AssertNumOverallReportsEquals(int n_overall_reports)
    : AssertNumOverallReportsEquals(DelegateConstruction{}, n_overall_reports)
  {}

 private:

  class Action : public Action_ {

   public:

    Action(int n_overall_reports)
      : n_overall_reports_(n_overall_reports) {}

    virtual void describe(const char *add_indent = "") const override {
      this->getSimulator()->log() << add_indent << n_overall_reports_ << " overall " << _ReportType::typeString() << " reports expected";
    }

    virtual void describeState(const char *add_indent = "") const {
      this->getSimulator()->log() << add_indent << this->getSimulator()->getNumTypedOverallReports<_ReportType>() << " overall " << _ReportType::typeString() << " reports encountered";
    }

    virtual bool evalInternal() override {
      return this->getSimulator()->getNumTypedOverallReports<_ReportType>() == n_overall_reports_;
    }

   private:

    int n_overall_reports_ = -1;
  };

  SIMULATOR_AUTO_DEFINE_ACTION_INVENTORY_TMPL(AssertNumOverallReportsEquals<_ReportType>)
};

} // namespace actions
} // namespace interface
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
