/* -*- mode: c++ -*-
 * Kaleidoscope-UnderglowControl -- RGB Underglow Control for Kaleidoscope
 * Copyright (C) 2019  Keyboard.io, Inc
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of version 3 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Kaleidoscope.h>
#include <Kaleidoscope-UnderglowControl.h>

namespace kaleidoscope {
namespace plugin {

#if KALEIDOSCOPE_HARDWARE_HAS_UNDERGLOW
const int8_t UnderglowControl::led_count() {
  return KeyboardHardware.underglow.led_count();
}

const bool UnderglowControl::has_leds() {
  return true;
}

void UnderglowControl::sync() {
  if (paused_)
    return;

  KeyboardHardware.underglow.sync();
}

void UnderglowControl::setColorAt(uint8_t index, cRGB color) {
  KeyboardHardware.underglow.setColorAt(index, color.r, color.g, color.b);
}

void UnderglowControl::setColorAt(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
  KeyboardHardware.underglow.setColorAt(index, r, g, b);
}

cRGB UnderglowControl::getColorAt(uint8_t index) {
  cRGB ret;

  auto color = KeyboardHardware.underglow.getColorAt(index);
  ret.r = color.r;
  ret.g = color.g;
  ret.b = color.b;
  return ret;
}
EventHandlerResult UnderglowControl::beforeReportingState()  {
  if (paused_)
    return kaleidoscope::EventHandlerResult::OK;

  updateEffect();

  uint16_t elapsed = Kaleidoscope.millisAtCycleStart() - sync_start_time_;
  if (elapsed > sync_delay_) {
    sync();
    sync_start_time_ = Kaleidoscope.millisAtCycleStart() + sync_delay_;
  }

  return kaleidoscope::EventHandlerResult::OK;
}

void UnderglowControl::setColor(cRGB color) {
  for (int8_t i = 0; i < led_count(); i++) {
    setColorAt(i, color);
  }
}

void UnderglowControl::setColor(uint8_t r, uint8_t g, uint8_t b) {
  for (int8_t i = 0; i < led_count(); i++) {
    setColorAt(i, r, g, b);
  }
}
#endif

void UnderglowOff::onActivate() {
  if (!::UnderglowControl.has_leds())
    return;

  for (int8_t i = 0; i < ::UnderglowControl.led_count(); i++) {
    ::UnderglowControl.setColorAt(i, 0, 0, 0);
  }
}

}
}

kaleidoscope::plugin::UnderglowControl UnderglowControl;
kaleidoscope::plugin::UnderglowOff UnderglowOff;
