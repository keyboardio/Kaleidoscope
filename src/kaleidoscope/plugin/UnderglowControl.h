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

#pragma once

#include <Kaleidoscope.h>

namespace kaleidoscope {
namespace plugin {

class UnderglowControl;

class UnderglowEffect : public kaleidoscope::Plugin {
  friend class UnderglowControl;
 public:
  UnderglowEffect() {}

 protected:
  virtual void onActivate() {};
  virtual void update() {}
  virtual void refreshAt(uint8_t index) {}
};

class UnderglowControl : public kaleidoscope::Plugin {
 public:
  UnderglowControl() {}

#if KALEIDOSCOPE_HARDWARE_HAS_UNDERGLOW
  const int8_t led_count();
  const bool has_leds();
  void sync();

  void setColorAt(uint8_t index, cRGB color);
  void setColorAt(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
  void setColor(cRGB color);
  void setColor(uint8_t r, uint8_t g, uint8_t b);
  cRGB getColorAt(uint8_t index);

  EventHandlerResult beforeReportingState();
#else
  const int8_t led_count() {
    return -1;
  }
  const bool has_leds() {
    return false;
  }
  void sync() {}
  void setColorAt(uint8_t index, cRGB color) {}
  void setColorAt(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {}
  cRGB getColorAt(uint8_t index) {
    return CRGB(0, 0, 0);
  }
  void setColor(cRGB color) {}
  void setColor(uint8_t r, uint8_t g, uint8_t b) {}

  EventHandlerResult beforeReportingState() {
    return EventHandlerResult::OK;
  }
#endif

  void activateEffect(UnderglowEffect &effect) {
    effect_ = &effect;
    effect_->onActivate();
  }
  void updateEffect() {
    if (!effect_)
      return;
    effect_->update();
  }
  void refreshEffectAt(uint8_t index) {
    if (!effect_)
      return;
    effect_->refreshAt(index);
  }
  void deactivateEffect() {
    effect_ = NULL;
  }
  const UnderglowEffect *currentEffect() {
    return effect_;
  }

  bool isPaused() {
    return paused_;
  }
  void pause() {
    paused_ = true;
  }
  void resume() {
    paused_ = false;
  }

  uint16_t sync_delay() {
    return sync_delay_;
  }
  void sync_delay(uint16_t delay) {
    sync_delay_ = delay;
  }

 private:
  bool paused_ = false;
  uint16_t sync_delay_ = 40;
  uint16_t sync_start_time_ = 0;
  UnderglowEffect *effect_;
};

class UnderglowOff : public UnderglowEffect {
 public:
  UnderglowOff() {}

 protected:
  void onActivate() final;
};

}
}

extern kaleidoscope::plugin::UnderglowControl UnderglowControl;
extern kaleidoscope::plugin::UnderglowOff UnderglowOff;
