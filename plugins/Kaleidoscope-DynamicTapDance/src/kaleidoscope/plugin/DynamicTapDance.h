/* DynamicTapDance -- Dynamic TapDance support for Kaleidoscope
 * Copyright (C) 2019  Keyboard.io, Inc
 * Copyright (C) 2019  Dygma Lab S.L.
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
#include <Kaleidoscope-TapDance.h>

namespace kaleidoscope {
namespace plugin {

class DynamicTapDance: public kaleidoscope::Plugin {
 public:
  DynamicTapDance() {}

  EventHandlerResult onFocusEvent(const char *command);

  static void setup(uint8_t dynamic_offset, uint16_t size);

  static bool dance(uint8_t tap_dance_index, KeyAddr key_addr, uint8_t tap_count,
                    TapDance::ActionType tap_dance_action);

 private:
  static uint16_t storage_base_;
  static uint16_t storage_size_;
  static constexpr uint8_t reserved_tap_dance_key_count_ = ranges::TD_LAST - ranges::TD_FIRST + 1;
  static uint16_t map_[reserved_tap_dance_key_count_];
  static uint8_t dance_count_;
  static uint8_t offset_;
  static void updateDynamicTapDanceCache();
};

}
}

extern kaleidoscope::plugin::DynamicTapDance DynamicTapDance;
