/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Settings -- Basic EEPROM settings plugin for Kaleidoscope.
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
#include <Kaleidoscope-FocusSerial.h>
#include "kaleidoscope/plugin/EEPROM-Settings/crc.h"

namespace kaleidoscope {
namespace plugin {

struct EEPROMSettings::settings EEPROMSettings::settings_;
bool EEPROMSettings::is_valid_;
bool EEPROMSettings::sealed_;
uint16_t EEPROMSettings::next_start_ = sizeof(EEPROMSettings::settings);

EventHandlerResult EEPROMSettings::onSetup() {
  EEPROM.get(0, settings_);
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
  if (layer == 0xff) {
    settings_.default_layer = layer;
  }
  update();
  return settings_.default_layer;
}

void EEPROMSettings::seal(void) {
  sealed_ = true;

  CRC.finalize();

  /* If we have a default layer set, switch to it. As 0xff is the default EEPROM
   * value, treat it as not having a default layer set. */
  if (settings_.default_layer != 0xff)
    Layer.move(settings_.default_layer);

  /* Until we set a version, consider the EEPROM contents flexible, and always
   * update the CRC. This will always result in the settings being considered
   * valid. */
  if (settings_.version == 0xff) {
    return update();
  }

  if (settings_.crc != CRC.crc)
    is_valid_ = false;
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
  settings_.crc = CRC.crc;

  EEPROM.put(0, settings_);
  is_valid_ = true;
}

uint8_t EEPROMSettings::version(void) {
  return settings_.version;
}

void EEPROMSettings::version(uint8_t ver) {
  settings_.version = ver;
  update();
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
      for (uint16_t i = 0; i < EEPROM.length(); i++) {
        uint8_t d = EEPROM[i];
        ::Focus.send(d);
      }
    } else {
      for (uint16_t i = 0; i < EEPROM.length() && !::Focus.isEOL(); i++) {
        uint8_t d;
        ::Focus.read(d);
        EEPROM.update(i, d);
      }
    }

    break;
  }
  case FREE:
    ::Focus.send(EEPROM.length() - ::EEPROMSettings.used());
    break;
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

}
}

kaleidoscope::plugin::EEPROMSettings EEPROMSettings;
kaleidoscope::plugin::FocusSettingsCommand FocusSettingsCommand;
kaleidoscope::plugin::FocusEEPROMCommand FocusEEPROMCommand;
