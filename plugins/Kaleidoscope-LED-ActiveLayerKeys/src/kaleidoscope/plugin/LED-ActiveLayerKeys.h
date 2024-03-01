/* -*- mode: c++ -*-
 * Kaleidoscope-LED-ActiveLayerKeys -- Light up the LEDs based on the keys specified in the active layer
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
class LEDActiveLayerKeysEffect : public Plugin,
                                 public LEDModeInterface,
                                 public AccessTransientLEDMode {
 public:
  LEDActiveLayerKeysEffect() { led_mode_name_ = "ActiveLayerKeys"; }
  explicit LEDActiveLayerKeysEffect(const char *led_mode_name) { led_mode_name_ = led_mode_name; }

  EventHandlerResult onLayerChange();
  template<uint8_t _colormap_size>
  void setColormap(cRGB const (&colormap)[_colormap_size]) {
    colormap_      = colormap;
    colormap_size_ = _colormap_size;
  }
  static void lightLowerLayers(bool show) {
    light_lower_layers = show;
  }
  static void setDefaultColor(cRGB color) {
    default_layer_color_ = color;
  }

  // This class' instance has dynamic lifetime
  //
  class TransientLEDMode : public LEDMode {
   public:
    // Please note that storing the parent ptr is only required
    // for those LED modes that require access to
    // members of their parent class. Most LED modes can do without.
    //
    explicit TransientLEDMode(const LEDActiveLayerKeysEffect *parent);

   protected:
    void onActivate() final;
    void refreshAt(KeyAddr key_addr) final;

   private:
    const LEDActiveLayerKeysEffect *parent_;

    cRGB active_color_;

    cRGB getLayerColor(uint8_t layer);

    friend class LEDActiveLayerKeysEffect;
  };

 private:
  // An array of Colormap entries in PROGMEM.
  cRGB const *colormap_{nullptr};
  uint8_t colormap_size_{0};

  static cRGB default_layer_color_;
  static bool light_lower_layers;
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::LEDActiveLayerKeysEffect LEDActiveLayerKeysEffect;
