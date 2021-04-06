/* -*- mode: c++ -*-
 * Kaleidoscope-Escape-OneShot -- Turn ESC into a key that cancels OneShots, if active.
 * Copyright (C) 2016-2020  Keyboard.io, Inc
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

constexpr Key OneShotCancelKey {kaleidoscope::ranges::OS_CANCEL};

namespace kaleidoscope {
namespace plugin {
class EscapeOneShot : public kaleidoscope::Plugin {
 public:
  EscapeOneShot(void) {}

  EventHandlerResult onKeyEvent(KeyEvent &event);

  void setCancelKey(Key cancel_key) {
    cancel_oneshot_key_ = cancel_key;
  }

 private:
  static Key cancel_oneshot_key_;
};
}
}

extern kaleidoscope::plugin::EscapeOneShot EscapeOneShot;
