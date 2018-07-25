/* -*- mode: c++ -*-
 * Kaleidoscope-MagicCombo -- Magic combo framework
 * Copyright (C) 2016, 2017, 2018  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
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

#define MAX_COMBO_LENGTH 5

#define USE_MAGIC_COMBOS(...)                                             \
  namespace kaleidoscope {                                                \
  namespace magiccombo {                                                  \
  const kaleidoscope::MagicCombo::Combo combos[] PROGMEM = {__VA_ARGS__}; \
                                                                          \
  const uint8_t combos_length = sizeof(combos) / sizeof(*combos);         \
  }                                                                       \
  }

#define _MAGICCOMBO_API_CHANGE                                            \
  "The MagicCombo API changed in an incompatible way, you will need to\n" \
  "upgrade.\n"                                                            \
  "\n"                                                                    \
  "Please see the `UPGRADING.md` document shipped with the source:\n"     \
  " https://github.com/keyboardio/Kaleidoscope-MagicCombo/blob/master/UPGRADING.md"

namespace kaleidoscope {

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

#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
  kaleidoscope::EventHandlerResult onSetup() {
    return kaleidoscope::EventHandlerResult::OK;
  }

 protected:
  void begin();
  static void legacyLoopHook(bool is_post_clear);
#endif

 private:
  static uint32_t end_time_;
};

namespace magiccombo {
extern const MagicCombo::Combo combos[];
extern const uint8_t combos_length;
}

}

extern kaleidoscope::MagicCombo MagicCombo;
