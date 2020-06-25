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

#include "kaleidoscope/simulator/executor/actions/generic_report/GenerateHostEvent.h"
#include "kaleidoscope/simulator/executor/reports/BootKeyboardReport.h"
#include "kaleidoscope/simulator/executor/reports/KeyboardReport.h"
#include "kaleidoscope/simulator/executor/reports/MouseReport.h"
#include "kaleidoscope/simulator/executor/reports/AbsoluteMouseReport.h"
#include "kaleidoscope/simulator/framework/Simulator.h"

#ifdef __unix__ /* __unix__ is usually defined by compilers targeting Unix systems */

#include <X11/extensions/XTest.h>
#include <unistd.h>
#include <linux/input-event-codes.h>

#include <set>

// see /usr/include/linux/input-event-codes.h
// and /usr/share/X11/xkb/keycodes/evdev
// for information of Linux and X11 keycodes

namespace kaleidoscope {
namespace simulator {
namespace executor {
namespace actions {

HostEventAction::HostEventAction()
  :  display_{XOpenDisplay(NULL)} {
}

HostEventAction::~HostEventAction() {
  auto d = static_cast<Display*>(display_);
  XSync(d, 0);
  XCloseDisplay(d);
}

namespace {
unsigned const int modifiers[] = {
  KEY_LEFTCTRL, KEY_LEFTSHIFT, KEY_LEFTALT, KEY_LEFTMETA,
  KEY_RIGHTCTRL, KEY_RIGHTSHIFT, KEY_RIGHTALT, KEY_RIGHTMETA
};

unsigned const int keycodes[][8] = {
  { 0, 0, 0, 0, KEY_A, KEY_B, KEY_C, KEY_D },
  { KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L },
  { KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T },
  { KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z, KEY_1, KEY_2 },
  { KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0 },
  { KEY_ENTER, KEY_ESC, KEY_BACKSPACE, KEY_TAB, KEY_SPACE, KEY_MINUS, KEY_EQUAL, KEY_LEFTBRACE },
  { KEY_RIGHTBRACE, KEY_BACKSLASH, KEY_BACKSLASH, KEY_SEMICOLON, KEY_APOSTROPHE, KEY_GRAVE, KEY_COMMA, KEY_DOT },
  { KEY_SLASH, KEY_CAPSLOCK, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6 },
  { KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12, KEY_SYSRQ, KEY_SCROLLLOCK },
  { KEY_PAUSE, KEY_INSERT, KEY_HOME, KEY_PAGEUP, KEY_DELETE, KEY_END, KEY_PAGEDOWN, KEY_RIGHT },
  { KEY_LEFT, KEY_DOWN, KEY_UP, KEY_NUMLOCK, KEY_KPSLASH, KEY_KPASTERISK, KEY_KPMINUS, KEY_KPPLUS },
  { KEY_KPENTER, KEY_KP1, KEY_KP2, KEY_KP3, KEY_KP4, KEY_KP5, KEY_KP6, KEY_KP7 },
  { KEY_KP8, KEY_KP9, KEY_KP0, KEY_KPDOT, KEY_SLASH, KEY_APPSELECT, KEY_POWER, KEY_KPEQUAL },
  { KEY_F13, KEY_F14, KEY_F15, KEY_F16, KEY_F17, KEY_F18, KEY_F19, KEY_F20 },
  { KEY_F21, KEY_F22, KEY_F23, KEY_F24, KEY_OPEN, KEY_HELP, KEY_MENU, KEY_SELECT },
  { KEY_STOP, KEY_AGAIN, KEY_UNDO, KEY_CUT, KEY_COPY, KEY_PASTE, KEY_FIND, KEY_MUTE },
  { KEY_VOLUMEUP, KEY_VOLUMEDOWN, KEY_UNKNOWN, KEY_UNKNOWN, KEY_UNKNOWN, KEY_UNKNOWN, KEY_UNKNOWN, KEY_UNKNOWN }
};

class KeyboardReportEventCheck {

 public:

  KeyboardReportEventCheck(const framework::Simulator &simulator,
                           Display *display,
                           const KeyboardReport &previous_report,
                           const KeyboardReport &current_report)
    :  simulator_{simulator},
       display_{display},
       previous_report_data_{previous_report.getReportData()},
       current_report_data_{current_report.getReportData()}
  {}

  void compareReports() {

    for (int j = 0; j < 8; ++j) {
      this->modifierCheck(j);
    }

    for (int i = 0; i < 17; ++i) {
      for (int j = 0; j < 8; ++j) {
        this->keyCheck(i, j);
      }
    }
  }

