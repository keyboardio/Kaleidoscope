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

class QukeysBasic : public VirtualDeviceTest {};

TEST_F(QukeysBasic, TapQukeyAlone) {

  PressKey(Millis{10}, key_addr_A);
  ReleaseKey(Millis{50}, key_addr_A);
  ExpectReport(Cycles{1}, AddKeycodes{Key_A}, "Report should contain only `A`");
  ExpectReport(Cycles{1}, RemoveKeycodes{Key_A}, "Report should be empty");

  Run(Cycles{3});
  CHECK_EXPECTED_REPORTS();
}

TEST_F(QukeysBasic, HoldQukeyAlone) {
  constexpr uint32_t hold_time = 400;

  PressKey(Millis{10}, key_addr_A);
  ExpectReport(Millis{QUKEYS_HOLD_TIMEOUT}, AddKeycodes{Key_LeftGui},
               "The first report should include only `LeftGui`");
  ReleaseKey(Millis{hold_time}, key_addr_A);
  ExpectReport(Cycles{1}, RemoveKeycodes{Key_LeftGui},
               "The second report should be empty");

  Run(Cycles{3});

  CHECK_EXPECTED_REPORTS();

  Millis measured_delay = ReportTimestamp(0) - input_timestamps_[0];
  EXPECT_THAT(measured_delay, ::testing::Ge(QUKEYS_HOLD_TIMEOUT))
      << "The HID report should be sent after the hold timeout has elapsed";

  EXPECT_THAT(ReportTimestamp(1), ::testing::Ge(EventTimestamp(1)))
      << "The key should stay active until release";

}

TEST_F(QukeysBasic, FullOverlap) {

  PressKey(Millis{10}, key_addr_F);
  PressKey(Millis{20}, key_addr_X);
  ReleaseKey(Millis{50}, key_addr_X);
  ExpectReport(Cycles{1}, AddKeycodes{Key_LeftShift},
               "The first report should contain only `LeftShift`");
  ExpectReport(Cycles{1}, AddKeycodes{Key_X},
               "The second report should add `X`");
  ExpectReport(Cycles{1}, RemoveKeycodes{Key_X},
               "The third report should remove `X`");
  ReleaseKey(Millis{10}, key_addr_F);
  ExpectReport(Cycles{1}, RemoveKeycodes{Key_LeftShift},
               "The fourth report should remove `LeftShift`");

  Run(Cycles{3});
  CHECK_EXPECTED_REPORTS();
}

TEST_F(QukeysBasic, RolloverPrimary) {

  PressKey(Millis{10}, key_addr_F);
  PressKey(Millis{40}, key_addr_X);
  ReleaseKey(Millis{20}, key_addr_F);
  ExpectReport(Millis{30}, AddKeycodes{Key_F},
               "The first report should add `F`");
  ExpectReport(Cycles{1}, AddKeycodes{Key_X},
               "The second report should add `X`");
  ExpectReport(Cycles{1}, RemoveKeycodes{Key_F},
               "The third report should remove `F`");
  ReleaseKey(Millis{40}, key_addr_X);
  ExpectReport(Cycles{1}, RemoveKeycodes{Key_X},
               "The fourth report should remove `X`");

  Run(Cycles{3});
  CHECK_EXPECTED_REPORTS();
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
