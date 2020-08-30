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

#include "Arduino.h"

#include "kaleidoscope/simulator/interface/Simulator_.h"
#include "kaleidoscope/simulator/interface/ActionContainer_Impl.h"
#include "kaleidoscope/simulator/interface/actions/Action_.h"
#include "kaleidoscope/simulator/interface/aux/WallTimer.h"
#include "kaleidoscope/simulator/interface/SimulatorCore_.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <thread>
#include <mutex>

namespace kaleidoscope {
namespace simulator {
namespace interface {

// Explicit template instanciations
//
    template class ActionContainer<ReportAction<BootKeyboardReport_>>;
    template class ActionContainer<ReportAction<KeyboardReport_>>;
    template class ActionContainer<ReportAction<MouseReport_>>;
    template class ActionContainer<ReportAction<AbsoluteMouseReport_>>;
    template class ActionContainer<ReportAction_>;
    template class ActionContainer<Action_>;

    std::ostream &SimulatorStream_::getOStream() const {
      return simulator_->getOStream();
    }

    bool SimulatorStream_::mute() const {
      return simulator_->getQuiet();
    }

    void SimulatorStream_::checkLineStart() {

      if (this->mute()) {
        return;
      }

      if (!line_start_) {
        return;
      }

      line_start_ = false;

      this->reactOnLineStart();
    }

    void SimulatorStream_::reactOnLineStart() {
      if (this->mute()) {
        return;
      }

      this->getOStream() << "t=" << /*std::fixed << std::setw(4) << */simulator_->getTime()
                         << ", c=" << /*std::fixed << std::setw(4) << */simulator_->getCycleId()
                         << ": ";
    }

    void SimulatorStream_::reactOnLineEnd() {
      if (this->mute()) {
        return;
      }

      this->getOStream() << "\n";
    }

    ErrorStream::ErrorStream(const Simulator_ *simulator)
      :  SimulatorStream_(simulator) {
      auto &out = this->getOStream();

      // Foreground color red
      //
      out << "\x1B[31;1m";

      this->SimulatorStream_::reactOnLineStart();
      out << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Error !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    }

    ErrorStream::~ErrorStream() {
      this->getOStream() << "\n";
      this->SimulatorStream_::reactOnLineStart();
      this->getOStream() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";

      if (simulator_->getAbortOnFirstError()) {
        this->SimulatorStream_::reactOnLineStart();
        this->getOStream() << "Bailing out.";
        exit(1);
      }

      // Restore color to neutral.
      //
      this->getOStream() << "\x1B[0m";
    }

    void ErrorStream::reactOnLineStart() {
      this->SimulatorStream_::reactOnLineStart();
      this->getOStream() << "!!! ";
    }

    DebugStream::DebugStream(const Simulator_ *simulator)
      :  SimulatorStream_(simulator) {
      if (this->mute()) {
        return;
      }

      auto &out = this->getOStream();

      // Foreground color blue
      //
      out << "\x1B[34;1m";

      this->SimulatorStream_::reactOnLineStart();
    }

    DebugStream::~DebugStream() {
      if (this->mute()) {
        return;
      }

      this->getOStream() << "\n";

      // Restore color to neutral.
      //
      this->getOStream() << "\x1B[0m";
    }

    void DebugStream::reactOnLineStart() {
      if (this->mute()) {
        return;
      }

      this->SimulatorStream_::reactOnLineStart();
      this->getOStream() << "***Debug: ";
    }

    LogStream::LogStream(const Simulator_ *simulator)
      :  SimulatorStream_(simulator) {
    }

    LogStream::~LogStream() {
      if (this->mute()) {
        return;
      }

      this->getOStream() << "\n";
    }

    HeaderStream::HeaderStream(const Simulator_ *simulator)
      :  SimulatorStream_(simulator) {
      if (this->mute()) {
        return;
      }

      // Foreground color red
      //
      this->getOStream() << "\x1B[32;1m";

      this->SimulatorStream_::reactOnLineStart();
      this->getOStream() << "################################################################################\n";
    }

    HeaderStream::~HeaderStream() {

      if (this->mute()) {
        return;
      }

      this->getOStream() << "\n";
      this->SimulatorStream_::reactOnLineStart();
      this->getOStream() << "################################################################################\n";

      // Restore color to neutral.
      //
      this->getOStream() << "\x1B[0m";
    }

