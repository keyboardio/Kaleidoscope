/* -*- mode: c++ -*-
 * Kaleidoscope-OneShot -- One-shot modifiers and layers
 * Copyright (C) 2021  Keyboard.io, Inc.
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

#include <Kaleidoscope-OneShot.h>
#include <Kaleidoscope-Ranges.h>

#include "kaleidoscope/event_handler_result.h"
#include "kaleidoscope/key_defs.h"
#include "kaleidoscope/KeyEvent.h"
#include "kaleidoscope/plugin.h"

// ----------------------------------------------------------------------------
// Key constants
constexpr Key OneShot_MetaStickyKey {kaleidoscope::ranges::OS_META_STICKY};
constexpr Key OneShot_ActiveStickyKey {kaleidoscope::ranges::OS_ACTIVE_STICKY};

namespace kaleidoscope {
namespace plugin {

class OneShotMetaKeys : public kaleidoscope::Plugin {
 public:
  // --------------------------------------------------------------------------
  // Plugin hook functions

  EventHandlerResult onNameQuery();
  EventHandlerResult onKeyEvent(KeyEvent &event);

 private:
  static bool isMetaStickyActive();

};

} // namespace plugin
} // namespace kaleidoscope

extern kaleidoscope::plugin::OneShotMetaKeys OneShotMetaKeys;