 private:

  void modifierCheck(int j) {
    auto old_state = bitRead(previous_report_data_.modifiers, j);
    auto new_state = bitRead(current_report_data_.modifiers, j);

    if (old_state == new_state) {
      return;
    }

    auto keycode = modifiers[j] + 8;

    bool is_pressed = (new_state) ? true : false;

    XTestFakeKeyEvent(display_, keycode, is_pressed, CurrentTime);
  }

  void keyCheck(int i, int j) {
    auto old_state = bitRead(previous_report_data_.keys[i], j);
    auto new_state = bitRead(current_report_data_.keys[i], j);

    if (old_state == new_state) {
      return;
    }

    auto keycode = keycodes[i][j] + 8;

    bool is_pressed = (new_state) ? true : false;

    //simulator_.debug() << "Generating system keycode " << keycode << " (pressed = " << is_pressed << ")";
    XTestFakeKeyEvent(display_, keycode, is_pressed, CurrentTime);
  }

 private:

  const framework::Simulator &simulator_;
  Display *display_;
  const KeyboardReport::ReportDataType &previous_report_data_;
  const KeyboardReport::ReportDataType &current_report_data_;
};

class BootKeyboardReportEventCheck {

 public:

  BootKeyboardReportEventCheck(const framework::Simulator &simulator,
                               Display *display,
                               const BootKeyboardReport &previous_report,
                               const BootKeyboardReport &current_report)
    :  simulator_{simulator},
       display_{display},
       previous_report_data_{previous_report.getReportData()},
       current_report_data_{current_report.getReportData()} {
  }

  void compareReports() {

    for (int j = 0; j < 8; ++j) {
      this->modifierCheck(j);
    }

    this->keyCheck();
  }

 private:

  void modifierCheck(int j) {
    auto old_state = bitRead(previous_report_data_.modifiers, j);
    auto new_state = bitRead(current_report_data_.modifiers, j);

    if (old_state == new_state) {
      return;
    }

    auto keycode = modifiers[j] + 8;

    bool is_pressed = (new_state) ? true : false;

    XTestFakeKeyEvent(display_, keycode, is_pressed, CurrentTime);
  }

  void keyCheck() {
    std::set<uint8_t> previous_report_keycodes,
        current_report_keycodes;

    for (int i = 0; i < 6; ++i) {
      if (previous_report_data_.keycodes[i] != 0) {
        previous_report_keycodes.insert(
          previous_report_data_.keycodes[i]
        );
      }
      if (current_report_data_.keycodes[i] != 0) {
        current_report_keycodes.insert(
          current_report_data_.keycodes[i]
        );
      }
    }

    for (const auto &k : previous_report_keycodes) {
      if (current_report_keycodes.find(k) == current_report_keycodes.end()) {

        auto actual_keycode = getActualKeycode(k);

        // Keycode only present in previous report
        // => key released
        XTestFakeKeyEvent(display_, actual_keycode, False, CurrentTime);
      }
    }
    for (const auto &k : current_report_keycodes) {
      if (previous_report_keycodes.find(k) == previous_report_keycodes.end()) {

        auto actual_keycode = getActualKeycode(k);

        // Keycode only present in current report
        // => key pressed
        XTestFakeKeyEvent(display_, actual_keycode, True, CurrentTime);
      }
    }
  }

  static uint8_t getActualKeycode(uint8_t k) {
    return keycodes[k / 8][k % 8] + 8;
  }

 private:

