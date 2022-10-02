/* DynamicTapDance -- Dynamic TapDance support for Kaleidoscope
 * Copyright (C) 2019, 2021  Keyboard.io, Inc
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

#include <Kaleidoscope-Ranges.h>    // for TD_FIRST, TD_LAST
#include <Kaleidoscope-TapDance.h>  // for TapDance, TapDance::ActionType
#include <stdint.h>                 // for uint8_t, uint16_t

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/plugin.h"                // for Plugin

namespace kaleidoscope {
namespace plugin {

class DynamicTapDance : public kaleidoscope::Plugin {
 public:
  EventHandlerResult onNameQuery();
  EventHandlerResult onFocusEvent(const char *input);

  void setup(uint8_t dynamic_offset, uint16_t size);

  bool dance(uint8_t tap_dance_index, KeyAddr key_addr, uint8_t tap_count, TapDance::ActionType tap_dance_action);

 private:
  uint16_t storage_base_;
  uint16_t storage_size_;
  static constexpr uint8_t reserved_tap_dance_key_count_ = ranges::TD_LAST - ranges::TD_FIRST + 1;
  uint16_t map_[reserved_tap_dance_key_count_];  // NOLINT(runtime/arrays)
  uint8_t dance_count_;
  uint8_t offset_;
  void updateDynamicTapDanceCache();
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::DynamicTapDance DynamicTapDance;
