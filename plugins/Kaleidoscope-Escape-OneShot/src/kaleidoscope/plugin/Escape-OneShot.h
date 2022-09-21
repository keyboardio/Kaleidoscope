/* -*- mode: c++ -*-
 * Kaleidoscope-Escape-OneShot -- Turn ESC into a key that cancels OneShots, if active.
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

#include <Kaleidoscope-Ranges.h>  // for OS_CANCEL
#include <stdint.h>               // for uint16_t

#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/key_defs.h"              // for Key, Key_Escape
#include "kaleidoscope/plugin.h"                // for Plugin

// DEPRECATED: `OneShotCancelKey` doesn't match our normal naming, and should
// eventually be removed.
constexpr Key OneShotCancelKey{kaleidoscope::ranges::OS_CANCEL};
constexpr Key Key_OneShotCancel{kaleidoscope::ranges::OS_CANCEL};

namespace kaleidoscope {
namespace plugin {

class EscapeOneShot : public kaleidoscope::Plugin {
 public:
  EventHandlerResult onKeyEvent(KeyEvent &event);

  void setCancelKey(Key cancel_key) {
    settings_.cancel_oneshot_key = cancel_key;
  }
  Key getCancelKey() {
    return settings_.cancel_oneshot_key;
  }

  friend class EscapeOneShotConfig;

 private:
  struct Settings {
    Key cancel_oneshot_key;
  };
  Settings settings_ = {.cancel_oneshot_key = Key_Escape};
};

class EscapeOneShotConfig : public Plugin {
 public:
  EventHandlerResult onSetup();
  EventHandlerResult onFocusEvent(const char *input);
  EventHandlerResult onNameQuery();

 private:
  uint16_t settings_base_;
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::EscapeOneShot EscapeOneShot;
extern kaleidoscope::plugin::EscapeOneShotConfig EscapeOneShotConfig;
