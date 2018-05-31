#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-Focus.h"

namespace kaleidoscope {

LEDMode *LEDControl::modes[LED_MAX_MODES];
uint8_t LEDControl::mode;
uint16_t LEDControl::syncDelay = 16;
uint16_t LEDControl::syncTimer;
bool LEDControl::paused = false;

void LEDMode::activate(void) {
  ::LEDControl.activate(this);
}

kaleidoscope::EventHandlerResult LEDMode::onSetup() {
  ::LEDControl.mode_add(this);
  setup();

  return EventHandlerResult::OK;
}

#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
void LEDMode::begin() {
  onSetup();
}
#endif

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
    mode = LED_MAX_MODES - 1;
    // then  count down until reaching a valid mode
    while (mode > 0 && !modes[mode]) mode--;
  } else {
    mode--;
  }

  return set_mode(mode);
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

kaleidoscope::EventHandlerResult LEDControl::onSetup() {
  set_all_leds_to({0, 0, 0});

  for (uint8_t i = 0; i < LED_MAX_MODES; i++) {
    if (modes[i])
      (modes[i]->setup)();
  }

  syncTimer = millis() + syncDelay;

  return EventHandlerResult::OK;
}

kaleidoscope::EventHandlerResult LEDControl::onKeyswitchEvent(Key &mappedKey, byte row, byte col, uint8_t keyState) {
  if (mappedKey.flags != (SYNTHETIC | IS_INTERNAL | LED_TOGGLE))
    return kaleidoscope::EventHandlerResult::OK;

  if (keyToggledOn(keyState)) {
    if (mappedKey == Key_LEDEffectNext) {
      next_mode();
    } else if (mappedKey == Key_LEDEffectPrevious) {
      prev_mode();
    }
  }

  return kaleidoscope::EventHandlerResult::EVENT_CONSUMED;
}

kaleidoscope::EventHandlerResult LEDControl::beforeReportingState(void) {
  if (paused)
    return kaleidoscope::EventHandlerResult::OK;

  uint16_t current_time = millis();
  if ((current_time - syncTimer) > syncDelay) {
    syncLeds();
    syncTimer += syncDelay;
  }
  update();

  return kaleidoscope::EventHandlerResult::OK;
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

      ::Focus.readColor(c);

      setCrgbAt(idx, c);
    }
    break;
  }
  case SETALL: {
    cRGB c;

    ::Focus.readColor(c);

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
      prev_mode();
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

      ::Focus.readColor(color);

      setCrgbAt(idx, color);
      idx++;
    }
    break;
  }
  }

  return true;
}

// Legacy V1 API
#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
void LEDControl::begin() {
  ::LEDControl.onSetup();
  Kaleidoscope.useEventHandlerHook(legacyEventHandler);
  Kaleidoscope.useLoopHook(legacyLoopHook);
}

Key LEDControl::legacyEventHandler(Key mapped_key, byte row, byte col, uint8_t key_state) {
  EventHandlerResult r = ::LEDControl.onKeyswitchEvent(mapped_key, row, col, key_state);
  if (r == EventHandlerResult::OK)
    return mapped_key;
  return Key_NoKey;
}

void LEDControl::legacyLoopHook(bool is_post_clear) {
  if (is_post_clear)
    return;
  ::LEDControl.beforeReportingState();
}
#endif


}

kaleidoscope::LEDControl LEDControl;
