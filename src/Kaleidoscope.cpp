#include "Kaleidoscope.h"
#include <stdarg.h>

namespace kaleidoscope {

uint32_t Kaleidoscope_::millis_at_cycle_start_;

Kaleidoscope_::Kaleidoscope_(void) {
}

void
Kaleidoscope_::setup(void) {
  kaleidoscope::Hooks::onSetup();

  KeyboardHardware.setup();

  kaleidoscope::hid::initializeKeyboard();
  kaleidoscope::hid::initializeConsumerControl();
  kaleidoscope::hid::initializeSystemControl();

  // A workaround, so that the compiler does not optimize handleKeyswitchEvent out...
  // This is a no-op, but tricks the compiler into not being too helpful
  // TODO(anyone): figure out how to hint the compiler in a more reasonable way
  handleKeyswitchEvent(Key_NoKey, 255, 255, 0);

  // Update the keymap cache, so we start with a non-empty state.
  Layer.updateActiveLayers();
  for (byte row = 0; row < ROWS; row++) {
    for (byte col = 0; col < COLS; col++) {
      Layer.updateLiveCompositeKeymap(row, col);
    }
  }
}

void
Kaleidoscope_::loop(void) {
  millis_at_cycle_start_ = millis();

  kaleidoscope::Hooks::beforeEachCycle();

  KeyboardHardware.scanMatrix();

  kaleidoscope::Hooks::beforeReportingState();

  kaleidoscope::hid::sendKeyboardReport();
  kaleidoscope::hid::releaseAllKeys();

  kaleidoscope::Hooks::afterEachCycle();
}

bool
Kaleidoscope_::focusHook(const char *command) {
  enum {
    ON,
    OFF,
    GETSTATE,
  } subCommand;

  if (strncmp_P(command, PSTR("layer."), 6) != 0)
    return false;

  if (strcmp_P(command + 6, PSTR("on")) == 0)
    subCommand = ON;
  else if (strcmp_P(command + 6, PSTR("off")) == 0)
    subCommand = OFF;
  else if (strcmp_P(command + 6, PSTR("getState")) == 0)
    subCommand = GETSTATE;
  else
    return false;

  switch (subCommand) {
  case ON: {
    uint8_t layer = Serial.parseInt();
    Layer.on(layer);
    break;
  }

  case OFF: {
    uint8_t layer = Serial.parseInt();
    Layer.off(layer);
    break;
  }

  case GETSTATE:
    Serial.println(Layer.getLayerState(), BIN);
    break;
  }

  return true;
}

Kaleidoscope_ Kaleidoscope;

} // namespace kaleidoscope
