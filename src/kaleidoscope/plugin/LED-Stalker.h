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

#pragma once

#include "kaleidoscope/Runtime.h"
#include <Kaleidoscope-LEDControl.h>

#define STALKER(v, ...) ({static kaleidoscope::plugin::stalker::v _effect __VA_ARGS__; &_effect;})

namespace kaleidoscope {
namespace plugin {
class StalkerEffect : public Plugin,
  public LEDModeInterface,
  public AccessTransientLEDMode {
 public:
  class ColorComputer {
   public:
    virtual cRGB compute(uint8_t *step) = 0;
  };

  StalkerEffect(void) {}

  static ColorComputer *variant;
  static uint16_t step_length;
  static cRGB inactive_color;

  EventHandlerResult onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t keyState);

  // This class' instance has dynamic lifetime
  //
  class TransientLEDMode : public LEDMode {
   public:

    // Please note that storing the parent ptr is only required
    // for those LED modes that require access to
    // members of their parent class. Most LED modes can do without.
    //
    explicit TransientLEDMode(const StalkerEffect *parent);

   protected:

    void update() final;

   private:

    const StalkerEffect *parent_;

    uint16_t step_start_time_;
    uint8_t map_[Runtime.device().numKeys()];

    friend class StalkerEffect;
  };
};

namespace stalker {

class Haunt : public StalkerEffect::ColorComputer {
 public:
  explicit Haunt(const cRGB highlight_color);
  Haunt(void) : Haunt(CRGB(0x40, 0x80, 0x80)) {}

  cRGB compute(uint8_t *step) final;
 private:
  static cRGB highlight_color_;
};

class BlazingTrail : public StalkerEffect::ColorComputer {
 public:
  BlazingTrail(void);

  cRGB compute(uint8_t *step) final;
};

class Rainbow : public StalkerEffect::ColorComputer {
 public:
  Rainbow(void);

  cRGB compute(uint8_t *step) final;
};

}
}
}

extern kaleidoscope::plugin::StalkerEffect StalkerEffect;
