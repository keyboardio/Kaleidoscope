#include "Kaleidoscope-Macros.h"

__attribute__((weak))
const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  return MACRO_NONE;
}

byte Macros_::row, Macros_::col;

static void readKeyCodeAndPlay(const macro_t *macro_p, uint8_t flags, uint8_t keyStates) {
  Key key;
  key.flags = flags;
  key.keyCode = pgm_read_byte(macro_p++);

  if (keyStates & IS_PRESSED) {
    handle_keyswitch_event(key, UNKNOWN_KEYSWITCH_LOCATION, IS_PRESSED | INJECTED);
    Keyboard.sendReport();
  }
  if (keyStates & WAS_PRESSED) {
    handle_keyswitch_event(key, UNKNOWN_KEYSWITCH_LOCATION, WAS_PRESSED | INJECTED);
    Keyboard.sendReport();
  }
}

void Macros_::play(const macro_t *macro_p) {
  macro_t macro = END;
  uint8_t interval = 0;
  uint8_t flags;

  if (!macro_p)
    return;

  while (true) {
    switch (macro = pgm_read_byte(macro_p++)) {
    case MACRO_ACTION_STEP_INTERVAL:
      interval = pgm_read_byte(macro_p++);
      break;
    case MACRO_ACTION_STEP_WAIT: {
      uint8_t wait = pgm_read_byte(macro_p++);
      delay(wait);
      break;
    }
    case MACRO_ACTION_STEP_KEYDOWN:
      flags = pgm_read_byte(macro_p++);
      readKeyCodeAndPlay(macro_p++, flags, IS_PRESSED);
      break;
    case MACRO_ACTION_STEP_KEYUP:
      flags = pgm_read_byte(macro_p++);
      readKeyCodeAndPlay(macro_p++, flags, WAS_PRESSED);
      break;
    case MACRO_ACTION_STEP_TAP:
      flags = pgm_read_byte(macro_p++);
      readKeyCodeAndPlay(macro_p++, flags, IS_PRESSED | WAS_PRESSED);
      break;

    case MACRO_ACTION_STEP_KEYCODEDOWN:
      readKeyCodeAndPlay(macro_p++, 0, IS_PRESSED);
      break;
    case MACRO_ACTION_STEP_KEYCODEUP:
      readKeyCodeAndPlay(macro_p++, 0, WAS_PRESSED);
      break;
    case MACRO_ACTION_STEP_TAPCODE:
      readKeyCodeAndPlay(macro_p++, 0, IS_PRESSED | WAS_PRESSED);
      break;

    case END:
    default:
      return;
    }

    delay(interval);
  }
}
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

  // 0x5b ... 0x5f
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

void Macros_::type(const char *string) {
  while (true) {
    uint8_t ascii_code = pgm_read_byte(string++);
    if (!ascii_code)
      break;

    Key key = Key_NoKey;

    switch (ascii_code) {
    case 0x08 ... 0x09:
      key.keyCode = Key_Backspace.keyCode + ascii_code - 0x08;
      break;
    case 0x0A:
      key.keyCode = Key_Enter.keyCode;
      break;
    case 0x1B:
      key.keyCode = Key_Escape.keyCode;
      break;
    case 0x20:
      key.keyCode = Key_Spacebar.keyCode;
      break;
    case 0x21 ... 0x30:
      key.raw = pgm_read_word(&ascii_to_key_map[ascii_code - 0x21]);
      break;
    case 0x31 ... 0x39:
      key.keyCode = Key_1.keyCode + ascii_code - 0x31;
      break;
    case 0x3A ... 0x40:
      key.raw = pgm_read_word(&ascii_to_key_map[ascii_code - 0x3A + 16]);
      break;
    case 0x41 ... 0x5A:
      key.flags = SHIFT_HELD;
      key.keyCode = Key_A.keyCode + ascii_code - 0x41;
      break;
    case 0x5B ... 0x5F:
      key.raw = pgm_read_word(&ascii_to_key_map[ascii_code - 0x5B + 23]);
      break;
    case 0x61 ... 0x7A:
      key.keyCode = Key_A.keyCode + ascii_code - 0x61;
      break;
    case 0x7B ... 0x7E:
      key.raw = pgm_read_word(&ascii_to_key_map[ascii_code - 0x7B + 29]);
      break;
    }

    if (key.raw == Key_NoKey.raw)
      continue;

    handle_keyswitch_event(key, UNKNOWN_KEYSWITCH_LOCATION, IS_PRESSED | INJECTED);
    Keyboard.sendReport();
    handle_keyswitch_event(key, UNKNOWN_KEYSWITCH_LOCATION, WAS_PRESSED | INJECTED);
    Keyboard.sendReport();
  }
}

static Key handleMacroEvent(Key mappedKey, byte row, byte col, uint8_t keyState) {
  if (mappedKey.flags != (SYNTHETIC | IS_MACRO))
    return mappedKey;

  Macros_::row = row;
  Macros_::col = col;
  const macro_t *m = macroAction(mappedKey.keyCode, keyState);

  Macros.play(m);
  return Key_NoKey;
}

Macros_::Macros_(void) {
}

void
Macros_::begin(void) {
  event_handler_hook_use(handleMacroEvent);
}

Macros_ Macros;
