/* -*- mode: c++ -*-
 * Kaleidoscope-MagicCombo -- Magic combo framework
 * Copyright (C) 2016, 2017, 2018  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Kaleidoscope-MagicCombo.h>

namespace kaleidoscope {

uint16_t MagicCombo::min_interval = 500;
uint32_t MagicCombo::end_time_;

EventHandlerResult MagicCombo::beforeReportingState() {
  for (byte i = 0; i < magiccombo::combos_length; i++) {
    bool match = true;
    byte j;

    for (j = 0; j < MAX_COMBO_LENGTH; j++) {
      int8_t comboKey = pgm_read_byte(&(magiccombo::combos[i].keys[j]));

      if (comboKey == 0)
        break;

      match &= KeyboardHardware.isKeyswitchPressed(comboKey);
      if (!match)
        break;
    }

    if (j != KeyboardHardware.pressedKeyswitchCount())
      match = false;

    if (match && (millis() >= end_time_)) {
      ComboAction action = (ComboAction) pgm_read_ptr(&(magiccombo::combos[i].action));

      (*action)(i);
      end_time_ = millis() + min_interval;
    }
  }

  return EventHandlerResult::OK;
}

// Legacy V1 API
#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
void MagicCombo::begin() {
  Kaleidoscope.useLoopHook(legacyLoopHook);
}

void MagicCombo::legacyLoopHook(bool is_post_clear) {
  if (is_post_clear)
    return;
  ::MagicCombo.beforeReportingState();
}
#endif

};

kaleidoscope::MagicCombo MagicCombo;
