/* Kaleidoscope-Macros - Macro keys for Kaleidoscope.
 * Copyright (C) 2017-2021  Keyboard.io, Inc.
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

#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-FocusSerial.h"
#include "kaleidoscope/keyswitch_state.h"

// =============================================================================
// Default `macroAction()` function definitions
__attribute__((weak))
const macro_t *macroAction(uint8_t macro_id, KeyEvent &event) {
  return MACRO_NONE;
}

// =============================================================================
// `Macros` plugin code
namespace kaleidoscope {
namespace plugin {

constexpr uint8_t press_state = IS_PRESSED | INJECTED;
constexpr uint8_t release_state = WAS_PRESSED | INJECTED;

// Initialized to zeroes (i.e. `Key_NoKey`)
Key Macros::active_macro_keys_[];

// -----------------------------------------------------------------------------
// Public helper functions

void Macros::press(Key key) {
  Runtime.handleKeyEvent(KeyEvent{KeyAddr::none(), press_state, key});
  // This key may remain active for several subsequent events, so we need to
  // store it in the active macro keys array.
  for (Key &macro_key : active_macro_keys_) {
    if (macro_key == Key_NoKey) {
      macro_key = key;
      break;
    }
  }
}

void Macros::release(Key key) {
  // Before sending the release event, we need to remove the key from the active
  // macro keys array, or it will get inserted into the report anyway.
  for (Key &macro_key : active_macro_keys_) {
    if (macro_key == key) {
      macro_key = Key_NoKey;
    }
  }
  Runtime.handleKeyEvent(KeyEvent{KeyAddr::none(), release_state, key});
}

void Macros::clear() {
  // Clear the active macro keys array.
  for (Key &macro_key : active_macro_keys_) {
    if (macro_key == Key_NoKey)
      continue;
    Runtime.handleKeyEvent(KeyEvent{KeyAddr::none(), release_state, macro_key});
    macro_key = Key_NoKey;
  }
}

void Macros::tap(Key key) const {
  // No need to call `press()` & `release()`, because we're immediately
  // releasing the key after pressing it. It is possible for some other plugin
  // to insert an event in between, but very unlikely.
  Runtime.handleKeyEvent(KeyEvent{KeyAddr::none(), press_state, key});
  Runtime.handleKeyEvent(KeyEvent{KeyAddr::none(), release_state, key});
}

void Macros::play(const macro_t *macro_p) {
  macro_t macro = MACRO_ACTION_END;
  uint8_t interval = 0;
  Key key;

  if (macro_p == MACRO_NONE)
    return;

  while (true) {
    switch (macro = pgm_read_byte(macro_p++)) {
    // These are unlikely to be useful now that we have KeyEvent. I think the
    // whole `explicit_report` came about as a result of scan-order bugs.
    case MACRO_ACTION_STEP_EXPLICIT_REPORT:
    case MACRO_ACTION_STEP_IMPLICIT_REPORT:
    case MACRO_ACTION_STEP_SEND_REPORT:
      break;
    // End legacy macro step commands

    // Timing
    case MACRO_ACTION_STEP_INTERVAL:
      interval = pgm_read_byte(macro_p++);
      break;
    case MACRO_ACTION_STEP_WAIT: {
      uint8_t wait = pgm_read_byte(macro_p++);
      delay(wait);
      break;
    }

    case MACRO_ACTION_STEP_KEYDOWN:
      key.setFlags(pgm_read_byte(macro_p++));
      key.setKeyCode(pgm_read_byte(macro_p++));
      press(key);
      break;
    case MACRO_ACTION_STEP_KEYUP:
      key.setFlags(pgm_read_byte(macro_p++));
      key.setKeyCode(pgm_read_byte(macro_p++));
      release(key);
      break;
    case MACRO_ACTION_STEP_TAP:
      key.setFlags(pgm_read_byte(macro_p++));
      key.setKeyCode(pgm_read_byte(macro_p++));
      tap(key);
      break;

    case MACRO_ACTION_STEP_KEYCODEDOWN:
      key.setFlags(0);
      key.setKeyCode(pgm_read_byte(macro_p++));
      press(key);
      break;
    case MACRO_ACTION_STEP_KEYCODEUP:
      key.setFlags(0);
      key.setKeyCode(pgm_read_byte(macro_p++));
      release(key);
      break;
    case MACRO_ACTION_STEP_TAPCODE:
      key.setFlags(0);
      key.setKeyCode(pgm_read_byte(macro_p++));
      tap(key);
      break;

    case MACRO_ACTION_STEP_TAP_SEQUENCE: {
      while (true) {
        key.setFlags(0);
        key.setKeyCode(pgm_read_byte(macro_p++));
        if (key == Key_NoKey)
          break;
        tap(key);
        delay(interval);
      }
      break;
    }
    case MACRO_ACTION_STEP_TAP_CODE_SEQUENCE: {
      while (true) {
        key.setFlags(0);
        key.setKeyCode(pgm_read_byte(macro_p++));
        if (key.getKeyCode() == 0)
          break;
        tap(key);
        delay(interval);
      }
      break;
    }

    case MACRO_ACTION_END:
    default:
      return;
    }

    delay(interval);
  }
}

const macro_t *Macros::type(const char *string) const {
  while (true) {
    uint8_t ascii_code = pgm_read_byte(string++);
    if (ascii_code == 0)
      break;

    Key key = lookupAsciiCode(ascii_code);

    if (key == Key_NoKey)
      continue;

    tap(key);
  }

  return MACRO_NONE;
}

// -----------------------------------------------------------------------------
// Translation from ASCII to keycodes

static const Key ascii_to_key_map[] PROGMEM = {
  // 0x21 - 0x30
  LSHIFT(Key_1),
  LSHIFT(Key_Quote),
  LSHIFT(Key_3),
  LSHIFT(Key_4),
  LSHIFT(Key_5),
  LSHIFT(Key_7),
  Key_Quote,
  LSHIFT(Key_9),
  LSHIFT(Key_0),
  LSHIFT(Key_8),
  LSHIFT(Key_Equals),
  Key_Comma,
  Key_Minus,
  Key_Period,
  Key_Slash,
  Key_0,

  // 0x3a ... 0x40
  LSHIFT(Key_Semicolon),
  Key_Semicolon,
  LSHIFT(Key_Comma),
  Key_Equals,
  LSHIFT(Key_Period),
  LSHIFT(Key_Slash),
  LSHIFT(Key_2),

  // 0x5b ... 0x60
  Key_LeftBracket,
  Key_Backslash,
  Key_RightBracket,
  LSHIFT(Key_6),
  LSHIFT(Key_Minus),
  Key_Backtick,

  // 0x7b ... 0x7e
  LSHIFT(Key_LeftBracket),
  LSHIFT(Key_Backslash),
  LSHIFT(Key_RightBracket),
  LSHIFT(Key_Backtick),
};

Key Macros::lookupAsciiCode(uint8_t ascii_code) const {
  Key key = Key_NoKey;

  switch (ascii_code) {
  case 0x08 ... 0x09:
    key.setKeyCode(Key_Backspace.getKeyCode() + ascii_code - 0x08);
    break;
  case 0x0A:
    key.setKeyCode(Key_Enter.getKeyCode());
    break;
  case 0x1B:
    key.setKeyCode(Key_Escape.getKeyCode());
    break;
  case 0x20:
    key.setKeyCode(Key_Spacebar.getKeyCode());
    break;
  case 0x21 ... 0x30:
    key = ascii_to_key_map[ascii_code - 0x21].readFromProgmem();
    break;
  case 0x31 ... 0x39:
    key.setKeyCode(Key_1.getKeyCode() + ascii_code - 0x31);
    break;
  case 0x3A ... 0x40:
    key = ascii_to_key_map[ascii_code - 0x3A + 16].readFromProgmem();
    break;
  case 0x41 ... 0x5A:
    key.setFlags(SHIFT_HELD);
    key.setKeyCode(Key_A.getKeyCode() + ascii_code - 0x41);
    break;
  case 0x5B ... 0x60:
    key = ascii_to_key_map[ascii_code - 0x5B + 23].readFromProgmem();
    break;
  case 0x61 ... 0x7A:
    key.setKeyCode(Key_A.getKeyCode() + ascii_code - 0x61);
    break;
  case 0x7B ... 0x7E:
    key = ascii_to_key_map[ascii_code - 0x7B + 29].readFromProgmem();
    break;
  }
  return key;
}

// -----------------------------------------------------------------------------
// Event handlers

EventHandlerResult Macros::onKeyEvent(KeyEvent &event) {
  // Ignore everything except Macros keys
  if (!isMacrosKey(event.key))
    return EventHandlerResult::OK;

  // Decode the macro ID from the Macros `Key` value.
  uint8_t macro_id = event.key.getRaw() - ranges::MACRO_FIRST;

  // Call the new `macroAction(event)` function.
  const macro_t* macro_ptr = macroAction(macro_id, event);

  // Play back the macro pointed to by `macroAction()`
  play(macro_ptr);

  if (keyToggledOff(event.state) || !isMacrosKey(event.key)) {
    // If we toggled off or if the value of `event.key` has changed, send
    // release events for any active macro keys. Simply clearing
    // `active_macro_keys_` might be sufficient, but it's probably better to
    // send the toggle off events so that other plugins get a chance to act on
    // them.
    clear();

    // Return `OK` to let Kaleidoscope finish processing this event as
    // normal. This is so that, if the user-defined `macroAction(id, &event)`
    // function changes the value of `event.key`, it will take effect properly.
    return EventHandlerResult::OK;
  }

  // No other plugin should be handling Macros keys, and there's nothing more
  // for Kaleidoscope to do with a key press of a Macros key, so we return
  // `EVENT_CONSUMED`, causing Kaleidoscope to update `live_keys[]` correctly,
  // ensuring that the above block will clear `active_macro_keys_` on release,
  // but not allowing any other plugins to change the `event.key` value, which
  // would interfere.
  //return EventHandlerResult::EVENT_CONSUMED;
  return EventHandlerResult::OK;
}

EventHandlerResult Macros::beforeReportingState(const KeyEvent &event) {
  // Do this in beforeReportingState(), instead of `onAddToReport()` because
  // `live_keys` won't get updated until after the macro sequence is played from
  // the keypress. This could be changed by either updating `live_keys` manually
  // ahead of time, or by executing the macro sequence on key release instead of
  // key press. This is probably the simplest solution.
  for (Key key : active_macro_keys_) {
    if (key != Key_NoKey)
      Runtime.addToReport(key);
  }
  return EventHandlerResult::OK;
}

EventHandlerResult Macros::onNameQuery() {
  return ::Focus.sendName(F("Macros"));
}

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::Macros Macros;
