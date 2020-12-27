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


#include <Kaleidoscope-Colormap.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-FocusSerial.h>
#include "kaleidoscope/layers.h"

namespace kaleidoscope {
namespace plugin {

uint16_t ColormapEffect::map_base_;
uint8_t ColormapEffect::max_layers_;
uint8_t ColormapEffect::top_layer_;

void ColormapEffect::max_layers(uint8_t max_) {
  if (map_base_ != 0)
    return;

  max_layers_ = max_;
  map_base_ = ::LEDPaletteTheme.reserveThemes(max_layers_);
}

void ColormapEffect::TransientLEDMode::onActivate(void) {
  if (!Runtime.has_leds)
    return;

  parent_->top_layer_ = Layer.mostRecent();
  if (parent_->top_layer_ <= parent_->max_layers_)
    ::LEDPaletteTheme.updateHandler(parent_->map_base_, parent_->top_layer_);
}

void ColormapEffect::TransientLEDMode::refreshAt(KeyAddr key_addr) {
  if (parent_->top_layer_ <= parent_->max_layers_)
    ::LEDPaletteTheme.refreshAt(parent_->map_base_, parent_->top_layer_, key_addr);
}

EventHandlerResult ColormapEffect::onLayerChange() {
  if (::LEDControl.get_mode_index() == led_mode_id_)
    ::LEDControl.get_mode<TransientLEDMode>()->onActivate();
  return EventHandlerResult::OK;
}

EventHandlerResult ColormapEffect::onFocusEvent(const char *command) {
  return ::LEDPaletteTheme.themeFocusEvent(command, PSTR("colormap.map"),
                                           map_base_, max_layers_);
}

}
}

kaleidoscope::plugin::ColormapEffect ColormapEffect;
