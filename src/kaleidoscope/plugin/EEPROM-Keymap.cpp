/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Keymap -- EEPROM-based keymap support.
 * Copyright (C) 2017, 2018  Keyboard.io, Inc
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
EEPROMKeymap::Mode EEPROMKeymap::mode_;
uint16_t EEPROMKeymap::keymap_base_;
uint8_t EEPROMKeymap::max_layers_;
uint8_t EEPROMKeymap::progmem_layers_;

EventHandlerResult EEPROMKeymap::onSetup() {
  ::EEPROMSettings.onSetup();
  progmem_layers_ = layer_count;
  return EventHandlerResult::OK;
}

void EEPROMKeymap::setup(uint8_t max, Mode mode) {
  switch (mode) {
  case Mode::CUSTOM:
    break;
  case Mode::EXTEND:
    layer_count = progmem_layers_ + max;
    Layer.getKey = getKeyExtended;
    break;
  }
  mode_ = mode;
  max_layers(max);
}

void EEPROMKeymap::max_layers(uint8_t max) {
  max_layers_ = max;
  keymap_base_ = ::EEPROMSettings.requestSlice(max_layers_ * ROWS * COLS * 2);
}

Key EEPROMKeymap::getKey(uint8_t layer, byte row, byte col) {
  Key key;

  if (layer >= max_layers_)
    return Key_NoKey;

  uint16_t pos = ((layer * ROWS * COLS) + (row * COLS) + col) * 2;

  key.flags = EEPROM.read(keymap_base_ + pos);
  key.keyCode = EEPROM.read(keymap_base_ + pos + 1);

  return key;
}

Key EEPROMKeymap::getKeyExtended(uint8_t layer, byte row, byte col) {
  Key key;

  // If the layer is within PROGMEM bounds, look it up from there
  if (layer < progmem_layers_) {
    return Layer.getKeyFromPROGMEM(layer, row, col);
  }

  // If the layer is outside of PROGMEM, look up from EEPROM
  return getKey(layer - progmem_layers_, row, col);
}

uint16_t EEPROMKeymap::keymap_base(void) {
  return keymap_base_;
}

void EEPROMKeymap::updateKey(uint16_t base_pos, Key key) {
  EEPROM.update(keymap_base_ + base_pos * 2, key.flags);
  EEPROM.update(keymap_base_ + base_pos * 2 + 1, key.keyCode);
}

Key EEPROMKeymap::parseKey(void) {
  Key key;

  key.raw  = Serial.parseInt();

  return key;
}

void EEPROMKeymap::printKey(Key k) {
  ::Focus.printNumber(k.raw);
}

EventHandlerResult EEPROMKeymap::onFocusEvent(const char *command) {
  const char *cmd = PSTR("keymap.map");
  if (::Focus.handleHelp(command, PSTR("keymap.map\nkeymap.roLayers")))
    return EventHandlerResult::OK;

  if (strncmp_P(command, PSTR("keymap."), 7) != 0)
    return EventHandlerResult::OK;

  if (strcmp_P(command + 7, PSTR("roLayers")) == 0) {
    if (mode_ != Mode::EXTEND)
      return EventHandlerResult::OK;
    Serial.println(progmem_layers_);
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (strcmp_P(command + 7, PSTR("map")) != 0)
    return EventHandlerResult::OK;

  if (Serial.peek() == '\n') {
    for (uint8_t layer = 0; layer < layer_count; layer++) {
      for (uint8_t row = 0; row < ROWS; row++) {
        for (uint8_t col = 0; col < COLS; col++) {
          Key k = Layer.getKey(layer, row, col);

          printKey(k);
          ::Focus.printSpace();
        }
      }
    }
    Serial.println();
  } else {
    uint16_t i = 0;
    uint8_t layers = layer_count;
    if (layers > 0)
      layers--;
    while ((Serial.peek() != '\n') && (i < ROWS * COLS * layers)) {
      Key k = parseKey();

      if (mode_ == Mode::EXTEND) {
        uint8_t layer = i / (ROWS * COLS);
        if (layer >= progmem_layers_)
          updateKey(i - (progmem_layers_ * ROWS * COLS), k);
      } else {
        updateKey(i, k);
      }
      i++;
    }
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

}
}

kaleidoscope::plugin::EEPROMKeymap EEPROMKeymap;
