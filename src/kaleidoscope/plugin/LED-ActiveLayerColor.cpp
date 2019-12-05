/* -*- mode: c++ -*-
 * Kaleidoscope-LED-ActiveLayerColor -- Light up the LEDs based on the active layers
 * Copyright (C) 2018  Keyboard.io, Inc
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

#include <Kaleidoscope-LED-ActiveLayerColor.h>

namespace kaleidoscope {
namespace plugin {

const cRGB *LEDActiveLayerColorEffect::colormap_;

LEDActiveLayerColorEffect::TransientLEDMode::TransientLEDMode(
  const LEDActiveLayerColorEffect *parent)
  : parent_(parent),
    active_color_{0, 0, 0}
{}

void LEDActiveLayerColorEffect::setColormap(const cRGB colormap[]) {
  colormap_ = colormap;
}

cRGB LEDActiveLayerColorEffect::TransientLEDMode::getActiveColor(uint8_t top_layer) {
  cRGB color;

  color.r = pgm_read_byte(&(parent_->colormap_[top_layer].r));
  color.g = pgm_read_byte(&(parent_->colormap_[top_layer].g));
  color.b = pgm_read_byte(&(parent_->colormap_[top_layer].b));

  return color;
}

void LEDActiveLayerColorEffect::TransientLEDMode::onActivate(void) {
  if (!Kaleidoscope.has_leds)
    return;

  for (auto key_addr : KeyAddr::all()) {
    refreshAt(key_addr);
  }
}

void LEDActiveLayerColorEffect::TransientLEDMode::refreshAt(KeyAddr key_addr) {
  uint8_t key_group = groupOfKey(key_addr);
  uint8_t top_layer = ::Layer.top(key_group);
  cRGB active_color = getActiveColor(top_layer);
  ::LEDControl.setCrgbAt(key_addr, active_color);
}

EventHandlerResult LEDActiveLayerColorEffect::onLayerChange() {
  if (::LEDControl.get_mode_index() == led_mode_id_)
    ::LEDControl.get_mode<TransientLEDMode>()->onActivate();

  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::LEDActiveLayerColorEffect LEDActiveLayerColorEffect;
