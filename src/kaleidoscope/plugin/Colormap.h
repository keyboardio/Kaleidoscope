/* -*- mode: c++ -*-
 * Kaleidoscope-Colormap -- Per-layer colormap effect
 * Copyright (C) 2016, 2017, 2018  Keyboard.io, Inc
 *
 * This program is free software: you can redistribute it and/or modify it under it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdint.h>                                      // for uint8_t, uin...
#include "Arduino.h"                                     // for byte
#include "kaleidoscope/KeyAddr.h"                        // for KeyAddr
#include "kaleidoscope/event_handler_result.h"           // for EventHandler...
#include "kaleidoscope/plugin.h"                         // for Plugin
#include "kaleidoscope/plugin/AccessTransientLEDMode.h"  // for AccessTransi...
#include "kaleidoscope/plugin/LEDMode.h"                 // for LEDMode
#include "kaleidoscope/plugin/LEDModeInterface.h"        // for LEDModeInter...
#include "kaleidoscope_internal/deprecations.h"          // for DEPRECATED

namespace kaleidoscope {
namespace plugin {
class ColormapEffect : public Plugin,
  public LEDModeInterface,
  public AccessTransientLEDMode {
 public:
  ColormapEffect(void) {}

  void max_layers(uint8_t max_);

  EventHandlerResult onLayerChange();
  EventHandlerResult onFocusEvent(const char *command);

  // This class' instance has dynamic lifetime
  //
  class TransientLEDMode : public LEDMode {
   public:

    // Please note that storing the parent ptr is only required
    // for those LED modes that require access to
    // members of their parent class. Most LED modes can do without.
    //
    TransientLEDMode(const ColormapEffect *parent) : parent_(parent) {}

   protected:

    friend class ColormapEffect;

    virtual void onActivate(void) final;
    virtual void refreshAt(KeyAddr key_addr) final;
    DEPRECATED(ROW_COL_FUNC) void refreshAt(byte row, byte col) final {
      refreshAt(KeyAddr(row, col));
    }
   private:

    const ColormapEffect *parent_;
  };

 private:
  static uint8_t top_layer_;
  static uint8_t max_layers_;
  static uint16_t map_base_;
};
}
}

extern kaleidoscope::plugin::ColormapEffect ColormapEffect;
