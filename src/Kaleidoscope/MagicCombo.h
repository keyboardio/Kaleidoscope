/* -*- mode: c++ -*-
 * Kaleidoscope-MagicCombo -- Magic combo framework
 * Copyright (C) 2016, 2017  Gergely Nagy
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

namespace kaleidoscope {

class MagicCombo : public KaleidoscopePlugin {
 public:
  typedef struct {
    uint32_t left_hand, right_hand;
  } combo_t;

  MagicCombo(void);

  void begin(void) final;

  static const combo_t *magic_combos;
  static uint16_t min_interval;

 private:
  static uint32_t end_time_;

  static void loopHook(bool is_post_clear);
};

}

void magicComboActions(uint8_t combo_index, uint32_t left_hand, uint32_t right_hand);

extern kaleidoscope::MagicCombo MagicCombo;
