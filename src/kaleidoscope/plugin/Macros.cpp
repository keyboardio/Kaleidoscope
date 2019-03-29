/* Kaleidoscope-Macros - Macro keys for Kaleidoscope.
 * Copyright (C) 2017-2018  Keyboard.io, Inc.
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

#include "Kaleidoscope-Macros.h"
#include "kaleidoscope/hid.h"

__attribute__((weak))
const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  return MACRO_NONE;
}

namespace kaleidoscope {
namespace plugin {

MacroKeyEvent Macros_::active_macros[];
byte Macros_::active_macro_count;
KeyAddr Macros_::key_addr;

void playMacroKeyswitchEvent(Key key, uint8_t keyswitch_state, bool explicit_report) {
  handleKeyswitchEvent(key, UnknownKeyswitchLocation, keyswitch_state | INJECTED);

  if (explicit_report)
    return;

  kaleidoscope::hid::sendKeyboardReport();
  kaleidoscope::hid::sendMouseReport();
}

static void readKeyCodeAndPlay(const macro_t *macro_p, uint8_t flags, uint8_t keyStates, bool explicit_report) {
  Key key;
  key.flags = flags;
  key.keyCode = pgm_read_byte(macro_p++);

  if (keyIsPressed(keyStates)) {
    playMacroKeyswitchEvent(key, IS_PRESSED, explicit_report);
  }
  if (keyWasPressed(keyStates)) {
    playMacroKeyswitchEvent(key, WAS_PRESSED, explicit_report);
  }
}

void Macros_::play(const macro_t *macro_p) {
  macro_t macro = MACRO_ACTION_END;
  uint8_t interval = 0;
  uint8_t flags;
  bool explicit_report = false;

  if (!macro_p)
    return;

  while (true) {
    switch (macro = pgm_read_byte(macro_p++)) {
    case MACRO_ACTION_STEP_EXPLICIT_REPORT:
      explicit_report = true;
      break;
    case MACRO_ACTION_STEP_IMPLICIT_REPORT:
      explicit_report = false;
      break;
    case MACRO_ACTION_STEP_SEND_REPORT:
      kaleidoscope::hid::sendKeyboardReport();
      kaleidoscope::hid::sendMouseReport();
      break;
    case MACRO_ACTION_STEP_INTERVAL:
      interval = pgm_read_byte(macro_p++);
      break;
    case MACRO_ACTION_STEP_WAIT: {
      uint8_t wait = pgm_read_byte(macro_p++);
      delay(wait);
      break;
    }
    case MACRO_ACTION_STEP_KEYDOWN:
      flags = pgm_read_byte(macro_p++);
      readKeyCodeAndPlay(macro_p++, flags, IS_PRESSED, explicit_report);
      break;
    case MACRO_ACTION_STEP_KEYUP:
      flags = pgm_read_byte(macro_p++);
      readKeyCodeAndPlay(macro_p++, flags, WAS_PRESSED, explicit_report);
      break;
    case MACRO_ACTION_STEP_TAP:
      flags = pgm_read_byte(macro_p++);
      readKeyCodeAndPlay(macro_p++, flags, IS_PRESSED | WAS_PRESSED, false);
      break;

    case MACRO_ACTION_STEP_KEYCODEDOWN:
      readKeyCodeAndPlay(macro_p++, 0, IS_PRESSED, explicit_report);
      break;
    case MACRO_ACTION_STEP_KEYCODEUP:
      readKeyCodeAndPlay(macro_p++, 0, WAS_PRESSED, explicit_report);
      break;
    case MACRO_ACTION_STEP_TAPCODE:
      readKeyCodeAndPlay(macro_p++, 0, IS_PRESSED | WAS_PRESSED, false);
      break;

    case MACRO_ACTION_END:
    default:
      return;
    }

    delay(interval);
  }
}

static const Key ascii_to_key_map[] PROGMEM = {
  // 0x21 - 0x30
  LSHIFT(Key_1),
  LSHIFT(Key_Quote),
  LSHIFT(Key_3),
  LSHIFT(Key_4),
  LSHIFT(Key_5),
  LSHIFT(Key_7),
  Key_Quote,
  LSHIFT(Key_9),
  LSHIFT(Key_0),
  LSHIFT(Key_8),
  LSHIFT(Key_Equals),
  Key_Comma,
  Key_Minus,
  Key_Period,
  Key_Slash,
  Key_0,

  // 0x3a ... 0x40
  LSHIFT(Key_Semicolon),
  Key_Semicolon,
  LSHIFT(Key_Comma),
  Key_Equals,
  LSHIFT(Key_Period),
  LSHIFT(Key_Slash),
  LSHIFT(Key_2),

  // 0x5b ... 0x60
  Key_LeftBracket,
  Key_Backslash,
  Key_RightBracket,
  LSHIFT(Key_6),
  LSHIFT(Key_Minus),
  Key_Backtick,

  // 0x7b ... 0x7e
  LSHIFT(Key_LeftBracket),
  LSHIFT(Key_Backslash),
  LSHIFT(Key_RightBracket),
  LSHIFT(Key_Backtick),
};


Key Macros_::lookupAsciiCode(uint8_t ascii_code) {
  Key key = Key_NoKey;

  switch (ascii_code) {
  case 0x08 ... 0x09:
    key.keyCode = Key_Backspace.keyCode + ascii_code - 0x08;
    break;
  case 0x0A:
    key.keyCode = Key_Enter.keyCode;
    break;
  case 0x1B:
    key.keyCode = Key_Escape.keyCode;
    break;
  case 0x20:
    key.keyCode = Key_Spacebar.keyCode;
    break;
  case 0x21 ... 0x30:
    key.raw = pgm_read_word(&ascii_to_key_map[ascii_code - 0x21]);
    break;
  case 0x31 ... 0x39:
    key.keyCode = Key_1.keyCode + ascii_code - 0x31;
    break;
  case 0x3A ... 0x40:
    key.raw = pgm_read_word(&ascii_to_key_map[ascii_code - 0x3A + 16]);
    break;
  case 0x41 ... 0x5A:
    key.flags = SHIFT_HELD;
    key.keyCode = Key_A.keyCode + ascii_code - 0x41;
    break;
  case 0x5B ... 0x60:
    key.raw = pgm_read_word(&ascii_to_key_map[ascii_code - 0x5B + 23]);
    break;
  case 0x61 ... 0x7A:
    key.keyCode = Key_A.keyCode + ascii_code - 0x61;
    break;
  case 0x7B ... 0x7E:
    key.raw = pgm_read_word(&ascii_to_key_map[ascii_code - 0x7B + 29]);
    break;
  }
  return key;
}

const macro_t *Macros_::type(const char *string) {
  while (true) {
    uint8_t ascii_code = pgm_read_byte(string++);
    if (!ascii_code)
      break;

    Key key = lookupAsciiCode(ascii_code);


    if (key.raw == Key_NoKey.raw)
      continue;

    playMacroKeyswitchEvent(key, IS_PRESSED, false);
    playMacroKeyswitchEvent(key, WAS_PRESSED, false);

  }

  return MACRO_NONE;
}

EventHandlerResult Macros_::onKeyswitchEvent2(Key &mappedKey, KeyAddr key_addr, uint8_t keyState) {
  if (mappedKey.flags != (SYNTHETIC | IS_MACRO))
    return EventHandlerResult::OK;

  addActiveMacroKey(mappedKey.keyCode, key_addr.toInt(), keyState);

  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult Macros_::afterEachCycle() {
  active_macro_count = 0;

  return EventHandlerResult::OK;
}

EventHandlerResult Macros_::beforeReportingState() {
  for (byte i = 0; i < active_macro_count; ++i) {
    if (active_macros[i].key_id == 0xFF) {
      key_addr = UnknownKeyswitchLocation;
    } else {
      key_addr = KeyAddr(active_macros[i].key_id);
    }
    const macro_t *m = macroAction(active_macros[i].key_code,
                                   active_macros[i].key_state);
    Macros.play(m);
  }
  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::Macros_ Macros;
