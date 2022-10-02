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

#include "kaleidoscope/plugin/EEPROM-Keymap-Programmer.h"

#include <Arduino.h>                     // for PSTR
#include <Kaleidoscope-EEPROM-Keymap.h>  // for EEPROMKeymap
#include <Kaleidoscope-FocusSerial.h>    // for Focus, FocusSerial
#include <stdint.h>                      // for uint16_t, uint8_t

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"         // for Device, Base<>::Storage, VirtualProps::St...
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::E...
#include "kaleidoscope/key_defs.h"              // for Key, Key_0, Key_1, Key_NoKey
#include "kaleidoscope/keyswitch_state.h"       // for keyToggledOn, keyToggledOff
#include "kaleidoscope/layers.h"                // for Layer, Layer_

namespace kaleidoscope {
namespace plugin {

void EEPROMKeymapProgrammer::nextState() {
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

void EEPROMKeymapProgrammer::cancel() {
  update_position_ = 0;
  new_key_         = Key_NoKey;
  state_           = INACTIVE;
}

EventHandlerResult EEPROMKeymapProgrammer::onKeyEvent(KeyEvent &event) {
  if (state_ == INACTIVE)
    return EventHandlerResult::OK;

  if (state_ == WAIT_FOR_KEY) {
    if (keyToggledOn(event.state)) {
      update_position_ = Layer.mostRecent() * Runtime.device().numKeys() + event.addr.toInt();
    }
    if (keyToggledOff(event.state)) {
      if ((uint16_t)(Layer.mostRecent() * Runtime.device().numKeys() + event.addr.toInt()) == update_position_)
        nextState();
    }
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (state_ == WAIT_FOR_SOURCE_KEY) {
    if (keyToggledOn(event.state)) {
      new_key_ = Layer.getKeyFromPROGMEM(Layer.mostRecent(), event.addr);
    }
    if (keyToggledOff(event.state)) {
      if (new_key_ == Layer.getKeyFromPROGMEM(Layer.mostRecent(), event.addr))
        nextState();
    }
    return EventHandlerResult::EVENT_CONSUMED;
  }

  // WAIT_FOR_CODE state

  if (event.key < Key_1 || event.key > Key_0)
    return EventHandlerResult::OK;

  if (!keyToggledOn(event.state)) {
    return EventHandlerResult::EVENT_CONSUMED;
  }

  uint8_t n;
  if (event.key.getKeyCode() == Key_0.getKeyCode())
    n = 0;
  else
    n = event.key.getKeyCode() - Key_1.getKeyCode() + 1;

  new_key_.setRaw(new_key_.getRaw() * 10 + n);

  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult EEPROMKeymapProgrammer::onFocusEvent(const char *input) {
  const char *cmd = PSTR("keymap.toggleProgrammer");

  if (::Focus.inputMatchesHelp(input))
    return ::Focus.printHelp(cmd);

  if (!::Focus.inputMatchesCommand(input, cmd))
    return EventHandlerResult::OK;

  if (state_ == INACTIVE)
    activate();
  else
    cancel();

  return EventHandlerResult::EVENT_CONSUMED;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::EEPROMKeymapProgrammer EEPROMKeymapProgrammer;
