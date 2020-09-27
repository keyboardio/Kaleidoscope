/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2019  Keyboard.io, Inc.
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

#include "kaleidoscope/remote_call.h"

#include "kaleidoscope/plugin.h"

namespace kaleidoscope {
namespace plugin {
class RemoteCall_ : public kaleidoscope::Plugin {
 public:

// If the remote call feature is disabled, the plugin is a no-op
//
#ifndef KALEIDOSCOPE_REMOTE_CALL_DISABLED
  EventHandlerResult onFocusEvent(const char *command);
#endif
};
}
}

extern kaleidoscope::plugin::RemoteCall_ RemoteCall;
