/* -*- mode: c++ -*-
 * Kaleidoscope-MagicCombo -- Magic combo framework
 * Copyright (C) 2016-2021  Keyboard.io, Inc
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

#include "kaleidoscope/plugin/MagicCombo.h"

#include <Arduino.h>                   // for F, __FlashStringHelper, pgm_read_byte
#include <Kaleidoscope-FocusSerial.h>  // for Focus, FocusSerial
#include <stdint.h>                    // for uint16_t, int8_t, uint8_t

#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"         // for Device
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK

namespace kaleidoscope {
namespace plugin {

#ifndef NDEPRECATED
uint16_t MagicCombo::min_interval = 500;
#endif

EventHandlerResult MagicCombo::onNameQuery() {
  return ::Focus.sendName(F("MagicCombo"));
}

EventHandlerResult MagicCombo::afterEachCycle() {
  for (uint8_t i = 0; i < magiccombo::combos_length; i++) {
    bool match = true;
    uint8_t j;

    for (j = 0; j < MAX_COMBO_LENGTH; j++) {
      int8_t comboKey = pgm_read_byte(&(magiccombo::combos[i].keys[j]));

      if (comboKey == 0)
        break;

      match &= Runtime.device().isKeyswitchPressed(comboKey);
      if (!match)
        break;
    }

    if (j != Runtime.device().pressedKeyswitchCount())
      match = false;

    if (match && Runtime.hasTimeExpired(start_time_, getMinInterval())) {
      ComboAction action = (ComboAction)pgm_read_ptr((void const **)&(magiccombo::combos[i].action));

      (*action)(i);
      start_time_ = Runtime.millisAtCycleStart();
    }
  }

  return EventHandlerResult::OK;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::MagicCombo MagicCombo;
