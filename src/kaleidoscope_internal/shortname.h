/* kaleidoscope_internal::shortname -- HID short name override helper
 * Copyright (C) 2019  Keyboard.io, Inc.
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

// clang-format off

#pragma once

/*
 * We want to be able to override the short name of the device. For that, we
 * currently need to override `HID_::getShortName`, but due to link ordering, we
 * need to do that in the user sketch. For this reason, `_INIT_HID_GETSHORTNAME`
 * gets called from the `KEYMAPS` macro.
 *
 * TODO(anyone): Once we have a better way to override the short name, remove
 * this workaround.
 */

#ifndef _INIT_HID_GETSHORTNAME

#define _INIT_HID_GETSHORTNAME                        __NL__ \
  uint8_t HID_::getShortName(char *name) {            __NL__ \
    return Kaleidoscope.device().getShortName(name);  __NL__ \
  }

#endif