    void HeaderStream::reactOnLineStart() {
      if (this->mute()) {
        return;
      }

      this->SimulatorStream_::reactOnLineStart();
      this->getOStream() << "### ";
    }

    Test::Test(Simulator_ *simulator, const char *name)
      :  simulator_(simulator),
         name_(name),
         error_count_start_(simulator->getErrorCount()) {
      simulator->header() << "Test " << name;
    }

    Test::~Test() {
      simulator_->assertNothingQueued();

      auto error_count_end = simulator_->getErrorCount();

      if (error_count_start_ != error_count_end) {
        simulator_->error() << "Test " << name_ << " failed";
      }
    }

    Simulator_::Simulator_(std::ostream &out,
                           bool debug,
                           int cycle_duration,
                           bool abort_on_first_error)

      :  out_{&out},
         debug_{debug},
         cycle_duration_{cycle_duration}, // ms
         abort_on_first_error_{abort_on_first_error},

         queued_report_actions_{*this},

         permanent_boot_keyboard_report_actions_{*this},
         permanent_keyboard_report_actions_{*this},
         permanent_mouse_report_actions_{*this},
         permanent_absolute_mouse_report_actions_{*this},
         permanent_generic_report_actions_{*this},

         queued_cycle_actions_{*this},
         permanent_cycle_actions_{*this} {
      this->headerText();
    }

    Simulator_::~Simulator_() {
      this->footerText();

      if (!test_success_) {
        this->error() << "Terminating with exit code 1";
        exit(1);
      }
    }

    void Simulator_::pressKey(uint8_t row, uint8_t col) {
      this->log() << "+ Activating key (" << (unsigned)row << ", " << (unsigned)col << ")";
      simulator_core_->pressKey(row, col);
    }

    void Simulator_::releaseKey(uint8_t row, uint8_t col) {
      this->log() << "+ Releasing key (" << (unsigned)row << ", " << (unsigned)col << ")";
      simulator_core_->releaseKey(row, col);
    }

    void Simulator_::tapKey(uint8_t row, uint8_t col) {
      this->log() << "+- Tapping key (" << (unsigned)row << ", " << (unsigned)col << ")";
      simulator_core_->tapKey(row, col);
    }

    void Simulator_::multiTapKeyInternal(int num_taps, uint8_t row, uint8_t col,
                                         int tap_interval_cycles,
                                         std::shared_ptr<Action_> after_tap_and_cycles_action) {
      if (after_tap_and_cycles_action) {
        after_tap_and_cycles_action->setSimulator(this);
      }

      this->log() << "+- Tapping key (" << (unsigned)row << ", " << (unsigned)col << ") "
                  << num_taps << " times";
      for (int i = 0; i < num_taps; ++i) {

        // Do the tap
        //
        simulator_core_->tapKey(row, col);

        // Run a user-defined number of cycles
        //
        for (int c = 0; c < tap_interval_cycles; ++c) {
          this->cycleInternal(true /* only log reports */);
        }

        // Check and execute the action
        //
        if (after_tap_and_cycles_action) {
          this->log() << "Checking action after tap no. " << i;
          if (!after_tap_and_cycles_action->eval()) {
            this->error() << "Action after tap " << i << " failed";
            after_tap_and_cycles_action->report();
          }
        }
      }
    }

    void Simulator_::clearAllKeys() {
      this->log() << "- Clearing all keys";

      uint8_t rows = 0, cols = 0;

      simulator_core_->getKeyMatrixDimensions(rows, cols);

      for (uint8_t row = 0; row < rows; row++) {
        for (uint8_t col = 0; col < cols; col++) {
          simulator_core_->releaseKey(row, col);
        }
      }
    }

    void Simulator_::cycle(bool suppress_cycle_info_log) {
      if (!suppress_cycle_info_log) {
        this->log() << "Running single scan cycle";
      }

      this->cycleInternal(true);

      if (!suppress_cycle_info_log) {
        this->log() << "";
      }
    }

    void Simulator_::cyclesInternal(int n,
                                    const std::vector<std::shared_ptr<Action_>> &cycle_action_list) {
      if (n == 0) {
        n = scan_cycles_default_count_;
      }

      this->log() << "Running " << n << " scan cycles";

      for (int i = 0; i < n; ++i) {
        this->cycleInternal(true /* on_log_reports */);
        this->evaluateActionsInternal(cycle_action_list);
      }

      this->log() << "";
    }

