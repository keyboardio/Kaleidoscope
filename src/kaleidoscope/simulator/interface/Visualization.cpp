/* -*- mode: c++ -*-
 * kaleidoscope::simulator - Testing API for Kaleidoscope
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

#include "kaleidoscope/simulator/interface/Visualization.h"
#include "kaleidoscope/simulator/interface/Simulator_.h"
#include "kaleidoscope/simulator/interface/SimulatorCore_.h"
#include "kaleidoscope/simulator/interface/aux/terminal_escape_sequences.h"

// Undef those stupid Arduino-macros conflicting with stl stuff
//
#undef min
#undef max

#include <map>
#include <sstream>
#include <regex>
#include <iostream>

namespace kaleidoscope {
namespace simulator {
namespace interface {

typedef std::map<uint8_t, std::string> KeyAddrToKeyString;

class KeyboardRenderer {
 public:

  KeyboardRenderer(const SimulatorCore_ &simulator_core)
    :  simulator_core_(simulator_core)
  {}

  std::string generateColorEscSeq(uint8_t row, uint8_t col) {

    using namespace terminal_escape_sequences;

    uint8_t red = 0, green = 0, blue = 0;

    uint8_t rows, cols;
    simulator_core_.getKeyMatrixDimensions(rows, cols);

    uint8_t key_offset = row * cols + col;

    simulator_core_.getCurrentKeyLEDColor(key_offset, red, green, blue);

    int col_norm = red * red + green * green + blue * blue;

    int foreground_color = 30;

    // Have dark grey text on light background color and light grey
    // on dark background.
    //
    if (col_norm <= 49152) {
      foreground_color = 37;
    }

    static const char * const empty = "";
    const char *activation_highlight = empty;
    if (simulator_core_.isKeyPressed(row, col)) {
      activation_highlight = underlined;
    }

    std::ostringstream o;
    o << "\x1B[48;2;" << (int)red << ";" << (int)green << ";" << (int)blue << "m"
      "\x1B[" << foreground_color << "m"
      << activation_highlight;
    return o.str();
  }

  void generateLookup(KeyAddrToKeyString &lookup) {

    using namespace terminal_escape_sequences;

    uint8_t rows = 0, cols = 0;

    simulator_core_.getKeyMatrixDimensions(rows, cols);

    for (uint8_t row = 0; row < rows; ++row) {
      for (uint8_t col = 0; col < cols; ++col) {

        std::string key_string("****");
        simulator_core_.getCurrentKeyLabel(row, col, key_string);

        std::string color_string = this->generateColorEscSeq(row, col);

        uint8_t pos = row * cols + col;

        //std::cout << (int)pos << ": color: \"" << color_string << "\", key_string: \"" << key_string << "\"\n";

        lookup[pos] = color_string + key_string + reset_formatting;
      }
    }
  }

 private:

  const SimulatorCore_ &simulator_core_;
};

void renderKeyboard(const Simulator_ &simulator, const char *ascii_keyboard) {

  KeyAddrToKeyString lookup;
  KeyboardRenderer kr{simulator.getCore()};
  kr.generateLookup(lookup);

  std::istringstream ss(ascii_keyboard);
  std::string line;

  std::regex re("\\{\\d+\\}");
  while (std::getline(ss, line, '\n')) {

    std::string output_text;

    auto callback = [&](std::string const & m) {
      if (m[0] == '{') {
        std::istringstream iss(m);
        int n;
        iss.ignore(1);
        iss >> n;
        output_text += lookup[uint8_t(n)];
      } else {
        output_text += m;
      }
    };

    //std::cout << "Line: " << line << std::endl;

    std::sregex_token_iterator
    begin(line.begin(), line.end(), re, {-1, 0}),
          end;
    std::for_each(begin, end, callback);

    simulator.log() << output_text;
  }
}

} // namespace interface
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
