/* -*- mode: c++ -*-
 * Kaleidoscope-TopsyTurvy -- Turn the effect of Shift upside down for certain keys
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

#include <Kaleidoscope-Ranges.h>  // for TT_FIRST, TT_LAST

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/key_defs.h"              // for Key
#include "kaleidoscope/plugin.h"                // for Plugin

#define TOPSY(k) ::kaleidoscope::plugin::TopsyTurvyKey(Key_##k)

namespace kaleidoscope {
namespace plugin {

constexpr Key TopsyTurvyKey(Key key) {
  return Key(kaleidoscope::ranges::TT_FIRST + key.getKeyCode());
}

class TopsyTurvy : public kaleidoscope::Plugin {
 public:
  EventHandlerResult onKeyEvent(KeyEvent &event);
  EventHandlerResult onNameQuery();
  EventHandlerResult beforeReportingState(const KeyEvent &event);

  static bool isTopsyTurvyKey(Key key) {
    return (key >= ranges::TT_FIRST &&
            key <= ranges::TT_LAST);
  }

 private:
  KeyAddr tt_addr_ = KeyAddr::none();
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::TopsyTurvy TopsyTurvy;
