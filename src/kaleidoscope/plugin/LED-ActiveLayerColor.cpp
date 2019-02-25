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

cRGB LEDActiveLayerColorEffect::active_color_;
const cRGB *LEDActiveLayerColorEffect::colormap_;

void LEDActiveLayerColorEffect::setColormap(const cRGB colormap[]) {
  colormap_ = colormap;
}

cRGB LEDActiveLayerColorEffect::getActiveColor() {
  cRGB color;

  uint8_t top_layer = ::Layer.top();

  color.r = pgm_read_byte(&(colormap_[top_layer].r));
  color.g = pgm_read_byte(&(colormap_[top_layer].g));
  color.b = pgm_read_byte(&(colormap_[top_layer].b));

  return color;
}

void LEDActiveLayerColorEffect::onActivate(void) {
  if (!Kaleidoscope.has_leds)
    return;

  active_color_ = getActiveColor();
  ::LEDControl.set_all_leds_to(active_color_);
}

void LEDActiveLayerColorEffect::refreshAt(LEDAddr led_addr) {
  ::LEDControl.setCrgbAt(led_addr, active_color_);
}

EventHandlerResult LEDActiveLayerColorEffect::onLayerChange() {
  if (::LEDControl.get_mode() == this)
    onActivate();
  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::LEDActiveLayerColorEffect LEDActiveLayerColorEffect;
