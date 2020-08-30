/* -*- mode: c++ -*-
 * Kaleidoscope-Simulator -- A C++ testing API for the Kaleidoscope keyboard
 *                         firmware.
 * Copyright (C) 2019  noseglasses (shinynoseglasses@gmail.com)
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

#include "Kaleidoscope-Simulator.h"
#include "kaleidoscope/simulator/vendors/keyboardio/model01.h"

KALEIDOSCOPE_SIMULATOR_INIT

namespace kaleidoscope {
namespace simulator {

void runSimulator(Simulator &simulator) {

  using namespace actions;
  using namespace interface;
  using namespace interface::actions;

//    std::shared_ptr<framework::Action_> test = AssertKeycodesActive{Key_A};
//    std::shared_ptr<framework::ReportAction<framework::KeyboardReport_>> test = AssertKeycodesActive{Key_A};
//    std::shared_ptr<AssertKeycodesActive::Action> test = AssertKeycodesActive{Key_A};
//    std::shared_ptr<framework::ReportAction<framework::KeyboardReport_>> test2 = test;
//    decltype(AssertKeycodesActive{Key_A}.operator std::shared_ptr<framework::actions::AssertKeycodesActive::Action> ()) test = AssertKeycodesActive{Key_A};
//    std::shared_ptr<framework::ReportAction<framework::KeyboardReport_>> test = AssertKeycodesActive{Key_A}.operator std::shared_ptr<framework::actions::AssertKeycodesActive::Action> ();

  //simulator.permanentKeyboardReportActions().add(DumpReport{});

  //***************************************************************************
  {
    auto test = simulator.newTest("0");

    // Assert that the next cycle generates exactly one keyboard report.
    //
    simulator.cycleActionsQueue().queue(AssertCycleGeneratesNReports<KeyboardReport_> {1});

    simulator.tapKey(2, 1); // A
    simulator.cycleExpectReports(AssertKeycodesActive{Key_A});

    simulator.cycleExpectReports(AssertReportEmpty{});
  }

  //***************************************************************************
  {
    auto test = simulator.newTest("1");

    simulator.pressKey(2, 1); // A
    simulator.cycleExpectReports(AssertKeycodesActive{Key_A});

    simulator.reportActionsQueue().queue(
      group(
        /*AssertKeycodesActive{Key_A},
        AssertKeycodesActive{Key_B},
        AssertReportEmpty{}.negate(),
        AssertAnyModifierActive{}.negate(),
        AssertAnyKeycodeActive{},
        AssertReportIsNthInCycle{1},*/
        DumpReport{}
      )
    );
    simulator.pressKey(3, 5); // B
    simulator.cycle();
    simulator.releaseKey(2, 1); // A
    simulator.releaseKey(3, 5); // B

    simulator.cycles(5);
  }

  //***************************************************************************
  {
    auto test = simulator.newTest("2");

    simulator.pressKey(2, 1); // A
    simulator.cycleExpectReports(AssertKeycodesActive{Key_A});

    simulator.pressKey(3, 5); // B
    simulator.cycleExpectReports(
      group(
        AssertKeycodesActive{Key_A},
        AssertKeycodesActive{Key_B}
      )
    );

    simulator.releaseKey(2, 1);
    simulator.releaseKey(3, 5);

    simulator.cycles(5);
  }

  //***************************************************************************
  {
    auto test = simulator.newTest("3");

    simulator.pressKey(2, 1); // A
    simulator.cycle();
    simulator.pressKey(3, 5); // B
    simulator.cycleExpectReports(AssertKeycodesActive{Key_A, Key_B});

    simulator.releaseKey(2, 1);
    simulator.releaseKey(3, 5);
    simulator.cycles(5);
  }

  //***************************************************************************
  {
    auto test = simulator.newTest("4");

    auto layer_check = AssertTopActiveLayerIs{0};
    simulator.permanentCycleActions().add(layer_check);
    simulator.cycle();
    simulator.permanentCycleActions().remove(layer_check);
  }

  //***************************************************************************
  {
    auto test = simulator.newTest("5");

    simulator.cycleActionsQueue().queue(AssertCycleGeneratesNReports<KeyboardReport_> {1});
    simulator.tapKey(3, 7); // left shift
    simulator.cycleExpectReports(AssertModifiersActive{Key_LeftShift});

    simulator.cycleActionsQueue().queue(AssertCycleGeneratesNReports<KeyboardReport_> {1});
    simulator.cycleExpectReports(AssertReportEmpty{});
  }

  //***************************************************************************
  {
    auto test = simulator.newTest("6");

    simulator.cycleActionsQueue().queue(AssertCycleGeneratesNReports<KeyboardReport_> {1});
    simulator.pressKey(3, 7); // left shift
    simulator.cycleExpectReports(
      group(
        AssertModifiersActive{Key_LeftShift},
        AssertAnyModifierActive{}
      )
    );

    simulator.pressKey(0, 7); // left control
    simulator.cycleExpectReports(
      group(
        AssertModifiersActive{Key_LeftShift, Key_LeftControl},
        AssertAnyKeycodeActive{} .negate()
      )
    );

    simulator.releaseKey(3, 7); // left shift
    simulator.releaseKey(0, 7); // left control
    simulator.cycleExpectReports(AssertReportEmpty{});

    simulator.cycles(4);
  }

  //***************************************************************************
  {
    auto test = simulator.newTest("7");

    simulator.cycleActionsQueue().queueGrouped(
      AssertNumOverallReportsEquals<KeyboardReport_> {16},
      AssertCycleIsNth{34},
      AssertElapsedTimeGreater{33}
    );
    simulator.cycle();
  }

  //***************************************************************************
  {
    auto test = simulator.newTest("8");

    simulator.cycles(10);
  }

  //***************************************************************************
  {
    auto test = simulator.newTest("9");

    simulator.advanceTimeBy(1000); // ms
  }

  //***************************************************************************
  {
    auto test = simulator.newTest("10");
    simulator.tapKey(3, 7); // left shift
    simulator.tapKey(2, 1); // A
    simulator.cycleExpectReports(DumpReport{});
  }

  //***************************************************************************
  {
    auto test = simulator.newTest("11");

    simulator.tapKey(3, 7); // left shift
    simulator.cycleExpectReports(
    CustomReportAction<KeyboardReport_> {
      [&](const KeyboardReport_ & kr) -> bool {
        simulator.log() << "Custom keyboard report action triggered";
        return true;
      }
    }
    );
    simulator.cycles(5);
  }

  //***************************************************************************
  {
    auto test = simulator.newTest("12");

    simulator.tapKey(3, 7); // left shift
    simulator.cycleExpectReports(
    CustomReportAction<Report_> {
      [&](const Report_ & report) -> bool {
        simulator.log() << "Custom cycle action triggered";
        return true;
      }
    }
    );
    simulator.cycles(5);
  }

  //***************************************************************************
  {
    auto test = simulator.newTest("13");

    simulator.advanceTimeTo(2000); // ms
  }

  //***************************************************************************
  {
    auto test = simulator.newTest("14");

    // Cycle through the color effects and output the keyboard
    // after some cycles.
    //
    simulator.multiTapKey(15 /*num. taps*/,
                          0 /*row*/, 6/*col*/,
                          50 /* num. cycles after each tap */,
    CustomAction{
      [&]() -> bool {
        renderKeyboard(simulator, keyboardio::model01::ascii_keyboard);
        return true;
      }
    }
                         );
  }

  //***************************************************************************
  {
    auto test = simulator.newTest("15");

    // LED effect solid red is the fourth LED effect. Tap the
    // LED effect forward key four times to get there.
    //
    simulator.multiTapKey(4 /*num. taps*/,
                          0 /*row*/, 6/*col*/,
                          1 /* num. cycles after each tap */,
    CustomAction{
      [&]() -> bool {
        simulator.log() << "Kaleidoscope.device().getCrgbAt(KeyAddr{0, 0}).r = "
        << (int)Kaleidoscope.device().getCrgbAt(KeyAddr{0, 0}).r;
        return true;
      }
    }
                         );

    SIMULATOR_ASSERT_CONDITION(simulator,
                               Kaleidoscope.device().getCrgbAt(KeyAddr{0, 0}).r == solid_red_level);
  }

  //***************************************************************************
  {
    auto test = simulator.newTest("16");

    // Use dumpKeyLEDState to generate a representation of the current
    // LED state.
    //
    //renderKeyboard(simulator, keyboardio::model01::ascii_keyboard);
    //dumpKeyLEDState(simulator);

    const uint8_t key_led_colors[][3] = {
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {0, 2, 171},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
      {160, 0, 0},
    };

    assertKeyLEDState(simulator, key_led_colors);
  }
}

} // namespace simulator
} // namespace kaleidoscope

#endif
