/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2018  Keyboard.io, Inc.
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

#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
#error The V1 plugin API has been removed, please see UPGRADING.md.
#endif

namespace kaleidoscope {

class Plugin {

 public:
  // Please see "event_handlers.h" for a list of supported event handlers and
  // their documentation!
};

}  // namespace kaleidoscope