    void Simulator_::advanceTimeBy(Simulator_::TimeType delta_t) {

      this->checkCycleDurationSet();

      this->log() << "Skipping dt >= " << delta_t << " ms";

      this->skipTimeInternal(delta_t);
    }

    void Simulator_::skipTimeInternal(Simulator_::TimeType delta_t) {

      auto start_cycle = cycle_id_;

      auto start_time = time_;

      Simulator_::TimeType elapsed_time = 0;
      while (elapsed_time < delta_t) {
        this->cycleInternal(true /* only_log_reports */);
        elapsed_time = time_ - start_time;
      }

      this->log() << elapsed_time << " ms (" << (cycle_id_ - start_cycle) << " cycles) skipped";

      this->log() << "";
    }

    void Simulator_::advanceTimeTo(TimeType time) {
      if (time <= time_) {
        this->error() << "Failed cycling to time " << time << " ms. Target time is in the past.";
        return;
      }

      this->log() << "Cycling to t = " << time << " ms";

      TimeType delta_t = time - time_;

      this->skipTimeInternal(delta_t);
    }

    void Simulator_::init() {
      this->clearAllKeys();
      simulator_core_->init();
    }

    bool Simulator_::checkStatus() {

      if (!queued_report_actions_.empty()) {
        this->error() << "There are " << queued_report_actions_.size()
                      << " left over actions in the queue.";
        test_success_ = false;
      }

      if (!actions_passed_) {
        this->error() << "Not all actions passed.";
        test_success_ = false;
      }

      if (error_count_ != 0) {
        test_success_ = false;
      }

      if (test_success_) {
        this->log() << "All tests passed.";
        return true;
      } else {
        this->error() << "Errors occurred.";
      }

      return false;
    }

    void Simulator_::headerText() {

      // Foreground color yellow
      //
      this->getOStream() << "\x1B[33;1m";

      this->log() << "";
      this->log() << "################################################################################";
      this->log() << "";
      this->log() << "Papilio Keyboard Simulator";
      this->log() << "";
      this->log() << "author: noseglasses (https://github.com/noseglasses, shinynoseglasses@gmail.com)";
      this->log() << "";
      this->log() << "cycle duration: " << cycle_duration_ << " ms";
      this->log() << "debug: " << std::boolalpha << debug_;
      this->log() << "aborting on first error: " << abort_on_first_error_;
      this->log() << "################################################################################";
      this->log() << "";

      // Restore color to neutral.
      //
      //this->getOStream() << "\x1B[0m";
      this->getOStream() << "\x1B[0m";
    }

    void Simulator_::footerText() {

      // Foreground color yellow
      //
      this->getOStream() << "\x1B[33;1m";

      this->log() << "";
      this->log() << "################################################################################";
      this->log() << "Testing done";
      this->log() << "";
      this->log() << "duration: " << time_ << " ms = " << cycle_id_ << " cycles";
      this->log() << "error_count: " << error_count_;
      this->log() << "";
      this->log() << "num. overall reports processed: " << n_typed_overall_reports_[AnyTypeReportTypeId];
      this->log() << "num. boot keyboard reports processed: " << n_typed_overall_reports_[BootKeyboardReportTypeIdId];
      this->log() << "num. keyboard reports processed: " << n_typed_overall_reports_[KeyboardReportTypeId];
      this->log() << "num. mouse reports processed: " << n_typed_overall_reports_[MouseReportTypeId];
      this->log() << "num. absolute mouse reports processed: " << n_typed_overall_reports_[AbsoluteMouseReportTypeId];
      this->log() << "";
      this->checkStatus();
      this->getOStream() << "\x1B[33;1m";
      this->log() << "";
      this->log() <<  "################################################################################";
      this->log() << "";

      // Restore color to neutral.
      //
      //this->getOStream() << "\x1B[0m";
      this->getOStream() << "\x1B[0m";
    }

