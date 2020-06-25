/* -*- mode: c++ -*-
 * kaleidoscope::simulator::executor -- A C++ testing API for the Kaleidoscope keyboard firmware.
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

#include "kaleidoscope/simulator/executor/reports/MouseReport.h"

#include "kaleidoscope/simulator/framework/Simulator.h"

namespace kaleidoscope {
namespace simulator {
namespace executor {

MouseReport::MouseReport()
  :  report_data_{} {
}

MouseReport::MouseReport(const HID_MouseReport_Data_t &report_data) {
  this->setReportData(report_data);
}

MouseReport::MouseReport(const void *data) {
  const ReportDataType &report_data
    = *static_cast<const ReportDataType *>(data);

  this->setReportData(report_data);
}

std::shared_ptr<framework::Report_> MouseReport::clone() const {
  return std::shared_ptr<framework::Report_> { new MouseReport{*this} };
}

bool MouseReport::equals(const framework::Report_ &other) const {
  const MouseReport *other_mr =
    dynamic_cast<const MouseReport *>(&other);

  if (!other_mr) {
    return false;
  }

  return memcmp(&report_data_, &other_mr->report_data_, sizeof(report_data_)) == 0;
}

bool MouseReport::areButtonsPressed(uint8_t button_state) const {
  return report_data_.buttons == button_state;
}

bool MouseReport::isLeftButtonPressed() const {
  return report_data_.buttons & MOUSE_LEFT;
}

bool MouseReport::isMiddleButtonPressed() const {
  return report_data_.buttons & MOUSE_MIDDLE;
}

bool MouseReport::isRightButtonPressed() const {
  return report_data_.buttons & MOUSE_RIGHT;
}

int8_t MouseReport::getXMovement() const {
  return report_data_.xAxis;
}

int8_t MouseReport::getYMovement() const {
  return report_data_.yAxis;
}

int8_t MouseReport::getVerticalWheel() const {
  return report_data_.vWheel;
}

int8_t MouseReport::getHorizontalWheel() const {
  return report_data_.hWheel;
}

bool MouseReport::isEmpty() const {
  return (report_data_.buttons == 0)
         && (report_data_.xAxis == 0)
         && (report_data_.yAxis == 0)
         && (report_data_.vWheel == 0)
         && (report_data_.hWheel == 0);
}

void MouseReport::dump(const framework::Simulator &simulator, const char *add_indent) const {
  simulator.log() << add_indent << "Mouse report content:";
  simulator.log() << add_indent << "  left button: " << this->isLeftButtonPressed();
  simulator.log() << add_indent << "  middle button: " << this->isMiddleButtonPressed();
  simulator.log() << add_indent << "  right button: " << this->isRightButtonPressed();
  simulator.log() << add_indent << "  x-axis motion: " << (int)this->getXMovement();
  simulator.log() << add_indent << "  y-axis motion: " << (int)this->getYMovement();
  simulator.log() << add_indent << "  horizontal wheel motion: " << (int)this->getHorizontalWheel();
  simulator.log() << add_indent << "  vertical wheel motion: " << (int)this->getVerticalWheel();
}

void MouseReport::setReportData(const HID_MouseReport_Data_t &report_data) {
  memcpy(&report_data_, &report_data, sizeof(report_data_));
}

} // namespace executor
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
