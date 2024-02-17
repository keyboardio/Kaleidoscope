/* -*- mode: c++ -*-
 * Kaleidoscope-Colormap -- Per-layer colormap effect
 * Copyright (C) 2016-2022  Keyboard.io, Inc
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

#include "kaleidoscope/plugin/Colormap.h"

#include <Arduino.h>                         // for F, PSTR, __FlashStringHelper
#include <Kaleidoscope-FocusSerial.h>        // for Focus, FocusSerial
#include <Kaleidoscope-LED-Palette-Theme.h>  // for LEDPaletteTheme
#include <stdint.h>                          // for uint8_t, uint16_t

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/layers.h"                // for Layer, Layer_
#include "kaleidoscope/plugin/LEDControl.h"     // for LEDControl

namespace kaleidoscope {
namespace plugin {

uint16_t ColormapEffect::map_base_;
uint8_t ColormapEffect::max_layers_;
uint8_t ColormapEffect::top_layer_;

void ColormapEffect::max_layers(uint8_t max_) {
  if (map_base_ != 0)
    return;

  max_layers_ = max_;
  map_base_   = ::LEDPaletteTheme.reserveThemes(max_layers_);
}

EventHandlerResult ColormapEffect::onNameQuery() {
  return ::Focus.sendName(F("ColormapEffect"));
}

bool ColormapEffect::isUninitialized() {
  return ::LEDPaletteTheme.isThemeUninitialized(map_base_, max_layers_);
}

void ColormapEffect::updateColorIndexAtPosition(uint8_t layer, uint16_t position, uint8_t palette_index) {
  if (layer >= max_layers_) return;

  uint16_t index = Runtime.device().led_count * layer + position;
  ::LEDPaletteTheme.updateColorIndexAtPosition(map_base_, index, palette_index);
}

void ColormapEffect::TransientLEDMode::onActivate() {
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

EventHandlerResult ColormapEffect::onFocusEvent(const char *input) {
  return ::LEDPaletteTheme.themeFocusEvent(input, PSTR("colormap.map"), map_base_, max_layers_);
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::ColormapEffect ColormapEffect;
