/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Settings -- Basic EEPROM settings plugin for Kaleidoscope.
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
#include <Kaleidoscope-FocusSerial.h>
#include "kaleidoscope/plugin/EEPROM-Settings/crc.h"

namespace kaleidoscope {
namespace plugin {

struct EEPROMSettings::settings EEPROMSettings::settings_;
bool EEPROMSettings::is_valid_;
bool EEPROMSettings::sealed_;
uint16_t EEPROMSettings::next_start_ = sizeof(EEPROMSettings::settings);

EventHandlerResult EEPROMSettings::onSetup() {
  KeyboardHardware.storage().get(0, settings_);

  /* If the version is undefined, set up sensible defaults. */
  if (settings_.version == VERSION_UNDEFINED) {
    if (settings_.default_layer == 127 &&
        settings_.ignore_hardcoded_layers) {
      /* If both of these are set, that means that the EEPROM is uninitialized,
         and setting sensible defaults is safe. If either of them is not at it's
         uninitialized state, we do not override them, to avoid overwriting user
         settings. */
      settings_.ignore_hardcoded_layers = false;
      settings_.default_layer = 0;
    }

    /* If the version is undefined, we'll set it to our current one. */
    settings_.version = VERSION_CURRENT;

    /* Ideally, we'd save the defaults set here on the first write, but we are
     * not able to catch all writes yet. For the sake of consistency, if we
     * encounter a firmware with no version defined, we'll set sensible
     * defaults. */
    KeyboardHardware.storage().put(0, settings_);
  }
  return EventHandlerResult::OK;
}

EventHandlerResult EEPROMSettings::beforeEachCycle() {
  if (!sealed_)
    seal();

  return EventHandlerResult::OK;
}

bool EEPROMSettings::isValid(void) {
  return is_valid_;
}

uint16_t EEPROMSettings::crc(void) {
  if (sealed_)
    return settings_.crc;
  return 0;
}

uint8_t EEPROMSettings::default_layer(uint8_t layer) {
  if (layer < layer_count) {
    Layer.move(layer);
    settings_.default_layer = layer;
  }

  /*
   * We set default_layer to IGNORE_HARDCODED_LAYER (instead of `value`) because
   * due to compatibility reasons, we might get passed 0xff, yet, we want to set
   * a different value to signal an explicit "no default".
   */
  if (layer >= IGNORE_HARDCODED_LAYER) {
    settings_.default_layer = IGNORE_HARDCODED_LAYER;
  }
  update();
  return settings_.default_layer;
}

void EEPROMSettings::ignoreHardcodedLayers(bool value) {
  settings_.ignore_hardcoded_layers = value;
  if (settings_.default_layer > IGNORE_HARDCODED_LAYER)
    settings_.default_layer = IGNORE_HARDCODED_LAYER;
  update();
}

void EEPROMSettings::seal(void) {
  sealed_ = true;

  CRC.finalize();

  if (settings_.version != VERSION_CURRENT) {
    is_valid_ = false;
    return;
  }

  if (settings_.crc == 0xffff) {
    settings_.crc = CRC.crc;
    update();
  } else if (settings_.crc != CRC.crc) {
    return;
  }

  /* If we have a default layer set, switch to it.
   *
   * We check if the layer is smaller than IGNORE_HARDCODED_LAYER (0x7e),
   * because we want to avoid setting a default layer in two cases:
   *
   * - When the EEPROM is uninitialized (0x7f)
   * - When such layer switching is explicitly turned off (0x7e)
   */
  if (settings_.default_layer < IGNORE_HARDCODED_LAYER)
    Layer.move(settings_.default_layer);
}

uint16_t EEPROMSettings::requestSlice(uint16_t size) {
  if (sealed_)
    return 0;

  uint16_t start = next_start_;
  next_start_ += size;

  CRC.update((const void *)&size, sizeof(size));

  return start;
}

void EEPROMSettings::invalidate(void) {
  is_valid_ = false;
}

uint16_t EEPROMSettings::used(void) {
  return next_start_;
}

void EEPROMSettings::update(void) {
  KeyboardHardware.storage().put(0, settings_);
  is_valid_ = true;
}

/** Focus **/
EventHandlerResult FocusSettingsCommand::onFocusEvent(const char *command) {
  enum {
    DEFAULT_LAYER,
    IS_VALID,
    GET_VERSION,
    CRC,
  } sub_command;

  if (::Focus.handleHelp(command, PSTR("settings.defaultLayer\nsettings.valid?\nsettings.version\nsettings.crc")))
    return EventHandlerResult::OK;

  if (strncmp_P(command, PSTR("settings."), 9) != 0)
    return EventHandlerResult::OK;

  if (strcmp_P(command + 9, PSTR("defaultLayer")) == 0)
    sub_command = DEFAULT_LAYER;
  else if (strcmp_P(command + 9, PSTR("valid?")) == 0)
    sub_command = IS_VALID;
  else if (strcmp_P(command + 9, PSTR("version")) == 0)
    sub_command = GET_VERSION;
  else if (strcmp_P(command + 9, PSTR("crc")) == 0)
    sub_command = CRC;
  else
    return EventHandlerResult::OK;

  switch (sub_command) {
  case DEFAULT_LAYER: {
    if (::Focus.isEOL()) {
      ::Focus.send(::EEPROMSettings.default_layer());
    } else {
      uint8_t layer;
      ::Focus.read(layer);
      ::EEPROMSettings.default_layer(layer);
    }
    break;
  }
  case IS_VALID:
    ::Focus.send(::EEPROMSettings.isValid());
    break;
  case GET_VERSION:
    ::Focus.send(::EEPROMSettings.version());
    break;
  case CRC:
    ::Focus.sendRaw(::CRC.crc, F("/"), ::EEPROMSettings.crc());
    break;
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult FocusEEPROMCommand::onFocusEvent(const char *command) {
  enum {
    CONTENTS,
    FREE,
  } sub_command;

  if (::Focus.handleHelp(command, PSTR("eeprom.contents\neeprom.free")))
    return EventHandlerResult::OK;

  if (strcmp_P(command, PSTR("eeprom.contents")) == 0)
    sub_command = CONTENTS;
  else if (strcmp_P(command, PSTR("eeprom.free")) == 0)
    sub_command = FREE;
  else
    return EventHandlerResult::OK;

  switch (sub_command) {
  case CONTENTS: {
    if (::Focus.isEOL()) {
      for (uint16_t i = 0; i < KeyboardHardware.storage().length(); i++) {
        uint8_t d = KeyboardHardware.storage().read(i);
        ::Focus.send(d);
      }
    } else {
      for (uint16_t i = 0; i < KeyboardHardware.storage().length() && !::Focus.isEOL(); i++) {
        uint8_t d;
        ::Focus.read(d);
        KeyboardHardware.storage().update(i, d);
      }
    }

    break;
  }
  case FREE:
    ::Focus.send(KeyboardHardware.storage().length() - ::EEPROMSettings.used());
    break;
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

}
}

kaleidoscope::plugin::EEPROMSettings EEPROMSettings;
kaleidoscope::plugin::FocusSettingsCommand FocusSettingsCommand;
kaleidoscope::plugin::FocusEEPROMCommand FocusEEPROMCommand;
