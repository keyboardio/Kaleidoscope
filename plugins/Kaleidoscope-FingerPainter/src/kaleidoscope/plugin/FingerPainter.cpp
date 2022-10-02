/* -*- mode: c++ -*-
 * Kaleidoscope-FingerPainter -- On-the-fly keyboard painting.
 * Copyright (C) 2017-2022  Keyboard.io, Inc
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

#include "kaleidoscope/plugin/FingerPainter.h"

#include <Arduino.h>                         // for PSTR, F, __FlashStringHelper
#include <Kaleidoscope-FocusSerial.h>        // for Focus, FocusSerial
#include <Kaleidoscope-LED-Palette-Theme.h>  // for LEDPaletteTheme
#include <stdint.h>                          // for uint16_t, uint8_t
#include <string.h>                          // for memcmp

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"         // for Device, cRGB, VirtualProps::Storage, Base...
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/keyswitch_state.h"       // for keyToggledOff

namespace kaleidoscope {
namespace plugin {

EventHandlerResult FingerPainter::onNameQuery() {
  return ::Focus.sendName(F("FingerPainter"));
}

EventHandlerResult FingerPainter::onSetup() {
  color_base_ = ::LEDPaletteTheme.reserveThemes(1);
  return EventHandlerResult::OK;
}

void FingerPainter::update() {
  ::LEDPaletteTheme.updateHandler(color_base_, 0);
}

void FingerPainter::refreshAt(KeyAddr key_addr) {
  ::LEDPaletteTheme.refreshAt(color_base_, 0, key_addr);
}

void FingerPainter::toggle() {
  edit_mode_ = !edit_mode_;
}

EventHandlerResult FingerPainter::onKeyEvent(KeyEvent &event) {
  if (!Runtime.has_leds || !edit_mode_)
    return EventHandlerResult::OK;

  if (keyToggledOff(event.state)) {
    return EventHandlerResult::OK;
  }

  if (!event.addr.isValid())
    return EventHandlerResult::OK;

  // TODO(anyone): The following works only for keyboards with LEDs for each key.

  uint8_t color_index = ::LEDPaletteTheme
                          .lookupColorIndexAtPosition(color_base_,
                                                      Runtime.device().getLedIndex(event.addr));

  // Find the next color in the palette that is different.
  // But do not loop forever!
  bool turn_around = false;
  cRGB old_color = ::LEDPaletteTheme.lookupPaletteColor(color_index), new_color = old_color;
  while (memcmp(&old_color, &new_color, sizeof(cRGB)) == 0) {
    color_index++;
    if (color_index > 15) {
      color_index = 0;
      if (turn_around)
        break;
      turn_around = true;
    }
    new_color = ::LEDPaletteTheme.lookupPaletteColor(color_index);
  }

  ::LEDPaletteTheme.updateColorIndexAtPosition(color_base_,
                                               Runtime.device().getLedIndex(event.addr),
                                               color_index);
  Runtime.storage().commit();

  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult FingerPainter::onFocusEvent(const char *input) {
  enum {
    TOGGLE,
    CLEAR,
  } sub_command;

  const char *cmd_toggle = PSTR("fingerpainter.toggle");
  const char *cmd_clear  = PSTR("fingerpainter.clear");

  if (::Focus.inputMatchesHelp(input))
    return ::Focus.printHelp(cmd_toggle, cmd_clear);

  if (::Focus.inputMatchesCommand(input, cmd_toggle))
    sub_command = TOGGLE;
  else if (::Focus.inputMatchesCommand(input, cmd_clear))
    sub_command = CLEAR;
  else
    return EventHandlerResult::OK;

  if (sub_command == CLEAR) {
    for (uint16_t i = 0; i < Runtime.device().numKeys() / 2; i++) {
      Runtime.storage().update(color_base_ + i, 0);
    }
    Runtime.storage().commit();
    return EventHandlerResult::OK;
  }

  ::FingerPainter.activate();
  toggle();

  return EventHandlerResult::OK;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::FingerPainter FingerPainter;
