/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Keymap-Programmer -- On-the-fly reprogrammable keymap.
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

#include <Kaleidoscope-EEPROM-Keymap-Programmer.h>
#include <Kaleidoscope-FocusSerial.h>
#include "kaleidoscope/keyswitch_state.h"
#include "kaleidoscope/layers.h"

namespace kaleidoscope {
namespace plugin {
uint16_t EEPROMKeymapProgrammer::update_position_;
EEPROMKeymapProgrammer::state_t EEPROMKeymapProgrammer::state_;
EEPROMKeymapProgrammer::mode_t EEPROMKeymapProgrammer::mode;
Key EEPROMKeymapProgrammer::new_key_;

void EEPROMKeymapProgrammer::nextState(void) {
  switch (state_) {
  case INACTIVE:
    state_ = WAIT_FOR_KEY;
    break;
  case WAIT_FOR_KEY:
    if (mode == CODE)
      state_ = WAIT_FOR_CODE;
    else
      state_ = WAIT_FOR_SOURCE_KEY;
    break;
  case WAIT_FOR_CODE:
  case WAIT_FOR_SOURCE_KEY:
    ::EEPROMKeymap.updateKey(update_position_, new_key_);
    Runtime.storage().commit();
    cancel();
    break;
  }
}

void EEPROMKeymapProgrammer::cancel(void) {
  update_position_ = 0;
  new_key_ = Key_NoKey;
  state_ = INACTIVE;
}

EventHandlerResult EEPROMKeymapProgrammer::onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t key_state) {
  if (state_ == INACTIVE)
    return EventHandlerResult::OK;

  if (state_ == WAIT_FOR_KEY) {
    if (keyToggledOn(key_state)) {
      update_position_ = Layer.mostRecent() * Runtime.device().numKeys() + key_addr.toInt();
    }
    if (keyToggledOff(key_state)) {
      if ((uint16_t)(Layer.mostRecent() * Runtime.device().numKeys() + key_addr.toInt()) == update_position_)
        nextState();
    }
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (state_ == WAIT_FOR_SOURCE_KEY) {
    if (keyToggledOn(key_state)) {
      new_key_ = Layer.getKeyFromPROGMEM(Layer.mostRecent(), key_addr);
    }
    if (keyToggledOff(key_state)) {
      if (new_key_ == Layer.getKeyFromPROGMEM(Layer.mostRecent(), key_addr))
        nextState();
    }
    return EventHandlerResult::EVENT_CONSUMED;
  }

  // WAIT_FOR_CODE state

  if (mapped_key < Key_1 || mapped_key > Key_0)
    return EventHandlerResult::OK;

  if (!keyToggledOn(key_state)) {
    return EventHandlerResult::EVENT_CONSUMED;
  }

  uint8_t n;
  if (mapped_key.getKeyCode() == Key_0.getKeyCode())
    n = 0;
  else
    n = mapped_key.getKeyCode() - Key_1.getKeyCode() + 1;

  new_key_.setRaw(new_key_.getRaw() * 10 + n);

  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult EEPROMKeymapProgrammer::onFocusEvent(const char *command) {
  const char *cmd = PSTR("keymap.toggleProgrammer");

  if (::Focus.handleHelp(command, cmd))
    return EventHandlerResult::OK;

  if (strcmp_P(command, cmd) != 0)
    return EventHandlerResult::OK;

  if (state_ == INACTIVE)
    activate();
  else
    cancel();

  return EventHandlerResult::EVENT_CONSUMED;
}

}
}

kaleidoscope::plugin::EEPROMKeymapProgrammer EEPROMKeymapProgrammer;
