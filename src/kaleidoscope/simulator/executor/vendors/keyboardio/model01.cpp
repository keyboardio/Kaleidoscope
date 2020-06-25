/* -*- mode: c++ -*-
 * kaleidoscope::simulator::executor -- A C++ testing API for the Kaleidoscope keyboard firmware.
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

#include "kaleidoscope/simulator/executor/vendors/keyboardio/model01.h"

namespace kaleidoscope {
namespace simulator {
namespace keyboardio {
namespace model01 {
const char *ascii_keyboard =
  "┏━━━━┳━━━━┳━━━━┳━━━━┳━━━━┳━━━━┳━━━━┓        ┏━━━━┳━━━━┳━━━━┳━━━━┳━━━━┳━━━━┳━━━━┓\n"
  "┃{00}┃{01}┃{02}┃{03}┃{04}┃{05}┃{06}┃        ┃{09}┃{10}┃{11}┃{12}┃{13}┃{14}┃{15}┃\n"
  "┣━━━━╋━━━━╋━━━━╋━━━━╋━━━━╋━━━━┫    ┃        ┃    ┣━━━━╋━━━━╋━━━━╋━━━━╋━━━━╋━━━━┫\n"
  "┃{16}┃{17}┃{18}┃{19}┃{20}┃{21}┣━━━━┫        ┣━━━━┫{26}┃{27}┃{28}┃{29}┃{30}┃{31}┃\n"
  "┣━━━━╋━━━━╋━━━━╋━━━━╋━━━━╋━━━━┫{22}┃        ┃{25}┣━━━━╋━━━━╋━━━━╋━━━━╋━━━━╋━━━━┫\n"
  "┃{32}┃{33}┃{34}┃{35}┃{36}┃{37}┃    ┃        ┃    ┃{42}┃{43}┃{44}┃{45}┃{46}┃{47}┃\n"
  "┣━━━━╋━━━━╋━━━━╋━━━━╋━━━━╋━━━━╋━━━━┫        ┣━━━━╋━━━━╋━━━━╋━━━━╋━━━━╋━━━━╋━━━━┫\n"
  "┃{48}┃{49}┃{50}┃{51}┃{52}┃{53}┃{38}┃        ┃{41}┃{58}┃{59}┃{60}┃{61}┃{62}┃{63}┃\n"
  "┗━━━━┻━━━━┻━━━━┻━━━━┻━━━━┻━━━━┻━━━━┛        ┗━━━━┻━━━━┻━━━━┻━━━━┻━━━━┻━━━━┻━━━━┛\n"
  "                ┏━━━━┓                                    ┏━━━━┓                \n"
  "                ┃{07}┣━━━━┓                          ┏━━━━┫{08}┃                \n"
  "                ┗━━━━┫{23}┣━━━━┓                ┏━━━━┫{24}┣━━━━┛                \n"
  "                     ┗━━━━┫{39}┣━━━━┓      ┏━━━━┫{40}┣━━━━┛                     \n"
  "                          ┗━━━━┫{55}┃      ┃{56}┣━━━━┛                          \n"
  "                               ┗━━━━┛      ┗━━━━┛                               \n"
  "                    ┏━━━━━━┓                        ┏━━━━━━┓                    \n"
  "                    ┃ {54} ┃                        ┃ {57} ┃                    \n"
  "                    ┗━━━━━━┛                        ┗━━━━━━┛                    \n";
} // namespace model01
} // namespace keyboardio
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
