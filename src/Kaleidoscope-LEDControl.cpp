#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-Focus.h"

namespace kaleidoscope {

LEDMode *LEDControl::modes[LED_MAX_MODES];
uint8_t LEDControl::mode;
uint16_t LEDControl::syncDelay = 16;
uint32_t LEDControl::syncTimer;
bool LEDControl::paused = false;

void LEDMode::activate(void) {
  ::LEDControl.activate(this);
}

void LEDMode::begin(void) {
  Kaleidoscope.use(&::LEDControl);
  ::LEDControl.mode_add(this);
  setup();
}

LEDControl::LEDControl(void) {
  mode = 0;
  memset(modes, 0, LED_MAX_MODES * sizeof(modes[0]));
}

void LEDControl::next_mode(void) {
  mode++;

  if (mode >= LED_MAX_MODES || !modes[mode]) {
    return set_mode(0);
  }

  return set_mode(mode);
}

void LEDControl::prev_mode(void) {
  if (mode == 0) {
    // wrap around
    mode = LED_MAX_MODES;
    // then  count down until reaching a valid mode
    while (mode > 0 && !modes[mode]) mode--;
  } else {
    mode--;
  }
  set_mode(mode);
}

void
LEDControl::set_mode(uint8_t mode_) {
  if (mode_ >= LED_MAX_MODES)
    return;

  mode = mode_;
  refreshAll();
}

uint8_t LEDControl::get_mode_index(void) {
  return mode;
}

LEDMode *LEDControl::get_mode(void) {
  return modes[mode];
}

void LEDControl::activate(LEDMode *mode) {
  for (uint8_t i = 0; i < LED_MAX_MODES; i++) {
    if (modes[i] == mode)
      return set_mode(i);
  }
}

int8_t LEDControl::mode_add(LEDMode *mode) {
  for (int i = 0; i < LED_MAX_MODES; i++) {
    if (modes[i])
      continue;

    modes[i] = mode;
    return i;
  }
  return -1;
}

void LEDControl::set_all_leds_to(uint8_t r, uint8_t g, uint8_t b) {
  cRGB color;
  color.r = r;
  color.g = g;
  color.b = b;
  set_all_leds_to(color);
}

void LEDControl::set_all_leds_to(cRGB color) {
  for (uint8_t i = 0; i < LED_COUNT; i++) {
    setCrgbAt(i, color);
  }
}

void LEDControl::setCrgbAt(uint8_t i, cRGB crgb) {
  KeyboardHardware.setCrgbAt(i, crgb);
}

void LEDControl::setCrgbAt(byte row, byte col, cRGB color) {
  KeyboardHardware.setCrgbAt(row, col, color);
}

cRGB LEDControl::getCrgbAt(uint8_t i) {
  return KeyboardHardware.getCrgbAt(i);
}

void LEDControl::syncLeds(void) {
  KeyboardHardware.syncLeds();
}

void LEDControl::begin(void) {
  set_all_leds_to({0, 0, 0});

  for (uint8_t i = 0; i < LED_MAX_MODES; i++) {
    if (modes[i])
      (modes[i]->setup)();
  }

  Kaleidoscope.useEventHandlerHook(eventHandler);
  Kaleidoscope.useLoopHook(loopHook);

  syncTimer = millis() + syncDelay;
}

Key LEDControl::eventHandler(Key mappedKey, byte row, byte col, uint8_t keyState) {
  if (mappedKey.flags != (SYNTHETIC | IS_INTERNAL | LED_TOGGLE))
    return mappedKey;

  if (keyToggledOn(keyState))
    next_mode();

  return Key_NoKey;
}

void LEDControl::loopHook(bool postClear) {
  if (postClear || paused)
    return;

  if (millis() > syncTimer) {
    syncLeds();
    syncTimer = millis() + syncDelay;
  }
  update();
}

bool LEDControl::focusHook(const char *command) {
  enum {
    SETALL,
    MODE,
    AT,
    THEME,
  } subCommand;

  if (strncmp_P(command, PSTR("led."), 4) != 0)
    return false;
  if (strcmp_P(command + 4, PSTR("at")) == 0)
    subCommand = AT;
  else if (strcmp_P(command + 4, PSTR("setAll")) == 0)
    subCommand = SETALL;
  else if (strcmp_P(command + 4, PSTR("mode")) == 0)
    subCommand = MODE;
  else if (strcmp_P(command + 4, PSTR("theme")) == 0)
    subCommand = THEME;
  else
    return false;

  switch (subCommand) {
  case AT: {
    uint8_t idx = Serial.parseInt();

    if (Serial.peek() == '\n') {
      cRGB c = getCrgbAt(idx);

      ::Focus.printColor(c.r, c.g, c.b);
      Serial.println();
    } else {
      cRGB c;

      c.r = Serial.parseInt();
      c.g = Serial.parseInt();
      c.b = Serial.parseInt();

      setCrgbAt(idx, c);
    }
    break;
  }
  case SETALL: {
    cRGB c;

    c.r = Serial.parseInt();
    c.g = Serial.parseInt();
    c.b = Serial.parseInt();

    set_all_leds_to(c);

    break;
  }
  case MODE: {
    char peek = Serial.peek();
    if (peek == '\n') {
      Serial.println(get_mode_index());
    } else if (peek == 'n') {
      next_mode();
      Serial.read();
    } else if (peek == 'p') {
      // TODO(algernon)
      Serial.read();
    } else {
      uint8_t mode = Serial.parseInt();

      set_mode(mode);
    }
    break;
  }
  case THEME: {
    if (Serial.peek() == '\n') {
      for (uint8_t idx = 0; idx < LED_COUNT; idx++) {
        cRGB c = getCrgbAt(idx);

        ::Focus.printColor(c.r, c.g, c.b);
        ::Focus.printSpace();
      }
      Serial.println();
      break;
    }

    uint8_t idx = 0;
    while (idx < LED_COUNT && Serial.peek() != '\n') {
      cRGB color;

      color.r = Serial.parseInt();
      color.g = Serial.parseInt();
      color.b = Serial.parseInt();

      setCrgbAt(idx, color);
      idx++;
    }
    break;
  }
  }

  return true;
}

}

kaleidoscope::LEDControl LEDControl;
