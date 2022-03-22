/* DynamicMacros - Dynamic macro support for Kaleidoscope.
 * Copyright (C) 2019, 2021  Keyboard.io, Inc.
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
#include "Kaleidoscope-FocusSerial.h"
#include "kaleidoscope/keyswitch_state.h"

namespace kaleidoscope {
namespace plugin {

uint16_t DynamicMacros::storage_base_;
uint16_t DynamicMacros::storage_size_;
uint16_t DynamicMacros::map_[];

void DynamicMacros::updateDynamicMacroCache() {
  uint16_t pos = storage_base_;
  uint8_t current_id = 0;
  macro_t macro = MACRO_ACTION_END;
  bool previous_macro_ended = false;

  map_[0] = 0;

  while (pos < storage_base_ + storage_size_) {
    macro = Runtime.storage().read(pos++);
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
        flags = Runtime.storage().read(pos++);
        keyCode = Runtime.storage().read(pos++);
      } while (!(flags == 0 && keyCode == 0));
      break;
    }

    case MACRO_ACTION_STEP_TAP_CODE_SEQUENCE: {
      previous_macro_ended = false;
      uint8_t keyCode, flags;
      do {
        keyCode = Runtime.storage().read(pos++);
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

// public
void DynamicMacros::setup(uint16_t storage_size) {
  Macros::readMacroByte = &DynamicMacros::readMacroByte;

  reserve_storage(storage_size);
}

uint8_t DynamicMacros::readMacroByteFromEEPROM(const macro_t *ptr, uint8_t source) {
  int idx = (int)*(ptr++);
  return Runtime.storage().read(idx);
}

uint8_t DynamicMacros::readMacroByte(const macro_t *ptr, uint8_t source) {
  if (source == MACRO_SOURCE_PROGMEM) {
    return Macros::readMacroByteFromPROGMEM(ptr, source);
  } else {
    return readMacroByteFromEEPROM(ptr, source);
  }
}

void DynamicMacros::play(uint8_t macro_id) {
  uint16_t pos = storage_base_ + map_[macro_id];

  ::Macros.play((const macro_t *)&pos, MACRO_SOURCE_EEPROM);
}

bool isDynamicMacrosKey(Key key) {
  return (key.getRaw() >= ranges::DYNAMIC_MACRO_FIRST &&
          key.getRaw() <= ranges::DYNAMIC_MACRO_LAST);
}

// -----------------------------------------------------------------------------
EventHandlerResult DynamicMacros::onKeyEvent(KeyEvent &event) {
  if (!isDynamicMacrosKey(event.key))
    return EventHandlerResult::OK;

  if (keyToggledOn(event.state)) {
    uint8_t macro_id = event.key.getRaw() - ranges::DYNAMIC_MACRO_FIRST;
    play(macro_id);
  } else {
    ::Macros.release(event.key);
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult DynamicMacros::onNameQuery() {
  return ::Focus.sendName(F("DynamicMacros"));
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
        b = Runtime.storage().read(storage_base_ + i);
        ::Focus.send(b);
      }
    } else {
      uint16_t pos = 0;

      while (!::Focus.isEOL()) {
        uint8_t b;
        ::Focus.read(b);

        Runtime.storage().update(storage_base_ + pos++, b);
      }
      Runtime.storage().commit();
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

// public
void DynamicMacros::reserve_storage(uint16_t size) {
  storage_base_ = ::EEPROMSettings.requestSlice(size);
  storage_size_ = size;
  updateDynamicMacroCache();
}

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::DynamicMacros DynamicMacros;
