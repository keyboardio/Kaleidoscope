/* -*- mode: c++ -*-
 * Kaleidoscope-TapDance -- Tap-dance keys
 * Copyright (C) 2016-2021  Keyboard.io, Inc
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

#include <Arduino.h>              // for PROGMEM
#include <Kaleidoscope-Ranges.h>  // for TD_FIRST, TD_LAST
#include <stdint.h>               // for uint8_t, uint16_t

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/KeyAddrEventQueue.h"     // for KeyAddrEventQueue
#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/KeyEventTracker.h"       // for KeyEventTracker
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/key_defs.h"              // for Key
#include "kaleidoscope/plugin.h"                // for Plugin

#define TD(n)                                                kaleidoscope::plugin::TapDanceKey(n)

#define tapDanceActionKeys(tap_count, tap_dance_action, ...) ({                         \
  static const Key __k[] PROGMEM = {__VA_ARGS__};                                       \
  TapDance.actionKeys(tap_count, tap_dance_action, sizeof(__k) / sizeof(Key), &__k[0]); \
})

namespace kaleidoscope {
namespace plugin {

constexpr Key TapDanceKey(uint8_t n) {
  return Key(kaleidoscope::ranges::TD_FIRST + n);
}

class TapDance : public kaleidoscope::Plugin {
 public:
  enum ActionType {
    Tap,
    Hold,
    Interrupt,
    Timeout,
    Release,
  };

  TapDance(void) {}

  static uint16_t time_out;

  void actionKeys(uint8_t tap_count, ActionType tap_dance_action, uint8_t max_keys, const Key tap_keys[]);

  EventHandlerResult onNameQuery();
  EventHandlerResult onKeyswitchEvent(KeyEvent &event);
  EventHandlerResult afterEachCycle();

  static constexpr bool isTapDanceKey(Key key) {
    return (key.getRaw() >= ranges::TD_FIRST &&
            key.getRaw() <= ranges::TD_LAST);
  }

 private:
  // The maximum number of events in the queue at a time.
  static constexpr uint8_t queue_capacity_{8};

  // The event queue stores a series of press and release events.
  KeyAddrEventQueue<queue_capacity_> event_queue_;

  static KeyEventTracker event_tracker_;

  // The number of taps in the current TapDance sequence.
  static uint8_t tap_count_;

  void flushQueue(KeyAddr ignored_addr = KeyAddr::none());
};

}  // namespace plugin
}  // namespace kaleidoscope

void tapDanceAction(uint8_t tap_dance_index, KeyAddr key_addr, uint8_t tap_count, kaleidoscope::plugin::TapDance::ActionType tap_dance_action);

extern kaleidoscope::plugin::TapDance TapDance;
