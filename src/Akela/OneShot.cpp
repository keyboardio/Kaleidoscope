/* -*- mode: c++ -*-
 * Akela -- Animated Keyboardio Extension Library for Anything
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

#include <Akela-OneShot.h>

using namespace Akela::Ranges;

namespace Akela {
  // ---- state ---------

  uint8_t OneShot::Timer = 0;
  uint8_t OneShot::timeOut = 40;
  uint32_t OneShot::State = 0;
  uint32_t OneShot::stickyState = 0;
  uint32_t OneShot::pressedState = 0;
  uint32_t OneShot::leftMask;
  uint32_t OneShot::rightMask;
  Key OneShot::prevKey;
  bool OneShot::shouldCancel = false;
  bool OneShot::shouldCancelStickies = false;

  // --- helper macros ------

#define isOS(key) (key.raw >= OS_FIRST && key.raw <= OS_LAST)
#define isModifier(key) (key.raw >= Key_LCtrl.raw && key.raw <= Key_RGUI.raw)
#define isLayerKey(key) (key.flags == (KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP) && key.keyCode >= MOMENTARY_OFFSET && key.keyCode <= MOMENTARY_OFFSET + 23)

#define isOneShot(idx) (bitRead (State, (idx)))
#define setOneShot(idx) (bitWrite (State, idx, 1))
#define clearOneShot(idx) (bitWrite (State, idx, 0))

#define isSticky(idx) (bitRead (stickyState, idx))
#define setSticky(idx) (bitWrite (stickyState, idx, 1))
#define clearSticky(idx) bitWrite (stickyState, idx, 0)

#define setPressed(idx) bitWrite(pressedState, idx, 1)
#define clearPressed(idx) bitWrite(pressedState, idx, 0)
#define isPressed(idx) bitRead (pressedState, idx)

#define isSameAsPrevious(key) (key.raw == prevKey.raw)
#define saveAsPrevious(key) prevKey.raw = key.raw

#define toNormalMod(key, idx) {key.flags = 0; key.keyCode = Key_LCtrl.keyCode + idx;}
#define toNormalMT(key, idx) { key.raw = Key_NoKey.raw; Layer.on (idx - 8); }
#define hasTimedOut() (Timer >= timeOut)

  // ----- passthrough ------

  Key
  OneShot::eventHandlerPassthroughHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (!isOS (mappedKey))
      return mappedKey;

    uint8_t idx = mappedKey.raw - OS_FIRST;

    if (idx >= 8) {
      toNormalMT (mappedKey, idx);
    } else {
      toNormalMod (mappedKey, idx);
    }

    return mappedKey;
  }

  void
  OneShot::loopNoOpHook (bool postClear) {
  }

  // ---- OneShot stuff ----

  Key
  OneShot::eventHandlerAutoHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    // If mappedKey is an injected key, we don't fiddle with those.
    if (keyState & INJECTED)
      return mappedKey;

    if (!isModifier (mappedKey) && !isLayerKey (mappedKey))
      return mappedKey;

    if (isModifier (mappedKey)) {
      uint8_t idx = mappedKey.keyCode - Key_LCtrl.keyCode;

      return (Key){.raw = OSM_FIRST + idx};
    } else {
      uint8_t idx = mappedKey.keyCode - MOMENTARY_OFFSET;

      return (Key){.raw = OSL_FIRST + idx};
    }
  }

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

    handle_key_event (key, 255, 255, keyState | INJECTED);
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

    if (!State) {
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
      } else if (key_toggled_on (keyState)){
        Timer = 0;
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
          if (hasTimedOut ()) {
            cancelOneShot (idx);
          }
        }

        if (key_toggled_on (keyState)) {
          setPressed (idx);
          if (isSameAsPrevious (mappedKey)) {
            setSticky (idx);

            saveAsPrevious (mappedKey);
          } else {
            Timer = 0;

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
    if (!State)
      return;

    if (postClear) {
      if (Timer < timeOut)
        Timer++;

      if (hasTimedOut ())
        cancel ();

      for (uint8_t i = 0; i < 32; i++) {
        if (shouldCancel) {
          if (isSticky (i)) {
            if (shouldCancelStickies) {
              Timer = 0;
              clearSticky (i);
            }
          } else if (isOneShot (i) && !isPressed (i)) {
            Timer = 0;
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
    return (State && !hasTimedOut ());
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
  OneShot::on (void) {
    event_handler_hook_replace (eventHandlerPassthroughHook, eventHandlerHook);
    loop_hook_replace (loopNoOpHook, loopHook);
  }

  void
  OneShot::off (void) {
    event_handler_hook_replace (eventHandlerHook, eventHandlerPassthroughHook);
    loop_hook_replace (loopHook, loopNoOpHook);
  }

  void
  OneShot::enableAuto (void) {
    event_handler_hook_use (eventHandlerAutoHook);
  }

  void
  OneShot::inject (Key key, uint8_t keyState) {
    eventHandlerHook (key, 255, 255, keyState);
  }

};

Akela::OneShot OneShot;
