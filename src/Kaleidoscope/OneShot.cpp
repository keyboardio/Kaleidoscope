/* -*- mode: c++ -*-
 * Kaleidoscope-OneShot -- One-shot modifiers and layers
 * Copyright (C) 2016, 2017  Gergely Nagy
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

#include <Kaleidoscope-OneShot.h>

namespace kaleidoscope {

// ---- state ---------

uint32_t OneShot::start_time_ = 0;
uint16_t OneShot::time_out = 2500;
uint16_t OneShot::hold_time_out = 250;
OneShot::state_t OneShot::state_;
OneShot::state_t OneShot::sticky_state_;
OneShot::state_t OneShot::pressed_state_;
uint32_t OneShot::left_mask_;
uint32_t OneShot::right_mask_;
Key OneShot::prev_key_;
bool OneShot::should_cancel_ = false;
bool OneShot::should_cancel_stickies_ = false;

// --- helper macros ------

#define isOS(key) (key.raw >= ranges::OS_FIRST && key.raw <= ranges::OS_LAST)
#define isModifier(key) (key.raw >= Key_LeftControl.raw && key.raw <= Key_RightGui.raw)
#define isLayerKey(key) (key.flags == (KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP) && key.keyCode >= MOMENTARY_OFFSET && key.keyCode <= MOMENTARY_OFFSET + 23)

#define isOneShot(idx) (bitRead (state_.all, (idx)))
#define setOneShot(idx) (bitWrite (state_.all, idx, 1))
#define clearOneShot(idx) (bitWrite (state_.all, idx, 0))

#define isSticky(idx) (bitRead (sticky_state_.all, idx))
#define setSticky(idx) (bitWrite (sticky_state_.all, idx, 1))
#define clearSticky(idx) bitWrite (sticky_state_.all, idx, 0)

#define setPressed(idx) bitWrite(pressed_state_.all, idx, 1)
#define clearPressed(idx) bitWrite(pressed_state_.all, idx, 0)
#define isPressed(idx) bitRead (pressed_state_.all, idx)

#define isSameAsPrevious(key) (key.raw == prev_key_.raw)
#define saveAsPrevious(key) prev_key_.raw = key.raw

#define hasTimedOut() (millis () - start_time_ >= time_out)

// ---- OneShot stuff ----
void OneShot::injectNormalKey(uint8_t idx, uint8_t key_state) {
  Key key;

  if (idx < 8) {
    key.flags = Key_LeftControl.flags;
    key.keyCode = Key_LeftControl.keyCode + idx;
  } else {
    key.flags = KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP;
    key.keyCode = MOMENTARY_OFFSET + idx - 8;
  }

  handleKeyswitchEvent(key, UNKNOWN_KEYSWITCH_LOCATION, key_state | INJECTED);
}

void OneShot::activateOneShot(uint8_t idx) {
  injectNormalKey(idx, IS_PRESSED);
}

void OneShot::cancelOneShot(uint8_t idx) {
  clearOneShot(idx);
  injectNormalKey(idx, WAS_PRESSED);
}

void OneShot::mask(byte row, byte col) {
  if (row >= ROWS || col >= COLS)
    return;

  if (col >= 8) {
    col = col - 8;
    right_mask_ |= SCANBIT(row, col);
  } else {
    left_mask_ |= SCANBIT(row, col);
  }
}

void OneShot::unmask(byte row, byte col) {
  if (row >= ROWS || col >= COLS)
    return;

  if (col >= 8) {
    col = col - 8;
    right_mask_ &= ~(SCANBIT(row, col));
  } else {
    left_mask_ &= ~(SCANBIT(row, col));
  }
}

bool OneShot::isMasked(byte row, byte col) {
  if (row >= ROWS || col >= COLS)
    return false;

  if (col >= 8) {
    col = col - 8;
    return right_mask_ & SCANBIT(row, col);
  } else {
    return left_mask_ & SCANBIT(row, col);
  }
}

Key OneShot::eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state) {
  uint8_t idx;

  if (key_state & INJECTED)
    return mapped_key;

  if (!state_.all) {
    if (!isOS(mapped_key)) {
      if (isMasked(row, col)) {
        if (keyToggledOff(key_state))
          unmask(row, col);
        return Key_NoKey;
      }

      return mapped_key;
    }

    idx = mapped_key.raw - ranges::OS_FIRST;
    if (keyToggledOff(key_state)) {
      clearPressed(idx);
    } else if (keyToggledOn(key_state)) {
      start_time_ = millis();
      setPressed(idx);
      setOneShot(idx);
      saveAsPrevious(mapped_key);

      activateOneShot(idx);
    }

    return Key_NoKey;
  }

  if (!keyIsPressed(key_state) && !keyWasPressed(key_state))
    return mapped_key;

  if (isOS(mapped_key)) {
    idx = mapped_key.raw - ranges::OS_FIRST;

    if (isSticky(idx)) {
      if (keyToggledOn(key_state)) {  // maybe on _off instead?
        saveAsPrevious(mapped_key);
        clearSticky(idx);
        cancelOneShot(idx);
      }
    } else {
      if (keyToggledOff(key_state)) {
        clearPressed(idx);
        if ((millis() - start_time_) >= hold_time_out) {
          cancelOneShot(idx);
        }
      }

      if (keyToggledOn(key_state)) {
        setPressed(idx);
        if (isSameAsPrevious(mapped_key)) {
          setSticky(idx);

          saveAsPrevious(mapped_key);
        } else {
          start_time_ = millis();

          setOneShot(idx);
          saveAsPrevious(mapped_key);

          activateOneShot(idx);
        }
      }
    }

    return Key_NoKey;
  }

  // ordinary key here, with some event

  if (keyIsPressed(key_state)) {
    mask(row, col);
    saveAsPrevious(mapped_key);
    should_cancel_ = true;
  }

  return mapped_key;
}

void OneShot::loopHook(bool is_post_clear) {
  if (!state_.all)
    return;

  if (is_post_clear) {
    if (hasTimedOut())
      cancel();

    for (uint8_t i = 0; i < 32; i++) {
      if (should_cancel_) {
        if (isSticky(i)) {
          if (should_cancel_stickies_) {
            clearSticky(i);
          }
        } else if (isOneShot(i) && !isPressed(i)) {
          cancelOneShot(i);
        }
      }
    }

    if (should_cancel_) {
      should_cancel_ = false;
      should_cancel_stickies_ = false;
    }
  } else {
    for (uint8_t i = 0; i < 8; i++) {
      if (isOneShot(i)) {
        activateOneShot(i);
      }
    }
  }
}

// --- glue code ---

OneShot::OneShot(void) {
}

void OneShot::begin(void) {
  event_handler_hook_use(eventHandlerHook);
  loop_hook_use(loopHook);
}

bool OneShot::isActive(void) {
  return (state_.all && !hasTimedOut());
}

bool OneShot::isModifierActive(Key key) {
  if (key.raw < Key_LeftControl.raw || key.raw > Key_RightGui.raw)
    return false;

  return isOneShot(key.keyCode - Key_LeftControl.keyCode);
}

void OneShot::cancel(bool with_stickies) {
  should_cancel_ = true;
  should_cancel_stickies_ = with_stickies;
}

void OneShot::inject(Key key, uint8_t key_state) {
  eventHandlerHook(key, UNKNOWN_KEYSWITCH_LOCATION, key_state);
}

}

kaleidoscope::OneShot OneShot;
