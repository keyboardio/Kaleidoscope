/* -*- mode: c++ -*-
 * Kaleidoscope-MagicCombo -- Magic combo framework
 * Copyright (C) 2016-2021  Keyboard.io, Inc
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

#include <Arduino.h>  // for PROGMEM
#include <stdint.h>   // for uint16_t, uint8_t, int8_t

#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/plugin.h"                // for Plugin
// -----------------------------------------------------------------------------
// Deprecation warning messages
#include "kaleidoscope_internal/deprecations.h"  // for DEPRECATED

#define _DEPRECATED_MESSAGE_MAGICCOMBO_MIN_INTERVAL                        \
  "The `MagicCombo.min_interval` variable is deprecated. Please use the\n" \
  "`MagicCombo.setMinInterval()` function instead.\n"                      \
  "This variable will be removed after 2022-09-01."
// -----------------------------------------------------------------------------

#define MAX_COMBO_LENGTH 5

#define USE_MAGIC_COMBOS(...)                                      \
  namespace kaleidoscope {                                         \
  namespace plugin {                                               \
  namespace magiccombo {                                           \
  const kaleidoscope::plugin::MagicCombo::Combo combos[] PROGMEM = \
    {__VA_ARGS__};                                                 \
                                                                   \
  const uint8_t combos_length = sizeof(combos) / sizeof(*combos);  \
  }                                                                \
  }                                                                \
  }

namespace kaleidoscope {
namespace plugin {

class MagicCombo : public kaleidoscope::Plugin {
 public:
  typedef void (*ComboAction)(uint8_t combo_index);
  struct Combo {
    ComboAction action;
    int8_t keys[MAX_COMBO_LENGTH + 1];
  };

#ifndef NDEPRECATED
  DEPRECATED(MAGICCOMBO_MIN_INTERVAL)
  static uint16_t min_interval;
#endif

  void setMinInterval(uint16_t interval) {
#ifndef NDEPRECATED
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    min_interval = interval;
#pragma GCC diagnostic pop
#else
    min_interval_ = interval;
#endif
  }

  uint16_t getMinInterval() {
#ifndef NDEPRECATED
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    return min_interval;
#pragma GCC diagnostic pop
#else
    return min_interval_;
#endif
  }

  EventHandlerResult onNameQuery();
  EventHandlerResult afterEachCycle();

 private:
  uint16_t start_time_   = 0;
  uint16_t min_interval_ = 500;
};

namespace magiccombo {
extern const MagicCombo::Combo combos[];
extern const uint8_t combos_length;
}  // namespace magiccombo

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::MagicCombo MagicCombo;
