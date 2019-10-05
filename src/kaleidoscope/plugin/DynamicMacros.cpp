/* DynamicMacros - Dynamic macro support for Kaleidoscope.
 * Copyright (C) 2019  Keyboard.io, Inc.
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

#include "Kaleidoscope-DynamicMacros.h"
#include "kaleidoscope/hid.h"
#include "Kaleidoscope-FocusSerial.h"

using namespace kaleidoscope::ranges;

namespace kaleidoscope {
namespace plugin {

uint16_t DynamicMacros::storage_base_;
uint16_t DynamicMacros::storage_size_;
uint16_t DynamicMacros::map_[];

static void playMacroKeyswitchEvent(Key key, uint8_t keyswitch_state, bool explicit_report) {
  handleKeyswitchEvent(key, UnknownKeyswitchLocation, keyswitch_state | INJECTED);

  if (explicit_report)
    return;

  kaleidoscope::hid::sendKeyboardReport();
  kaleidoscope::hid::sendMouseReport();
}

static void playKeyCode(Key key, uint8_t keyStates, bool explicit_report) {
  if (keyIsPressed(keyStates)) {
    playMacroKeyswitchEvent(key, IS_PRESSED, explicit_report);
  }
  if (keyWasPressed(keyStates)) {
    playMacroKeyswitchEvent(key, WAS_PRESSED, explicit_report);
  }
}

static void readKeyCodeAndPlay(uint16_t pos, uint8_t flags, uint8_t keyStates, bool explicit_report) {
  Key key;
  key.flags = flags;
  key.keyCode = EEPROM.read(pos++);

  playKeyCode(key, keyStates, explicit_report);
}

void DynamicMacros::updateDynamicMacroCache(void) {
  uint16_t pos = storage_base_;
  uint8_t current_id = 0;
  macro_t macro = MACRO_ACTION_END;
  bool previous_macro_ended = false;

  map_[0] = 0;

  while (pos < storage_base_ + storage_size_) {
    macro = EEPROM.read(pos++);
    switch (macro) {
    case MACRO_ACTION_STEP_EXPLICIT_REPORT:
    case MACRO_ACTION_STEP_IMPLICIT_REPORT:
    case MACRO_ACTION_STEP_SEND_REPORT:
      previous_macro_ended = false;
      break;

    case MACRO_ACTION_STEP_INTERVAL:
    case MACRO_ACTION_STEP_WAIT:
    case MACRO_ACTION_STEP_KEYCODEDOWN:
    case MACRO_ACTION_STEP_KEYCODEUP:
    case MACRO_ACTION_STEP_TAPCODE:
      previous_macro_ended = false;
      pos++;
      break;

    case MACRO_ACTION_STEP_KEYDOWN:
    case MACRO_ACTION_STEP_KEYUP:
    case MACRO_ACTION_STEP_TAP:
      previous_macro_ended = false;
      pos += 2;
      break;

    case MACRO_ACTION_STEP_TAP_SEQUENCE: {
      previous_macro_ended = false;
      uint8_t keyCode, flags;
      do {
        flags = EEPROM.read(pos++);
        keyCode = EEPROM.read(pos++);
      } while (!(flags == 0 && keyCode == 0));
      break;
    }

    case MACRO_ACTION_STEP_TAP_CODE_SEQUENCE: {
      previous_macro_ended = false;
      uint8_t keyCode, flags;
      do {
        keyCode = EEPROM.read(pos++);
      } while (keyCode != 0);
      break;
    }

    case MACRO_ACTION_END:
      map_[++current_id] = pos - storage_base_;

      if (previous_macro_ended)
        return;

      previous_macro_ended = true;
      break;
    }
  }
}

void DynamicMacros::play(uint8_t macro_id) {
  macro_t macro = MACRO_ACTION_END;
  uint8_t interval = 0;
  uint8_t flags;
  bool explicit_report = false;
  uint16_t pos;

  pos = storage_base_ + map_[macro_id];

  while (true) {
    switch (macro = EEPROM.read(pos++)) {
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
      interval = EEPROM.read(pos++);
      break;
    case MACRO_ACTION_STEP_WAIT: {
      uint8_t wait = EEPROM.read(pos++);
      delay(wait);
      break;
    }
    case MACRO_ACTION_STEP_KEYDOWN:
      flags = EEPROM.read(pos++);
      readKeyCodeAndPlay(pos++, flags, IS_PRESSED, explicit_report);
      break;
    case MACRO_ACTION_STEP_KEYUP:
      flags = EEPROM.read(pos++);
      readKeyCodeAndPlay(pos++, flags, WAS_PRESSED, explicit_report);
      break;
    case MACRO_ACTION_STEP_TAP:
      flags = EEPROM.read(pos++);
      readKeyCodeAndPlay(pos++, flags, IS_PRESSED | WAS_PRESSED, false);
      break;

    case MACRO_ACTION_STEP_KEYCODEDOWN:
      readKeyCodeAndPlay(pos++, 0, IS_PRESSED, explicit_report);
      break;
    case MACRO_ACTION_STEP_KEYCODEUP:
      readKeyCodeAndPlay(pos++, 0, WAS_PRESSED, explicit_report);
      break;
    case MACRO_ACTION_STEP_TAPCODE:
      readKeyCodeAndPlay(pos++, 0, IS_PRESSED | WAS_PRESSED, false);
      break;

    case MACRO_ACTION_STEP_TAP_SEQUENCE: {
      uint8_t keyCode;
      do {
        flags = EEPROM.read(pos++);
        keyCode = EEPROM.read(pos++);
        playKeyCode(Key(keyCode, flags), IS_PRESSED | WAS_PRESSED, false);
        delay(interval);
      } while (!(flags == 0 && keyCode == 0));
      break;
    }
    case MACRO_ACTION_STEP_TAP_CODE_SEQUENCE: {
      uint8_t keyCode;
      do {
        keyCode = EEPROM.read(pos++);
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

EventHandlerResult DynamicMacros::onKeyswitchEvent(Key &mappedKey, KeyAddr key_addr, uint8_t keyState) {
  if (mappedKey.raw < DYNAMIC_MACRO_FIRST || mappedKey.raw > DYNAMIC_MACRO_LAST)
    return EventHandlerResult::OK;

  if (keyToggledOn(keyState)) {
    play(mappedKey.raw - DYNAMIC_MACRO_FIRST);
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult DynamicMacros::onFocusEvent(const char *command) {
  if (::Focus.handleHelp(command, PSTR("macros.map\nmacros.trigger")))
    return EventHandlerResult::OK;

  if (strncmp_P(command, PSTR("macros."), 7) != 0)
    return EventHandlerResult::OK;

  if (strcmp_P(command + 7, PSTR("map")) == 0) {
    if (::Focus.isEOL()) {
      for (uint16_t i = 0; i < storage_size_; i++) {
        uint8_t b;
        b = EEPROM.read(storage_base_ + i);
        ::Focus.send(b);
      }
    } else {
      uint16_t pos = 0;

      while (!::Focus.isEOL()) {
        uint8_t b;
        ::Focus.read(b);

        EEPROM.update(storage_base_ + pos++, b);
      }
      updateDynamicMacroCache();
    }
  }

  if (strcmp_P(command + 7, PSTR("trigger")) == 0) {
    uint8_t id = 0;
    ::Focus.read(id);
    play(id);
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

void DynamicMacros::reserve_storage(uint16_t size) {
  storage_base_ = ::EEPROMSettings.requestSlice(size);
  storage_size_ = size;
  updateDynamicMacroCache();
}


}
}

kaleidoscope::plugin::DynamicMacros DynamicMacros;
