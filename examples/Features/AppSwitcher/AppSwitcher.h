/* -*- mode: c++ -*-
 * AppSwitcher -- A Kaleidoscope Example
 * Copyright (C) 2021  Keyboardio, Inc.
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

#include <Kaleidoscope.h>
#include "Kaleidoscope-Ranges.h"

constexpr Key AppSwitcher_Next{kaleidoscope::ranges::SAFE_START};
constexpr uint16_t _prev_val = AppSwitcher_Next.getRaw() + 1;
constexpr Key AppSwitcher_Prev{_prev_val};

namespace kaleidoscope {
namespace plugin {

class AppSwitcher : public kaleidoscope::Plugin {

 public:
  EventHandlerResult onKeyEvent(KeyEvent &event);

 private:
  KeyAddr active_addr_ = KeyAddr::none();
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::AppSwitcher AppSwitcher;
