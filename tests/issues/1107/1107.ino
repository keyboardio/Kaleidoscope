/* -*- mode: c++ -*-
 * Copyright (C) 2022  Keyboard.io, Inc.
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

#include "Kaleidoscope.h"

// *INDENT-OFF*
KEYMAPS(
  [0] = KEYMAP_STACKED
  (
    XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX

   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
          ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX
  )
) // KEYMAPS(
// *INDENT-ON*

#include <kaleidoscope/KeyAddrEventQueue.h>

namespace kaleidoscope {
namespace plugin {

class QueueLeaker : public Plugin {
 public:
  EventHandlerResult afterEachCycle() {
    // Shift queue without checking to be sure that queue is not empty.  If
    // KeyAddrEventQueue uses an unsigned integer for queue length, this will
    // result in some bad behavior.
    queue_.shift();
    return EventHandlerResult::OK;
  }
 private:
  KeyAddrEventQueue<8> queue_;
};

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::QueueLeaker QueueLeaker;

KALEIDOSCOPE_INIT_PLUGINS(QueueLeaker);

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}
