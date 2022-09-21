/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2022  Keyboard.io, Inc.
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

#include "kaleidoscope/plugin/LayerNames.h"

#include <Arduino.h>                   // for delay, PSTR, F, __FlashStri...
#include <Kaleidoscope-FocusSerial.h>  // for Focus, FocusSerial

#include "kaleidoscope/Runtime.h"                 // for Runtime, Runtime_
#include "kaleidoscope/plugin/EEPROM-Settings.h"  // for EEPROMSettings

namespace kaleidoscope {
namespace plugin {

// =============================================================================

EventHandlerResult LayerNames::onNameQuery() {
  return ::Focus.sendName(F("LayerNames"));
}

EventHandlerResult LayerNames::onFocusEvent(const char *input) {
  const char *cmd_layerNames = PSTR("keymap.layerNames");

  if (::Focus.inputMatchesHelp(input))
    return ::Focus.printHelp(cmd_layerNames);

  if (!::Focus.inputMatchesCommand(input, cmd_layerNames))
    return EventHandlerResult::OK;

  if (::Focus.isEOL()) {
    uint16_t pos = 0;
    while (pos < storage_size_) {
      uint8_t name_size = Runtime.storage().read(storage_base_ + pos++);

      if (name_size == 0 || name_size == 255) break;

      ::Focus.send(name_size);

      for (uint8_t i = 0; i < name_size; i++) {
        uint8_t b = Runtime.storage().read(storage_base_ + pos++);
        ::Focus.sendRaw(static_cast<char>(b));
      }
      ::Focus.sendRaw(::Focus.NEWLINE);
    }
    ::Focus.sendRaw(0, ::Focus.SEPARATOR, F("size="), storage_size_);
  } else {
    uint16_t pos = 0;

    while (pos < storage_size_) {
      uint8_t name_size;
      ::Focus.read(name_size);

      // size is followed by a space, ignore that.
      char spc;
      ::Focus.read(spc);

      Runtime.storage().update(storage_base_ + pos++, name_size);

      if (name_size == 0 ||
          name_size == ::EEPROMSettings.EEPROM_UNINITIALIZED_BYTE)
        break;

      for (uint8_t i = 0; i < name_size; i++) {
        char c;
        ::Focus.read(c);

        Runtime.storage().update(storage_base_ + pos++, c);
      }
    }
    Runtime.storage().commit();
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

// public
void LayerNames::reserve_storage(uint16_t size) {
  storage_base_ = ::EEPROMSettings.requestSlice(size);
  storage_size_ = size;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::LayerNames LayerNames;
