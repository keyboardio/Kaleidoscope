/* -*- mode: c++ -*-
 * Kaleidoscope-TypingBreaks -- Enforced typing breaks
 * Copyright (C) 2017, 2018  Keyboard.io, Inc
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

#include <Kaleidoscope-TypingBreaks.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-FocusSerial.h>
#include "kaleidoscope/keyswitch_state.h"

namespace kaleidoscope {
namespace plugin {

TypingBreaks::settings_t TypingBreaks::settings = {
  .idle_time_limit = 300, //  5m
  .lock_time_out = 2700,  //  45m
  .lock_length = 300,    //   5m
  .left_hand_max_keys = 0,
  .right_hand_max_keys = 0
};

bool TypingBreaks::keyboard_locked_{false};
uint32_t TypingBreaks::session_start_time_;
uint32_t TypingBreaks::last_key_time_;
uint32_t TypingBreaks::lock_start_time_;
uint16_t TypingBreaks::left_hand_keys_;
uint16_t TypingBreaks::right_hand_keys_;
uint16_t TypingBreaks::settings_base_;

EventHandlerResult TypingBreaks::onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t key_state) {
  uint32_t lock_length = settings.lock_length * 1000;
  uint32_t idle_time_limit = settings.idle_time_limit * 1000;
  uint32_t lock_time_out = settings.lock_time_out * 1000;

  // If we are locked...
  if (keyboard_locked_) {
    // ...and the lock has not expired yet
    if (!Runtime.hasTimeExpired(lock_start_time_, lock_length)) {
      return EventHandlerResult::EVENT_CONSUMED;  // remain locked
    }

    // ...otherwise clear the lock
    keyboard_locked_ = false;
    left_hand_keys_ = right_hand_keys_ = 0;
    session_start_time_ = Runtime.millisAtCycleStart();

    TypingBreak(false);
  }

  // Any other case, we are not locked yet! (or we just unlocked)

  // Are we still in the same session?
  if (Runtime.hasTimeExpired(last_key_time_, idle_time_limit)) {
    // No, we are not. Clear timers and start over.
    left_hand_keys_ = right_hand_keys_ = 0;
    session_start_time_ = Runtime.millisAtCycleStart();
  }

  // If we have a limit on the either hand, and we reached it, lock up!
  if ((settings.left_hand_max_keys && left_hand_keys_ >= settings.left_hand_max_keys) ||
      (settings.right_hand_max_keys && right_hand_keys_ >= settings.right_hand_max_keys)) {
    keyboard_locked_ = true;
    lock_start_time_ = last_key_time_;
    TypingBreak(true);
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (lock_time_out) {
    // Is the session longer than lock_time_out?
    if (Runtime.hasTimeExpired(session_start_time_, lock_time_out)) {
      // Yeah, it is.
      keyboard_locked_ = true;
      lock_start_time_ = last_key_time_;
      TypingBreak(true);
      return EventHandlerResult::EVENT_CONSUMED;
    }
  }

  // So it seems we did not need to lock up. In this case, lets increase key
  // counters if need be.

  if (keyToggledOn(key_state)) {
    if (key_addr.col() <= Runtime.device().matrix_columns / 2)
      left_hand_keys_++;
    else
      right_hand_keys_++;
    last_key_time_ = Runtime.millisAtCycleStart();
  }

  return EventHandlerResult::OK;
}

EventHandlerResult TypingBreaks::onSetup() {
  settings_base_ = ::EEPROMSettings.requestSlice(sizeof(settings));

  // If idleTime is max, assume that EEPROM is uninitialized, and store the
  // defaults.
  uint32_t idle_time;
  Runtime.storage().get(settings_base_, idle_time);
  if (idle_time == 0xffffffff) {
    Runtime.storage().put(settings_base_, settings);
    Runtime.storage().commit();
  }

  Runtime.storage().get(settings_base_, settings);
  return EventHandlerResult::OK;
}

#define FOCUS_HOOK_TYPINGBREAKS FOCUS_HOOK(TypingBreaks.focusHook,      \
                                           "typingbreaks.idleTimeLimit\n" \
                                           "typingbreaks.lockTimeOut\n" \
                                           "typingbreaks.lockLength\n"  \
                                           "typingbreaks.leftMaxKeys\n" \
                                           "typingbreaks.rightMaxKeys")

EventHandlerResult TypingBreaks::onFocusEvent(const char *command) {
  enum {
    IDLE_TIME_LIMIT,
    LOCK_TIMEOUT,
    LOCK_LENGTH,
    LEFT_MAX,
    RIGHT_MAX,
  } subCommand;

  if (::Focus.handleHelp(command, PSTR("typingbreaks.idleTimeLimit\n"
                                       "typingbreaks.lockTimeOut\n"
                                       "typingbreaks.lockLength\n"
                                       "typingbreaks.leftMaxKeys\n"
                                       "typingbreaks.rightMaxKeys")))
    return EventHandlerResult::OK;

  if (strncmp_P(command, PSTR("typingbreaks."), 13) != 0)
    return EventHandlerResult::OK;
  if (strcmp_P(command + 13, PSTR("idleTimeLimit")) == 0)
    subCommand = IDLE_TIME_LIMIT;
  else if (strcmp_P(command + 13, PSTR("lockTimeOut")) == 0)
    subCommand = LOCK_TIMEOUT;
  else if (strcmp_P(command + 13, PSTR("lockLength")) == 0)
    subCommand = LOCK_LENGTH;
  else if (strcmp_P(command + 13, PSTR("leftMaxKeys")) == 0)
    subCommand = LEFT_MAX;
  else if (strcmp_P(command + 13, PSTR("rightMaxKeys")) == 0)
    subCommand = RIGHT_MAX;
  else
    return EventHandlerResult::OK;

  switch (subCommand) {
  case IDLE_TIME_LIMIT:
    if (::Focus.isEOL()) {
      ::Focus.send(settings.idle_time_limit);
    } else {
      ::Focus.read(settings.idle_time_limit);
    }
    break;
  case LOCK_TIMEOUT:
    if (::Focus.isEOL()) {
      ::Focus.send(settings.lock_time_out);
    } else {
      ::Focus.read(settings.lock_time_out);
    }
    break;
  case LOCK_LENGTH:
    if (::Focus.isEOL()) {
      ::Focus.send(settings.lock_length);
    } else {
      ::Focus.read(settings.lock_length);
    }
    break;
  case LEFT_MAX:
    if (::Focus.isEOL()) {
      ::Focus.send(settings.left_hand_max_keys);
    } else {
      ::Focus.read(settings.left_hand_max_keys);
    }
    break;
  case RIGHT_MAX:
    if (::Focus.isEOL()) {
      ::Focus.send(settings.right_hand_max_keys);
    } else {
      ::Focus.read(settings.right_hand_max_keys);
    }
    break;
  }

  Runtime.storage().put(settings_base_, settings);
  Runtime.storage().commit();
  return EventHandlerResult::EVENT_CONSUMED;
}

}
}

kaleidoscope::plugin::TypingBreaks TypingBreaks;

__attribute__((weak)) void TypingBreak(bool isLocked) {
}
