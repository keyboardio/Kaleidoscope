/* -*- mode: c++ -*-
 * Kaleidoscope-Heatmap -- Heatmap LED effect for Kaleidoscope.
 * Copyright (C) 2016, 2017  Keyboard.io, Inc.
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

#include "kaleidoscope/plugin/Heatmap.h"

#include <Arduino.h>  // for pgm_read_byte, PROGMEM
#include <stdint.h>   // for uint16_t, uint8_t, INT16_MAX

#include "kaleidoscope/KeyAddr.h"               // for MatrixAddr, MatrixAddr<>::Range, KeyAddr
#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"         // for cRGB
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/keyswitch_state.h"       // for keyIsInjected, keyToggledOn
#include "kaleidoscope/plugin/LEDControl.h"     // for LEDControl

namespace kaleidoscope {
namespace plugin {

// in the cRGB struct the order is blue, green, red (It should be called cBGR…)
// default heat_colors                                black        green         yellow           red
static const cRGB heat_colors_default_[] PROGMEM = {{0, 0, 0}, {25, 255, 25}, {25, 255, 255}, {25, 25, 255}};

// colors from cold to hot
const cRGB *Heatmap::heat_colors    = heat_colors_default_;
uint8_t Heatmap::heat_colors_length = 4;
// number of millisecond to wait between each heatmap computation
uint16_t Heatmap::update_delay = 1000;

uint16_t Heatmap::highest_ = 1;
uint16_t Heatmap::heatmap_[];

Heatmap::TransientLEDMode::TransientLEDMode(const Heatmap *parent)
  :  // last heatmap computation time
    last_heatmap_comp_time_(Runtime.millisAtCycleStart()),
    parent_(parent) {}

cRGB Heatmap::TransientLEDMode::computeColor(float v) {
  // compute the color corresponding to a value between 0 and 1

  /*
   * for exemple, if:
   *   v=0.8
   *   heat_colors_lenth=4 (hcl)
   *   the red components of heat_colors are: 0, 25, 25, 255 (rhc)
   * the red component returned by computeColor will be: 117
   *
   * 255 |                 /
   *     |                /
   *     |               /
   * 117 | - - - - - - -/
   *     |             /
   *  25 |      ______/ |
   *     |   __/
   *     | _/           |
   *     |/_________________
   *     0     1     2  ↑  3
   *                 ↑ 2.4 ↑
   *               idx1 |  idx2
   *                 <—–>
   *                  fb
   *
   * in this exemple, I call red heat_colors: rhc
   * idx1 = floor(v×(hcl-1)) = floor(0.8×3) = floor(2.4) = 2
   * idx2 = idx1 + 1 = 3
   * fb = v×(hcl-1)-idx1 = 0.8×3 - 2 = 0.4
   * red = (rhc[idx2]-rhc[idx1])×fb + rhc[idx1] = (255-25)×(2.4-2) + 25 = 117
   */

  float fb = 0;
  uint8_t idx1, idx2;

  if (v <= 0) {
    // if v = 0, don't bother computing fb and use heat_colors[0]
    idx1 = idx2 = 0;
  } else if (v >= 1) {
    // if v = 1,
    // don't bother computing fb and use heat_colors[heat_colors_length-1]
    idx1 = idx2 = heat_colors_length - 1;
  } else {
    float val = v * (heat_colors_length - 1);
    // static_cast from float to int just drop the decimal part of the number
    // static_cast<int>(5.9) → 5
    idx1 = static_cast<int>(val);
    idx2 = idx1 + 1;
    fb   = val - static_cast<float>(idx1);
  }

  uint8_t r = static_cast<uint8_t>((pgm_read_byte(&(heat_colors[idx2].r)) - pgm_read_byte(&(heat_colors[idx1].r))) * fb + pgm_read_byte(&(heat_colors[idx1].r)));
  uint8_t g = static_cast<uint8_t>((pgm_read_byte(&(heat_colors[idx2].g)) - pgm_read_byte(&(heat_colors[idx1].g))) * fb + pgm_read_byte(&(heat_colors[idx1].g)));
  uint8_t b = static_cast<uint8_t>((pgm_read_byte(&(heat_colors[idx2].b)) - pgm_read_byte(&(heat_colors[idx1].b))) * fb + pgm_read_byte(&(heat_colors[idx1].b)));

  return {b, g, r};
}

void Heatmap::TransientLEDMode::shiftStats() {
  // this method is called when:
  // 1. a value in heatmap_ reach INT8_MAX
  // 2. highest_ reach heat_colors_length*512 (see Heatmap::loopHook)

  // we divide every heatmap element by 2
  for (auto key_addr : KeyAddr::all()) {
    parent_->heatmap_[key_addr.toInt()] = parent_->heatmap_[key_addr.toInt()] >> 1;
  }

  // and also divide highest_ accordingly
  parent_->highest_ = parent_->highest_ >> 1;
}

