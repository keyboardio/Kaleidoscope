/* kailedoscope::sim - Harness for Unit Testing Kaleidoscope
 * Copyright (C) 2020  Eric Paniagua <epaniagua@google.com>
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

#include "testing/SimHarness.h"

#include <Arduino.h>  // for millis

#include "kaleidoscope/Runtime.h"        // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"  // for Device, VirtualProps::KeyScanner

namespace kaleidoscope {
namespace testing {

void SimHarness::RunCycle() {
  if (CycleTime() > 1) {
    // We incrememnt the time before running the loop so that
    // millisAtCycleStart ends up where we want it to
    for (size_t i = 1; i < CycleTime(); i++) {
      // The current millis implementation gets us 1 milli per call to millis.
      millis();
    }
  }
  kaleidoscope::Runtime.loop();
}

void SimHarness::RunCycles(size_t n) {
  for (size_t i = 0; i < n; ++i) RunCycle();
}

void SimHarness::RunForMillis(size_t t) {
  auto start_time = kaleidoscope::Runtime.millisAtCycleStart();
  while (kaleidoscope::Runtime.millisAtCycleStart() - start_time < t) {
    RunCycle();
  }
}

void SimHarness::Press(KeyAddr key_addr) {
  kaleidoscope::Runtime.device().keyScanner().setKeystate(
    key_addr,
    kaleidoscope::Device::Props::KeyScanner::KeyState::Pressed);
}

void SimHarness::Release(KeyAddr key_addr) {
  kaleidoscope::Runtime.device().keyScanner().setKeystate(
    key_addr,
    kaleidoscope::Device::Props::KeyScanner::KeyState::NotPressed);
}

void SimHarness::Press(uint8_t row, uint8_t col) {
  Press(KeyAddr{row, col});
}

void SimHarness::Release(uint8_t row, uint8_t col) {
  Release(KeyAddr{row, col});
}

void SimHarness::SetCycleTime(uint8_t millis) {
  millis_per_cycle_ = millis;
}

uint8_t SimHarness::CycleTime() const {
  return millis_per_cycle_;
}

// Serial support implementation
void SimHarness::ProcessSerialInput() {
  // This will be called by RunCycle() to process any pending serial input
  // The actual processing is handled by the virtual HardwareSerial implementation
}

void SimHarness::SendSerialData(const uint8_t *data, size_t length) {
  // Get direct access to the virtual device's serial port
  auto &serial = Serial;
  serial.injectInput(data, length);
}

std::vector<uint8_t> SimHarness::GetSerialOutput() {
  // Get direct access to the virtual device's serial port
  auto &serial = Serial;
  return serial.getOutputBuffer();
}

// Focus protocol helper implementations
std::string SimHarness::SendFocusCommand(const std::string &command) {
  // Ensure command ends with newline
  std::string cmd = command;
  if (cmd.empty() || cmd.back() != '\n') {
    cmd += '\n';
  }

  // Send command
  SendString(cmd);

  // Run cycles until we get a complete response
  // (ends with \r\n.\r\n)
  std::string response;
  size_t max_cycles = 100;  // Prevent infinite loops
  size_t cycles     = 0;

  while (cycles++ < max_cycles) {
    RunCycle();
    response = GetSerialOutputAsString();
    if (IsFocusResponse(response)) break;
  }

  return StripFocusTerminator(response);
}

bool SimHarness::IsFocusResponse(const std::string &response) {
  static const std::string terminator = "\r\n.\r\n";
  return response.length() >= terminator.length() &&
         response.substr(response.length() - terminator.length()) == terminator;
}

std::string SimHarness::StripFocusTerminator(const std::string &response) {
  static const std::string terminator = "\r\n.\r\n";
  if (IsFocusResponse(response)) {
    return response.substr(0, response.length() - terminator.length());
  }
  return response;
}

}  // namespace testing
}  // namespace kaleidoscope
