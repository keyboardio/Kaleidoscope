/* Kaleidoscope-OneShotMetaKeys -- OneShot_ActiveStickyKey & OneShot_MetaStickyKey
 * Copyright 2021-2025 Keyboard.io, inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * Additional Permissions:
 * As an additional permission under Section 7 of the GNU General Public
 * License Version 3, you may link this software against a Vendor-provided
 * Hardware Specific Software Module under the terms of the MCU Vendor
 * Firmware Library Additional Permission Version 1.0.
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

#include <Kaleidoscope-Ranges.h>  // for OS_ACTIVE_STICKY, OS_META_STICKY

#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/key_defs.h"              // for Key
#include "kaleidoscope/plugin.h"                // for Plugin

// ----------------------------------------------------------------------------
// Key constants
constexpr Key OneShot_MetaStickyKey{kaleidoscope::ranges::OS_META_STICKY};
constexpr Key OneShot_ActiveStickyKey{kaleidoscope::ranges::OS_ACTIVE_STICKY};

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

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::OneShotMetaKeys OneShotMetaKeys;
