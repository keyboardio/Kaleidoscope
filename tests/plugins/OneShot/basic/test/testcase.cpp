/* -*- mode: c++ -*-
 * Copyright (C) 2020  Keyboard.io, Inc.
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

#include "testing/setup-googletest.h"

#include "../common.h"

SETUP_GOOGLETEST();

namespace kaleidoscope {
namespace testing {
namespace {

using namespace kaleidoscope::testing::keymap;

constexpr KeyAddr key_addr_OSM_LeftControl = addr_BC;
constexpr KeyAddr key_addr_OSM_LeftGui = addr_BB;
constexpr KeyAddr key_addr_OSM_LeftShift = addr_BA;
constexpr KeyAddr key_addr_OSL_1 = addr_CA;
constexpr KeyAddr key_addr_S = addr_AA;
constexpr KeyAddr key_addr_D = addr_AB;

class OneShotBasic : public VirtualDeviceTest {};

TEST_F(OneShotBasic, OneShotModifier) {

  // Tap `OSM(LeftShift)`
  PressKey(Millis{10}, key_addr_OSM_LeftShift);
  ExpectReport(Cycles{1}, AddKeycodes{Key_LeftShift},
               "Report should contain only `LeftShift`");
  ReleaseKey(Millis{20}, key_addr_OSM_LeftShift);

  // Tap `Key_S`
  PressKey(Millis{10}, key_addr_S);
  ExpectReport(Cycles{1}, AddKeycodes{Key_S},
               "Report should contain `LeftShift` & `S`");
  ExpectReport(Cycles{1}, RemoveKeycodes{Key_LeftShift},
               "Report should be contain only `S`");
  ReleaseKey(Millis{10}, key_addr_S);
  ExpectReport(Cycles{1}, RemoveKeycodes{Key_S},
               "Report should be empty");

  sim_.RunForMillis(10);


  CHECK_EXPECTED_REPORTS()
}

TEST_F(OneShotBasic, OsmTimeout) {

  ClearState();

  // Tap `OSM(LeftShift)`
  PressKey(Millis{10}, key_addr_OSM_LeftShift);
  ExpectReport(Cycles{1}, AddKeycodes{Key_LeftShift},
               "Report should contain only `LeftShift`");
  ReleaseKey(Millis{20}, key_addr_OSM_LeftShift);

  RunFrom(EventTimestamp(0), Millis{ONESHOT_TIMEOUT});
  ExpectReport(Cycles{3}, RemoveKeycodes{Key_LeftShift},
               "Report should be empty");

  CHECK_EXPECTED_REPORTS()
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