  const framework::Simulator &simulator_;
  Display *display_;
  const BootKeyboardReport::ReportDataType &previous_report_data_;
  const BootKeyboardReport::ReportDataType &current_report_data_;
};

} // namespace

template<>
bool GenerateHostEvent<BootKeyboardReport>::Action::evalInternal() {
  auto d = static_cast<Display*>(display_);

  BootKeyboardReportEventCheck{
    *this->getSimulator(),
    d,
    *previous_report_,
    static_cast<const BootKeyboardReport&>(this->getReport())
  } .compareReports();

  XSync(d, 0);

  this->cachePreviousReport();

  // We ignore any further key bytes in the report as we do not have
  // any defined keycodes for those.

  return true;
}

template<>
bool GenerateHostEvent<KeyboardReport>::Action::evalInternal() {
  auto d = static_cast<Display*>(display_);

  KeyboardReportEventCheck{
    *this->getSimulator(),
    d,
    *previous_report_,
    static_cast<const KeyboardReport&>(this->getReport())
  } .compareReports();

  XSync(d, 0);

  this->cachePreviousReport();

  // We ignore any further key bytes in the report as we do not have
  // any defined keycodes for those.

  return true;
}

template<>
bool GenerateHostEvent<MouseReport>::Action::evalInternal() {
  const auto &report = this->getReport();

  auto d = static_cast<Display*>(display_);

  XTestFakeRelativeMotionEvent(d,
                               report.getXMovement(),
                               report.getYMovement(),
                               CurrentTime);

  XTestFakeButtonEvent(d, 1, report.isLeftButtonPressed(),  CurrentTime);
  XTestFakeButtonEvent(d, 2, report.isMiddleButtonPressed(),  CurrentTime);
  XTestFakeButtonEvent(d, 3, report.isRightButtonPressed(),  CurrentTime);

  auto v_wheel = report.getVerticalWheel();
  auto h_wheel = report.getHorizontalWheel();

  // X11 defines mouse button 4 as vertical scroll wheel up action and mouse
  // button 5 as vertical scroll wheel down action.
  //
  if (v_wheel > 0) {
    for (int i = 0; i < v_wheel; ++i) {
      XTestFakeButtonEvent(d, 4, True,  CurrentTime);
      // Release events can be ignored
    }
  } else if (v_wheel < 0) {
    for (int i = 0; i > v_wheel; --i) {
      XTestFakeButtonEvent(d, 5, True,  CurrentTime);
    }
  }

  // X11 defines mouse button 6 as horizontal scroll wheel left action and mouse
  // button 7 as horizontal scroll wheel down action.
  //
  if (h_wheel > 0) {
    for (int i = 0; i < h_wheel; ++i) {
      XTestFakeButtonEvent(d, 6, True,  CurrentTime);
    }
  } else if (h_wheel < 0) {
    for (int i = 0; i > h_wheel; --i) {
      XTestFakeButtonEvent(d, 7, True,  CurrentTime);
    }
  }

  XSync(d, 0);

  return true;
}

template<>
bool GenerateHostEvent<AbsoluteMouseReport>::Action::evalInternal() {
  const auto &report = this->getReport();

  auto d = static_cast<Display*>(display_);

  auto x_pos = DisplayWidth(d, 0) * report.getXPosition()
               / AbsoluteMouseReport::max_x_coordinate;
  auto y_pos = DisplayHeight(d, 0) * report.getYPosition()
               / AbsoluteMouseReport::max_y_coordinate;

  XTestFakeMotionEvent(d, 0,
                       x_pos,
                       y_pos,
                       CurrentTime);

  XTestFakeButtonEvent(d, 1, report.isLeftButtonPressed(),  CurrentTime);
  XTestFakeButtonEvent(d, 2, report.isMiddleButtonPressed(),  CurrentTime);
  XTestFakeButtonEvent(d, 3, report.isRightButtonPressed(),  CurrentTime);

  auto v_wheel = report.getVerticalWheel();
  //auto h_wheel = report.getHorizontalWheel();

  // X11 defines mouse button 4 as vertical scroll wheel up action and mouse
  // button 5 as vertical scroll wheel down action.
  //
  if (v_wheel > 0) {
    for (int i = 0; i < v_wheel; ++i) {
      XTestFakeButtonEvent(d, 4, True,  CurrentTime);
      // Release events can be ignored
    }
  } else if (v_wheel < 0) {
    for (int i = 0; i > v_wheel; --i) {
      XTestFakeButtonEvent(d, 5, True,  CurrentTime);
    }
  }

  // TODO: Why does the absolute mouse report not two types of wheel info?

  // X11 defines mouse button 6 as horizontal scroll wheel left action and mouse
  // button 7 as horizontal scroll wheel down action.
  //
//    if(h_wheel > 0) {
//       for(int i = 0; i < h_wheel; ++i) {
//          XTestFakeButtonEvent (d, 6, True,  CurrentTime);
//       }
//    }
//    else if(h_wheel < 0) {
//       for(int i = 0; i > h_wheel; --i) {
//          XTestFakeButtonEvent (d, 7, True,  CurrentTime);
//       }
//    }

  XSync(d, 0);

  return true;
}

} // namespace actions
} // namespace executor
} // namespace simulator
} // namespace kaleidoscope

#else
#error __CLANG_ATOMIC_LLONG_LOCK_FREE
#endif

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
