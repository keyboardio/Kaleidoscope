/* -*- mode: c++ -*-
 * Kaleidoscope-TypingBreaks -- Enforced typing breaks
 * Copyright (C) 2017, 2018  Gergely Nagy
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

#include <Kaleidoscope-TypingBreaks.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-Focus.h>

namespace kaleidoscope {

TypingBreaks::settings_t TypingBreaks::settings = {
  .idle_time_limit = 300, //  5m
  .lock_time_out = 2700,  //  45m
  .lock_length = 300,    //   5m
  .left_hand_max_keys = 0,
  .right_hand_max_keys = 0
};

uint32_t TypingBreaks::session_start_time_;
uint32_t TypingBreaks::last_key_time_;
uint32_t TypingBreaks::lock_start_time_;
uint16_t TypingBreaks::left_hand_keys_;
uint16_t TypingBreaks::right_hand_keys_;
uint16_t TypingBreaks::settings_base_;

EventHandlerResult TypingBreaks::onKeyswitchEvent(Key &mapped_key, byte row, byte col, uint8_t key_state) {
  uint32_t lock_length = settings.lock_length * 1000;
  uint32_t idle_time_limit = settings.idle_time_limit * 1000;
  uint32_t lock_time_out = settings.lock_time_out * 1000;

  // If we are locked, and didn't time out yet, no key has to be pressed.
  if (lock_start_time_ && (millis() - lock_start_time_ <= lock_length)) {
    return EventHandlerResult::EVENT_CONSUMED;
  }

  // If we are locked...
  if (lock_start_time_) {
    // ...and the lock has not expired yet
    if (millis() - lock_start_time_ <= lock_length) {
      return EventHandlerResult::EVENT_CONSUMED;  // remain locked
    }

    // ...otherwise clear the lock
    lock_start_time_ = 0;
    left_hand_keys_ = right_hand_keys_ = 0;
    session_start_time_ = millis();

    TypingBreak(false);
  }

  // Any other case, we are not locked yet! (or we just unlocked)

  // Are we still in the same session?
  if (last_key_time_ && (millis() - last_key_time_) >= idle_time_limit) {
    // No, we are not. Clear timers and start over.
    lock_start_time_ = 0;
    left_hand_keys_ = right_hand_keys_ = 0;
    session_start_time_ = millis();
  }

  // If we have a limit on the left hand, and we reached it, lock up!
  if (settings.left_hand_max_keys && left_hand_keys_ >= settings.left_hand_max_keys) {
    lock_start_time_ = millis();
    TypingBreak(true);
    return EventHandlerResult::EVENT_CONSUMED;
  }

  // If we have a limit on the right hand, and we reached it, lock up!
  if (settings.right_hand_max_keys && right_hand_keys_ >= settings.right_hand_max_keys) {
    lock_start_time_ = millis();
    TypingBreak(true);
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (lock_time_out) {
    // Is the session longer than lock_time_out?
    if (millis() - session_start_time_ >= lock_time_out) {
      // Yeah, it is.
      lock_start_time_ = last_key_time_;
      TypingBreak(true);
      return EventHandlerResult::EVENT_CONSUMED;
    }
  }

  // So it seems we did not need to lock up. In this case, lets increase key
  // counters if need be.

  if (keyToggledOn(key_state)) {
    if (col <= COLS / 2)
      left_hand_keys_++;
    else
      right_hand_keys_++;
    last_key_time_ = millis();
  }

  return EventHandlerResult::OK;
}

void TypingBreaks::enableEEPROM(void) {
  settings_base_ = ::EEPROMSettings.requestSlice(sizeof(settings));

  // If idleTime is max, assume that EEPROM is uninitialized, and store the
  // defaults.
  uint32_t idle_time;
  EEPROM.get(settings_base_, idle_time);
  if (idle_time == 0xffffffff) {
    EEPROM.put(settings_base_, settings);
  }

  EEPROM.get(settings_base_, settings);
}

bool TypingBreaks::focusHook(const char *command) {
  enum {
    IDLE_TIME_LIMIT,
    LOCK_TIMEOUT,
    LOCK_LENGTH,
    LEFT_MAX,
    RIGHT_MAX,
  } subCommand;

  if (strncmp_P(command, PSTR("typingbreaks."), 13) != 0)
    return false;
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
    return false;

  switch (subCommand) {
  case IDLE_TIME_LIMIT:
    if (Serial.peek() == '\n') {
      Serial.println(settings.idle_time_limit);
    } else {
      settings.idle_time_limit = Serial.parseInt();
    }
    break;
  case LOCK_TIMEOUT:
    if (Serial.peek() == '\n') {
      Serial.println(settings.lock_time_out);
    } else {
      settings.lock_time_out = Serial.parseInt();
    }
    break;
  case LOCK_LENGTH:
    if (Serial.peek() == '\n') {
      Serial.println(settings.lock_length);
    } else {
      settings.lock_length = Serial.parseInt();
    }
    break;
  case LEFT_MAX:
    if (Serial.peek() == '\n') {
      Serial.println(settings.left_hand_max_keys);
    } else {
      settings.left_hand_max_keys = Serial.parseInt();
    }
    break;
  case RIGHT_MAX:
    if (Serial.peek() == '\n') {
      Serial.println(settings.right_hand_max_keys);
    } else {
      settings.right_hand_max_keys = Serial.parseInt();
    }
    break;
  }

  EEPROM.put(settings_base_, settings);
  return true;
}

// Legacy V1 API

#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
void TypingBreaks::begin() {
  Kaleidoscope.useEventHandlerHook(legacyEventHandler);
}

Key TypingBreaks::legacyEventHandler(Key mapped_key, byte row, byte col, uint8_t key_state) {
  EventHandlerResult r = ::TypingBreaks.onKeyswitchEvent(mapped_key, row, col, key_state);
  if (r == EventHandlerResult::OK)
    return mapped_key;
  return Key_NoKey;
}
#endif

}

kaleidoscope::TypingBreaks TypingBreaks;

__attribute__((weak)) void TypingBreak(bool isLocked) {
}
