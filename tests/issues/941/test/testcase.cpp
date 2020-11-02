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

SETUP_GOOGLETEST();

namespace kaleidoscope {
namespace testing {
namespace {

constexpr KeyAddr key_addr_A{2, 1};
constexpr KeyAddr key_addr_S{2, 2};
constexpr KeyAddr key_addr_D{2, 3};

class Issue941 : public VirtualDeviceTest {};

TEST_F(Issue941, OneKeypressPerCycle) {

  PressKey(Millis{10}, key_addr_A);
  ExpectReport(Cycles{1}, AddKeycodes{Key_A}, "Report should contain only `A`");
  PressKey(key_addr_S);
  ExpectReport(Cycles{1}, AddKeycodes{Key_S}, "Report should contain `S`");
  ReleaseKey(Millis{25}, key_addr_A);
  ExpectReport(Cycles{1}, RemoveKeycodes{Key_A}, "Report should contain only `S`");
  ReleaseKey(key_addr_S);
  ExpectReport(Cycles{1}, RemoveKeycodes{Key_S}, "Report should be empty");

  sim_.RunForMillis(10);
  LoadState();

  constexpr int expected_report_count = 4;
  ASSERT_EQ(HIDReports()->Keyboard().size(), expected_report_count)
      << "There should be " << expected_report_count << " HID reports";

  for (int i = 0; i < expected_report_count; ++i) {
    EXPECT_THAT(HIDReports()->Keyboard(i).ActiveKeycodes(),
                ::testing::ElementsAreArray(expected_reports_[i].Keycodes()))
        << expected_reports_[i].Message();
  }
}


TEST_F(Issue941, SimultaneousKeypresses) {

  // Press three keys in one scan cycle:
  PressKey(Millis{10}, key_addr_A);
  PressKey(key_addr_S);
  PressKey(key_addr_D);
  // This test is expected to fail when Kaleidoscope becomes event-driven;
  // instead, there will be three reports here: the first will contain `D`, the
  // second will add `S`, and the third will add `A` (I could have that wrong;
  // it should be in keyscan order).
  ExpectReport(Cycles{1}, AddKeycodes{Key_A, Key_S},
               "Report should contain `A` and `S`");
  ExpectReport(AddKeycodes{Key_D}, "Report should contain `A`, `S`, & `D`");

  // Release all three in one scan cycle:
  ReleaseKey(Millis{25}, key_addr_A);
  ReleaseKey(key_addr_S);
  ReleaseKey(key_addr_D);
  ExpectReport(Cycles{1}, RemoveKeycodes{Key_A, Key_S, Key_D},
               "Report should be empty");

  sim_.RunForMillis(10);
  LoadState();

  // The total number of reports could change
  constexpr int expected_report_count = 3;
  ASSERT_EQ(HIDReports()->Keyboard().size(), expected_report_count)
      << "There should be " << expected_report_count << " HID reports";

  for (int i = 0; i < expected_report_count; ++i) {
    EXPECT_THAT(HIDReports()->Keyboard(i).ActiveKeycodes(),
                ::testing::ElementsAreArray(expected_reports_[i].Keycodes()))
        << expected_reports_[i].Message();
  }
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
