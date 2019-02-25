/* -*- mode: c++ -*-
 * Kaleidoscope-LED-Stalker -- Stalk keys pressed by lighting up and fading back the LED under them
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

#include <Kaleidoscope-LED-Stalker.h>
#include <Kaleidoscope-LEDControl.h>

namespace kaleidoscope {
namespace plugin {

StalkerEffect::ColorComputer *StalkerEffect::variant;
uint16_t StalkerEffect::step_length = 50;
cRGB StalkerEffect::inactive_color = (cRGB) {
  0, 0, 0
};

StalkerEffect::TransientLEDMode::TransientLEDMode(const StalkerEffect *parent)
  : parent_(parent),
    step_start_time_(0),
    map_{}
{}

EventHandlerResult StalkerEffect::onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t keyState) {
  if (!Kaleidoscope.has_leds)
    return EventHandlerResult::OK;

  if (!key_addr.isValid())
    return EventHandlerResult::OK;

  if (::LEDControl.get_mode_index() != led_mode_id_)
    return EventHandlerResult::OK;

  if (keyIsPressed(keyState)) {
    ::LEDControl.get_mode<TransientLEDMode>()->map_[key_addr.toInt()] = 0xff;
  }

  return EventHandlerResult::OK;
}

void StalkerEffect::TransientLEDMode::update(void) {
  if (!Kaleidoscope.has_leds)
    return;

  if (!parent_->variant)
    return;

  uint16_t elapsed = Kaleidoscope.millisAtCycleStart() - step_start_time_;
  if (elapsed < parent_->step_length)
    return;

  for (auto key_addr : KeyAddr::all()) {
    uint8_t step = map_[key_addr.toInt()];
    if (step) {
      ::LEDControl.setCrgbAt(key_addr, parent_->variant->compute(&step));
    }

    map_[key_addr.toInt()] = step;

    if (!map_[key_addr.toInt()])
      ::LEDControl.setCrgbAt(key_addr, parent_->inactive_color);
  }

  step_start_time_ = Kaleidoscope.millisAtCycleStart();
}

namespace stalker {

cRGB Haunt::highlight_color_;

// Haunt
Haunt::Haunt(const cRGB highlight_color) {
  highlight_color_ = highlight_color;
}

cRGB Haunt::compute(uint8_t *step) {
  cRGB color = CRGB((uint8_t)min(*step * highlight_color_.r / 255, 255),
                    (uint8_t)min(*step * highlight_color_.g / 255, 255),
                    (uint8_t)min(*step * highlight_color_.b / 255, 255));

  if (*step >= 0xf0)
    *step -= 1;
  else if (*step >= 0x40)
    *step -= 16;
  else if (*step >= 32)
    *step -= 32;
  else
    *step = 0;

  return color;
}

// BlazingTrail
BlazingTrail::BlazingTrail(void) {
}

cRGB BlazingTrail::compute(uint8_t *step) {
  cRGB color;

  if (*step >= 0xff - 30) {
    color = hsvToRgb(0xff - *step, 255, 255);
  } else {
    color = hsvToRgb(30, 255, 255);

    color.r = min(*step * color.r / 255, 255);
    color.g = min(*step * color.g / 255, 255);
  }

  if (*step >= 0xff - 30)
    *step -= 1;
  else if (*step >= 0x40)
    *step -= 16;
  else if (*step >= 32)
    *step -= 32;
  else
    *step = 0;

  return color;
}

// Rainbow
Rainbow::Rainbow(void) {
}

cRGB Rainbow::compute(uint8_t *step) {
  if (*step > 0)
    *step -= 1;
  else
    *step = 0;

  return hsvToRgb(255 - *step, 255, *step);
}

}
}

}

kaleidoscope::plugin::StalkerEffect StalkerEffect;