    void Simulator_::cycleInternal(bool only_log_reports) {

      ++cycle_id_;
      n_reports_in_cycle_ = 0;

      for (int i = 0; i < 3; ++i) {
        n_typed_reports_in_cycle_[i] = 0;
      }

      if (!only_log_reports) {
        this->log() << "Scan cycle " << cycle_id_;
      }

      // Set the global simulator time.
      //
      simulator_core_->setTime(time_);

      simulator_core_->loop();

      if (n_reports_in_cycle_ == 0) {
        if (!only_log_reports) {
          this->log() << "No keyboard reports processed";
        }
      } else {
        this->log() << n_reports_in_cycle_ << " keyboard reports processed";
      }

      time_ += cycle_duration_;

      if (!queued_cycle_actions_.empty()) {
        this->log() << "Processing " << queued_cycle_actions_.size()
                    << " queued cycle actions";
        this->evaluateActionsInternal(queued_cycle_actions_.directAccess());

        queued_cycle_actions_.clear();
      }

      if (!permanent_cycle_actions_.empty()) {
        this->log() << "Processing " << permanent_cycle_actions_.size()
                    << " permanent cycle actions";

        this->evaluateActionsInternal(permanent_cycle_actions_.directAccess());
      }
    }

    void Simulator_::checkCycleDurationSet() {
      if (cycle_duration_ == 0) {
        this->error() << "Please set test.cycle_duration_ to a value in "
                      "[ms] greater zero before using time based testing";
      }
    }

    void Simulator_::assertNothingQueued() const {
      if (!queued_report_actions_.empty()) {
        this->error() << "Report actions are left in queue";
      }

      if (!queued_cycle_actions_.empty()) {
        this->error() << "Cycle actions are left in queue";
      }
    }

    void Simulator_::assertCondition(bool cond, const char *action_code) const {
      if (!cond) {
        this->error() << "Action failed: " << action_code;
      }
    }

    void Simulator_::runRealtime(TimeType duration,
                                 const std::function<void()> &cycle_function) {
      auto n_cycles = duration / cycle_duration_;

      aux::WallTimer timer;

      for (decltype(n_cycles) i = 0; i < n_cycles; ++i) {

        timer.start();

        this->cycle();
        cycle_function();

        // Slow down the simulation if necessary.
        //
        do {} while (timer.elapsed() < cycle_duration_);
      }
    }

    class DataCollectorThread {
     public:

      static constexpr uint8_t n_bytes_per_row = 8;

      void operator()() {
        std::string line;

        while (1) {

          if (std::cin.peek() == EOF) {
            continue;
          }
          std::getline(std::cin, line);

          if (line.empty() || line[0] == '.') {
            continue;
          }

          std::unique_lock<std::mutex> lock(mutex_);

          std::istringstream line_stream(line);
          for (uint8_t byte_id = 0; byte_id < n_bytes_per_row; ++byte_id) {
            line_stream >> key_bitfield_[byte_id];
          }
        }
      }

     private:

      uint16_t key_bitfield_[n_bytes_per_row] = {};
      std::mutex mutex_;

      friend class Simulator_;
    };

    void Simulator_::runRemoteControlled(const std::function<void()> &cycle_callback,
                                         bool realtime
                                        ) {
      aux::WallTimer timer;

      DataCollectorThread dct;

      std::thread thread_obj(std::ref(dct));

      while (1) {

        if (realtime) {
          timer.start();
        }

        // The additional scope is necessary to limit the lifetime
        // of the mutex lock
        //
        {
          std::unique_lock<std::mutex> lock(dct.mutex_);

          uint8_t rows = 0, cols = 0;

          simulator_core_->getKeyMatrixDimensions(rows, cols);

          for (uint8_t row = 0; row < rows; row++) {
            for (uint8_t col = 0; col < cols; col++) {

              uint16_t pos = row * cols + col;

              uint8_t byte_id = pos / DataCollectorThread::n_bytes_per_row;
              uint8_t bit_id = pos % DataCollectorThread::n_bytes_per_row;

              bool is_keyswitch_pressed = bitRead(dct.key_bitfield_[byte_id], bit_id);// & (1 << bit_id);

              if (is_keyswitch_pressed) {

                if (!simulator_core_->isKeyPressed(row, col)) {
                  simulator_core_->pressKey(row, col);
                }
              } else {
                if (simulator_core_->isKeyPressed(row, col)) {
                  simulator_core_->releaseKey(row, col);
                }
              }
            }
          }
        }

        this->cycleInternal(true /*only log reports*/);

        cycle_callback();

        // Slow down the simulation if necessary.
        //
        if (realtime) {
          double elapsed = 0;
          do {
            elapsed = timer.elapsed();
            //std::cout << "Elapsed: " << elapsed << std::endl;
          } while (elapsed < cycle_duration_);
        }
      }

      thread_obj.join();
    }

  } // namespace interface
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
