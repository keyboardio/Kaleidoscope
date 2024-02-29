/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Keymap -- EEPROM-based keymap support.
 * Copyright (C) 2017, 2018, 2019, 2021  Keyboard.io, Inc
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

#include "kaleidoscope/plugin/EEPROM-Keymap.h"

#include <Arduino.h>                       // for PSTR, F, __FlashStringHelper
#include <Kaleidoscope-EEPROM-Settings.h>  // for EEPROMSettings
#include <Kaleidoscope-FocusSerial.h>      // for Focus, FocusSerial
#include <stdint.h>                        // for uint8_t, uint16_t

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr, MatrixAddr, MatrixAddr<>::Range
#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"         // for VirtualProps::Storage, Device, Base<>::St...
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/key_defs.h"              // for Key, Key_NoKey
#include "kaleidoscope/layers.h"                // for Layer_, Layer, layer_count

namespace kaleidoscope {
namespace plugin {
uint16_t EEPROMKeymap::keymap_base_;
uint8_t EEPROMKeymap::max_layers_;
uint8_t EEPROMKeymap::progmem_layers_;

EventHandlerResult EEPROMKeymap::onSetup() {
  ::EEPROMSettings.onSetup();
  progmem_layers_ = layer_count;
  return EventHandlerResult::OK;
}

EventHandlerResult EEPROMKeymap::onNameQuery() {
  return ::Focus.sendName(F("EEPROMKeymap"));
}

void EEPROMKeymap::setup(uint8_t max) {
  layer_count = max;
    Layer.getKey = getKey;
  max_layers(max);
}

void EEPROMKeymap::max_layers(uint8_t max) {
  max_layers_  = max;
  keymap_base_ = ::EEPROMSettings.requestSlice(max_layers_ * Runtime.device().numKeys() * 2);
}

Key EEPROMKeymap::getKey(uint8_t layer, KeyAddr key_addr) {
  if (layer >= max_layers_)
    return Key_NoKey;

  uint16_t pos = ((layer * Runtime.device().numKeys()) + key_addr.toInt()) * 2;

  return Key(Runtime.storage().read(keymap_base_ + pos + 1),  // key_code
             Runtime.storage().read(keymap_base_ + pos));     // flags
}

uint16_t EEPROMKeymap::keymap_base() {
  return keymap_base_;
}

void EEPROMKeymap::updateKey(uint16_t base_pos, Key key) {
  Runtime.storage().update(keymap_base_ + base_pos * 2, key.getFlags());
  Runtime.storage().update(keymap_base_ + base_pos * 2 + 1, key.getKeyCode());
}

void EEPROMKeymap::dumpKeymap(uint8_t layers, Key (*getkey)(uint8_t, KeyAddr)) {
  for (uint8_t layer = 0; layer < layers; layer++) {
    for (auto key_addr : KeyAddr::all()) {
      Key k = (*getkey)(layer, key_addr);

      ::Focus.send(k);
    }
  }
}

EventHandlerResult EEPROMKeymap::onFocusEvent(const char *input) {
  const char *cmd_custom     = PSTR("keymap.custom");
  const char *cmd_default    = PSTR("keymap.default");

  if (::Focus.inputMatchesHelp(input))
    return ::Focus.printHelp(cmd_custom, cmd_default);

  if (::Focus.inputMatchesCommand(input, cmd_default)) {
    // By using a cast to the appropriate function type,
    // tell the compiler which overload of getKeyFromPROGMEM
    // we actully want.
    //
    dumpKeymap(progmem_layers_,
               static_cast<Key (*)(uint8_t, KeyAddr)>(Layer_::getKeyFromPROGMEM));
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (!::Focus.inputMatchesCommand(input, cmd_custom))
    return EventHandlerResult::OK;

  if (::Focus.isEOL()) {
    // By using a cast to the appropriate function type,
    // tell the compiler which overload of getKey
    // we actually want.
    //
    dumpKeymap(max_layers_, static_cast<Key (*)(uint8_t, KeyAddr)>(getKey));
  } else {
    uint16_t i = 0;

    while (!::Focus.isEOL() && (i < (uint16_t)Runtime.device().numKeys() * max_layers_)) {
      Key k;

      ::Focus.read(k);
      updateKey(i, k);
      i++;
    }
    Runtime.storage().commit();
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::EEPROMKeymap EEPROMKeymap;
