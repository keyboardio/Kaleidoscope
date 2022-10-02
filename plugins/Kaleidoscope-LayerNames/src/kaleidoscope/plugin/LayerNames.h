/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2022  Keyboard.io, Inc.
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

#include <stdint.h>  // for uint16_t, uint8_t

#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/plugin.h"                // for Plugin

namespace kaleidoscope {
namespace plugin {

class LayerNames : public kaleidoscope::Plugin {
 public:
  EventHandlerResult onNameQuery();
  EventHandlerResult onFocusEvent(const char *input);

  void reserve_storage(uint16_t size);

 private:
  uint16_t storage_base_;
  uint16_t storage_size_;
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::LayerNames LayerNames;
