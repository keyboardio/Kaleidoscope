/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Keymap -- EEPROM-based keymap support.
 * Copyright (C) 2017, 2018, 2019  Keyboard.io, Inc
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

#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-EEPROM-Keymap.h>
#include <Kaleidoscope-FocusSerial.h>

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

void EEPROMKeymap::setup(uint8_t max) {
  layer_count = max;
  if (::EEPROMSettings.ignoreHardcodedLayers()) {
    Layer.getKey = getKey;
  } else {
    layer_count += progmem_layers_;
    Layer.getKey = getKeyExtended;
  }
  max_layers(max);
}

void EEPROMKeymap::max_layers(uint8_t max) {
  max_layers_ = max;
  keymap_base_ = ::EEPROMSettings.requestSlice(max_layers_ * ROWS * COLS * 2);
}

Key EEPROMKeymap::getKey(uint8_t layer, KeyAddr key_addr) {
  Key key;

  if (layer >= max_layers_)
    return Key_NoKey;

  uint16_t pos = ((layer * ROWS * COLS) + key_addr.offset()) * 2;

  key.flags = EEPROM.read(keymap_base_ + pos);
  key.keyCode = EEPROM.read(keymap_base_ + pos + 1);

  return key;
}

Key EEPROMKeymap::getKeyExtended(uint8_t layer, KeyAddr key_addr) {
  Key key;

  // If the layer is within PROGMEM bounds, look it up from there
  if (layer < progmem_layers_) {
    return Layer.getKeyFromPROGMEM(layer, key_addr);
  }

  // If the layer is outside of PROGMEM, look up from EEPROM
  return getKey(layer - progmem_layers_, key_addr);
}

uint16_t EEPROMKeymap::keymap_base(void) {
  return keymap_base_;
}

void EEPROMKeymap::updateKey(uint16_t base_pos, Key key) {
  EEPROM.update(keymap_base_ + base_pos * 2, key.flags);
  EEPROM.update(keymap_base_ + base_pos * 2 + 1, key.keyCode);
}

void EEPROMKeymap::dumpKeymap(uint8_t layers, Key(*getkey)(uint8_t, KeyAddr)) {
  for (uint8_t layer = 0; layer < layers; layer++) {
    for (uint8_t row = 0; row < ROWS; row++) {
      for (uint8_t col = 0; col < COLS; col++) {
        Key k = (*getkey)(layer, KeyAddr(row, col));

        ::Focus.send(k);
      }
    }
  }
}

void EEPROMKeymap::dumpKeymap(uint8_t layers, Key(*getkey)(uint8_t, byte, byte)) {
  for (uint8_t layer = 0; layer < layers; layer++) {
    for (uint8_t row = 0; row < ROWS; row++) {
      for (uint8_t col = 0; col < COLS; col++) {
        Key k = (*getkey)(layer, row, col);

        ::Focus.send(k);
      }
    }
  }
}

EventHandlerResult EEPROMKeymap::onFocusEvent(const char *command) {
  if (::Focus.handleHelp(command, PSTR("keymap.custom\nkeymap.default\nkeymap.onlyCustom")))
    return EventHandlerResult::OK;

  if (strncmp_P(command, PSTR("keymap."), 7) != 0)
    return EventHandlerResult::OK;

  if (strcmp_P(command + 7, PSTR("onlyCustom")) == 0) {
    if (::Focus.isEOL()) {
      ::Focus.send((uint8_t)::EEPROMSettings.ignoreHardcodedLayers());
    } else {
      bool v;

      ::Focus.read((uint8_t &)v);
      ::EEPROMSettings.ignoreHardcodedLayers(v);

      layer_count = max_layers_;
      if (v) {
        Layer.getKey = getKey;
      } else {
        layer_count += progmem_layers_;
        Layer.getKey = getKeyExtended;
      }
    }
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (strcmp_P(command + 7, PSTR("default")) == 0) {
    // By using a temporary callback ptr we
    // tell the compiler which version of getKeyFromPROGMEM
    // we want.
    Key(*callback)(uint8_t layer, KeyAddr key_addr) = Layer.getKeyFromPROGMEM;
    dumpKeymap(progmem_layers_, callback);
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (strcmp_P(command + 7, PSTR("custom")) != 0)
    return EventHandlerResult::OK;

  if (::Focus.isEOL()) {
    // By using a temporary callback ptr we
    // tell the compiler which version of getKey
    // we want.
    Key(*callback)(uint8_t layer, KeyAddr key_addr) = getKey;
    dumpKeymap(max_layers_, callback);
  } else {
    uint16_t i = 0;

    while (!::Focus.isEOL() && (i < ROWS * COLS * max_layers_)) {
      Key k;

      ::Focus.read(k);
      updateKey(i, k);
      i++;
    }
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

}
}

kaleidoscope::plugin::EEPROMKeymap EEPROMKeymap;
