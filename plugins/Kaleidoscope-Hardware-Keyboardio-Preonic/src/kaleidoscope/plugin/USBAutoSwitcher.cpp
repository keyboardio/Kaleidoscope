/* Kaleidoscope-USBAutoSwitcher -- Automatic USB/BLE device switching for Preonic
 * Copyright 2025 Keyboard.io, inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * Additional Permissions:
 * As an additional permission under Section 7 of the GNU General Public
 * License Version 3, you may link this software against a Vendor-provided
 * Hardware Specific Software Module under the terms of the MCU Vendor
 * Firmware Library Additional Permission Version 1.0.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "kaleidoscope/plugin/USBAutoSwitcher.h"
#include "kaleidoscope/Runtime.h"

namespace kaleidoscope {
namespace plugin {

// Initialize static members
uint8_t USBAutoSwitcher::fallback_ble_device_ = 1;  // Default to BLE device 1
uint32_t USBAutoSwitcher::startup_time_       = 0;
uint16_t USBAutoSwitcher::startup_delay_ms_   = 1000;  // 1 second startup delay
bool USBAutoSwitcher::startup_switch_done_    = false;
uint8_t USBAutoSwitcher::current_host_mode_   = MODE_USB;  // Default to USB mode

EventHandlerResult USBAutoSwitcher::onSetup() {
  // Record startup time for delay calculation
  startup_time_        = Runtime.millisAtCycleStart();
  startup_switch_done_ = false;

  // Initialize current mode tracking
  current_host_mode_ = getCurrentHostMode();

  // If no USB detected at startup, we'll need to check after delay
  // For now, we'll use afterEachCycle but only until startup_switch_done_

  return EventHandlerResult::OK;
}

EventHandlerResult USBAutoSwitcher::afterEachCycle() {
  // Immediate return after startup switching is done - minimal overhead
  if (startup_switch_done_) {
    return EventHandlerResult::OK;
  }

  // Only during first second of operation
  if ((Runtime.millisAtCycleStart() - startup_time_) >= startup_delay_ms_) {
    // Check USB status once and switch if needed
    if (!Runtime.device().mcu().USBPowerDetected()) {
      switchToFallbackBLE();
    }
    startup_switch_done_ = true;
  }

  return EventHandlerResult::OK;
}

EventHandlerResult USBAutoSwitcher::onHostConnectionStatusChanged(uint8_t device_id, HostConnectionStatus status) {
  // Handle USB connection events (device_id 0)
  if (device_id == 0) {
    switch (status) {
    case HostConnectionStatus::Connected:
      // USB data connection established
      if (current_host_mode_ != MODE_USB) {
        switchToUSB();
      }
      startup_switch_done_ = true;  // Cancel any pending startup switch
      break;

    case HostConnectionStatus::Disconnected:
      // USB fully disconnected - switch to BLE if we were on USB
      if (current_host_mode_ == MODE_USB) {
        switchToFallbackBLE();
      }
      break;

    case HostConnectionStatus::Connecting:
      // USB power only - handle startup switching
      if (!startup_switch_done_ && !isStartupPeriod()) {
        // Startup delay has passed and we only have USB power (no data)
        switchToFallbackBLE();
        startup_switch_done_ = true;
      }
      break;

    default:
      // Other USB states don't trigger switching
      break;
    }
  } else if (device_id >= 1 && device_id <= 4) {
    // Handle BLE device connection events (device_id 1-4)
    switch (status) {
    case HostConnectionStatus::Connected:
    case HostConnectionStatus::DeviceSelected:
      // BLE device connected or selected - update fallback
      updateFallbackBLEDevice(device_id);
      break;

    default:
      // Other BLE states don't affect fallback tracking
      break;
    }
  }

  return EventHandlerResult::OK;
}

bool USBAutoSwitcher::isStartupPeriod() {
  return (Runtime.millisAtCycleStart() - startup_time_) < startup_delay_ms_;
}

void USBAutoSwitcher::switchToFallbackBLE() {
  // Select the remembered BLE device and switch to BLE mode
  Runtime.device().ble().selectDevice(fallback_ble_device_);
  Runtime.device().setHostConnectionMode(MODE_BLE);
  current_host_mode_ = MODE_BLE;
}

void USBAutoSwitcher::switchToUSB() {
  // Disconnect from BLE if connected
  if (Runtime.device().ble().connected()) {
    Runtime.device().ble().disconnect();
  }

  // Switch to USB mode
  Runtime.device().setHostConnectionMode(MODE_USB);
  current_host_mode_ = MODE_USB;
}

void USBAutoSwitcher::updateFallbackBLEDevice(uint8_t device_id) {
  // Only update if this is a valid BLE device ID
  if (device_id >= 1 && device_id <= 4) {
    fallback_ble_device_ = device_id;
    current_host_mode_   = MODE_BLE;
  }
}

uint8_t USBAutoSwitcher::getCurrentHostMode() {
  // Query the device for current host connection mode
  return Runtime.device().getHostConnectionMode();
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::USBAutoSwitcher USBAutoSwitcher;