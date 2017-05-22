/* -*- mode: c++ -*-
 * Kaleidoscope-Unicode -- Unicode input helpers
 * Copyright (C) 2016, 2017  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Kaleidoscope-Unicode.h>

namespace KaleidoscopePlugins {

  Unicode::Unicode (void) {
  }

  void
  Unicode::begin (void) {
    ::HostOS.begin ();
  }

  void
  Unicode::start (void) {
    switch (::HostOS.os ()) {
    case HostOS::LINUX:
      Keyboard.press (Key_LeftControl.keyCode);
      Keyboard.press (Key_LeftShift.keyCode);
      Keyboard.press (Key_U.keyCode);
      Keyboard.sendReport ();
      Keyboard.release (Key_LeftControl.keyCode);
      Keyboard.release (Key_LeftShift.keyCode);
      Keyboard.release (Key_U.keyCode);
      Keyboard.sendReport ();
      break;
    case HostOS::WINDOWS:
      Keyboard.press (Key_RightAlt.keyCode);
      Keyboard.sendReport ();
      Keyboard.release (Key_RightAlt.keyCode);
      Keyboard.sendReport ();
      Keyboard.press (Key_U.keyCode);
      Keyboard.sendReport ();
      Keyboard.release (Key_U.keyCode);
      Keyboard.sendReport ();
      break;
    case HostOS::OSX:
      Keyboard.press (Key_LeftAlt.keyCode);
      break;
    default:
      unicodeCustomStart ();
      break;
    }
  }

  void
  Unicode::input (void) {
    switch (::HostOS.os ()) {
    case HostOS::LINUX:
    case HostOS::WINDOWS:
      break;
    case HostOS::OSX:
      Keyboard.press (Key_LeftAlt.keyCode);
      break;
    default:
      unicodeCustomInput ();
      break;
    }
  }

  void
  Unicode::end (void) {
    switch (::HostOS.os ()) {
    case HostOS::LINUX:
      Keyboard.press (Key_Spacebar.keyCode);
      Keyboard.sendReport ();
      Keyboard.release (Key_Spacebar.keyCode);
      Keyboard.sendReport ();
      break;
    case HostOS::WINDOWS:
      break;
    case HostOS::OSX:
      Keyboard.release (Key_LeftAlt.keyCode);
      Keyboard.sendReport ();
      break;
    default:
      unicodeCustomEnd ();
      break;
    }
  }

  void
  Unicode::typeCode (uint32_t unicode) {
    bool onZeroStart = true;

    for (int8_t i = 7; i >= 0; i--) {
      if (i <= 3) {
        onZeroStart = false;
      }

      uint8_t digit = ((unicode >> (i * 4)) & 0xF);
      if (digit == 0) {
        if (onZeroStart == false) {
          Key key = hexToKey (digit);

          input ();
          Keyboard.press (key.keyCode);
          Keyboard.sendReport ();
          input ();
          Keyboard.release (key.keyCode);
          Keyboard.sendReport ();
        }
      } else {
        Key key = hexToKey (digit);

        input ();
        Keyboard.press (key.keyCode);
        Keyboard.sendReport ();
        input ();
        Keyboard.release (key.keyCode);
        Keyboard.sendReport ();

        onZeroStart = false;
      }

      delay (5);
    }
  }

  void
  Unicode::type (uint32_t unicode) {
    start ();
    typeCode (unicode);
    end ();
  }
};

__attribute__((weak))
Key
hexToKey (uint8_t hex) {
  uint8_t m;

  if (hex == 0x0) {
    return Key_0;
  }

  if (hex < 0xA) {
    m = Key_1.keyCode + (hex - 0x1);
  } else {
    m = Key_A.keyCode + (hex - 0xA);
  }

  return { m, KEY_FLAGS };
}

__attribute__((weak))
void
unicodeCustomStart (void) {
}

__attribute__((weak))
void
unicodeCustomEnd (void) {
}

__attribute__((weak))
void
unicodeCustomInput (void) {
}

KaleidoscopePlugins::Unicode Unicode;
