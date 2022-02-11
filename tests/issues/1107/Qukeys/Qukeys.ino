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

#include <Kaleidoscope.h>
#include <Kaleidoscope-Qukeys.h>

#include "./common.h"

// *INDENT-OFF*
KEYMAPS(
    [0] = KEYMAP_STACKED
    (
        Key_A, Key_B, ___, ___, ___, ___, ___,
        SFT_T(X), CTL_T(Y), ___, ___, ___, ___, ___,
        Key_C, Key_D, ___, ___, ___, ___,
        ___, ___, ___, ___, ___, ___, ___,
        ___, ___, ___, ___,
        ___,

        ___, ___, ___, ___, ___, ___, ___,
        ___, ___, ___, ___, ___, ___, ___,
        ___, ___, ___, ___, ___, ___,
        ___, ___, ___, ___, ___, ___, ___,
        ___, ___, ___, ___,
        ___
    ),
)
// *INDENT-ON*

#include <kaleidoscope/KeyAddrEventQueue.h>

namespace kaleidoscope {
namespace plugin {

class QueueLeaker : public Plugin {
 public:
  EventHandlerResult afterEachCycle() {
    queue_.shift();
    return EventHandlerResult::OK;
  }
 private:
  KeyAddrEventQueue<8> queue_;
};

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::QueueLeaker QueueLeaker;

KALEIDOSCOPE_INIT_PLUGINS(Qukeys, QueueLeaker);

void setup() {
  Qukeys.setHoldTimeout(kaleidoscope::testing::QUKEYS_HOLD_TIMEOUT);
  Qukeys.setOverlapThreshold(kaleidoscope::testing::QUKEYS_OVERLAP_THRESHOLD);
  Qukeys.setMinimumHoldTime(kaleidoscope::testing::QUKEYS_MINIMUM_HOLD_TIME);
  Qukeys.setMinimumPriorInterval(kaleidoscope::testing::QUKEYS_MIN_PRIOR_INTERVAL);
  Qukeys.setMaxIntervalForTapRepeat(kaleidoscope::testing::QUKEYS_MAX_INTERVAL_FOR_TAP_REPEAT);

  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}
