/* -*- mode: c++ -*-
 * Kaleidoscope-LED-ActiveLayerColor -- Light up the LEDs based on the active layers
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

#include "kaleidoscope/KeyAddr.h"                        // for KeyAddr
#include "kaleidoscope/device/device.h"                  // for cRGB
#include "kaleidoscope/event_handler_result.h"           // for EventHandlerResult
#include "kaleidoscope/plugin.h"                         // for Plugin
#include "kaleidoscope/plugin/AccessTransientLEDMode.h"  // for AccessTransientLEDMode
#include "kaleidoscope/plugin/LEDMode.h"                 // for LEDMode
#include "kaleidoscope/plugin/LEDModeInterface.h"        // for LEDModeInterface

namespace kaleidoscope {
namespace plugin {
class LEDActiveLayerColorEffect : public Plugin,
                                  public LEDModeInterface,
                                  public AccessTransientLEDMode {
 public:
  EventHandlerResult onLayerChange();
  void setColormap(const cRGB colormap[]);

  // This class' instance has dynamic lifetime
  //
  class TransientLEDMode : public LEDMode {
   public:
    // Please note that storing the parent ptr is only required
    // for those LED modes that require access to
    // members of their parent class. Most LED modes can do without.
    //
    explicit TransientLEDMode(const LEDActiveLayerColorEffect *parent);

   protected:
    void onActivate() final;
    void refreshAt(KeyAddr key_addr) final;

   private:
    const LEDActiveLayerColorEffect *parent_;

    cRGB active_color_;

    cRGB getActiveColor();

    friend class LEDActiveLayerColorEffect;
  };

 private:
  static const cRGB *colormap_;
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::LEDActiveLayerColorEffect LEDActiveLayerColorEffect;