void Heatmap::resetMap() {

  if (::LEDControl.get_mode_index() != led_mode_id_)
    return;

  ::LEDControl.get_mode<TransientLEDMode>()->resetMap();
}

void Heatmap::TransientLEDMode::resetMap() {

  // this method can be used as a way to work around an existing bug with a single key
  // getting special attention or if the user just wants a button to reset the map
  for (auto key_addr : KeyAddr::all()) {
    parent_->heatmap_[key_addr.toInt()] = 0;
  }

  parent_->highest_ = 1;
}

// It may be better to use `onKeyswitchEvent()` here
EventHandlerResult Heatmap::onKeyEvent(KeyEvent &event) {
  // If the keyboard has no LEDs, return
  if (!Runtime.has_leds)
    return EventHandlerResult::OK;

  // If the event doesn't correspond to a physical key, skip it
  if (!event.addr.isValid())
    return EventHandlerResult::OK;

  // if it is a synthetic key, skip it
  if (keyIsInjected(event.state))
    return EventHandlerResult::OK;

  // if the key is not toggled on, skip it
  if (!keyToggledOn(event.state))
    return EventHandlerResult::OK;

  // if the LED mode is not current, skip it
  if (::LEDControl.get_mode_index() != led_mode_id_)
    return EventHandlerResult::OK;

  return ::LEDControl.get_mode<TransientLEDMode>()->onKeyEvent(event);
}

EventHandlerResult Heatmap::TransientLEDMode::onKeyEvent(KeyEvent &event) {
  // increment the heatmap_ value related to the key
  parent_->heatmap_[event.addr.toInt()]++;

  // check highest_
  if (parent_->highest_ < parent_->heatmap_[event.addr.toInt()]) {
    parent_->highest_ = parent_->heatmap_[event.addr.toInt()];

    // if highest_ (and so heatmap_ value related to the key)
    // is close to overflow: call shiftStats
    // NOTE: this is barely impossible since shiftStats should be
    //       called much sooner by Heatmap::loopHook
    if (parent_->highest_ == INT16_MAX)
      shiftStats();
  }

  return EventHandlerResult::OK;
}

EventHandlerResult Heatmap::beforeEachCycle() {
  if (!Runtime.has_leds)
    return EventHandlerResult::OK;

  if (::LEDControl.get_mode_index() != led_mode_id_)
    return EventHandlerResult::OK;

  return ::LEDControl.get_mode<TransientLEDMode>()->beforeEachCycle();
}

EventHandlerResult Heatmap::TransientLEDMode::beforeEachCycle() {
  // this methode is called frequently by Kaleidoscope
  // even if the module isn't activated

  // call shiftStats (divide every heatmap value by 2)
  // if highest_ reach heat_colors_length*512.
  // So after the shift, highest_ will be heat_colors_length*256. We
  // didn't lose any precision in our heatmap since between each color we have a
  // maximum precision of 256 ; said differently, there is 256 state (at max)
  // between heat_colors[x] and heat_colors[x+1].
  if (parent_->highest_ > (static_cast<uint16_t>(heat_colors_length) << 9))
    shiftStats();

  return EventHandlerResult::OK;
}

void Heatmap::TransientLEDMode::update() {
  if (!Runtime.has_leds)
    return;

  // this methode is called frequently by the LEDControl::loopHook

  // do nothing if the update interval hasn't elapsed since the previous update
  if (!Runtime.hasTimeExpired(last_heatmap_comp_time_, update_delay))
    return;
  // do the heatmap computing
  // (update_delay milliseconds elapsed since last_heatmap_comp_time)

  // schedule the next heatmap computing
  last_heatmap_comp_time_ = Runtime.millisAtCycleStart();

  // for each key
  for (auto key_addr : KeyAddr::all()) {
    // how much the key was pressed compared to the others (between 0 and 1)
    // (total_keys_ can't be equal to 0)
    float v = static_cast<float>(parent_->heatmap_[key_addr.toInt()]) / parent_->highest_;
    // we could have used an interger instead of a float, but then we would
    // have had to change some multiplication in division.
    // / on uint is slower than * on float, so I stay with the float
    // https://forum.arduino.cc/index.php?topic=92684.msg2733723#msg2733723

    // set the LED color accordingly
    ::LEDControl.setCrgbAt(KeyAddr(key_addr), computeColor(v));
  }
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::Heatmap HeatmapEffect;
