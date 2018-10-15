/* -*- mode: c++ -*-
 * Kaleidoscope-MagicCombo -- Magic combo framework
 * Copyright (C) 2016, 2017, 2018  Keyboard.io, Inc
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

#include <Kaleidoscope.h>

#define MAX_COMBO_LENGTH 5

#define USE_MAGIC_COMBOS(...)                                             \
  namespace kaleidoscope {                                                \
  namespace plugin {                                                      \
  namespace magiccombo {                                                  \
  const kaleidoscope::plugin::MagicCombo::Combo combos[] PROGMEM =        \
    {__VA_ARGS__};                                                        \
                                                                          \
  const uint8_t combos_length = sizeof(combos) / sizeof(*combos);         \
  }                                                                       \
  }                                                                       \
  }

#define _MAGICCOMBO_API_CHANGE                                            \
  "The MagicCombo API changed in an incompatible way, you will need to\n" \
  "upgrade.\n"                                                            \
  "\n"                                                                    \
  "Please see the `UPGRADING.md` document shipped with the source:\n"     \
  " https://github.com/keyboardio/Kaleidoscope-MagicCombo/blob/master/UPGRADING.md"

namespace kaleidoscope {
namespace plugin {

class MagicCombo : public kaleidoscope::Plugin {
 public:
  typedef void (*ComboAction)(uint8_t combo_index);
  typedef struct {
    ComboAction action;
    int8_t keys[MAX_COMBO_LENGTH + 1];
  } Combo;
  typedef struct combo_t {
    uint32_t left_hand, right_hand;

    template <typename T>
    combo_t(T l, T r) {
      static_assert(sizeof(T) < 0, _DEPRECATE(_MAGICCOMBO_API_CHANGE));
    }
  } combo_t;

  MagicCombo(void) {}

  static const combo_t *magic_combos;
  static uint16_t min_interval;

  EventHandlerResult beforeReportingState();

 private:
  static uint32_t end_time_;
};

namespace magiccombo {
extern const MagicCombo::Combo combos[];
extern const uint8_t combos_length;
}

}

// Backward compatibility
typedef plugin::MagicCombo MagicCombo;
}

extern kaleidoscope::plugin::MagicCombo MagicCombo;
