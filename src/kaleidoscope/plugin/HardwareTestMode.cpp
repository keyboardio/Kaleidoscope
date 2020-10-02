/* Kaleidoscope-HardwareTestMode - A factory test mode for the Model 01.
 * Copyright (C) 2017-2019  Keyboard.io, Inc.
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

#include "kaleidoscope/Runtime.h"
#include "Kaleidoscope-HardwareTestMode.h"
#include "Kaleidoscope-LEDEffect-Rainbow.h"

namespace kaleidoscope {
namespace plugin {


constexpr uint8_t CHATTER_CYCLE_LIMIT = 30;

uint8_t HardwareTestMode::actionKey;

void HardwareTestMode::setActionKey(uint8_t key) {
  actionKey = key;
}

void HardwareTestMode::waitForKeypress() {
  while (1) {
    Runtime.device().readMatrix();
    if (Runtime.device().isKeyswitchPressed(actionKey) &&
        ! Runtime.device().wasKeyswitchPressed(actionKey)) {
      break;
    }
  }
}

void HardwareTestMode::setLeds(cRGB color) {
  ::LEDControl.set_all_leds_to(color);
  ::LEDControl.syncLeds();
  waitForKeypress();
}

void HardwareTestMode::testLeds(void) {
  constexpr cRGB red = CRGB(201, 0, 0);
  constexpr cRGB blue = CRGB(0, 0, 201);
  constexpr cRGB green = CRGB(0, 201, 0);
  constexpr cRGB brightWhite = CRGB(160, 160, 160);

  setLeds(brightWhite);
  setLeds(blue);
  setLeds(green);
  setLeds(red);

  // This works under the assumption that LEDRainbowEffect
  // has been registered with KALEIDOSCOPE_INIT_PLUGINS in
  // the sketch. Otherwise LEDRainbowEffect would not be
  // known to LEDControl.
  //
  ::LEDControl.activate(&::LEDRainbowEffect);

  // rainbow for 10 seconds
  ::LEDRainbowEffect.update_delay(5);
  for (auto i = 0; i < 300; i++) {
    ::LEDControl.update();
    ::LEDControl.syncLeds();
  }
}



void HardwareTestMode::testMatrix() {
  // Reset bad keys from previous tests.
  chatter_data state[Runtime.device().numKeys()] = {{0, 0, 0}};

  constexpr cRGB red = CRGB(201, 0, 0);
  constexpr cRGB blue = CRGB(0, 0, 201);
  constexpr cRGB green = CRGB(0, 201, 0);
  constexpr cRGB yellow = CRGB(201, 100, 0);

  while (1) {
    Runtime.device().readMatrix();
    for (auto key_addr : KeyAddr::all()) {
      uint8_t keynum = key_addr.toInt();

      // If the key is toggled on
      if (Runtime.device().isKeyswitchPressed(key_addr) && ! Runtime.device().wasKeyswitchPressed(key_addr)) {
        // And it's too soon (in terms of cycles between changes)
        state[keynum].tested = 1;
        if (state[keynum].cyclesSinceStateChange < CHATTER_CYCLE_LIMIT) {
          state[keynum].bad = 1;
        }
        state[keynum].cyclesSinceStateChange = 0;
      } else if (state[keynum].cyclesSinceStateChange < CHATTER_CYCLE_LIMIT) {
        state[keynum].cyclesSinceStateChange++;
      }
      // If the key is held down
      if (Runtime.device().isKeyswitchPressed(key_addr) && Runtime.device().wasKeyswitchPressed(key_addr)) {
        Runtime.device().setCrgbAt(key_addr, green);
      } else if (state[keynum].bad == 1) {
        // If we triggered chatter detection ever on this key
        Runtime.device().setCrgbAt(key_addr, red);
      } else if (state[keynum].tested == 0) {
        Runtime.device().setCrgbAt(key_addr, yellow);
      } else if (! Runtime.device().isKeyswitchPressed(key_addr)) {
        // If the key is not currently pressed and was not just released and is not marked bad
        Runtime.device().setCrgbAt(key_addr, blue);
      }
    }
    ::LEDControl.syncLeds();
  }
}

void HardwareTestMode::runTests() {
  // When we start test mode, we -may- have some keys held, so empty it
  // out and send a new report
  kaleidoscope::Runtime.hid().keyboard().releaseAllKeys();
  kaleidoscope::Runtime.hid().keyboard().sendReport();
  Runtime.device().enableHardwareTestMode();
  testLeds();
  testMatrix();
}

}
}

kaleidoscope::plugin::HardwareTestMode HardwareTestMode;
