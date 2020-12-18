/* Kaleidoscope-Steno -- Steno protocols for Kaleidoscope
 * Copyright (C) 2017  Joseph Wasson
 * Copyright (C) 2017, 2018  Keyboard.io, Inc.
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
#include <Kaleidoscope-Ranges.h>

#define S(n) Key(kaleidoscope::plugin::steno::geminipr::n)

namespace kaleidoscope {
namespace plugin {
namespace steno {
class GeminiPR : public kaleidoscope::Plugin {
 public:
  GeminiPR(void) {}

  EventHandlerResult onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t keyState);
 private:
  static uint8_t keys_held_;
  static uint8_t state_[6];
};

namespace geminipr {
enum {
  START = kaleidoscope::ranges::STENO_FIRST,
  FN    = START,
  NUM,
  N1    = NUM,
  N2,
  N3,
  N4,
  N5,
  N6,
  SL,
  S1    = SL,
  S2,
  TL,
  KL,
  PL,
  WL,
  HL,
  RL,
  A,
  O,
  STR,
  ST1   = STR,
  ST2,
  RES1,
  RE1   = RES1,
  RES2,
  RE2   = RES2,
  PWR,
  ST3,
  ST4,
  E,
  U,
  FR,
  RR,
  PR,
  BR,
  LR,
  GR,
  TR,
  SR,
  DR,
  N7,
  N8,
  N9,
  NA,
  NB,
  NC,
  ZR,
  END   = ZR,
};
}
}
}
}

extern kaleidoscope::plugin::steno::GeminiPR GeminiPR;
