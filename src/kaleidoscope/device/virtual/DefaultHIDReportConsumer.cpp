/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2019  Keyboard.io, Inc.
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

#include "kaleidoscope/device/virtual/DefaultHIDReportConsumer.h"

// From KeyboardioHID:
#include <HID-Settings.h>               // for HID_REPORTID_NKRO_KEYBOARD
#include <BootKeyboard/BootKeyboard.h>  // for HID_BootKeyboardReport_Data_t, (anonymous u...
// From system:
#include <stdint.h>  // for uint8_t
// From Arduino core:
#include <virtual_io.h>  // for logUSBEvent_keyboard

// From Kaleidoscope:
#include "kaleidoscope/device/virtual/Logging.h"  // for log_info, logging

#undef min
#undef max

#include <sstream>  // for operator<<, stringstream, basic_ostream
#include <string>   // for char_traits, operator+, basic_string

namespace kaleidoscope {

using namespace logging;  // NOLINT(build/namespaces)

// For each bit set in 'bitfield', output the corresponding string to 'stream'
#define FOREACHBIT(bitfield, stream, str0, str1, str2, str3, str4, str5, str6, str7) \
  if ((bitfield)&1 << 0) stream << str0;                                             \
  if ((bitfield)&1 << 1) stream << str1;                                             \
  if ((bitfield)&1 << 2) stream << str2;                                             \
  if ((bitfield)&1 << 3) stream << str3;                                             \
  if ((bitfield)&1 << 4) stream << str4;                                             \
  if ((bitfield)&1 << 5) stream << str5;                                             \
  if ((bitfield)&1 << 6) stream << str6;                                             \
  if ((bitfield)&1 << 7) stream << str7;

void DefaultHIDReportConsumer::processHIDReport(
  uint8_t id, const void *data, int len, int result) {
  if (id != HID_REPORTID_KEYBOARD) {
    log_info("***Ignoring hid report with id = %d\n", id);
    return;
  }

  const HID_BootKeyboardReport_Data_t &report_data = *static_cast<const HID_BootKeyboardReport_Data_t *>(data);

  std::stringstream keypresses;
  bool anything = false;

  if (report_data.modifiers) {
    anything = true;
  } else {
    for (int i = 0; i < NKRO_KEY_BYTES; i++) {
      if (report_data.nkro_keys[i]) {
        anything = true;
        break;
      }
    }
  }

  if (!anything) {
    keypresses << "none";
  } else {
    // clang-format off
    FOREACHBIT(report_data.modifiers, keypresses,
               "lctrl ", "lshift ", "lalt ", "lgui ",
               "rctrl ", "rshift ", "ralt ", "rgui ")
    FOREACHBIT(report_data.nkro_keys[0], keypresses,
               "NO_EVENT ", "ERROR_ROLLOVER ", "POST_FAIL ", "ERROR_UNDEFINED ",
               "a ", "b ", "c ", "d ")
    FOREACHBIT(report_data.nkro_keys[1], keypresses,
               "e ", "f ", "g ", "h ", "i ", "j ", "k ", "l ")
    FOREACHBIT(report_data.nkro_keys[2], keypresses,
               "m ", "n ", "o ", "p ", "q ", "r ", "s ", "t ")
    FOREACHBIT(report_data.nkro_keys[3], keypresses,
               "u ", "v ", "w ", "x ", "y ", "z ", "1/! ", "2/@ ")
    FOREACHBIT(report_data.nkro_keys[4], keypresses,
               "3/# ", "4/$ ", "5/% ", "6/^ ", "7/& ", "8/* ", "9/( ", "0/) ")
    FOREACHBIT(report_data.nkro_keys[5], keypresses,
               "enter ", "esc ", "del/bksp ", "tab ",
               "space ", "-/_ ", "=/+ ", "[/{ ")
    FOREACHBIT(report_data.nkro_keys[6], keypresses,
               "]/} ", "\\/| ", "#/~ ", ";/: ", "'/\" ", "`/~ ", ",/< ", "./> ")
    FOREACHBIT(report_data.nkro_keys[7], keypresses,
               "//? ", "capslock ", "F1 ", "F2 ", "F3 ", "F4 ", "F5 ", "F6 ")
    FOREACHBIT(report_data.nkro_keys[8], keypresses,
               "F7 ", "F8 ", "F9 ", "F10 ", "F11 ", "F12 ", "prtscr ", "scrolllock ")
    FOREACHBIT(report_data.nkro_keys[9], keypresses,
               "pause ", "ins ", "home ", "pgup ", "del ", "end ", "pgdn ", "r_arrow ")
    FOREACHBIT(report_data.nkro_keys[10], keypresses,
               "l_arrow ", "d_arrow ", "u_arrow ", "numlock ",
               "num/ ", "num* ", "num- ", "num+ ")
    FOREACHBIT(report_data.nkro_keys[11], keypresses,
               "numenter ", "num1 ", "num2 ", "num3 ",
               "num4 ", "num5 ", "num6 ", "num7 ")
    FOREACHBIT(report_data.nkro_keys[12], keypresses,
               "num8 ", "num9 ", "num0 ", "num. ", "\\/| ", "app ", "power ", "num= ")
    FOREACHBIT(report_data.nkro_keys[13], keypresses,
               "F13 ", "F14 ", "F15 ", "F16 ", "F17 ", "F18 ", "F19 ", "F20 ")
    FOREACHBIT(report_data.nkro_keys[14], keypresses,
               "F21 ", "F22 ", "F23 ", "F24 ", "exec ", "help ", "menu ", "sel ")
    FOREACHBIT(report_data.nkro_keys[15], keypresses,
               "stop ", "again ", "undo ", "cut ", "copy ", "paste ", "find ", "mute ")
    FOREACHBIT(report_data.nkro_keys[16], keypresses,
               "volup ", "voldn ", "capslock_l ", "numlock_l ",
               "scrolllock_l ", "num, ", "num= ", "(other) ")
    // clang-format on

    for (int i = 17; i < NKRO_KEY_BYTES; i++) {
      // A little imprecise, in two ways:
      //   (1) obviously, "(other)" refers to many distinct keys
      //   (2) this might undercount the number of "other" keys pressed
      // Therefore, if any keys are frequently used, they should be handled above and not via "other"
      if (report_data.nkro_keys[i]) keypresses << "(other) ";
    }
  }

  log_info("Sent virtual HID report. Pressed keys: %s\n", keypresses.str().c_str());
  logUSBEvent_keyboard("Keyboard HID report; pressed keys: " + keypresses.str());
}

}  // namespace kaleidoscope

#endif  // ifdef KALEIDOSCOPE_VIRTUAL_BUILD
