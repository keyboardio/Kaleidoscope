/* -*- mode: c++ -*-
 * Copyright (C) 2021  Keyboard.io, Inc.
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

#include "Kaleidoscope-Ranges/src/Kaleidoscope-Ranges.h"

SETUP_GOOGLETEST();

namespace kaleidoscope {
namespace testing {

class Issue1010 : public ::testing::Test {
 public:
  static constexpr uint8_t MAX_MACROS         = 255;
  static constexpr uint8_t MAX_OSM_KEYS       = 7;
  static constexpr uint8_t MAX_OSL_KEYS       = 7;
  static constexpr uint16_t MAX_DUM_KEYS      = 2048;
  static constexpr uint16_t MAX_DUL_KEYS      = 2048;
  static constexpr uint8_t MAX_TD_KEYS        = 15;
  static constexpr uint8_t MAX_LEAD_KEYS      = 7;
  static constexpr uint8_t MAX_TT_KEYS        = 255;
  static constexpr uint8_t MAX_STENO_KEYS     = 42;
  static constexpr uint8_t MAX_DYNAMIC_MACROS = 31;
  static constexpr uint8_t MAX_CS_KEYS        = 64;
  static constexpr uint8_t MAX_QK_KEYS        = 64;

  enum : uint16_t {
    MACRO_FIRST = (SYNTHETIC | 0b00100000) << 8,
    MACRO_LAST  = MACRO_FIRST + MAX_MACROS,

    FIRST              = 0xc000,
    KALEIDOSCOPE_FIRST = FIRST,
    OS_FIRST,
    OSM_FIRST = OS_FIRST,
    OSM_LAST  = OSM_FIRST + MAX_OSM_KEYS,
    OSL_FIRST,
    OSL_LAST = OSL_FIRST + MAX_OSL_KEYS,
    OS_LAST  = OSL_LAST,
    DU_FIRST,
    DUM_FIRST = DU_FIRST,
    DUM_LAST  = DUM_FIRST + MAX_DUM_KEYS,
    DUL_FIRST,
    DUL_LAST = DUL_FIRST + MAX_DUL_KEYS,
    DU_LAST  = DUL_LAST,
    TD_FIRST,
    TD_LAST = TD_FIRST + MAX_TD_KEYS,
    LEAD_FIRST,
    LEAD_LAST = LEAD_FIRST + MAX_LEAD_KEYS,
    CYCLE,
    SYSTER,
    TT_FIRST,
    TT_LAST = TT_FIRST + MAX_TT_KEYS,
    STENO_FIRST,
    STENO_LAST = STENO_FIRST + MAX_STENO_KEYS,
    SC_FIRST,
    SC_LAST,
    REDIAL,
    TURBO,
    DYNAMIC_MACRO_FIRST,
    DYNAMIC_MACRO_LAST = DYNAMIC_MACRO_FIRST + MAX_DYNAMIC_MACROS,
    OS_META_STICKY,
    OS_ACTIVE_STICKY,
    OS_CANCEL,
    CS_FIRST,
    CS_LAST = CS_FIRST + MAX_CS_KEYS,
    QK_FIRST,
    QK_LAST = QK_FIRST + MAX_QK_KEYS,

    SAFE_START,
    KALEIDOSCOPE_SAFE_START = SAFE_START
  };
};


TEST_F(Issue1010, RangesHaveNotChanged) {
  ASSERT_EQ(uint16_t(Issue1010::MACRO_FIRST),
            uint16_t(kaleidoscope::ranges::MACRO_FIRST));
  ASSERT_EQ(uint16_t(Issue1010::MACRO_LAST),
            uint16_t(kaleidoscope::ranges::MACRO_LAST));
  ASSERT_EQ(uint16_t(Issue1010::FIRST),
            uint16_t(kaleidoscope::ranges::FIRST));
  ASSERT_EQ(uint16_t(Issue1010::KALEIDOSCOPE_FIRST),
            uint16_t(kaleidoscope::ranges::KALEIDOSCOPE_FIRST));
  ASSERT_EQ(uint16_t(Issue1010::OS_FIRST),
            uint16_t(kaleidoscope::ranges::OS_FIRST));
  ASSERT_EQ(uint16_t(Issue1010::OSM_FIRST),
            uint16_t(kaleidoscope::ranges::OSM_FIRST));
  ASSERT_EQ(uint16_t(Issue1010::OSM_LAST),
            uint16_t(kaleidoscope::ranges::OSM_LAST));
  ASSERT_EQ(uint16_t(Issue1010::OSL_FIRST),
            uint16_t(kaleidoscope::ranges::OSL_FIRST));
  ASSERT_EQ(uint16_t(Issue1010::OSL_LAST),
            uint16_t(kaleidoscope::ranges::OSL_LAST));
  ASSERT_EQ(uint16_t(Issue1010::OS_LAST),
            uint16_t(kaleidoscope::ranges::OS_LAST));
  ASSERT_EQ(uint16_t(Issue1010::DU_FIRST),
            uint16_t(kaleidoscope::ranges::DU_FIRST));
  ASSERT_EQ(uint16_t(Issue1010::DUM_FIRST),
            uint16_t(kaleidoscope::ranges::DUM_FIRST));
  ASSERT_EQ(uint16_t(Issue1010::DUM_LAST),
            uint16_t(kaleidoscope::ranges::DUM_LAST));
  ASSERT_EQ(uint16_t(Issue1010::DUL_FIRST),
            uint16_t(kaleidoscope::ranges::DUL_FIRST));
  ASSERT_EQ(uint16_t(Issue1010::DUL_LAST),
            uint16_t(kaleidoscope::ranges::DUL_LAST));
  ASSERT_EQ(uint16_t(Issue1010::DU_LAST),
            uint16_t(kaleidoscope::ranges::DU_LAST));
  ASSERT_EQ(uint16_t(Issue1010::TD_FIRST),
            uint16_t(kaleidoscope::ranges::TD_FIRST));
  ASSERT_EQ(uint16_t(Issue1010::TD_LAST),
            uint16_t(kaleidoscope::ranges::TD_LAST));
  ASSERT_EQ(uint16_t(Issue1010::LEAD_FIRST),
            uint16_t(kaleidoscope::ranges::LEAD_FIRST));
  ASSERT_EQ(uint16_t(Issue1010::LEAD_LAST),
            uint16_t(kaleidoscope::ranges::LEAD_LAST));
  ASSERT_EQ(uint16_t(Issue1010::CYCLE),
            uint16_t(kaleidoscope::ranges::CYCLE));
  ASSERT_EQ(uint16_t(Issue1010::SYSTER),
            uint16_t(kaleidoscope::ranges::SYSTER));
  ASSERT_EQ(uint16_t(Issue1010::TT_FIRST),
            uint16_t(kaleidoscope::ranges::TT_FIRST));
  ASSERT_EQ(uint16_t(Issue1010::TT_LAST),
            uint16_t(kaleidoscope::ranges::TT_LAST));
  ASSERT_EQ(uint16_t(Issue1010::STENO_FIRST),
            uint16_t(kaleidoscope::ranges::STENO_FIRST));
  ASSERT_EQ(uint16_t(Issue1010::STENO_LAST),
            uint16_t(kaleidoscope::ranges::STENO_LAST));
  ASSERT_EQ(uint16_t(Issue1010::SC_FIRST),
            uint16_t(kaleidoscope::ranges::SC_FIRST));
  ASSERT_EQ(uint16_t(Issue1010::SC_LAST),
            uint16_t(kaleidoscope::ranges::SC_LAST));
  ASSERT_EQ(uint16_t(Issue1010::REDIAL),
            uint16_t(kaleidoscope::ranges::REDIAL));
  ASSERT_EQ(uint16_t(Issue1010::TURBO),
            uint16_t(kaleidoscope::ranges::TURBO));
  ASSERT_EQ(uint16_t(Issue1010::DYNAMIC_MACRO_FIRST),
            uint16_t(kaleidoscope::ranges::DYNAMIC_MACRO_FIRST));
  ASSERT_EQ(uint16_t(Issue1010::DYNAMIC_MACRO_LAST),
            uint16_t(kaleidoscope::ranges::DYNAMIC_MACRO_LAST));
  ASSERT_EQ(uint16_t(Issue1010::OS_META_STICKY),
            uint16_t(kaleidoscope::ranges::OS_META_STICKY));
  ASSERT_EQ(uint16_t(Issue1010::OS_ACTIVE_STICKY),
            uint16_t(kaleidoscope::ranges::OS_ACTIVE_STICKY));
  ASSERT_EQ(uint16_t(Issue1010::OS_CANCEL),
            uint16_t(kaleidoscope::ranges::OS_CANCEL));
  ASSERT_EQ(uint16_t(Issue1010::CS_FIRST),
            uint16_t(kaleidoscope::ranges::CS_FIRST));
  ASSERT_EQ(uint16_t(Issue1010::CS_LAST),
            uint16_t(kaleidoscope::ranges::CS_LAST));
  ASSERT_EQ(uint16_t(Issue1010::QK_FIRST),
            uint16_t(kaleidoscope::ranges::QK_FIRST));
  ASSERT_EQ(uint16_t(Issue1010::QK_LAST),
            uint16_t(kaleidoscope::ranges::QK_LAST));

  ASSERT_EQ(uint16_t(Issue1010::SAFE_START),
            uint16_t(kaleidoscope::ranges::SAFE_START));
  ASSERT_EQ(uint16_t(Issue1010::KALEIDOSCOPE_SAFE_START),
            uint16_t(kaleidoscope::ranges::KALEIDOSCOPE_SAFE_START));
}

}  // namespace testing
}  // namespace kaleidoscope
