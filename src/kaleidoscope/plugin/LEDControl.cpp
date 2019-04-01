/* Kaleidoscope-LEDControl - LED control plugin for Kaleidoscope
 * Copyright (C) 2017-2018  Keyboard.io, Inc.
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

#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-FocusSerial.h"

namespace kaleidoscope {
namespace plugin {

LEDMode *LEDControl::modes[LED_MAX_MODES];
uint8_t LEDControl::mode;
uint16_t LEDControl::syncDelay = 32; // 32ms interval => 30Hz refresh rate
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
  if (!Kaleidoscope.has_leds)
    return;

  cRGB color;
  color.r = r;
  color.g = g;
  color.b = b;
  set_all_leds_to(color);
}

void LEDControl::set_all_leds_to(cRGB color) {
  for (auto led_addr : LEDAddr{}) {
    setCrgbAt(led_addr, color);
  }
}

void LEDControl::setCrgbAt(int8_t i, cRGB crgb) {
  KeyboardHardware.setCrgbAt(i, crgb);
}

void LEDControl::setCrgbAt(LEDAddr led_addr, cRGB color) {
  KeyboardHardware.setCrgbAt(led_addr, color);
}

cRGB LEDControl::getCrgbAt(int8_t i) {
  return KeyboardHardware.getCrgbAt(i);
}
cRGB LEDControl::getCrgbAt(LEDAddr led_addr) {
  return KeyboardHardware.getCrgbAt(KeyboardHardware.getLedIndex(led_addr));
}

void LEDControl::syncLeds(void) {
  if (paused)
    return;

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

kaleidoscope::EventHandlerResult LEDControl::onKeyswitchEvent2(Key &mappedKey, KeyAddr key_addr, uint8_t keyState) {
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

  // unsigned subtraction means that as syncTimer rolls over
  // the same interval is kept
  uint16_t elapsed = Kaleidoscope.millisAtCycleStart() - syncTimer;
  // on some platforms, the subtraction in the comparison results in a signed
  // operation, resulting in syncLeds() no longer getting called.
  if (elapsed > syncDelay) {
    syncLeds();
    syncTimer += syncDelay;
    update();
  }

  return kaleidoscope::EventHandlerResult::OK;
}

EventHandlerResult FocusLEDCommand::onFocusEvent(const char *command) {
  enum {
    SETALL,
    MODE,
    AT,
    THEME,
  } subCommand;

  if (!Kaleidoscope.has_leds)
    return EventHandlerResult::OK;

  if (::Focus.handleHelp(command, PSTR("led.at\n"
                                       "led.setAll\n"
                                       "led.mode\n"
                                       "led.theme")))
    return EventHandlerResult::OK;

  if (strncmp_P(command, PSTR("led."), 4) != 0)
    return EventHandlerResult::OK;
  if (strcmp_P(command + 4, PSTR("at")) == 0)
    subCommand = AT;
  else if (strcmp_P(command + 4, PSTR("setAll")) == 0)
    subCommand = SETALL;
  else if (strcmp_P(command + 4, PSTR("mode")) == 0)
    subCommand = MODE;
  else if (strcmp_P(command + 4, PSTR("theme")) == 0)
    subCommand = THEME;
  else
    return EventHandlerResult::OK;

  switch (subCommand) {
  case AT: {
    uint8_t idx;

    ::Focus.read(idx);

    if (::Focus.isEOL()) {
      cRGB c = ::LEDControl.getCrgbAt(idx);

      ::Focus.send(c);
    } else {
      cRGB c;

      ::Focus.read(c);

      ::LEDControl.setCrgbAt(idx, c);
    }
    break;
  }
  case SETALL: {
    cRGB c;

    ::Focus.read(c);

    ::LEDControl.set_all_leds_to(c);

    break;
  }
  case MODE: {
    char peek = ::Focus.peek();
    if (peek == '\n') {
      ::Focus.send(::LEDControl.get_mode_index());
    } else if (peek == 'n') {
      ::LEDControl.next_mode();
    } else if (peek == 'p') {
      ::LEDControl.prev_mode();
    } else {
      uint8_t mode;

      ::Focus.read(mode);
      ::LEDControl.set_mode(mode);
    }
    break;
  }
  case THEME: {
    if (::Focus.isEOL()) {
      for (auto led_addr : LEDAddr{}) {
        cRGB c = ::LEDControl.getCrgbAt(led_addr);

        ::Focus.send(c);
      }
      break;
    }

    for (auto led_addr : LEDAddr{}) {
      if (::Focus.isEOL()) {
        break;
      }

      cRGB color;

      ::Focus.read(color);

      ::LEDControl.setCrgbAt(led_addr, color);
    }
    break;
  }
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

}
}

kaleidoscope::plugin::LEDControl LEDControl;
kaleidoscope::plugin::FocusLEDCommand FocusLEDCommand;
