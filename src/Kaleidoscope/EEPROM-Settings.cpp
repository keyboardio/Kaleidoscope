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
#include "crc.h"

namespace kaleidoscope {

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

void EEPROMSettings::seal(void) {
  sealed_ = true;

  CRC.finalize();

  /* Until we set a version, consider the EEPROM contents flexible, and always
   * update the CRC. This will always result in the settings being considered
   * valid.
   */
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

}

kaleidoscope::EEPROMSettings EEPROMSettings;
