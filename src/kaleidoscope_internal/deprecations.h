/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2020  Keyboard.io, Inc.
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

#define DEPRECATED(tag)                                                 \
  __attribute__((deprecated(_DEPRECATE(_DEPRECATED_MESSAGE_ ## tag))))

#define _DEPRECATE(message) "\n"                                               \
  "------------------------------------------------------------------------\n" \
  message                                                                      \
  "\n"                                                                         \
  "------------------------------------------------------------------------\n" \

/* Messages */

#define _DEPRECATED_MESSAGE_LAYER_UPDATELIVECOMPOSITEKEYMAP             __NL__ \
  "`Layer.updateLiveCompositeKeymap()` is deprecated.\n"                __NL__ \
  "The 'live composite keymap' cache has been replaced with the\n"      __NL__ \
  "'active keys' cache, which now represents the state of the active\n" __NL__ \
  "keys at any given time. It is probably not necessary to directly\n"  __NL__ \
  "update that cache from a plugin, but if you need to, please use\n"   __NL__ \
  "the `live_keys.activate(key_addr, key)` function instead."

#define _DEPRECATED_MESSAGE_LAYER_EVENTHANDLER               __NL__ \
  "`Layer.eventHandler()` is deprecated.\n"                  __NL__ \
  "Please use `Layer.handleKeymapKeyswitchEvent()` instead."
