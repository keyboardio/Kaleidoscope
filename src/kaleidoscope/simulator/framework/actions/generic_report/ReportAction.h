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

#include "kaleidoscope/simulator/framework/actions/Action_.h"
#include "kaleidoscope/simulator/framework/reports/Report_.h"

#include <cassert>

namespace kaleidoscope {
namespace simulator {
namespace framework {

/// @brief An abstract report action.
/// @details This abstract class serves as base class for any
///        report actions.
///
///        **Important:** This class is not part of Papilio's
///                   public API. It is meant for internal use only.
///
class ReportAction_ : public Action_ {
 public:

  typedef Report_ ReportType;
  typedef ReportAction_ ActionBaseType;

  virtual uint8_t getReportTypeId() const {
    return AnyTypeReportTypeId;
  }

  virtual void setReport(const Report_ *report) override {
    report_ = report;
  }

 protected:

  virtual const Report_ &getReport() const {
    assert(report_);
    return *report_;
  }

 protected:

  const Report_ *report_ = nullptr;
};

/// @brief A report action.
///
///        **Important:** This class is not part of Papilio's
///                   public API. It is meant for internal use only.
///
template<typename _ReportType>
class ReportAction : public ReportAction_ {
 public:

  typedef _ReportType ReportType;

  virtual uint8_t getReportTypeId() const override {
    return _ReportType::type_;
  }

  static const char *typeString() {
    return _ReportType::typeString();
  }

  virtual const char *getTypeString() const override {
    return _ReportType::typeString();
  }

 protected:

  virtual const _ReportType &getReport() const override {
    assert(report_);
    return *static_cast<const _ReportType *>(report_);
  }
};

} // namespace framework
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
