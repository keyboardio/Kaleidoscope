/* -*- mode: c++ -*-
 * Kaleidoscope-TypingBreaks -- Enforced typing breaks
 * Copyright (C) 2017-2021  Keyboard.io, Inc
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

#include "kaleidoscope/plugin/TypingBreaks.h"

#include <Arduino.h>                       // for PSTR, F, __FlashStringHelper
#include <Kaleidoscope-EEPROM-Settings.h>  // for EEPROMSettings
#include <Kaleidoscope-FocusSerial.h>      // for Focus, FocusSerial
#include <stdint.h>                        // for uint32_t, uint16_t

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"         // for VirtualProps::Storage, Base<>::Storage
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/keyswitch_state.h"       // for keyToggledOff

namespace kaleidoscope {
namespace plugin {

TypingBreaks::settings_t TypingBreaks::settings = {
  .idle_time_limit     = 300,   //  5m
  .lock_time_out       = 2700,  //  45m
  .lock_length         = 300,   //   5m
  .left_hand_max_keys  = 0,
  .right_hand_max_keys = 0};

bool TypingBreaks::keyboard_locked_{false};
uint32_t TypingBreaks::session_start_time_;
uint32_t TypingBreaks::last_key_time_;
uint32_t TypingBreaks::lock_start_time_;
uint16_t TypingBreaks::left_hand_keys_;
uint16_t TypingBreaks::right_hand_keys_;
uint16_t TypingBreaks::settings_base_;

EventHandlerResult TypingBreaks::onKeyEvent(KeyEvent &event) {
  uint32_t lock_length     = settings.lock_length * 1000;
  uint32_t idle_time_limit = settings.idle_time_limit * 1000;
  uint32_t lock_time_out   = settings.lock_time_out * 1000;

  // Let key release events through regardless, so the last key pressed (and any
  // other held keys) finish getting processed when they're released.
  if (keyToggledOff(event.state))
    return EventHandlerResult::OK;

  // If we are locked...
  if (keyboard_locked_) {
    // ...and the lock has not expired yet
    if (!Runtime.hasTimeExpired(lock_start_time_, lock_length)) {
      return EventHandlerResult::ABORT;
    }

    // ...otherwise clear the lock
    keyboard_locked_ = false;
    left_hand_keys_ = right_hand_keys_ = 0;
    session_start_time_                = Runtime.millisAtCycleStart();

    TypingBreak(false);
  }

  // Any other case, we are not locked yet! (or we just unlocked)

  // Are we still in the same session?
  if (Runtime.hasTimeExpired(last_key_time_, idle_time_limit)) {
    // No, we are not. Clear timers and start over.
    left_hand_keys_ = right_hand_keys_ = 0;
    session_start_time_                = Runtime.millisAtCycleStart();
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
  if (event.addr.col() < Runtime.device().matrix_columns / 2)
    left_hand_keys_++;
  else
    right_hand_keys_++;

  last_key_time_ = Runtime.millisAtCycleStart();

  return EventHandlerResult::OK;
}

EventHandlerResult TypingBreaks::onNameQuery() {
  return ::Focus.sendName(F("TypingBreaks"));
}

EventHandlerResult TypingBreaks::onSetup() {
  bool success = ::EEPROMSettings.requestSliceAndLoadData(&settings_base_, &settings);

  if (!success) {
    // If our slice is uninitialized, set sensible defaults.
    Runtime.storage().put(settings_base_, settings);
    Runtime.storage().commit();
  }

  return EventHandlerResult::OK;
}


EventHandlerResult TypingBreaks::onFocusEvent(const char *input) {
  enum {
    IDLE_TIME_LIMIT,
    LOCK_TIMEOUT,
    LOCK_LENGTH,
    LEFT_MAX,
    RIGHT_MAX,
    LEFT_COUNT,
    RIGHT_COUNT,
    LOCK_SECS_REMAINING,
  } subCommand;

  const char *cmd_idleTimeLimit = PSTR("typingbreaks.idleTimeLimit");
  const char *cmd_lockTimeOut   = PSTR("typingbreaks.lockTimeOut");
  const char *cmd_lockLength    = PSTR("typingbreaks.lockLength");
  const char *cmd_leftMaxKeys   = PSTR("typingbreaks.leftMaxKeys");
  const char *cmd_rightMaxKeys  = PSTR("typingbreaks.rightMaxKeys");
  const char *cmd_leftKeys      = PSTR("typingbreaks.leftKeys");
  const char *cmd_rightKeys     = PSTR("typingbreaks.rightKeys");
  const char *cmd_lockSecsRem   = PSTR("typingbreaks.lockSecsRemaining");
  if (::Focus.inputMatchesHelp(input))
    return ::Focus.printHelp(cmd_idleTimeLimit,
                             cmd_lockTimeOut,
                             cmd_lockLength,
                             cmd_leftMaxKeys,
                             cmd_rightMaxKeys,
                             cmd_leftKeys,
                             cmd_rightKeys,
                             cmd_lockSecsRem);

  if (::Focus.inputMatchesCommand(input, cmd_idleTimeLimit))
    subCommand = IDLE_TIME_LIMIT;
  else if (::Focus.inputMatchesCommand(input, cmd_lockTimeOut))
    subCommand = LOCK_TIMEOUT;
  else if (::Focus.inputMatchesCommand(input, cmd_lockLength))
    subCommand = LOCK_LENGTH;
  else if (::Focus.inputMatchesCommand(input, cmd_leftMaxKeys))
    subCommand = LEFT_MAX;
  else if (::Focus.inputMatchesCommand(input, cmd_rightMaxKeys))
    subCommand = RIGHT_MAX;
  else if (::Focus.inputMatchesCommand(input, cmd_leftKeys))
    subCommand = LEFT_COUNT;
  else if (::Focus.inputMatchesCommand(input, cmd_rightKeys))
    subCommand = RIGHT_COUNT;
  else if (::Focus.inputMatchesCommand(input, cmd_lockSecsRem))
    subCommand = LOCK_SECS_REMAINING;
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
  case LEFT_COUNT:
    ::Focus.send(left_hand_keys_);
    return EventHandlerResult::EVENT_CONSUMED;
  case RIGHT_COUNT:
    ::Focus.send(right_hand_keys_);
    return EventHandlerResult::EVENT_CONSUMED;
  case LOCK_SECS_REMAINING:
    if (keyboard_locked_) {
      uint16_t elapsed   = Runtime.millisAtCycleStart() / 1000 - lock_start_time_ / 1000;
      uint16_t remaining = settings.lock_length - elapsed;
      ::Focus.send(remaining);
    } else {
      ::Focus.send(0);
    }
    return EventHandlerResult::EVENT_CONSUMED;
  }

  Runtime.storage().put(settings_base_, settings);
  Runtime.storage().commit();
  return EventHandlerResult::EVENT_CONSUMED;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::TypingBreaks TypingBreaks;

__attribute__((weak)) void TypingBreak(bool isLocked) {
}
