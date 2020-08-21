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

#ifdef KALEIDOSCOPE_VIRTUAL_BUILD

#include "kaleidoscope/simulator/executor/reports/AbsoluteMouseReport.h"
#include "kaleidoscope/simulator/interface/Simulator.h"

namespace kaleidoscope {
namespace simulator {
namespace executor {

AbsoluteMouseReport::AbsoluteMouseReport()
  :  report_data_{} {
}

AbsoluteMouseReport::AbsoluteMouseReport(const AbsoluteMouseReport::ReportDataType &report_data) {
  this->setReportData(report_data);
}

AbsoluteMouseReport &AbsoluteMouseReport::operator=(const AbsoluteMouseReport &other) {
  if (this == &other) {
    return *this;
  }

  this->setReportData(other.report_data_);

  return *this;
}

AbsoluteMouseReport::AbsoluteMouseReport(const void *data) {
  const ReportDataType &report_data
    = *static_cast<const ReportDataType *>(data);

  this->setReportData(report_data);
}

std::shared_ptr<interface::Report_> AbsoluteMouseReport::clone() const {
  return std::shared_ptr<interface::Report_> { new AbsoluteMouseReport{*this} };
}

bool AbsoluteMouseReport::equals(const interface::Report_ &other) const {
  const AbsoluteMouseReport *other_amr =
    dynamic_cast<const AbsoluteMouseReport *>(&other);

  if (!other_amr) {
    return false;
  }

  return memcmp(&report_data_, &other_amr->report_data_, sizeof(report_data_)) == 0;
}

bool AbsoluteMouseReport::areButtonsPressed(uint8_t button_state) const {
  return report_data_.buttons == button_state;
}

bool AbsoluteMouseReport::isLeftButtonPressed() const {
  return report_data_.buttons & MOUSE_LEFT;
}

bool AbsoluteMouseReport::isMiddleButtonPressed() const {
  return report_data_.buttons & MOUSE_MIDDLE;
}

bool AbsoluteMouseReport::isRightButtonPressed() const {
  return report_data_.buttons & MOUSE_RIGHT;
}

uint16_t AbsoluteMouseReport::getXPosition() const {
  return report_data_.xAxis;
}

uint16_t AbsoluteMouseReport::getYPosition() const {
  return report_data_.yAxis;
}

int8_t AbsoluteMouseReport::getVerticalWheel() const {
  return report_data_.wheel;
}

int8_t AbsoluteMouseReport::getHorizontalWheel() const {
  return 0;
}

bool AbsoluteMouseReport::isEmpty() const {
  return (report_data_.buttons == 0)
         && (report_data_.xAxis == 0)
         && (report_data_.yAxis == 0)
         && (report_data_.wheel == 0);
}

void AbsoluteMouseReport::dump(const interface::Simulator &simulator, const char *add_indent) const {
  simulator.log() << add_indent << "Absolute mouse report content:";
  simulator.log() << add_indent << "  left button: " << this->isLeftButtonPressed();
  simulator.log() << add_indent << "  middle button: " << this->isMiddleButtonPressed();
  simulator.log() << add_indent << "  right button: " << this->isRightButtonPressed();
  simulator.log() << add_indent << "  x-axis position: " << (int)this->getXPosition();
  simulator.log() << add_indent << "  y-axis position: " << (int)this->getYPosition();
  simulator.log() << add_indent << "  wheel position: " << (int)this->getVerticalWheel();
}

void AbsoluteMouseReport::setReportData(const AbsoluteMouseReport::ReportDataType &report_data) {
  memcpy(&report_data_, &report_data, sizeof(report_data_));
}

} // namespace executor
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
