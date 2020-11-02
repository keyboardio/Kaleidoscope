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
constexpr KeyAddr key_addr_LeftShift{3, 7};

class Keycodes : public VirtualDeviceTest {};

TEST_F(Keycodes, KeyboardNonModifier) {

  PressKey(Millis{10}, key_addr_A);
  ExpectReport(Cycles{1}, AddKeycodes{Key_A}, "Report should contain only `A`");
  ReleaseKey(Millis{25}, key_addr_A);
  ExpectReport(Cycles{1}, RemoveKeycodes{Key_A}, "Report should be empty");

  sim_.RunForMillis(10);
  LoadState();

  constexpr int expected_report_count = 2;
  ASSERT_EQ(HIDReports()->Keyboard().size(), expected_report_count)
      << "There should be " << expected_report_count << " HID reports";

  for (int i = 0; i < expected_report_count; ++i) {
    EXPECT_THAT(HIDReports()->Keyboard(i).ActiveKeycodes(),
                ::testing::ElementsAreArray(expected_reports_[i].Keycodes()))
        << expected_reports_[i].Message();
  }
}

TEST_F(Keycodes, KeyboardModifier) {

  PressKey(Millis{10}, key_addr_LeftShift);
  ExpectReport(Cycles{1}, AddKeycodes{Key_LeftShift},
               "Report should contain only `shift`");
  ReleaseKey(Millis{25}, key_addr_LeftShift);
  ExpectReport(Cycles{1}, RemoveKeycodes{Key_LeftShift},
               "Report should be empty");

  sim_.RunForMillis(10);
  LoadState();

  constexpr int expected_report_count = 2;
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
