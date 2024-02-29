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

#include "kaleidoscope/plugin/EEPROM-Settings.h"

#include <Arduino.h>                   // for PSTR, F, __FlashStringHelper
#include <Kaleidoscope-FocusSerial.h>  // for Focus, FocusSerial
#include <stdint.h>                    // for uint16_t, uint8_t
#include <stddef.h>                    // for size_t


#include "kaleidoscope/Runtime.h"                     // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"               // for VirtualProps::Storage, Base<>::Storage
#include "kaleidoscope/event_handler_result.h"        // for EventHandlerResult, EventHandlerRes...
#include "kaleidoscope/layers.h"                      // for Layer, Layer_, layer_count
#include "kaleidoscope/plugin/EEPROM-Settings/crc.h"  // for CRCCalculator, CRC_

namespace kaleidoscope {
namespace plugin {

EventHandlerResult EEPROMSettings::onSetup() {
  Runtime.storage().get(0, settings_);

  /* If the version is undefined, set up sensible defaults. */
  if (settings_.version == VERSION_UNDEFINED) {
    if (settings_.default_layer == 127 ) {
      /* If both of these are set, that means that the EEPROM is uninitialized,
         and setting sensible defaults is safe. If either of them is not at it's
         uninitialized state, we do not override them, to avoid overwriting user
         settings. */
      settings_.default_layer           = 0;
    }

    /* If the version is undefined, we'll set it to our current one. */
    settings_.version = VERSION_CURRENT;

    /* Ideally, we'd save the defaults set here on the first write, but we are
     * not able to catch all writes yet. For the sake of consistency, if we
     * encounter a firmware with no version defined, we'll set sensible
     * defaults. */
    Runtime.storage().put(0, settings_);
    Runtime.storage().commit();
  }
  return EventHandlerResult::OK;
}

EventHandlerResult EEPROMSettings::beforeEachCycle() {
  if (!sealed_)
    seal();

  return EventHandlerResult::OK;
}

bool EEPROMSettings::isValid() {
  return is_valid_;
}

uint16_t EEPROMSettings::crc() {
  if (sealed_)
    return settings_.crc;
  return 0;
}

uint8_t EEPROMSettings::default_layer(uint8_t layer) {
  if (layer < layer_count) {
    Layer.move(layer);
    settings_.default_layer = layer;
  }

  update();
  return settings_.default_layer;
}


void EEPROMSettings::seal() {
  sealed_ = true;

  CRCCalculator.finalize();

  if (settings_.version != VERSION_CURRENT) {
    is_valid_ = false;
    return;
  }

  if (settings_.crc == 0xffff) {
    settings_.crc = CRCCalculator.crc;
    update();
  } else if (settings_.crc != CRCCalculator.crc) {
    return;
  }

  is_valid_ = true;

<<<<<<< Updated upstream
  /* If we have a default layer set, switch to it.
   *
   * We check if the layer is smaller than IGNORE_HARDCODED_LAYER (0x7e),
   * because we want to avoid setting a default layer in two cases:
   *
   * - When the EEPROM is uninitialized (0x7f)
   * - When such layer switching is explicitly turned off (0x7e)
   */
  if (settings_.default_layer < IGNORE_HARDCODED_LAYER && settings_.default_layer < layer_count) {
    Layer.move(settings_.default_layer);
  }
||||||| Stash base
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
=======
  Layer.move(settings_.default_layer);
>>>>>>> Stashed changes
}

uint16_t EEPROMSettings::requestSlice(uint16_t size) {
  if (sealed_)
    return 0;

  uint16_t start = next_start_;
  next_start_ += size;

  CRCCalculator.update((const void *)&size, sizeof(size));

  return start;
}

void EEPROMSettings::invalidate() {
  is_valid_ = false;
}

uint16_t EEPROMSettings::used() {
  return next_start_;
}

void EEPROMSettings::update() {
  Runtime.storage().put(0, settings_);
  Runtime.storage().commit();
  is_valid_ = true;
}

// get a settings slice from the storage and stick it in the untyped struct
// Returns the slice start address or 0 if the  if the EEPROM is not initialized or the settings are invalid)
uint16_t EEPROMSettings::requestSliceAndData(void *data, size_t size) {

  // Request the slice for the struct from storage
  uint16_t start = requestSlice(size);
  if (isSliceValid(start, size)) {
    Runtime.storage().get(start, data);
  }
  return start;
}

bool EEPROMSettings::isSliceValid(uint16_t start, size_t size) {

  // If our slice is uninitialized, then return early.
  if (Runtime.storage().isSliceUninitialized(start, size)) {
    return false;
  }

  // If the slice of storage is initialized, but settings are invalid, then return early.
  if (!isValid()) {
    return false;
  }
  return true;
}

/** Focus **/
EventHandlerResult FocusSettingsCommand::onFocusEvent(const char *input) {
  enum {
    DEFAULT_LAYER,
    IS_VALID,
    GET_VERSION,
    GET_CRC,
  } sub_command;

  const char *cmd_defaultLayer = PSTR("settings.defaultLayer");
  const char *cmd_isValid      = PSTR("settings.valid?");
  const char *cmd_version      = PSTR("settings.version");
  const char *cmd_crc          = PSTR("settings.crc");

  if (::Focus.inputMatchesHelp(input))
    return ::Focus.printHelp(cmd_defaultLayer, cmd_isValid, cmd_version, cmd_crc);

  if (::Focus.inputMatchesCommand(input, cmd_defaultLayer))
    sub_command = DEFAULT_LAYER;
  else if (::Focus.inputMatchesCommand(input, cmd_isValid))
    sub_command = IS_VALID;
  else if (::Focus.inputMatchesCommand(input, cmd_version))
    sub_command = GET_VERSION;
  else if (::Focus.inputMatchesCommand(input, cmd_crc))
    sub_command = GET_CRC;
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
  case GET_CRC:
    ::Focus.sendRaw(::CRCCalculator.crc, F("/"), ::EEPROMSettings.crc());
    break;
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult FocusEEPROMCommand::onFocusEvent(const char *input) {
  enum {
    CONTENTS,
    FREE,
    ERASE,
  } sub_command;

  const char *cmd_contents = PSTR("eeprom.contents");
  const char *cmd_free     = PSTR("eeprom.free");
  const char *cmd_erase    = PSTR("eeprom.erase");

  if (::Focus.inputMatchesHelp(input))
    return ::Focus.printHelp(cmd_contents, cmd_free, cmd_erase);

  if (::Focus.inputMatchesCommand(input, cmd_contents))
    sub_command = CONTENTS;
  else if (::Focus.inputMatchesCommand(input, cmd_free))
    sub_command = FREE;
  else if (::Focus.inputMatchesCommand(input, cmd_erase))
    sub_command = ERASE;
  else
    return EventHandlerResult::OK;

  switch (sub_command) {
  case CONTENTS: {
    if (::Focus.isEOL()) {
      for (uint16_t i = 0; i < Runtime.storage().length(); i++) {
        uint8_t d = Runtime.storage().read(i);
        ::Focus.send(d);
      }
    } else {
      for (uint16_t i = 0; i < Runtime.storage().length() && !::Focus.isEOL(); i++) {
        uint8_t d;
        ::Focus.read(d);
        Runtime.storage().update(i, d);
      }
      Runtime.storage().commit();
    }

    break;
  }
  case FREE:
    ::Focus.send(Runtime.storage().length() - ::EEPROMSettings.used());
    break;
  case ERASE: {
    for (uint16_t i = 0; i < Runtime.storage().length(); i++) {
      Runtime.storage().update(i, EEPROMSettings::EEPROM_UNINITIALIZED_BYTE);
    }
    Runtime.storage().commit();
    Runtime.device().rebootBootloader();
    break;
  }
  }
  return EventHandlerResult::EVENT_CONSUMED;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::EEPROMSettings EEPROMSettings;
kaleidoscope::plugin::FocusSettingsCommand FocusSettingsCommand;
kaleidoscope::plugin::FocusEEPROMCommand FocusEEPROMCommand;
