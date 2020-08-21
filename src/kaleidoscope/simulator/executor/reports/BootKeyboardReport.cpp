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

// Important: Leave stdint.h the first header as some other Kaleidoscope
//            related stuff depends on standard integer types to be defined
//            (Arduino defines them auto-magically).
//
#include <stdint.h>

#include "kaleidoscope/simulator/executor/reports/BootKeyboardReport.h"
#include "kaleidoscope/simulator/executor/aux/exceptions.h"
#include "kaleidoscope/simulator/interface/Simulator.h"
#include "kaleidoscope/simulator/interface/SimulatorCore_.h"
#include "MultiReport/Keyboard.h"

#include <vector>

namespace kaleidoscope {
namespace simulator {
namespace executor {

BootKeyboardReport
::BootKeyboardReport()
  :  report_data_{} {
}

BootKeyboardReport
::BootKeyboardReport(const BootKeyboardReport::ReportDataType &report_data) {
  this->setReportData(report_data);
}

BootKeyboardReport
::BootKeyboardReport(const void *data) {
  const ReportDataType &report_data
    = *static_cast<const ReportDataType *>(data);

  this->setReportData(report_data);
}

std::shared_ptr<interface::Report_> BootKeyboardReport::clone() const {
  return std::shared_ptr<interface::Report_> { new BootKeyboardReport{*this} };
}

bool
BootKeyboardReport
::equals(const interface::Report_ &other) const {
  const BootKeyboardReport *other_bkr =
    dynamic_cast<const BootKeyboardReport *>(&other);

  if (!other_bkr) {
    return false;
  }

  return memcmp(&report_data_, &other_bkr->report_data_, sizeof(report_data_)) == 0;
}

bool
BootKeyboardReport
::isKeycodeActive(uint8_t k) const {
  for (int i = 0; i < 6; ++i) {
    if (report_data_.keycodes[i] == k) {
      return true;
    }
  }

  return false;
}

std::vector<uint8_t>
BootKeyboardReport
::getActiveKeycodes() const {
  std::vector<uint8_t> active_keycodes;
  for (int i = 0; i < 6; ++i) {
    if (report_data_.keycodes[i] != 0) {
      active_keycodes.push_back(report_data_.keycodes[i]);
    }
  }

  return active_keycodes;
}

bool
BootKeyboardReport
::isAnyKeyActive() const {
  for (int i = 0; i < 6; ++i) {
    if (report_data_.keycodes[i] != 0) {
      return true;
    }
  }

  return false;
}

bool
BootKeyboardReport
::isModifierKeycodeActive(uint8_t k) const {
  if (k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
    k = k - HID_KEYBOARD_FIRST_MODIFIER;
    return !!(report_data_.modifiers & (1 << k));
  }

  KS_T_EXCEPTION("isKeycodeActive: Unknown modifier type " << unsigned(k))

  return false;
}

bool
BootKeyboardReport
::isAssertAnyModifierActive() const {
  for (uint8_t k = HID_KEYBOARD_FIRST_MODIFIER; k <= HID_KEYBOARD_LAST_MODIFIER; ++k) {
    uint8_t kTmp = k - HID_KEYBOARD_FIRST_MODIFIER;
    if (!!(report_data_.modifiers & (1 << kTmp))) {
      return true;
    }
  }
  return false;
}

std::vector<uint8_t>
BootKeyboardReport
::getActiveModifiers() const {
  std::vector<uint8_t> activeModifiers;

  for (uint8_t k = HID_KEYBOARD_FIRST_MODIFIER; k <= HID_KEYBOARD_LAST_MODIFIER; ++k) {
    uint8_t kTmp = k - HID_KEYBOARD_FIRST_MODIFIER;
    if (!!(report_data_.modifiers & (1 << kTmp))) {
      activeModifiers.push_back(k);
    }
  }
  return activeModifiers;
}

bool
BootKeyboardReport
::isEmpty() const {
  return !(this->isAssertAnyModifierActive() || this->isAnyKeyActive());
}

void
BootKeyboardReport
::setReportData(const BootKeyboardReport::ReportDataType &report_data) {
  memcpy(&report_data_, &report_data, sizeof(report_data_));
}

// For each bit set in 'bitfield', output the corresponding string to 'stream'
#define FOREACHBIT(bitfield, stream, str0, str1, str2, str3, str4, str5, str6, str7) \
  if((bitfield) & 1<<0) stream << str0; \
  if((bitfield) & 1<<1) stream << str1; \
  if((bitfield) & 1<<2) stream << str2; \
  if((bitfield) & 1<<3) stream << str3; \
  if((bitfield) & 1<<4) stream << str4; \
  if((bitfield) & 1<<5) stream << str5; \
  if((bitfield) & 1<<6) stream << str6; \
  if((bitfield) & 1<<7) stream << str7;
void
BootKeyboardReport
::dump(const interface::Simulator &simulator, const char *add_indent) const {
  bool anything = false;
  if (report_data_.modifiers) {
    anything = true;
  } else {
    for (int i = 0; i < KEY_BYTES; i++) {
      if (report_data_.keycodes[i]) {
        anything = true;
        break;
      }
    }
  }

  auto out = simulator.log();

  out << "Boot keyboard report content:";

  if (!anything) {
    out << add_indent << "<none>";
  } else {
    out << add_indent;
    FOREACHBIT(report_data_.modifiers, out,
               "lctrl ", "lshift ", "lalt ", "lgui ",
               "rctrl ", "rshift ", "ralt ", "rgui ")

    for (int i = 0; i < 6; ++i) {
      if (report_data_.keycodes[i] != 0) {
        out << simulator.getCore().keycodeToName(report_data_.keycodes[i]) << ' ';
      }
    }
  }
}

} // namespace executor
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
