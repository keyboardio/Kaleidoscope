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

#include "kaleidoscope/simulator/executor/reports/KeyboardReport.h"
#include "kaleidoscope/simulator/executor/aux/exceptions.h"
#include "kaleidoscope/simulator/interface/Simulator_.h"

#include <vector>

namespace kaleidoscope {
namespace simulator {
namespace executor {

KeyboardReport
::KeyboardReport()
  :  report_data_{} {
}

KeyboardReport
::KeyboardReport(const KeyboardReport::ReportDataType &report_data) {
  this->setReportData(report_data);
}

KeyboardReport
::KeyboardReport(const void *data) {
  const ReportDataType &report_data
    = *static_cast<const ReportDataType *>(data);

  this->setReportData(report_data);
}

std::shared_ptr<interface::Report_> KeyboardReport::clone() const {
  return std::shared_ptr<interface::Report_> { new KeyboardReport{*this} };
}

bool
KeyboardReport
::equals(const interface::Report_ &other) const {
  const KeyboardReport *other_kr =
    dynamic_cast<const KeyboardReport *>(&other);

  if (!other_kr) {
    return false;
  }

  return memcmp(&report_data_, &other_kr->report_data_, sizeof(report_data_)) == 0;
}

bool
KeyboardReport
::isKeycodeActive(uint8_t k) const {
  if (k <= HID_LAST_KEY) {
    uint8_t bit = 1 << (uint8_t(k) % 8);
    return report_data_.keys[k / 8] & bit;
  }

  KS_T_EXCEPTION("isKeycodeActive: Unknown keycode type " << unsigned(k))

  return false;
}

std::vector<uint8_t>
KeyboardReport
::getActiveKeycodes() const {
  std::vector<uint8_t> activeKeys;

  for (uint8_t k = 0; k <= HID_LAST_KEY; ++k) {
    uint8_t bit = 1 << (uint8_t(k) % 8);
    uint8_t keyCode = report_data_.keys[k / 8] & bit;
    if (keyCode) {
      activeKeys.push_back(k);
    }
  }
  return activeKeys;
}

bool
KeyboardReport
::isAnyKeyActive() const {
  for (int k = 0; k <= HID_LAST_KEY; ++k) {
    uint8_t bit = 1 << (uint8_t(k) % 8);
    if (report_data_.keys[k / 8] & bit) {
      return true;
    }
  }

  return false;
}

bool
KeyboardReport
::isModifierKeycodeActive(uint8_t k) const {
  if (k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
    k = k - HID_KEYBOARD_FIRST_MODIFIER;
    return !!(report_data_.modifiers & (1 << k));
  }

  KS_T_EXCEPTION("isKeycodeActive: Unknown modifier type " << unsigned(k))

  return false;
}

bool
KeyboardReport
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
KeyboardReport
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
KeyboardReport
::isEmpty() const {
  return !(this->isAssertAnyModifierActive() || this->isAnyKeyActive());
}

void
KeyboardReport
::setReportData(const KeyboardReport::ReportDataType &report_data) {
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
KeyboardReport
::dump(const interface::Simulator_ &simulator, const char *add_indent) const {
  bool anything = false;
  if (report_data_.modifiers) anything = true;
  else for (int i = 0; i < KEY_BYTES; i++) if (report_data_.keys[i]) {
        anything = true;
        break;
      }

  auto out = simulator.log();
  out << "Keyboard report content:";
  if (!anything) {
    out << add_indent << "<none>";
  } else {
    out << add_indent;
    FOREACHBIT(report_data_.modifiers, out,
               "lctrl ", "lshift ", "lalt ", "lgui ",
               "rctrl ", "rshift ", "ralt ", "rgui ")
    FOREACHBIT(report_data_.keys[0], out,
               "NO_EVENT ", "ERROR_ROLLOVER ", "POST_FAIL ", "ERROR_UNDEFINED ",
               "a ", "b ", "c ", "d ")
    FOREACHBIT(report_data_.keys[1], out,
               "e ", "f ", "g ", "h ", "i ", "j ", "k ", "l ")
    FOREACHBIT(report_data_.keys[2], out,
               "m ", "n ", "o ", "p ", "q ", "r ", "s ", "t ")
    FOREACHBIT(report_data_.keys[3], out,
               "u ", "v ", "w ", "x ", "y ", "z ", "1/! ", "2/@ ")
    FOREACHBIT(report_data_.keys[4], out,
               "3/# ", "4/$ ", "5/% ", "6/^ ", "7/& ", "8/* ", "9/( ", "0/) ")
    FOREACHBIT(report_data_.keys[5], out,
               "enter ", "esc ", "del/bksp ", "tab ",
               "space ", "-/_ ", "=/+ ", "[/{ ")
    FOREACHBIT(report_data_.keys[6], out,
               "]/} ", "\\/| ", "#/~ ", ";/: ", "'/\" ", "`/~ ", ",/< ", "./> ")
    FOREACHBIT(report_data_.keys[7], out,
               "//? ", "capslock ", "F1 ", "F2 ", "F3 ", "F4 ", "F5 ", "F6 ")
    FOREACHBIT(report_data_.keys[8], out,
               "F7 ", "F8 ", "F9 ", "F10 ", "F11 ", "F12 ", "prtscr ", "scrolllock ")
    FOREACHBIT(report_data_.keys[9], out,
               "pause ", "ins ", "home ", "pgup ", "del ", "end ", "pgdn ", "r_arrow ")
    FOREACHBIT(report_data_.keys[10], out,
               "l_arrow ", "d_arrow ", "u_arrow ", "numlock ",
               "num/ ", "num* ", "num- ", "num+ ")
    FOREACHBIT(report_data_.keys[11], out,
               "numenter ", "num1 ", "num2 ", "num3 ",
               "num4 ", "num5 ", "num6 ", "num7 ")
    FOREACHBIT(report_data_.keys[12], out,
               "num8 ", "num9 ", "num0 ", "num. ", "\\/| ", "app ", "power ", "num= ")
    FOREACHBIT(report_data_.keys[13], out,
               "F13 ", "F14 ", "F15 ", "F16 ", "F17 ", "F18 ", "F19 ", "F20 ")
    FOREACHBIT(report_data_.keys[14], out,
               "F21 ", "F22 ", "F23 ", "F24 ", "exec ", "help ", "menu ", "sel ")
    FOREACHBIT(report_data_.keys[15], out,
               "stop ", "again ", "undo ", "cut ", "copy ", "paste ", "find ", "mute ")
    FOREACHBIT(report_data_.keys[16], out,
               "volup ", "voldn ", "capslock_l ", "numlock_l ",
               "scrolllock_l ", "num, ", "num= ", "(other) ")
    for (int i = 17; i < KEY_BYTES; i++) {
      // A little imprecise, in two ways:
      //   (1) obviously, "(other)" refers to many distinct keys
      //   (2) this might undercount the number of "other" keys pressed
      // Therefore, if any keys are frequently used, they should be handled above and not via "other"
      if (report_data_.keys[i]) out << "(other) ";
    }
  }
}

} // namespace executor
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
