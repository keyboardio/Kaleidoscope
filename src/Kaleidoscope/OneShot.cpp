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

using namespace KaleidoscopePlugins::Ranges;

namespace KaleidoscopePlugins {
  // ---- state ---------

  uint32_t OneShot::startTime = 0;
  uint16_t OneShot::timeOut = 2500;
  uint16_t OneShot::holdTimeOut = 250;
  OneShot::state_t OneShot::State;
  OneShot::state_t OneShot::stickyState;
  OneShot::state_t OneShot::pressedState;
  uint32_t OneShot::leftMask;
  uint32_t OneShot::rightMask;
  Key OneShot::prevKey;
  bool OneShot::shouldCancel = false;
  bool OneShot::shouldCancelStickies = false;

  // --- helper macros ------

#define isOS(key) (key.raw >= OS_FIRST && key.raw <= OS_LAST)
#define isModifier(key) (key.raw >= Key_LCtrl.raw && key.raw <= Key_RGUI.raw)
#define isLayerKey(key) (key.flags == (KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP) && key.keyCode >= MOMENTARY_OFFSET && key.keyCode <= MOMENTARY_OFFSET + 23)

#define isOneShot(idx) (bitRead (State.all, (idx)))
#define setOneShot(idx) (bitWrite (State.all, idx, 1))
#define clearOneShot(idx) (bitWrite (State.all, idx, 0))

#define isSticky(idx) (bitRead (stickyState.all, idx))
#define setSticky(idx) (bitWrite (stickyState.all, idx, 1))
#define clearSticky(idx) bitWrite (stickyState.all, idx, 0)

#define setPressed(idx) bitWrite(pressedState.all, idx, 1)
#define clearPressed(idx) bitWrite(pressedState.all, idx, 0)
#define isPressed(idx) bitRead (pressedState.all, idx)

#define isSameAsPrevious(key) (key.raw == prevKey.raw)
#define saveAsPrevious(key) prevKey.raw = key.raw

#define hasTimedOut() (millis () - startTime >= timeOut)

  // ---- OneShot stuff ----
  void
  OneShot::injectNormalKey (uint8_t idx, uint8_t keyState) {
    Key key;

    if (idx < 8) {
      key.flags = Key_LCtrl.flags;
      key.keyCode = Key_LCtrl.keyCode + idx;
    } else {
      key.flags = KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP;
      key.keyCode = MOMENTARY_OFFSET + idx - 8;
    }

    handle_keyswitch_event (key, UNKNOWN_KEYSWITCH_LOCATION, keyState | INJECTED);
  }

  void
  OneShot::activateOneShot (uint8_t idx) {
    injectNormalKey (idx, IS_PRESSED);
  }

  void
  OneShot::cancelOneShot (uint8_t idx) {
    clearOneShot (idx);
    injectNormalKey (idx, WAS_PRESSED);
  }

  void
  OneShot::mask (byte row, byte col) {
    if (row >= ROWS || col >= COLS)
      return;

    if (col >= 8) {
      col = col - 8;
      rightMask |= SCANBIT (row, col);
    } else
      leftMask |= SCANBIT (row, col);
  }

  void
  OneShot::unmask (byte row, byte col) {
    if (row >= ROWS || col >= COLS)
      return;

    if (col >= 8) {
      col = col - 8;
      rightMask &= ~(SCANBIT (row, col));
    } else
      leftMask &= ~(SCANBIT (row, col));
  }

  bool
  OneShot::isMasked (byte row, byte col) {
    if (row >= ROWS || col >= COLS)
      return false;

    if (col >= 8) {
      col = col - 8;
      return rightMask & SCANBIT (row, col);
    } else
      return leftMask & SCANBIT (row, col);
  }

  Key
  OneShot::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    uint8_t idx;

    if (keyState & INJECTED)
      return mappedKey;

    if (!State.all) {
      if (!isOS (mappedKey)) {
        if (isMasked (row, col)) {
          if (key_toggled_off (keyState))
            unmask (row, col);
          return Key_NoKey;
        }

        return mappedKey;
      }

      idx = mappedKey.raw - OS_FIRST;
      if (key_toggled_off (keyState)) {
        clearPressed (idx);
      } else if (key_toggled_on (keyState)) {
        startTime = millis ();
        setPressed (idx);
        setOneShot (idx);
        saveAsPrevious (mappedKey);

        activateOneShot (idx);
      }

      return Key_NoKey;
    }

    if (!key_is_pressed (keyState) && !key_was_pressed (keyState))
      return mappedKey;

    if (isOS (mappedKey)) {
      idx = mappedKey.raw - OS_FIRST;

      if (isSticky (idx)) {
        if (key_toggled_on (keyState)) { // maybe on _off instead?
          saveAsPrevious (mappedKey);
          clearSticky (idx);
          cancelOneShot (idx);
        }
      } else {
        if (key_toggled_off (keyState)) {
          clearPressed (idx);
          if ((millis () - startTime) >= holdTimeOut) {
            cancelOneShot (idx);
          }
        }

        if (key_toggled_on (keyState)) {
          setPressed (idx);
          if (isSameAsPrevious (mappedKey)) {
            setSticky (idx);

            saveAsPrevious (mappedKey);
          } else {
            startTime = millis ();

            setOneShot (idx);
            saveAsPrevious (mappedKey);

            activateOneShot (idx);
          }
        }
      }

      return Key_NoKey;
    }

    // ordinary key here, with some event

    if (key_is_pressed (keyState)) {
      mask (row, col);
      saveAsPrevious (mappedKey);
      shouldCancel = true;
    }

    return mappedKey;
  }

  void
  OneShot::loopHook (bool postClear) {
    if (!State.all)
      return;

    if (postClear) {
      if (hasTimedOut ())
        cancel ();

      for (uint8_t i = 0; i < 32; i++) {
        if (shouldCancel) {
          if (isSticky (i)) {
            if (shouldCancelStickies) {
              clearSticky (i);
            }
          } else if (isOneShot (i) && !isPressed (i)) {
            cancelOneShot (i);
          }
        }
      }

      if (shouldCancel) {
        shouldCancel = false;
        shouldCancelStickies = false;
      }
    } else {
      for (uint8_t i = 0; i < 8; i++) {
        if (isOneShot (i)) {
          activateOneShot (i);
        }
      }
    }
  }

  // --- glue code ---

  OneShot::OneShot (void) {
  }

  void
  OneShot::begin (void) {
    event_handler_hook_use (eventHandlerHook);
    loop_hook_use (loopHook);
  }

  bool
  OneShot::isActive (void) {
    return (State.all && !hasTimedOut ());
  }

  bool
  OneShot::isModifierActive (Key key) {
    if (key.raw < Key_LCtrl.raw || key.raw > Key_RGUI.raw)
      return false;

    return isOneShot (key.keyCode - Key_LCtrl.keyCode);
  }

  void
  OneShot::cancel (bool withStickies) {
    shouldCancel = true;
    shouldCancelStickies = withStickies;
  }

  void
  OneShot::inject (Key key, uint8_t keyState) {
    eventHandlerHook (key, 255, 255, keyState);
  }

};

KaleidoscopePlugins::OneShot OneShot;
