/* -*- mode: c++ -*-
 * Kaleidoscope-MagicCombo -- Magic combo framework
 * Copyright (C) 2016, 2017, 2018  Keyboard.io, Inc
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

#include <Kaleidoscope-MagicCombo.h>

namespace kaleidoscope {
namespace plugin {

uint16_t MagicCombo::min_interval = 500;
uint16_t MagicCombo::start_time_ = 0;

EventHandlerResult MagicCombo::beforeReportingState() {
  for (byte i = 0; i < magiccombo::combos_length; i++) {
    bool match = true;
    byte j;

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

    if (match && Runtime.hasTimeExpired(start_time_, min_interval)) {
      ComboAction action = (ComboAction) pgm_read_ptr((void const **) & (magiccombo::combos[i].action));

      (*action)(i);
      start_time_ = Runtime.millisAtCycleStart();
    }
  }

  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::MagicCombo MagicCombo;
