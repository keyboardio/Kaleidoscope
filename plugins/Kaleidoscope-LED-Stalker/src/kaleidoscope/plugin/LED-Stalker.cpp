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

#include "kaleidoscope/plugin/LED-Stalker.h"

#include <Arduino.h>  // for min
#include <stdint.h>   // for uint8_t, uint16_t, uint32_t

#include "kaleidoscope/KeyAddr.h"                     // for MatrixAddr, KeyAddr, MatrixAddr<>::...
#include "kaleidoscope/KeyEvent.h"                    // for KeyEvent
#include "kaleidoscope/Runtime.h"                     // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"               // for cRGB, CRGB
#include "kaleidoscope/event_handler_result.h"        // for EventHandlerResult, EventHandlerRes...
#include "kaleidoscope/plugin/LEDControl.h"           // for LEDControl
#include "kaleidoscope/plugin/LEDControl/LEDUtils.h"  // for hsvToRgb

namespace kaleidoscope {
namespace plugin {

StalkerEffect::ColorComputer *StalkerEffect::variant;
uint16_t StalkerEffect::step_length = 50;
cRGB StalkerEffect::inactive_color  = (cRGB){
  0, 0, 0};

StalkerEffect::TransientLEDMode::TransientLEDMode(const StalkerEffect *parent)
  : parent_(parent),
    step_start_time_(0),
    map_{} {}

EventHandlerResult StalkerEffect::onKeyEvent(KeyEvent &event) {
  if (!Runtime.has_leds)
    return EventHandlerResult::OK;

  if (!event.addr.isValid())
    return EventHandlerResult::OK;

  if (::LEDControl.get_mode_index() != led_mode_id_)
    return EventHandlerResult::OK;

  // The simplest thing to do is trigger on both press and release. The color
  // will fade while the key is held, and get restored to full brightness when
  // it's released.
  ::LEDControl.get_mode<TransientLEDMode>()->map_[event.addr.toInt()] = 0xff;

  return EventHandlerResult::OK;
}

void StalkerEffect::TransientLEDMode::update() {
  if (!Runtime.has_leds)
    return;

  if (!parent_->variant)
    return;

  if (!Runtime.hasTimeExpired(step_start_time_, parent_->step_length))
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

  step_start_time_ = Runtime.millisAtCycleStart();
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
BlazingTrail::BlazingTrail() {
}
constexpr uint8_t hue_start = 50.0 / 360 * 0xff;
constexpr uint8_t hue_end   = 0;

cRGB BlazingTrail::compute(uint8_t *step) {
  cRGB color;

  // Get the position in the animation, where 0 is start and 0xff is end
  uint8_t pos255 = 0xff - *step;

  // Fade hue linearly from orange to red
  uint8_t hue = hue_start + (pos255 * (hue_end - hue_start) >> 8);

  // Fade value from full following a 1-x^4 curve
  uint8_t val =
    0xff                                            // Maximum brightness
    - ((uint32_t)pos255 * pos255 * pos255 * pos255  // Animation position to 4th power
       >> 24)                                       // ...pulled down to 8-bit range (but this has a maximum of 0xfc rather than 0xff)
    - pos255 / (0x100 / 4);                         // Correction to bring the end result into a full 0 to 0xff range

  color = hsvToRgb(hue, 0xff, val);

  if (*step < 4) {
    *step = 0;
  } else {
    *step -= 4;
  }

  return color;
}

// Rainbow
Rainbow::Rainbow() {
}

cRGB Rainbow::compute(uint8_t *step) {
  if (*step > 0)
    *step -= 1;
  else
    *step = 0;

  return hsvToRgb(255 - *step, 255, *step);
}

}  // namespace stalker
}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::StalkerEffect StalkerEffect;
