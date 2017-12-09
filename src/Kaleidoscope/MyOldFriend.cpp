/* -*- mode: c++ -*-
 * Kaleidoscope-MyOldFriend -- Host sleep support plugin.
 * Copyright (C) 2017  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Kaleidoscope.h>
#include <Kaleidoscope-MyOldFriend.h>
#include <Kaleidoscope-LEDControl.h>
#include "LED-Off.h"

// This is a terrible hack until Arduino#6964 gets implemented.
// It makes the `_usbSuspendState` symbol available to us.
extern u8 _usbSuspendState;

namespace kaleidoscope {

bool MyOldFriend::was_suspended_ = false;
bool MyOldFriend::initial_suspend_ = true;

void MyOldFriend::begin(void) {
  Kaleidoscope.useLoopHook(loopHook);
  Kaleidoscope.use(&::LEDOff);
}

void MyOldFriend::toggleLEDs(MyOldFriend::Event event) {
  static uint8_t prev_led_mode = 0;

  switch (event) {
    case Suspend:
      prev_led_mode = ::LEDControl.get_mode_index();
      ::LEDOff.activate();
      break;
    case Resume:
      ::LEDControl.set_mode(prev_led_mode);
      ::LEDControl.refreshAll();
      break;
    case Sleep:
      break;
  }
}

void MyOldFriend::loopHook(bool post_clear) {
  if (post_clear)
    return;

  if ((_usbSuspendState & (1 << SUSPI))) {
    if (!initial_suspend_) {
      if (!was_suspended_) {
        was_suspended_ = true;
        myOldFriendEventHandler(Suspend);
      } else {
        myOldFriendEventHandler(Sleep);
      }
    }
  } else {
    if (initial_suspend_)
      initial_suspend_ = false;
    if (was_suspended_) {
      was_suspended_ = false;
      myOldFriendEventHandler(Resume);
    }
  }
}

}

__attribute__((weak)) void myOldFriendEventHandler(kaleidoscope::MyOldFriend::Event event) {
  MyOldFriend.toggleLEDs(event);
}

kaleidoscope::MyOldFriend MyOldFriend;
