/* -*- mode: c++ -*-
 * kaleidoscope::simulator::framework - A keyboard simulation framework
 * Copyright (C) 2019  noseglasses (shinynoseglasses@gmail.com)
 * Copyright (C) 2020  Keyboard.io, Inc
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

#ifdef KALEIDOSCOPE_VIRTUAL_BUILD

#include "kaleidoscope/simulator/framework/LED_Checks.h"
#include "kaleidoscope/simulator/framework/Simulator.h"
#include "kaleidoscope/simulator/framework/SimulatorCore_.h"

#include <iostream>

namespace kaleidoscope {
namespace simulator {
namespace framework {

void dumpKeyLEDState(const Simulator &simulator) {

  std::cout << "const uint8_t key_led_colors[][3] = {\n";

  const auto &simulator_core = simulator.getCore();

  uint8_t rows = 0, cols = 0;
  simulator_core.getKeyMatrixDimensions(rows, cols);

  for (uint8_t row = 0; row < rows; ++row) {
    for (uint8_t col = 0; col < cols; ++col) {

      uint8_t red = 0, green = 0, blue = 0;

      uint8_t key_offset = row * cols + col;

      simulator_core.getCurrentKeyLEDColor(key_offset,
                                           red, green, blue);

      std::cout << "   {" << (int)red << ", "
                << (int)green << ", "
                << (int)blue << "},\n";
    }
  }

  std::cout << "};\n\n";
}

void assertKeyLEDState(const Simulator &simulator,
                       const uint8_t key_led_colors[][3]) {
  const auto &simulator_core = simulator.getCore();

  uint8_t num_leds = simulator_core.getNumLEDs();

  uint8_t rows = 0, cols = 0;
  simulator_core.getKeyMatrixDimensions(rows, cols);

  for (uint8_t row = 0; row < rows; ++row) {
    for (uint8_t col = 0; col < cols; ++col) {

      uint8_t key_offset = row * cols + col;

      const uint8_t &red_expected = key_led_colors[key_offset][0];
      const uint8_t &green_expected = key_led_colors[key_offset][1];
      const uint8_t &blue_expected = key_led_colors[key_offset][2];

      uint8_t red_actual = 0, green_actual = 0, blue_actual = 0;

      simulator_core.getCurrentKeyLEDColor(key_offset,
                                           red_actual, green_actual, blue_actual);

      if ((red_expected != red_actual)
          || (green_expected != green_actual)
          || (blue_expected != blue_actual)) {

        simulator.error() << "LED color mismatch at key LED " << (int)key_offset;

        simulator.error() << "   expected: (" << (int)red_expected << ", "
                          << (int)green_expected << ", "
                          << (int)blue_expected << ")";

        simulator.error() << "   actual: ("   << (int)red_actual << ", "
                          << (int)green_actual << ", "
                          << (int)blue_actual << ")";
      }
    }
  }
}

} // namespace framework
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
