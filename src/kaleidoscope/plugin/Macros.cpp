/* Kaleidoscope-Macros - Macro keys for Kaleidoscope.
 * Copyright (C) 2017-2019  Keyboard.io, Inc.
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
#include "kaleidoscope/keyswitch_state.h"
#include "kaleidoscope/key_events.h"

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

  kaleidoscope::Runtime.hid().keyboard().sendReport();
  kaleidoscope::Runtime.hid().mouse().sendReport();
}

static void playKeyCode(Key key, uint8_t keyStates, bool explicit_report) {
  if (keyIsPressed(keyStates)) {
    playMacroKeyswitchEvent(key, IS_PRESSED, explicit_report);
  }
  if (keyWasPressed(keyStates)) {
    playMacroKeyswitchEvent(key, WAS_PRESSED, explicit_report);
  }
}

static void readKeyCodeAndPlay(const macro_t *macro_p, uint8_t flags, uint8_t keyStates, bool explicit_report) {
  Key key(pgm_read_byte(macro_p++), // key_code
          flags);

  playKeyCode(key, keyStates, explicit_report);
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
      kaleidoscope::Runtime.hid().keyboard().sendReport();
      kaleidoscope::Runtime.hid().mouse().sendReport();
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

    case MACRO_ACTION_STEP_TAP_SEQUENCE: {
      uint8_t keyCode;
      do {
        flags = pgm_read_byte(macro_p++);
        keyCode = pgm_read_byte(macro_p++);
        playKeyCode(Key(keyCode, flags), IS_PRESSED | WAS_PRESSED, false);
        delay(interval);
      } while (!(flags == 0 && keyCode == 0));
      break;
    }
    case MACRO_ACTION_STEP_TAP_CODE_SEQUENCE: {
      uint8_t keyCode;
      do {
        keyCode = pgm_read_byte(macro_p++);
        playKeyCode(Key(keyCode, 0), IS_PRESSED | WAS_PRESSED, false);
        delay(interval);
      } while (keyCode != 0);
      break;
    }

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
    key.setKeyCode(Key_Backspace.getKeyCode() + ascii_code - 0x08);
    break;
  case 0x0A:
    key.setKeyCode(Key_Enter.getKeyCode());
    break;
  case 0x1B:
    key.setKeyCode(Key_Escape.getKeyCode());
    break;
  case 0x20:
    key.setKeyCode(Key_Spacebar.getKeyCode());
    break;
  case 0x21 ... 0x30:
    key = ascii_to_key_map[ascii_code - 0x21].readFromProgmem();
    break;
  case 0x31 ... 0x39:
    key.setKeyCode(Key_1.getKeyCode() + ascii_code - 0x31);
    break;
  case 0x3A ... 0x40:
    key = ascii_to_key_map[ascii_code - 0x3A + 16].readFromProgmem();
    break;
  case 0x41 ... 0x5A:
    key.setFlags(SHIFT_HELD);
    key.setKeyCode(Key_A.getKeyCode() + ascii_code - 0x41);
    break;
  case 0x5B ... 0x60:
    key = ascii_to_key_map[ascii_code - 0x5B + 23].readFromProgmem();
    break;
  case 0x61 ... 0x7A:
    key.setKeyCode(Key_A.getKeyCode() + ascii_code - 0x61);
    break;
  case 0x7B ... 0x7E:
    key = ascii_to_key_map[ascii_code - 0x7B + 29].readFromProgmem();
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


    if (key == Key_NoKey)
      continue;

    playMacroKeyswitchEvent(key, IS_PRESSED, false);
    playMacroKeyswitchEvent(key, WAS_PRESSED, false);

  }

  return MACRO_NONE;
}

bool Macros_::isMacroKey(Key key) {
  if (key >= ranges::MACRO_FIRST && key <= ranges::MACRO_LAST)
    return true;
  return false;
}

EventHandlerResult Macros_::onKeyswitchEvent(Key &mappedKey, KeyAddr key_addr, uint8_t keyState) {
  if (! isMacroKey(mappedKey))
    return EventHandlerResult::OK;

  uint8_t macro_index = mappedKey.getRaw() - ranges::MACRO_FIRST;
  addActiveMacroKey(macro_index, key_addr.toInt(), keyState);

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
