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

#include "kaleidoscope/simulator/framework/actions/generic_report/ReportAction.h"
#include "kaleidoscope/simulator/framework/Simulator.h"

namespace kaleidoscope {
namespace simulator {
namespace framework {
namespace actions {

/// @brief Asserts that the current report equals another report.
///
template<typename _ReportType>
class AssertReportEquals {

 public:

  /// @brief Constructor.
  ///
  /// @param report The report to compare with.
  ///
  AssertReportEquals(const std::shared_ptr<_ReportType> &report)
    : AssertReportEquals(DelegateConstruction{}, report)
  {}

  /// @brief Constructor.
  ///
  /// @param data The data of the report to compare with.
  ///
  AssertReportEquals(const void *data)
    : AssertReportEquals(DelegateConstruction{}, data)
  {}

 private:

  class Action : public ReportAction<_ReportType> {

   public:

    Action(const _ReportType &report)
      :  report_(report)
    {}

    Action(const void *data)
      :  report_(_ReportType::create(data))
    {}

    virtual void describe(const char *add_indent = "") const override {
      this->getSimulator()->log() << add_indent << "Report equals: ";
      report_->dump(*this->getSimulator(), add_indent);
    }

    virtual void describeState(const char *add_indent = "") const {

      this->getSimulator()->log() << add_indent << "Reports differ: ";
      this->getSimulator()->log() << add_indent << "expected: ";
      report_->dump(*this->getSimulator(), add_indent);
      this->getSimulator()->log() << add_indent << "actual: ";
      this->getReport().dump(*this->getSimulator(), add_indent);
    }

    virtual bool evalInternal() override {

      return this->getReport().equals(*report_);
    }

   private:

    std::shared_ptr<_ReportType> report_;
  };

  SIMULATOR_AUTO_DEFINE_ACTION_INVENTORY_TMPL(AssertReportEquals<_ReportType>)
};

} // namespace actions
} // namespace framework
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
