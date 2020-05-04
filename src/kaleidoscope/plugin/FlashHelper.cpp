/* -*- mode: c++ -*-
 * Kaleidoscope-FlashHelper -- Firmware-assisted flashing
 * Copyright (C) 2020  Keyboard.io, Inc
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

#include <Kaleidoscope-FlashHelper.h>
#include <Kaleidoscope-FocusSerial.h>

#include "kaleidoscope/key_events.h"

namespace kaleidoscope {
namespace plugin {

uint8_t FlashHelper::timeout_ = 10;
uint32_t FlashHelper::start_time_;
bool FlashHelper::is_active_;

EventHandlerResult FlashHelper::onFocusEvent(const char *command) {
  if (::Focus.handleHelp(command, PSTR("flash.prepare\nflash.resume")))
    return EventHandlerResult::OK;

  if (strncmp_P(command, PSTR("flash."), 6) != 0)
    return EventHandlerResult::OK;

  if (strcmp_P(command + 6, PSTR("prepare")) == 0) {
    activate();
    return EventHandlerResult::EVENT_CONSUMED;
  }
  if (strcmp_P(command + 6, PSTR("resume")) == 0) {
    deactivate();
    return EventHandlerResult::EVENT_CONSUMED;
  }

  return EventHandlerResult::OK;

}

EventHandlerResult FlashHelper::beforeEachCycle() {
  if (!isActive())
    goto end;

  if (!Runtime.hasTimeExpired(start_time_, (uint16_t)(timeout_ * 1000)))
    goto end;

  deactivate();

end:
  return EventHandlerResult::OK;
}

EventHandlerResult FlashHelper::onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t key_state) {
  if (!isActive())
    return EventHandlerResult::OK;

  const auto progKeys = Runtime.device().progKeyAddresses();
  const uint8_t progKeyNum = Runtime.device().numProgKeys();

  for (int8_t i = 0; i < progKeyNum; i++) {
    if (key_addr.toInt() == progKeys[i]) {
      return EventHandlerResult::EVENT_CONSUMED;
    }
  }

  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::FlashHelper FlashHelper;
