/* -*- mode: c++ -*-
 * Kaleidoscope-LED-AlphaSquare -- 4x4 pixel LED alphabet
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

#pragma once

#include <stdint.h>                                      // for uint16_t
#include "Arduino.h"                                     // for byte
#include "kaleidoscope/KeyAddr.h"                        // for KeyAddr
#include "kaleidoscope/event_handler_result.h"           // for EventHandler...
#include "kaleidoscope/key_defs.h"                       // for Key
#include "kaleidoscope/plugin.h"                         // for Plugin
#include "kaleidoscope/plugin/AccessTransientLEDMode.h"  // for AccessTransi...
#include "kaleidoscope/plugin/LEDMode.h"                 // for LEDMode
#include "kaleidoscope/plugin/LEDModeInterface.h"        // for LEDModeInter...
#include "kaleidoscope_internal/deprecations.h"          // for DEPRECATED

namespace kaleidoscope {
namespace plugin {
class AlphaSquareEffect : public Plugin,
  public LEDModeInterface,
  public AccessTransientLEDMode {
 public:
  AlphaSquareEffect(void) {}

  static uint16_t length;

  EventHandlerResult onKeyswitchEvent(Key &mappedKey, KeyAddr key_addr, uint8_t keyState);

  // This class' instance has dynamic lifetime
  //
  class TransientLEDMode : public LEDMode {
   public:
    TransientLEDMode(AlphaSquareEffect *parent);

   protected:
    void update(void) final;
    void refreshAt(KeyAddr key_addr) final;
    DEPRECATED(ROW_COL_FUNC) void refreshAt(byte row, byte col) final {
      refreshAt(KeyAddr(row, col));
    }

   private:
    uint16_t start_time_left_, start_time_right_;
    Key last_key_left_, last_key_right_;

    friend class AlphaSquareEffect;
  };
};
}
}

extern kaleidoscope::plugin::AlphaSquareEffect AlphaSquareEffect;
